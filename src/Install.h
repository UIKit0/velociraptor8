
// TODO: a better place for it (WinUtil.h ?)
std::string GetLocalAppDir();

// TODO: move to PathUtil.h
void NormalizePathInPlace(WCHAR *src, size_t srcCchSize);

std::string GetInstallationBinDir();
std::string GetInstallationBaseDir();

bool IsRunningInstalled();
bool IsRunningPortable();
