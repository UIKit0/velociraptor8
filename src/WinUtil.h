
HCURSOR GetCursor(WCHAR*);
void SetCursor(WCHAR*);
void FillWndClassEx(WNDCLASSEX& wcex, const WCHAR* clsName, WNDPROC wndproc);

void EnableMenu(HMENU m, UINT id);
void DisableMenu(HMENU m, UINT id);
bool CreateShortcut(const std::string& shortcutPath, const std::string& exePath);

bool CreateRegKey(HKEY keySub, const char *keyName);
bool WriteRegDWORD(HKEY keySub, const char *keyName, const char *valName, DWORD value);
bool ReadRegDWORD(HKEY keySub, const char *keyName, const char *valName, DWORD& value);
bool ReadRegStr(HKEY keySub, const std::string& keyName, const std::string& valName, std::string& valOut);
bool WriteRegExpandStr(HKEY keySub, const char *keyName, const char *valName, const char *value);
bool WriteRegStr(HKEY keySub, const char *keyName, const char *valName, const char *value);
void BroadcastEnvRegistryChanged();

