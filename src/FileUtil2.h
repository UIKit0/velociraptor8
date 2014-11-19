namespace path {

char* GetDir(const char*);
void NormalizeInPlace(WCHAR* src, size_t srcCchSize);

} // namespace path

namespace file {

bool Exists(const char*);
bool Copy(const char* dst, const char *src);
bool Delete(const char *);

} // namespace file

namespace dir {

bool Exists(const char*);
bool Create(const char*);
bool CreateAll(const char*);
bool CreateForFile(const char*);
bool Delete(const char*);

} // namespace dir
