
#include "Common.h"
#include <atlcomcli.h>

#include "WinUtil.h"
#include "FileUtil.h"

static WCHAR* knownCursorIds[] = { IDC_ARROW,  IDC_IBEAM,  IDC_HAND, IDC_SIZEALL,
                                   IDC_SIZEWE, IDC_SIZENS, IDC_NO,   IDC_CROSS };

static HCURSOR cachedCursors[dimof(knownCursorIds)] = {};

HCURSOR GetCursor(WCHAR* id) {
    int cursorIdx = -1;
    for (int i = 0; i < dimof(knownCursorIds); i++) {
        if (id == knownCursorIds[i]) {
            cursorIdx = i;
            break;
        }
    }
    CrashIf(cursorIdx == -1);
    if (NULL == cachedCursors[cursorIdx]) {
        cachedCursors[cursorIdx] = LoadCursor(NULL, id);
        CrashIf(cachedCursors[cursorIdx] == NULL);
    }
    return cachedCursors[cursorIdx];
}

void SetCursor(WCHAR* id) { SetCursor(GetCursor(id)); }

void FillWndClassEx(WNDCLASSEX& wcex, const WCHAR* clsName, WNDPROC wndproc) {
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.hInstance = GetModuleHandleW(NULL);
    wcex.hCursor = GetCursor(IDC_ARROW);
    wcex.lpszClassName = clsName;
    wcex.lpfnWndProc = wndproc;
}

void EnableMenu(HMENU m, UINT id) {
    BOOL ret = EnableMenuItem(m, id, MF_BYCOMMAND | MF_ENABLED);
    CrashIf(ret == -1);
}

void DisableMenu(HMENU m, UINT id) {
    BOOL ret = EnableMenuItem(m, id, MF_BYCOMMAND | MF_GRAYED);
    CrashIf(ret == -1);
}

bool CreateShortcut(const std::string& shortcutPath, const std::string& exePath) {
    // TODO(kjk): expose args, description, iconIndex
    const WCHAR *args = NULL;
    const WCHAR *description = NULL;
    int iconIndex = 0;

    AutoUtf8ToWstr exePathW(exePath);
    AutoUtf8ToWstr shortcutPathW(shortcutPath);

    CComPtr<IShellLink> lnk;
    HRESULT hr = lnk.CoCreateInstance(CLSID_ShellLink);
    if (FAILED(hr)) {
        return false;
    }

    CComQIPtr<IPersistFile> file(lnk);
    if (!file) {
        return false;
    }

    hr = lnk->SetPath(exePathW.Get());
    if (FAILED(hr))
        return false;

    auto dir(path::GetDir(exePath));
    AutoUtf8ToWstr dirW(dir);
    lnk->SetWorkingDirectory(dirW.Get());
    // lnk->SetShowCmd(SW_SHOWNORMAL);
    // lnk->SetHotkey(0);
    lnk->SetIconLocation(exePathW, iconIndex);
    if (args)
        lnk->SetArguments(args);
    if (description)
        lnk->SetDescription(description);

    hr = file->Save(shortcutPathW, TRUE);
    return SUCCEEDED(hr);
}

bool WriteRegStr(HKEY keySub, const char *keyName, const char *valName, const char *value) {
    AutoUtf8ToWstr keyNameW(keyName);
    AutoUtf8ToWstr valNameW(valName);
    AutoUtf8ToWstr valueW(value);
    LSTATUS res = SHSetValueW(keySub, keyNameW, valNameW, REG_SZ, (const void *) valueW, (DWORD) (str::Len(valueW) + 1) * sizeof(WCHAR));
    return ERROR_SUCCESS == res;
}

bool ReadRegDWORD(HKEY keySub, const char *keyName, const char *valName, DWORD& value) {
    AutoUtf8ToWstr keyNameW(keyName);
    AutoUtf8ToWstr valNameW(valName);
    DWORD size = sizeof(DWORD);
    LSTATUS res = SHGetValueW(keySub, keyNameW, valNameW, NULL, &value, &size);
    return ERROR_SUCCESS == res && sizeof(DWORD) == size;
}

bool WriteRegDWORD(HKEY keySub, const char *keyName, const char *valName, DWORD value) {
    AutoUtf8ToWstr keyNameW(keyName);
    AutoUtf8ToWstr valNameW(valName);
    LSTATUS res = SHSetValueW(keySub, keyNameW, valNameW, REG_DWORD, (const VOID *) &value, sizeof(DWORD));
    return ERROR_SUCCESS == res;
}

bool CreateRegKey(HKEY keySub, const char *keyName) {
    HKEY hKey;
    AutoUtf8ToWstr keyNameW(keyName);
    if (RegCreateKeyExW(keySub, keyNameW, 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL) != ERROR_SUCCESS) {
        return false;
    }
    RegCloseKey(hKey);
    return true;
}
