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

} // namespace path
