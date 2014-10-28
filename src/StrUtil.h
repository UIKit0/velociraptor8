
namespace str {

size_t Len(const char *s);
size_t Len(const WCHAR *s);

}

char *WstrToUtf8Buf(const WCHAR *s, char *buf, size_t bufLen);

class AutoWstrToUtf8 {
    char buf[256];
    char *res;
public:
    AutoWstrToUtf8(const WCHAR *s) { res = WstrToUtf8Buf(s, buf, dimof(buf)); }
    ~AutoWstrToUtf8() { if (res != buf) free(res); }
    char *Get() const { return res; }
    operator char*() const { return res; }
};

