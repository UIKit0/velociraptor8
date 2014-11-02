namespace path {

void Join(std::string& sInOut, const std::string& s2);
std::string GetDir(const std::string&);
bool CreateDirForFile(const std::string&);
bool FileCopy(const std::string& dst, const std::string& src);

} // namespace path
