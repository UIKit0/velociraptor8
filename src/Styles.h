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
    char *pszName;
    char *pszDefault;
    char szValue[128];
} EDITSTYLE, *PEDITSTYLE;

struct KeywordList {
    char *pszKeyWords[9];
};

struct EditLexer {
    int iLexer;
    int rid;
    char *pszName;
    char *pszDefExt;
    WCHAR szExtensions[128];
    KeywordList *pKeyWords;
    EDITSTYLE Styles[];
};

// Number of Lexers in pLexArray
#define NUMLEXERS 37

void Style_Load();
void Style_Save();
BOOL Style_Import(HWND);
BOOL Style_Export(HWND);
void Style_SetLexer(HWND, EditLexer *);
void Style_SetLongLineColors(HWND);
void Style_SetCurrentLineBackground(HWND);
void Style_SetLexerFromFile(HWND, const WCHAR *);
void Style_SetLexerFromName(HWND, const WCHAR *, const WCHAR *);
void Style_SetDefaultLexer(HWND);
void Style_SetHTMLLexer(HWND);
void Style_SetXMLLexer(HWND);
void Style_SetLexerFromID(HWND, int);
void Style_SetDefaultFont(HWND);
void Style_ToggleUse2ndDefault(HWND);
BOOL Style_GetUse2ndDefault(HWND);
void Style_SetIndentGuides(HWND, BOOL);
BOOL Style_GetOpenDlgFilterStr(WCHAR *, int);
BOOL Style_StrGetFont(const WCHAR *, WCHAR *, int);
BOOL Style_StrGetFontQuality(const WCHAR *, WCHAR *, int);
BOOL Style_StrGetCharSet(const WCHAR *, int *);
BOOL Style_StrGetSizeStr(const WCHAR *, WCHAR *, int);
BOOL Style_StrGetCase(const WCHAR *, int *);
BOOL Style_SelectFont(HWND, WCHAR *, int, BOOL);
BOOL Style_SelectColor(HWND, BOOL, WCHAR *, int);
void Style_SetFontQuality(HWND, const WCHAR *);
void Style_GetCurrentLexerName(WCHAR *, int);
int Style_GetLexerIconId(EditLexer *);
HTREEITEM Style_AddLexerToTreeView(HWND, EditLexer *);
INT_PTR CALLBACK Styles_ConfigDlgProc(HWND, UINT, WPARAM, LPARAM);
void Style_ConfigDlg(HWND);
INT_PTR CALLBACK Style_SelectLexerDlgProc(HWND, UINT, WPARAM, LPARAM);
void Style_SelectLexerDlg(HWND);
