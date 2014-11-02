#include "Common.h"
#include "Install.h"
#include "PathUtil.h"
// Maybe: not ideal, decouple this via SetAppDirName() instead of APP_DIR_NAME
#include "ve8.h"

// http://msdn.microsoft.com/en-us/library/windows/desktop/bb773569(v=vs.85).aspx
// TODO: on win8 use PathCchCanonicalize or PathCchCanonicalizeEx
void NormalizePathInPlace(WCHAR *src, size_t srcCchSize) {
    // TODO: make it work even if srcCchSize is < MAX_PATH
    CrashIf(srcCchSize < MAX_PATH);
    WCHAR buf[MAX_PATH];
    BOOL ok = PathCanonicalizeW(buf, src);
    if (!ok) {
        return;
    }
    memcpy(src, buf, sizeof(buf));
}
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
    NormalizePathInPlace(buf, dimof(buf));
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

// we consider the program installed if the exe is in the installation
// directory
bool IsRunningInstalled() {
    auto exePath = GetExePath();
    auto binDir = GetLocalAppDir(APP_DIR_NAME, BIN_DIR_NAME);
    // Maybe: get the full binary path and compare for equality?
    auto pos = exePath.find(binDir);
    return pos == 0;
}

bool IsRunnignPortable() { return !IsRunningInstalled(); }

static bool InstallCopyFiles() {
    // TODO(kjk): kill processes that match the path we're writing to
    auto exePath = GetExePath();
    auto dstPath = GetLocalAppDir(APP_DIR_NAME, BIN_DIR_NAME, EXE_NAME);
    if (!path::CreateDirForFile(dstPath)) {
        return false;
    }
    return path::FileCopy(dstPath, exePath);
}

static bool InstallSetRegistryKeys() { return true; }

// adds our bin directory to %PATH%
static bool AddSelfToPath() { return true; }

void Install() {
    if (!InstallCopyFiles()) {
        // TODO(kjk): show error message or crash
        return;
    }
    if (!InstallSetRegistryKeys()) {
        return;
        // TODO(kjk): show error message or crash
    }
    if (!AddSelfToPath()) {
        return;
    }
    // TODO(kjk): relaunch the installed version
}
