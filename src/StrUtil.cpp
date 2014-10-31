
#include "Common.h"

namespace str {

size_t Len(const char *s) { return strlen(s); }
size_t Len(const WCHAR *s) { return wcslen(s); }

}

// returns either bufOut or newly allocated 
char *WstrToUtf8Buf(const WCHAR *s, char *bufOut, size_t cbBufOutSize) {
    CrashIf(!bufOut || (0 == cbBufOutSize));
    size_t cbNeeds = (size_t)WideCharToMultiByte(CP_UTF8, 0, s, -1, NULL, 0, NULL, NULL);
    if (cbNeeds >= cbBufOutSize) {
        bufOut = AllocMustN<char>(cbNeeds + 1);
    }
    size_t res = (size_t)WideCharToMultiByte(CP_UTF8, 0, s, (int) str::Len(s), bufOut, (int)cbNeeds, NULL, NULL);
    CrashIf(res > cbNeeds);
    bufOut[res] = '\0';
    return bufOut;
}

WCHAR *Utf8ToWstrBuf(const char *s, WCHAR *bufOut, size_t cchBufOutSize) {
    size_t cchNeeds = (size_t) MultiByteToWideChar(CP_UTF8, 0, s,-1, NULL, 0);
    if (cchNeeds >= cchBufOutSize) {
        bufOut = AllocMustN<WCHAR>(cchNeeds + 1);
    }
    size_t res = (size_t) MultiByteToWideChar(CP_UTF8, 0, s, -1, bufOut, cchNeeds);
    CrashIf(res > cchNeeds);
    bufOut[res] = 0;
    return bufOut;
}

WCHAR *Utf8ToWstr(const char *s) {
    return Utf8ToWstrBuf(s, NULL, 0);
}
