namespace path {

bool IsSep(char c);
void Join(std::string& sInOut, const char *s, size_t sLen = 0);
void Join(std::string& sInOut, const std::string& s2);
std::string GetDir(const std::string&);
void NormalizeInPlace(WCHAR *src, size_t srcCchSize);

} // namespace path

namespace file {

bool Exists(const std::string&);
bool Copy(const std::string& dst, const std::string& src);

} // namespace file

namespace dir {

bool Exists(const std::string&);
bool Create(const std::string&);
bool CreateAll(const std::string&);
bool CreateForFile(const std::string&);
bool Delete(const std::string&);

} // namespace dir

