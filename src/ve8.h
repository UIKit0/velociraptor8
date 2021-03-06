/*
Global definitions and declarations
See Readme.txt for more information about this source code.
Please send me your comments to this work.
*/

#define SETTINGS_FILE_NAME L"ve8.ini"
#define APP_NAME "Velociraptor8"
// when installed our files are under %LOCALAPPDATA%\APP_DIR_NAME
#define APP_DIR_NAME "ve8"
#define BIN_DIR_NAME "bin"
#define EXE_NAME "ve8.exe"

//==== Data Type for WM_COPYDATA ==============================================
#define DATA_NOTEPAD2_PARAMS 0xFB10
struct VeParams {
    int flagFileSpecified;
    int flagChangeNotify;
    int flagLexerSpecified;
    int iInitialLexer;
    int flagQuietCreate;
    int flagJumpTo;
    int iInitialLine;
    int iInitialColumn;
    int iSrcEncoding;
    int flagSetEncoding;
    int flagSetEOLMode;
    int flagTitleExcerpt;
    WCHAR wchData;
};

struct Document {
    // Note: eventually multiple Document instances will be associated with the
    // same hwndTopLevel (tabs!)
    HWND hwndTopLevel;
    HWND hwndScintilla;
    HWND hwndEditFrame;
    HWND hwndStatus;
    HWND hwndToolbar;
    HWND hwndReBar;
};

extern Document *gDoc;

//==== Toolbar Style ==========================================================
#define WS_TOOLBAR                                                                                 \
    (WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | TBSTYLE_TOOLTIPS | TBSTYLE_FLAT | TBSTYLE_ALTDRAG | \
     TBSTYLE_LIST | CCS_NODIVIDER | CCS_NOPARENTALIGN | CCS_ADJUSTABLE)

//==== ReBar Style ============================================================
#define WS_REBAR                                                                                   \
    (WS_CHILD | WS_CLIPCHILDREN | WS_BORDER | RBS_VARHEIGHT | RBS_BANDBORDERS | CCS_NODIVIDER |    \
     CCS_NOPARENTALIGN)

//==== Ids ====================================================================
#define IDC_STATUSBAR 0xFB00
#define IDC_TOOLBAR 0xFB01
#define IDC_REBAR 0xFB02
#define IDC_EDIT 0xFB03
#define IDC_EDITFRAME 0xFB04
#define IDC_FILENAME 0xFB05
#define IDC_REUSELOCK 0xFB06

//==== Statusbar ==============================================================
#define STATUS_DOCPOS 0
#define STATUS_DOCSIZE 1
#define STATUS_CODEPAGE 2
#define STATUS_EOLMODE 3
#define STATUS_OVRMODE 4
#define STATUS_LEXER 5
#define STATUS_HELP 255

//==== Change Notifications ===================================================
#define ID_WATCHTIMER 0xA000
#define WM_CHANGENOTIFY WM_USER + 1
//#define WM_CHANGENOTIFYCLEAR WM_USER+2

//==== Callback Message from System Tray ======================================
#define WM_TRAYMESSAGE WM_USER

//==== Paste Board Timer ======================================================
#define ID_PASTEBOARDTIMER 0xA001

//==== Reuse Window Lock Timeout ==============================================
#define REUSEWINDOWLOCKTIMEOUT 1000

//==== Function Declarations ==================================================
BOOL InitApplication(HINSTANCE);
HWND InitInstance(HINSTANCE, LPSTR, int);
BOOL ActivatePrevInst();
BOOL RelaunchMultiInst();
BOOL RelaunchElevated();
void SnapToDefaultPos(HWND);
void ShowNotifyIcon(HWND, BOOL);
void SetNotifyIconTitle(HWND);
void InstallFileWatching(const WCHAR *);
void CALLBACK WatchTimerProc(HWND, UINT, UINT_PTR, DWORD);
void CALLBACK PasteBoardTimer(HWND, UINT, UINT_PTR, DWORD);

void LoadSettings();
void SaveSettings(BOOL);
void ParseCommandLine();
void LoadFlags();
int CheckIniFile(WCHAR *, const WCHAR *);
int CheckIniFileRedirect(WCHAR *, const WCHAR *);
int FindIniFile();
int TestIniFile();
int CreateIniFile();
int CreateIniFileEx(const WCHAR *);

void UpdateStatusbar();
void UpdateToolbar();
void UpdateLineNumberWidth();

BOOL FileIO(BOOL, const WCHAR *, BOOL, int *, int *, BOOL *, BOOL *, BOOL *, BOOL);
BOOL FileLoad(BOOL, BOOL, BOOL, BOOL, const WCHAR *);
BOOL FileSave(BOOL, BOOL, BOOL, BOOL);
BOOL OpenFileDlg(HWND, WCHAR *, int, const WCHAR *);
BOOL SaveFileDlg(HWND, WCHAR *, int, const WCHAR *);

LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
void CreateBars(HWND, HINSTANCE);
void MsgInitMenu(HWND, WPARAM, LPARAM);
LRESULT MsgCommand(HWND, WPARAM, LPARAM);
LRESULT MsgNotify(HWND, WPARAM, LPARAM);
