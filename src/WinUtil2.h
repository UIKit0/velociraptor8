
void EnableMenu(HMENU m, UINT id);
void DisableMenu(HMENU m, UINT id);
bool CreateShortcut(const std::string &shortcutPath, const std::string &exePath);

bool CreateRegKey(HKEY keySub, const char *keyName);
bool WriteRegDWORD(HKEY keySub, const char *keyName, const char *valName, DWORD value);
bool ReadRegDWORD(HKEY keySub, const char *keyName, const char *valName, DWORD &value);
bool ReadRegStr(HKEY keySub, const std::string &keyName, const std::string &valName,
                std::string &valOut);
bool WriteRegExpandStr(HKEY keySub, const char *keyName, const char *valName, const char *value);
bool WriteRegStr(HKEY keySub, const char *keyName, const char *valName, const char *value);
void BroadcastEnvRegistryChanged();

BOOL SetWindowPos(HWND hwnd, const RECT &r, UINT flags);
HDWP DeferWindowPos(HDWP hdwp, HWND hwnd, const RECT &r, UINT flags);

void SetDy(RECT &r, int dy);
void SetDx(RECT &r, int dx);

void RectInflate(RECT &r, int dx, int dy);

void RectSplitX(const RECT &r, int dx, RECT &rLeft, RECT &rRight);
void RectSplitY(const RECT &r, int dy, RECT &rTop, RECT &rBottom);
