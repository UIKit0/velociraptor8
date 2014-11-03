/*
Definitions for general helper functions and macros
See Readme.txt for more information about this source code.
See License.txt for details about distribution and modification.
*/

extern HINSTANCE g_hInstance;
extern UINT16 g_uWinVer;

extern WCHAR szIniFile[MAX_PATH];
#define IniGetString(lpSection, lpName, lpDefault, lpReturnedStr, nSize)                           \
    GetPrivateProfileString(lpSection, lpName, lpDefault, lpReturnedStr, nSize, szIniFile)
#define IniGetInt(lpSection, lpName, nDefault)                                                     \
    GetPrivateProfileInt(lpSection, lpName, nDefault, szIniFile)
#define IniSetString(lpSection, lpName, lpString)                                                  \
    WritePrivateProfileString(lpSection, lpName, lpString, szIniFile)
#define IniDeleteSection(lpSection) WritePrivateProfileSection(lpSection, NULL, szIniFile)
__inline BOOL IniSetInt(const WCHAR *lpSection, const WCHAR *lpName, int i) {
    WCHAR tch[32];
    wsprintf(tch, L"%i", i);
    return IniSetString(lpSection, lpName, tch);
}
#define LoadIniSection(lpSection, lpBuf, cchBuf)                                                   \
    GetPrivateProfileSection(lpSection, lpBuf, cchBuf, szIniFile);
#define SaveIniSection(lpSection, lpBuf) WritePrivateProfileSection(lpSection, lpBuf, szIniFile)
int IniSectionGetString(const WCHAR *, const WCHAR *, const WCHAR *, WCHAR *, int);
int IniSectionGetInt(const WCHAR *, const WCHAR *, int);
BOOL IniSectionSetString(WCHAR *, const WCHAR *, const WCHAR *);

inline BOOL IniSectionSetInt(WCHAR *lpCachedIniSection, const WCHAR *lpName, int i) {
    WCHAR tch[32];
    wsprintf(tch, L"%i", i);
    return IniSectionSetString(lpCachedIniSection, lpName, tch);
}

void BeginWaitCursor();
void EndWaitCursor();

#define Is2k() (g_uWinVer >= 0x0500)
#define IsXP() (g_uWinVer >= 0x0501)
#define IsVista() (g_uWinVer >= 0x0600)
#define IsW7() (g_uWinVer >= 0x0601)

BOOL PrivateIsAppThemed();
HRESULT PrivateSetCurrentProcessExplicitAppUserModelID(PCWSTR);
BOOL IsElevated();
// BOOL SetExplorerTheme(HWND);

BOOL BitmapMergeAlpha(HBITMAP, COLORREF);
BOOL BitmapAlphaBlend(HBITMAP, COLORREF, BYTE);
BOOL BitmapGrayScale(HBITMAP);
BOOL VerifyContrast(COLORREF, COLORREF);
BOOL IsFontAvailable(const WCHAR *);

BOOL SetWindowTitle(HWND, UINT, BOOL, UINT, const WCHAR *, int, BOOL, UINT, BOOL, const WCHAR *);
void SetWindowTransparentMode(HWND, BOOL);

void CenterDlgInParent(HWND);
void GetDlgPos(HWND, LPINT, LPINT);
void SetDlgPos(HWND, int, int);
void ResizeDlg_Init(HWND, int, int, int);
void ResizeDlg_Destroy(HWND, int *, int *);
void ResizeDlg_Size(HWND, LPARAM, int *, int *);
void ResizeDlg_GetMinMaxInfo(HWND, LPARAM);
HDWP DeferCtlPos(HDWP, HWND, int, int, int, UINT);
void MakeBitmapButton(HWND, int, HINSTANCE, UINT);
void MakeColorPickButton(HWND, int, HINSTANCE, COLORREF);
void DeleteBitmapButton(HWND, int);

#define StatusSetSimple(hwnd, b) SendMessage(hwnd, SB_SIMPLE, (WPARAM)b, 0)
BOOL StatusSetText(HWND, UINT, const WCHAR *);
BOOL StatusSetTextID(HWND, UINT, UINT);
int StatusCalcPaneWidth(HWND, const WCHAR *);

int Toolbar_GetButtons(HWND, int, WCHAR *, int);
int Toolbar_SetButtons(HWND, int, const WCHAR *, const TBBUTTON *, int);

LRESULT SendWMSize(HWND);

#define EnableCmd(hmenu, id, b)                                                                    \
    EnableMenuItem(hmenu, id, (b) ? MF_BYCOMMAND | MF_ENABLED : MF_BYCOMMAND | MF_GRAYED)

#define CheckCmd(hmenu, id, b)                                                                     \
    CheckMenuItem(hmenu, id, (b) ? MF_BYCOMMAND | MF_CHECKED : MF_BYCOMMAND | MF_UNCHECKED)

BOOL IsCmdEnabled(HWND, UINT);

#define GetString(id, buf, cchBufMax) LoadStringW(g_hInstance, id, buf, cchBufMax)

/*
inline int GetString(UINT id, WCHAR *buf, int cchBufMax) {
    return LoadStringW(g_hInstance, id, buf, cchBufMax);
}
*/

#define StrEnd(pStart) (pStart + lstrlen(pStart))

int FormatString(WCHAR *, int, UINT, ...);

void PathRelativeToApp(WCHAR *, WCHAR *, int, BOOL, BOOL, BOOL);
void PathAbsoluteFromApp(WCHAR *, WCHAR *, int, BOOL);

BOOL PathIsLnkFile(const WCHAR *);
BOOL PathGetLnkPath(const WCHAR *, WCHAR *, int);
BOOL PathIsLnkToDirectory(const WCHAR *, WCHAR *, int);
BOOL PathCreateDeskLnk(const WCHAR *);
BOOL PathCreateFavLnk(const WCHAR *, const WCHAR *, const WCHAR *);

BOOL StrLTrim(WCHAR *, const WCHAR *);
BOOL TrimString(WCHAR *);
BOOL ExtractFirstArgument(const WCHAR *, WCHAR *, WCHAR *);

void PrepareFilterStr(WCHAR *);

void StrTab2Space(WCHAR *);
void PathFixBackslashes(WCHAR *);

void ExpandEnvironmentStringsEx(WCHAR *, DWORD);
void PathCanonicalizeEx(WCHAR *);
DWORD GetLongPathNameEx(WCHAR *, DWORD);
DWORD_PTR SHGetFileInfo2(const WCHAR *, DWORD, SHFILEINFO *, UINT, UINT);

int FormatNumberStr(WCHAR *);
BOOL SetDlgItemIntEx(HWND, int, UINT);

#define MBCSToWChar(c, a, w, i) MultiByteToWideChar(c, 0, a, -1, w, i)
#define WCharToMBCS(c, w, a, i) WideCharToMultiByte(c, 0, w, -1, a, i, NULL, NULL)

UINT GetDlgItemTextA2W(UINT, HWND, int, LPSTR, int);
UINT SetDlgItemTextA2W(UINT, HWND, int, LPSTR);
LRESULT ComboBox_AddStringA2W(UINT, HWND, LPCSTR);

UINT CodePageFromCharSet(UINT);

//==== MRU Functions ==========================================================
#define MRU_MAXITEMS 24
#define MRU_NOCASE 1
#define MRU_UTF8 2

typedef struct _mrulist {

    WCHAR szRegKey[256];
    int iFlags;
    int iSize;
    WCHAR *pszItems[MRU_MAXITEMS];

} MRULIST, *PMRULIST, *LPMRULIST;

LPMRULIST MRU_Create(const WCHAR *, int, int);
BOOL MRU_Destroy(LPMRULIST);
BOOL MRU_Add(LPMRULIST, const WCHAR *);
BOOL MRU_AddFile(LPMRULIST, const WCHAR *, BOOL, BOOL);
BOOL MRU_Delete(LPMRULIST, int);
BOOL MRU_DeleteFileFromStore(LPMRULIST, const WCHAR *);
BOOL MRU_Empty(LPMRULIST);
int MRU_Enum(LPMRULIST, int, WCHAR *, int);
BOOL MRU_Load(LPMRULIST);
BOOL MRU_Save(LPMRULIST);
BOOL MRU_MergeSave(LPMRULIST, BOOL, BOOL, BOOL);

//==== Themed Dialogs =========================================================
#ifndef DLGTEMPLATEEX
#pragma pack(push, 1)
typedef struct {
    WORD dlgVer;
    WORD signature;
    DWORD helpID;
    DWORD exStyle;
    DWORD style;
    WORD cDlgItems;
    short x;
    short y;
    short cx;
    short cy;
} DLGTEMPLATEEX;
#pragma pack(pop)
#endif

BOOL GetThemedDialogFont(WCHAR *, WORD *);
DLGTEMPLATE *LoadThemedDialogTemplate(LPCTSTR, HINSTANCE);
#define ThemedDialogBox(hInstance, lpTemplate, hWndParent, lpDialogFunc)                           \
    ThemedDialogBoxParam(hInstance, lpTemplate, hWndParent, lpDialogFunc, 0)
INT_PTR ThemedDialogBoxParam(HINSTANCE, LPCTSTR, HWND, DLGPROC, LPARAM);
HWND CreateThemedDialogParam(HINSTANCE, LPCTSTR, HWND, DLGPROC, LPARAM);

//==== UnSlash Functions ======================================================
void TransformBackslashes(char *, BOOL, UINT);

//==== MinimizeToTray Functions - see comments in Helpers.c ===================
BOOL GetDoAnimateMinimize(VOID);
VOID MinimizeWndToTray(HWND hWnd);
VOID RestoreWndFromTray(HWND hWnd);
