#include "Common.h"
#include "Install.h"

// http://msdn.microsoft.com/en-us/library/windows/desktop/ms683197(v=vs.85).aspx
static std::string GetExePath() {
    WCHAR buf[1024*4];
    // it's full path, not name
    DWORD bufCchSize = dimof(buf);
    // TODO: normalize path
    DWORD ret = GetModuleFileNameW(NULL, buf, bufCchSize);
    CrashIf(ret == 0);
    // on XP it returns bufCchSize if buffer too small
    CrashIf(ret == bufCchSize);
    // post XP error is indicated by GetLastError()
    CrashIfLastError();
    return WstrToUtf8Str(buf);
}

// http://msdn.microsoft.com/en-us/library/windows/desktop/bb762181(v=vs.85).aspx
// TODO: deprecated, on Vista+ use SHGetSpecialFolderPath
static std::string GetKnownFolderPathXp(int nFolder) {
    WCHAR buf[MAX_PATH];
    HRESULT hr = SHGetFolderPath(NULL, nFolder, NULL, SHGFP_TYPE_DEFAULT, buf);
    CrashIf(hr != S_OK);
    return WstrToUtf8Str(buf);
}

std::string GetLocalAppDir() {
    return GetKnownFolderPathXp(CSIDL_LOCAL_APPDATA);
}

bool IsRunningInstalled() {
    auto exePath = GetExePath();
    auto appDir = GetLocalAppDir();
    auto pos = exePath.find(appDir);
    return pos == 0;
}

bool IsRunnignPortable() {
    return !IsRunningInstalled();
}
