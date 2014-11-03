#include "Common.h"
#include "Install.h"
#include "FileUtil.h"
// Maybe: not ideal, decouple this via SetAppDirName() instead of APP_DIR_NAME
#include "ve8.h"
#include "WinUtil.h"
#include "Version.h"
#include "Helpers.h"

// http://msdn.microsoft.com/en-us/library/windows/desktop/ms683197(v=vs.85).aspx
static std::string GetExePath() {
    WCHAR buf[1024 * 4];
    // it's full path, not name
    DWORD bufCchSize = dimof(buf);
    DWORD ret = GetModuleFileNameW(NULL, buf, bufCchSize);
    CrashIf(ret == 0);
    // on XP it returns bufCchSize if buffer too small
    CrashIf(ret == bufCchSize);
    // post XP error is indicated by GetLastError()
    CrashIfLastError();
    path::NormalizeInPlace(buf, dimof(buf));
    return WstrToUtf8Str(buf);
}

// http://msdn.microsoft.com/en-us/library/windows/desktop/bb762181(v=vs.85).aspx
static std::string GetKnownFolderPathXp(int nFolder) {
    WCHAR buf[MAX_PATH];
    // TODO: on Vista+ use SHGetSpecialFolderPath, SHGetFolderPath is deprecated
    HRESULT hr = SHGetFolderPath(NULL, nFolder, NULL, SHGFP_TYPE_DEFAULT, buf);
    CrashIf(hr != S_OK);
    return WstrToUtf8Str(buf);
}

static std::string GetLocalAppDir(const char *p1 = NULL, const char *p2 = NULL, const char *p3 = NULL, const char *p4 = NULL) {
    std::string path(GetKnownFolderPathXp(CSIDL_LOCAL_APPDATA));
    if (p1 != NULL) {
        path::Join(path, p1);
    }
    if (p2 != NULL) {
        path::Join(path, p2);
    }
    if (p3 != NULL) {
        path::Join(path, p3);
    }
    if (p4 != NULL) {
        path::Join(path, p4);
    }
    return path;
}

static std::string GetInstalledExePath() {
    return GetLocalAppDir(APP_DIR_NAME, BIN_DIR_NAME, EXE_NAME);
}

static std::string GetShortcutPath(bool allUsers) {
    // CSIDL_COMMON_PROGRAMS => installing for all users
    std::string path(GetKnownFolderPathXp(allUsers ? CSIDL_COMMON_PROGRAMS : CSIDL_PROGRAMS));
    if (!path.empty()) {
        path::Join(path, APP_NAME ".lnk");
    }
    return path;
}

static bool CreateAppShortcut(bool allUsers) {
    std::string shortcutPath(GetShortcutPath(allUsers));
    if (shortcutPath.empty()) {
        return false;
    }
    std::string exePath(GetInstalledExePath());
    return CreateShortcut(shortcutPath, exePath);
}
// we consider the program installed if the exe is in the installation
// directory
bool IsRunningInstalled() {
    auto exePath = GetExePath();
    auto binDir = GetLocalAppDir(APP_DIR_NAME, BIN_DIR_NAME);
    // Maybe: get the full binary path and compare for equality?
    auto pos = exePath.find(binDir);
    return pos == 0;
}

bool IsRunningPortable() { return !IsRunningInstalled(); }

static int64_t GetFullSize(WIN32_FIND_DATA& d) {
    uint64_t tmp = d.nFileSizeHigh;
    tmp = tmp >> 32;
    return (int64_t) tmp + (int64_t) d.nFileSizeLow;
}

static int64_t GetDirSizeRecur(const std::string& dir) {
    std::string dirPatternA(dir);
    path::Join(dirPatternA, "*", 1);
    AutoUtf8ToWstr dirPattern(dirPatternA);
    WIN32_FIND_DATA findData;

    HANDLE h = FindFirstFile(dirPattern, &findData);
    if (h == INVALID_HANDLE_VALUE)
        return 0;

    int64_t totalSize = 0;
    do {
        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            totalSize += GetFullSize(findData);
        } else if (!str::Eq(findData.cFileName, L".") && !str::Eq(findData.cFileName, L"..")) {
            AutoWstrToUtf8 fileName(findData.cFileName);
            std::string subDir(dir);
            path::Join(subDir, fileName.Get());
            totalSize += GetDirSizeRecur(subDir);
        }
    } while (FindNextFile(h, &findData) != 0);
    FindClose(h);
    return totalSize;
}

// This is in HKLM. Note that on 64bit windows, if installing 32bit app
// the installer has to be 32bit as well, so that it goes into proper
// place in registry (under Software\Wow6432Node\Microsoft\Windows\...
#define REG_PATH_UNINST     "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\" APP_NAME
// REG_SZ, a path to installed executable (or "$path,0" to force the first icon)
#define DISPLAY_ICON "DisplayIcon"
// REG_SZ, e.g "SumatraPDF" (APP_NAME_STR)
#define DISPLAY_NAME "DisplayName"
// REG_SZ, e.g. "1.2" (CURR_VERSION_STR)
#define DISPLAY_VERSION "DisplayVersion"
// REG_DWORD, get size of installed directory after copying files
#define ESTIMATED_SIZE "EstimatedSize"
// REG_SZ, the current date as YYYYMMDD
#define INSTALL_DATE "InstallDate"
// REG_DWORD, set to 1
#define NO_MODIFY "NoModify"
// REG_DWORD, set to 1
#define NO_REPAIR "NoRepair"
// REG_SZ, e.g. "Krzysztof Kowalczyk"
#define PUBLISHER "Publisher"
// REG_SZ, command line for uninstaller
#define UNINSTALL_STRING "UninstallString"
// REG_SZ, e.g. "http://blog.kowalczyk.info/software/sumatrapdf/"
#define URL_INFO_ABOUT "URLInfoAbout"
// REG_SZ, e.g. "http://blog.kowalczyk.info/software/sumatrapdf/news.html"
#define URL_UPDATE_INFO "URLUpdateInfo"
// REG_SZ, same as INSTALL_DIR below
#define INSTALL_LOCATION "InstallLocation"

std::string GetInstallDate() {
    SYSTEMTIME st;
    GetSystemTime(&st);
    return str::Format("%04d%02d%02d", st.wYear, st.wMonth, st.wDay);
}

static bool InstallWriteRegistry(HKEY hkey) {
    bool ok = true;

    std::string exePath(GetInstalledExePath());
    std::string installDate(GetInstallDate());
    std::string installDir(GetLocalAppDir(APP_DIR_NAME));
    std::string uninstallCmdLine("\"");
    uninstallCmdLine.append(exePath);
    uninstallCmdLine.append("\" /uninstall");

    int64_t size = GetDirSizeRecur(installDir) / 1024;

    ok &= WriteRegStr(hkey, REG_PATH_UNINST, DISPLAY_ICON, exePath.c_str());
    ok &= WriteRegStr(hkey, REG_PATH_UNINST, DISPLAY_NAME, APP_NAME);
    ok &= WriteRegStr(hkey, REG_PATH_UNINST, DISPLAY_VERSION, CURR_VERSION_STRA);
    // Windows XP doesn't allow to view the version number at a glance,
    // so include it in the DisplayName
    if (!IsVista()) {
        ok &= WriteRegStr(hkey, REG_PATH_UNINST, DISPLAY_NAME, APP_NAME " " CURR_VERSION_STRA);
    }
    ok &= WriteRegDWORD(hkey, REG_PATH_UNINST, ESTIMATED_SIZE, (DWORD)size);
    ok &= WriteRegStr(hkey, REG_PATH_UNINST, INSTALL_DATE, installDate.c_str());
    ok &= WriteRegStr(hkey, REG_PATH_UNINST, INSTALL_LOCATION, installDir.c_str());
    ok &= WriteRegDWORD(hkey, REG_PATH_UNINST, NO_MODIFY, 1);
    ok &= WriteRegDWORD(hkey, REG_PATH_UNINST, NO_REPAIR, 1);
    ok &= WriteRegStr(hkey, REG_PATH_UNINST, PUBLISHER, "Krzysztof Kowalczyk");
    ok &= WriteRegStr(hkey, REG_PATH_UNINST, UNINSTALL_STRING, uninstallCmdLine.c_str());
    ok &= WriteRegStr(hkey, REG_PATH_UNINST, URL_INFO_ABOUT, "http://blog.kowalczyk.info/software/velociraptor8/");
    ok &= WriteRegStr(hkey, REG_PATH_UNINST, URL_UPDATE_INFO, "http://blog.kowalczyk.info/software/velociraptor8/news.html");

    return ok;
}

static bool InstallCopyFiles() {
    // TODO(kjk): kill processes that match the path we're writing to
    auto exePath = GetExePath();
    auto dstPath = GetInstalledExePath();
    if (!dir::CreateForFile(dstPath)) {
        return false;
    }
    bool allUsers = false;
    if (!CreateAppShortcut(allUsers)) {
        return false;
    }
    // TODO(kjk): copy settings file as well
    return file::Copy(dstPath, exePath);
}

static bool InstallSetRegistryKeys() { return true; }

// adds our bin directory to %PATH%
// Maybe: also check HKLM_keyName = @"SYSTEM\CurrentControlSet\Control\Session Manager\Environment";
// (although we don't write to it)
bool AddSelfToPath() {
    std::string path;
    bool ok = ReadRegStr(HKEY_CURRENT_USER, "Environment", "Path", path);
    if (!ok) {
        return false;
    }
    std::string binDir(GetLocalAppDir(APP_DIR_NAME, BIN_DIR_NAME));
    if (str::ContainsI(path, binDir)) {
        return true;
    }
    char last = str::LastChar(path);
    if (last != ';') {
        path.append(1, ';');
    }
    path.append(binDir);
    ok = WriteRegExpandStr(HKEY_CURRENT_USER, "Environment", "Path", path.c_str());
    BroadcastEnvRegistryChanged();
    return ok;
}

bool RemoveSelfFromPath() {
    std::string path;
    bool ok = ReadRegStr(HKEY_CURRENT_USER, "Environment", "Path", path);
    if (!ok) {
        return false;
    }
    std::string binDir(GetLocalAppDir(APP_DIR_NAME, BIN_DIR_NAME));
    size_t pos = str::FindIPos(path.c_str(), binDir.c_str());
    if (pos == std::string::npos) {
        return true;
    }
    size_t end = pos + binDir.size();
    bool removedSep = false;
    if (pos > 0 && path.at(pos - 1) == ';') {
        pos--;
        removedSep = true;
    }
    if (!removedSep && end < path.size() - 1 && path.at(end) == ';') {
        end++;
    }
    path.erase(pos, end - pos);
    //ok = WriteRegExpandStr(HKEY_CURRENT_USER, "Environment", "Path", path.c_str());
    BroadcastEnvRegistryChanged();
    return ok;
}

bool CanInstall() {
    if (IsRunningInstalled()) {
        return false;
    }
    // don't install if running as elevated as I think it can mess up
    // file permissions and stuff
    if (IsElevated()) {
        return false;
    }
    auto path(GetInstalledExePath());
    if (file::Exists(path)) {
        // TODO(kjk): allow installation if my version > installed version
        return false;
    }
    return true;
}

void Install() {
    CrashIf(!CanInstall());
    if (!InstallCopyFiles()) {
        // TODO(kjk): show error message or crash
        return;
    }
    // Maybe: also for HKEY_LOCAL_MACHINE ?
    if (!InstallWriteRegistry(HKEY_CURRENT_USER)) {
        // TODO(kjk): show error message or crash
        return;
    }

    if (!AddSelfToPath()) {
        return;
    }
    // TODO(kjk): relaunch the installed version
}

// called when /uninstall cmd-line arg is given
void Uninstall() {

}
