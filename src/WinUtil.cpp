#include "Common.h"
#include "WinUtil.h"

static LPWSTR knownCursorIds [] = {
    IDC_ARROW,
    IDC_IBEAM,
    IDC_HAND,
    IDC_SIZEALL,
    IDC_SIZEWE,
    IDC_SIZENS,
    IDC_NO,
    IDC_CROSS
};

static HCURSOR cachedCursors[dimof(knownCursorIds)] = {};

HCURSOR GetCursor(LPWSTR id) {
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

void SetCursor(LPWSTR id) {
    SetCursor(GetCursor(id));
}

void FillWndClassEx(WNDCLASSEX& wcex, const WCHAR *clsName, WNDPROC wndproc) {
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.hInstance = GetModuleHandleW(NULL);
    wcex.hCursor = GetCursor(IDC_ARROW);
    wcex.lpszClassName = clsName;
    wcex.lpfnWndProc = wndproc;
}
