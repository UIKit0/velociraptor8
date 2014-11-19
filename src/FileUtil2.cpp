#include "Common.h"
#include "FileUtil2.h"
#include "FileUtil.h"

namespace path {

// http://msdn.microsoft.com/en-us/library/windows/desktop/bb773569(v=vs.85).aspx
// TODO: on win8 use PathCchCanonicalize or PathCchCanonicalizeEx
void NormalizeInPlace(WCHAR* src, size_t srcCchSize) {
    // TODO: make it work even if srcCchSize is < MAX_PATH
    CrashIf(srcCchSize < MAX_PATH);
    WCHAR buf[MAX_PATH];
    BOOL ok = PathCanonicalizeW(buf, src);
    if (!ok) {
        return;
    }
    memcpy(src, buf, sizeof(buf));
}

void Join(std::string& sInOut, const char* s, size_t sLen) {
    char c = str::LastChar(sInOut);
    if (!IsSep(c)) {
        sInOut.append(1, '\\');
    }
    if (sLen == 0) {
        sLen = str::Len(s);
    }
    if (IsSep(*s)) {
        ++s;
        --sLen;
    }
    sInOut.append(s, sLen);
}

void Join(std::string& sInOut, const std::string& s2) {
    return Join(sInOut, s2.c_str(), s2.size());
}

// consider using PathRemoveFileSpec()
// http://msdn.microsoft.com/en-us/library/windows/desktop/bb773748(v=vs.85).aspx
// Caller has to free()
char *GetDir(const char*path) {
    const char *baseName = GetBaseName(path);
    if (baseName == path) // relative directory
        return str::Dup(".");
    if (baseName == path + 1) // relative root
        return str::DupN(path, 1);
    if (baseName == path + 3 && path[1] == ':') // local drive root
        return str::DupN(path, 3);
    if (baseName == path + 2 && str::StartsWith(path, "\\\\")) // server root
        return str::Dup(path);
    // any subdirectory
    return str::DupN(path, baseName - path - 1);
}

} // namespace path

namespace file {

bool Exists(const char* path) {
    if (str::IsEmpty(path)) {
        return false;
    }
    AutoUtf8ToWstr pathW(path);

    WIN32_FILE_ATTRIBUTE_DATA fileInfo;
    BOOL res = GetFileAttributesEx(pathW.Get(), GetFileExInfoStandard, &fileInfo);
    if (0 == res) {
        return false;
    }

    if (fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        return false;
    }
    return true;
}

bool Copy(const char *dst, const char *src) {
    AutoUtf8ToWstr dstW(dst);
    AutoUtf8ToWstr srcW(src);
    BOOL failIfExists = TRUE;
    BOOL res = CopyFileW(srcW.Get(), dstW.Get(), failIfExists);
    return res != 0;
}

// Return true if the file wasn't there or was successfully deleted
bool Delete(const char *path) {
    AutoUtf8ToWstr pathW(path);
    BOOL ok = DeleteFile(pathW.Get());
    return ok || GetLastError() == ERROR_FILE_NOT_FOUND;
}

} // namespace file

namespace dir {

bool Exists(const char* dir) {
    if (str::IsEmpty(dir)) {
        return false;
    }
    AutoUtf8ToWstr dirW(dir);

    WIN32_FILE_ATTRIBUTE_DATA fileInfo;
    BOOL res = GetFileAttributesEx(dirW, GetFileExInfoStandard, &fileInfo);
    if (0 == res) {
        return false;
    }

    if (fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        return true;
    }
    return false;
}

// Return true if a directory already exists or has been successfully created
bool Create(const char *dir) {
    AutoUtf8ToWstr dirW(dir);
    BOOL ok = CreateDirectoryW(dirW.Get(), NULL);
    if (ok)
        return true;
    return ERROR_ALREADY_EXISTS == GetLastError();
}

// creates a directory and all its parent directories that don't exist yet
bool CreateAll(const char *dir) {
    ScopedMem<char> parent(path::GetDir(dir));
    if (parent != dir && !Exists(parent)) {
        CreateAll(parent);
    }
    return Create(dir);
}

bool CreateForFile(const char *path) {
    ScopedMem<char> dir(path::GetDir(path));
    return dir::CreateAll(dir);
}

} // namespace dir
