
#include "Common.h"

// returns either bufOut or newly allocated
char *WstrToUtf8Buf(const WCHAR *s, char *bufOut, size_t cbBufOutSize) {
    CrashIf(!bufOut || (0 == cbBufOutSize));
    size_t cbNeeds = (size_t)WideCharToMultiByte(CP_UTF8, 0, s, -1, NULL, 0, NULL, NULL);
    if (cbNeeds >= cbBufOutSize) {
        bufOut = AllocMustN<char>(cbNeeds + 1);
    }
    size_t res = (size_t)WideCharToMultiByte(CP_UTF8, 0, s, (int)str::Len(s), bufOut, (int)cbNeeds,
                                             NULL, NULL);
    CrashIf(res > cbNeeds);
    bufOut[res] = '\0';
    return bufOut;
}

WCHAR *Utf8ToWstrBuf(const char *s, WCHAR *bufOut, size_t cchBufOutSize) {
    size_t cchNeeds = (size_t)MultiByteToWideChar(CP_UTF8, 0, s, -1, NULL, 0);
    if (cchNeeds >= cchBufOutSize) {
        bufOut = AllocMustN<WCHAR>(cchNeeds + 1);
    }
    size_t res = (size_t)MultiByteToWideChar(CP_UTF8, 0, s, -1, bufOut, cchNeeds);
    CrashIf(res > cchNeeds);
    bufOut[res] = 0;
    return bufOut;
}

WCHAR *Utf8ToWstr(const char *s) { return Utf8ToWstrBuf(s, NULL, 0); }

std::string WstrToUtf8Str(const WCHAR *s) {
    AutoWstrToUtf8 sA(s);
    char *cstr = sA.Get();
    return std::string(cstr);
}

namespace str {

int Len(Slice &s) {
    if (s.len <= 0) {
        s.len = (int)str::Len(s.s);
    }
    return s.len;
}

char *DupN(const char *s, int n) {
    char *res = (char *)calloc(n + 1, 1);
    memcpy(res, s, (size_t)n);
    return res;
}

char LastChar(const std::string &s) {
    if (s.empty()) {
        return 0;
    }
    return s.at(s.size() - 1);
}

bool EndsWith(const std::string &s, char c) { return c == LastChar(s); }

bool StartsWith(const std::string &s, char c) {
    if (s.empty()) {
        return false;
    }
    char first = s.at(0);
    return first == c;
}

size_t FindIPos(const char *s, const char *toFind) {
    const char *pos = FindI(s, toFind);
    if (pos == nullptr) {
        return std::string::npos;
    }
    return pos - s;
}

bool ContainsI(const std::string &s, const std::string &toFind) {
    const char *found = FindI(s.c_str(), toFind.c_str());
    return found != nullptr;
}

std::string StrFmtV(const char *fmt, va_list args) {
    char message[256];
    size_t bufCchSize = dimof(message);
    char *buf = message;
    for (;;) {
        int count = _vsnprintf_s(buf, bufCchSize, _TRUNCATE, fmt, args);
        if ((count >= 0) && ((size_t)count < bufCchSize)) {
            break;
        }
        /* we have to make the buffer bigger. The algorithm used to calculate
        the new size is arbitrary (aka. educated guess) */
        if (buf != message) {
            free(buf);
        }
        if (bufCchSize < 4 * 1024)
            bufCchSize += bufCchSize;
        else
            bufCchSize += 1024;
        buf = AllocMustN<char>(bufCchSize);
        if (!buf)
            break;
    }

    auto res(buf);
    if (buf != message) {
        free(buf);
    }

    return res;
}

std::string StrFormat(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    auto res(FmtV(fmt, args));
    va_end(args);
    return res;
}

} // namespace str
