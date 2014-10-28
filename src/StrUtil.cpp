
#include "Common.h"

namespace str {

size_t Len(const char *s) { return strlen(s); }
size_t Len(const WCHAR *s) { return wcslen(s); }

}

// returns either bufOut or newly allocated 
char *WcharToUtf8Buf(const WCHAR *s, char *bufOut, size_t cbBufOutSize) {
    CrashIf(!bufOut || (0 == cbBufOutSize));
    size_t cbNeeds = (size_t)WideCharToMultiByte(CP_UTF8, 0, s, -1, NULL, 0, NULL, NULL);
    if (cbNeeds >= cbBufOutSize) {
        bufOut = AllocMustN<char>(cbNeeds + 1);
    }
    size_t res = (size_t)WideCharToMultiByte(CP_UTF8, 0, s, (int) str::Len(s), bufOut, cbNeeds, NULL, NULL);
    CrashIf(res > cbNeeds);
    bufOut[res] = '\0';
    return bufOut;
}

