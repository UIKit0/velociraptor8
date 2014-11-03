
HCURSOR GetCursor(WCHAR*);
void SetCursor(WCHAR*);
void FillWndClassEx(WNDCLASSEX& wcex, const WCHAR* clsName, WNDPROC wndproc);

void EnableMenu(HMENU m, UINT id);
void DisableMenu(HMENU m, UINT id);
bool CreateShortcut(const std::string& shortcutPath, const std::string& exePath);

