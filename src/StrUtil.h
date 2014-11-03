
namespace str {

size_t Len(const char *s);
size_t Len(const WCHAR *s);
bool Eq(const char *s1, const char *s2);
bool Eq(const WCHAR *s1, const WCHAR *s2);
char LastChar(const std::string& s);
bool EndsWith(const std::string &s, char c);
bool StartsWith(const std::string &s, char c);
std::string FmtV(const char *fmt, va_list args);
std::string Format(const char *fmt, ...);

} // namespace str

char *WstrToUtf8Buf(const WCHAR *s, char *buf, size_t bufCbLen);
WCHAR *Utf8ToWstrBuf(const char *s, WCHAR *buf, size_t bufCchLen);
WCHAR *Utf8ToWstr(const char *s);

// TODO: allow optimizing by specifying size of s
std::string WstrToUtf8Str(const WCHAR *s);

class AutoWstrToUtf8 {
    char buf[256];
    char *res;

  public:
    AutoWstrToUtf8(const WCHAR *s) { res = WstrToUtf8Buf(s, buf, dimof(buf)); }
    ~AutoWstrToUtf8() {
        if (res != buf)
            free(res);
    }
    char *Get() const { return res; }
    operator char *() const { return res; }
};

class AutoUtf8ToWstr {
    WCHAR buf[256];
    WCHAR *res;

  public:
    AutoUtf8ToWstr(const char *s) { res = Utf8ToWstrBuf(s, buf, dimof(buf)); }
    // Maybe: optimize-out strlen
    AutoUtf8ToWstr(const std::string& s) { res = Utf8ToWstrBuf(s.c_str(), buf, dimof(buf)); }
    ~AutoUtf8ToWstr() {
        if (res != buf)
            free(res);
    }
    WCHAR *Get() const { return res; }
    operator WCHAR *() const { return res; }
};
