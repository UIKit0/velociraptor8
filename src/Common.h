#include "BaseUtil.h"

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#undef GetRValue
#define GetRValue GetRValueSafe
#undef GetGValue
#define GetGValue GetGValueSafe
#undef GetBValue
#define GetBValue GetBValueSafe

#include <commdlg.h>
#include <shellapi.h>
#include <uxtheme.h>

#include <memory>
#include <vector>

typedef uint16_t u16;

#define CSTRLEN(s) (dimof(s) - 1)

inline void CrashIfLastError() {
    DWORD err = GetLastError();
    CrashIf(err != ERROR_SUCCESS);
}

template <typename T> T *AllocMust() {
    auto res = (T *)calloc(1, sizeof(T));
    CrashAlwaysIf(!res);
    return res;
}

template <typename T> T *AllocMustN(size_t n) {
    auto res = (T *)calloc(n, sizeof(T));
    CrashAlwaysIf(!res);
    return res;
}

class AutoWStr {
    WCHAR *s;

  public:
    AutoWStr(size_t n) { s = AllocMustN<WCHAR>(n); }
    ~AutoWStr() { free(s); }
    WCHAR *Get() { return s; }
    operator WCHAR *() const { return s; }
};

class AutoStr {
    char *s;

  public:
    AutoStr(size_t n) { s = AllocMustN<char>(n); }
    ~AutoStr() { free(s); }
    char *Get() const { return s; }
    operator char *() const { return s; }
};

template <typename T> class AutoStruct {
    T *val;

  public:
    AutoStruct() { val = AllocMust<T>(); }
    ~AutoStruct() { free(val); }
    T *Get() const { return val; }
    operator T *() const { return val; }
};

#include "StrUtil2.h"