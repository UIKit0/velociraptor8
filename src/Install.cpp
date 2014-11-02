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
    WCHAR buf[1024*4];
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

// TODO: make it GetLocalAppDir(const char *add1 = NULL, const char *add2 = NULL, const char *add3 = NULL)
// so that GetInstallationBaseDir() is just return GetLocalAppDir("velociraptor8")
std::string GetLocalAppDir() {
    return GetKnownFolderPathXp(CSIDL_LOCAL_APPDATA);
}

std::string GetInstallationBaseDir() {
    auto path = GetLocalAppDir();
    path::Join(path, APP_DIR_NAME);
    return path;
}

std::string GetInstallationBinDir() {
    auto path = GetInstallationBaseDir();
    path::Join(path, "bin");
    return path;
}

// we consider the program installed if the exe is in the installation
// directory
bool IsRunningInstalled() {
    auto exePath = GetExePath();
    auto binDir = GetInstallationBinDir();
    // Maybe: compare for equality?
    auto pos = exePath.find(binDir);
    return pos == 0;
}

bool IsRunnignPortable() {
    return !IsRunningInstalled();
}
