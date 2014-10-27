
#pragma warning(push)
#pragma warning(disable: 6011) // silence /analyze: de-referencing a NULL pointer
inline void CrashMe() {
    char *s = nullptr;
    *s = 0;
};
#pragma warning(pop)

#define CrashAlwaysIf(cond) \
    do { if (cond) \
        CrashMe(); \
    __analysis_assume(!(cond)); } while (0)

#if defined(DEBUG) || defined(PRE_RELEASE)
#define CrashIf(cond) CrashAlwaysIf(cond)
#else
#define CrashIf(cond) __analysis_assume(!(cond))
#endif

template <typename T>
T* AllocMust() {
    auto res = (T*) calloc(1, sizeof(T));
    CrashAlwaysIf(!res);
    return res;
}

template <typename T>
T* AllocMustN(size_t n) {
    auto res = (T*) calloc(n, sizeof(T));
    CrashAlwaysIf(!res);
    return res;
}

class AutoWStr {
    WCHAR *s;
public:
    AutoWStr(size_t n) {
        s = AllocMustN<WCHAR>(n);
    }
    ~AutoWStr() {
        free(s);
    }
    WCHAR *Get() {
        return s;
    }
    operator WCHAR*() const {
        return s;
    }
};

class AutoStr {
    char *s;
public:
    AutoStr(size_t n) {
        s = AllocMustN<char>(n);
    }
    ~AutoStr() {
        free(s);
    }
    char *Get() const {
        return s;
    }
    operator char*() const {
        return s;
    }
};

template <typename T>
class AutoStruct {
    T *val;
public:
    AutoStruct() {
        val = AllocMust<T>();
    }
    ~AutoStruct() {
        free(val);
    }
    T *Get() const {
        return val;
    }
    operator T*() const {
        return val;
    }
};

