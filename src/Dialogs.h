/*
Definitions for Notepad2 dialog boxes
See Readme.txt for more information about this source code.
See License.txt for details about distribution and modification
*/

#define MBINFO 0
#define MBWARN 1
#define MBYESNO 2
#define MBYESNOWARN 3
#define MBYESNOCANCEL 4
#define MBOKCANCEL 8

int MsgBox(int, UINT, ...);
void DisplayCmdLineHelp();
BOOL GetDirectory(HWND, int, WCHAR *, const WCHAR *, BOOL);
INT_PTR CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);
void RunDlg(HWND, const WCHAR *);
BOOL OpenWithDlg(HWND, const WCHAR *);
BOOL FavoritesDlg(HWND, WCHAR *);
BOOL AddToFavDlg(HWND, const WCHAR *, const WCHAR *);
BOOL FileMRUDlg(HWND, WCHAR *);
BOOL ChangeNotifyDlg(HWND);
BOOL ColumnWrapDlg(HWND, UINT, int *);
BOOL WordWrapSettingsDlg(HWND, UINT, int *);
BOOL LongLineSettingsDlg(HWND, UINT, int *);
BOOL TabSettingsDlg(HWND, UINT, int *);
BOOL SelectDefEncodingDlg(HWND, int *);
BOOL SelectEncodingDlg(HWND, int *);
BOOL RecodeDlg(HWND, int *);
BOOL SelectDefLineEndingDlg(HWND, int *);
INT_PTR InfoBox(int, const WCHAR *, int, ...);
