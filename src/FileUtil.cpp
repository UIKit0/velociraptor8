#include "Common.h"
#include "FileUtil.h"

namespace path {

// http://msdn.microsoft.com/en-us/library/windows/desktop/bb773569(v=vs.85).aspx
// TODO: on win8 use PathCchCanonicalize or PathCchCanonicalizeEx
void NormalizeInPlace(WCHAR *src, size_t srcCchSize) {
    // TODO: make it work even if srcCchSize is < MAX_PATH
    CrashIf(srcCchSize < MAX_PATH);
    WCHAR buf[MAX_PATH];
    BOOL ok = PathCanonicalizeW(buf, src);
    if (!ok) {
        return;
    }
    memcpy(src, buf, sizeof(buf));
}

void Join(std::string& sInOut, const std::string& s2) {
    // TODO: support '/' as a separator
    if (!str::EndsWith(sInOut, '\\')) {
        sInOut.append(1, '\\');
    }
    const char* s2c = s2.c_str();
    size_t s2cLen = s2.size();
    if (*s2c == '\\') {
        s2c++;
        s2cLen--;
    }
    sInOut.append(s2c, s2cLen);
}

static bool IsSep(char c) {
    // little known fact: on windows both / and \ are valid although
    // \ is not as well supported
    return '\\' == c || '/' == c;
}

// consider using PathRemoveFileSpec()
// http://msdn.microsoft.com/en-us/library/windows/desktop/bb773748(v=vs.85).aspx
std::string GetDir(const std::string& path) {
    std::string res(path);
    if (path.empty()) {
        return res;
    }
    const char *start = res.c_str();
    const char *end = start + res.size() - 1;
    while (end >= start) {
        char c = *end;
        if (IsSep(c)) {
            size_t idx = end - start;
            res.erase(idx, res.size());
            return res;
        }
        --end;
    }
    return res;
}

} // namespace path

namespace file {

bool Exists(const std::string& path) {
    if (path.empty()) {
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

// Return true if the file wasn't there or was successfully deleted
bool Delete(const std::string& path) {
    AutoUtf8ToWstr pathW(path);
    BOOL ok = DeleteFile(pathW.Get());
    return ok || GetLastError() == ERROR_FILE_NOT_FOUND;
}

bool Copy(const std::string& dst, const std::string& src) {
    AutoUtf8ToWstr dstW(dst);
    AutoUtf8ToWstr srcW(src);
    BOOL failIfExists = TRUE;
    BOOL res = CopyFileW(srcW.Get(), dstW.Get(), failIfExists);
    return res != 0;
}

} // namespace file

namespace dir {

bool Exists(const std::string& dir) {
    if (dir.empty()) {
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
bool Create(const std::string& dir) {
    AutoUtf8ToWstr dirW(dir);
    BOOL ok = CreateDirectory(dirW.Get(), NULL);
    if (ok)
        return true;
    return ERROR_ALREADY_EXISTS == GetLastError();
}

// creates a directory and all its parent directories that don't exist yet
bool CreateAll(const std::string& dir) {
    std::string parent(path::GetDir(dir));
    if (parent != dir && !Exists(parent)) {
        CreateAll(parent);
    }
    return Create(dir);
}

bool CreateForFile(const std::string& path) {
    auto dir(path::GetDir(path));
    return dir::CreateAll(dir);
}

} // namespace dir

