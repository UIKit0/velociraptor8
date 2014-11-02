#include "Common.h"
#include "PathUtil.h"

namespace path {
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

bool CreateDirForFile(const std::string& path) {
    auto dir(path::GetDir(path));
    return false;
}

bool FileCopy(const std::string& dst, const std::string& src) {
    return false;
}

} // namespace path
