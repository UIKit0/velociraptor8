/*
Scintilla Style Management
See Readme.txt for more information about this source code.
See License.txt for details about distribution and modification.
*/

typedef struct _editstyle {
    union {
        INT32 iStyle;
        UINT8 iStyle8[4];
    };
    int rid;
    WCHAR* pszName;
    WCHAR* pszDefault;
    WCHAR szValue[128];

} EDITSTYLE, *PEDITSTYLE;

typedef struct _keywordlist {
    char* pszKeyWords[9];

} KEYWORDLIST, *PKEYWORDLIST;

struct EDITLEXER {
    int iLexer;
    int rid;
    WCHAR* pszName;
    WCHAR* pszDefExt;
    WCHAR szExtensions[128];
    PKEYWORDLIST pKeyWords;
    EDITSTYLE Styles[];
};

// Number of Lexers in pLexArray
#define NUMLEXERS 37

void Style_Load();
void Style_Save();
BOOL Style_Import(HWND);
BOOL Style_Export(HWND);
void Style_SetLexer(HWND, EDITLEXER*);
void Style_SetLongLineColors(HWND);
void Style_SetCurrentLineBackground(HWND);
void Style_SetLexerFromFile(HWND, LPCWSTR);
void Style_SetLexerFromName(HWND, LPCWSTR, LPCWSTR);
void Style_SetDefaultLexer(HWND);
void Style_SetHTMLLexer(HWND);
void Style_SetXMLLexer(HWND);
void Style_SetLexerFromID(HWND, int);
void Style_SetDefaultFont(HWND);
void Style_ToggleUse2ndDefault(HWND);
BOOL Style_GetUse2ndDefault(HWND);
void Style_SetIndentGuides(HWND, BOOL);
BOOL Style_GetOpenDlgFilterStr(WCHAR *, int);
BOOL Style_StrGetFont(LPCWSTR, WCHAR *, int);
BOOL Style_StrGetFontQuality(LPCWSTR, WCHAR *, int);
BOOL Style_StrGetCharSet(LPCWSTR, int*);
BOOL Style_StrGetSize(LPCWSTR, int*);
BOOL Style_StrGetSizeStr(LPCWSTR, WCHAR *, int);
BOOL Style_StrGetColor(BOOL, LPCWSTR, int*);
BOOL Style_StrGetCase(LPCWSTR, int*);
BOOL Style_StrGetAlpha(LPCWSTR, int*);
BOOL Style_SelectFont(HWND, WCHAR *, int, BOOL);
BOOL Style_SelectColor(HWND, BOOL, WCHAR *, int);
void Style_SetStyles(HWND, int, LPCWSTR);
void Style_SetFontQuality(HWND, LPCWSTR);
void Style_GetCurrentLexerName(WCHAR *, int);
int Style_GetLexerIconId(EDITLEXER*);
HTREEITEM Style_AddLexerToTreeView(HWND, EDITLEXER*);
INT_PTR CALLBACK Styles_ConfigDlgProc(HWND, UINT, WPARAM, LPARAM);
void Style_ConfigDlg(HWND);
INT_PTR CALLBACK Style_SelectLexerDlgProc(HWND, UINT, WPARAM, LPARAM);
void Style_SelectLexerDlg(HWND);

