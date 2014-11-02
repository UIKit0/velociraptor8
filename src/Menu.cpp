#include "Common.h"
#include "Menu.h"
#include "Translations.h"
#include "resource.h"

#define SEPARATOR "-----"

struct MenuDef {
    const char *title;
    UINT        id;
    MenuDef *   sub;
};

static MenuDef menuDefLaunch[] = {
    { "&New Window\tAlt+N", IDM_FILE_NEWWINDOW, nullptr },
    { "&Empty Window\tAlt+0", IDM_FILE_NEWWINDOW2, nullptr },
    { SEPARATOR, 0, nullptr },
    { "Execute &Document\tCtrl+L", IDM_FILE_LAUNCH, nullptr },
    { "&Open with...\tAlt+L", IDM_FILE_OPENWITH, nullptr },
    { "&Command...\tCtrl+R", IDM_FILE_RUN, nullptr },
    { nullptr, 0, nullptr }
};

static MenuDef menuDefEncoding[] = {
    { "&ANSI", IDM_ENCODING_ANSI, nullptr },
    { "&Unicode", IDM_ENCODING_UNICODE, nullptr },
    { "Unicode &Big Endian", IDM_ENCODING_UNICODEREV, nullptr },
    { "UTF-&8", IDM_ENCODING_UTF8, nullptr },
    { "UTF-8 with &Signature", IDM_ENCODING_UTF8SIGN, nullptr },
    { "&More...\tF9", IDM_ENCODING_SELECT, nullptr },
    { SEPARATOR, 0, nullptr },
    { "&Recode...\tF8", IDM_ENCODING_RECODE, nullptr },
    { SEPARATOR, 0, nullptr },
    { "&Default...", IDM_ENCODING_SETDEFAULT, nullptr },
    { nullptr, 0, nullptr }
};

static MenuDef menuDefLineEndings[] = {
    { "&Windows (CR+LF)", IDM_LINEENDINGS_CRLF, nullptr },
    { "&Unix (LF)", IDM_LINEENDINGS_LF, nullptr },
    { "&Mac (CR)", IDM_LINEENDINGS_CR, nullptr },
    { SEPARATOR, 0, nullptr },
    { "&Default...", IDM_LINEENDINGS_SETDEFAULT, nullptr },
    { nullptr, 0, nullptr }
};

static MenuDef menuDefFavorites [] = {
    { "&Open Favorites...\tAlt+I", IDM_FILE_OPENFAV, nullptr },
    { "&Add Current File...\tAlt+K", IDM_FILE_ADDTOFAV, nullptr },
    { "&Manage...\tAlt+F9", IDM_FILE_MANAGEFAV, nullptr },
    { nullptr, 0, nullptr }
};

static MenuDef menuDefFile[] = {
    { "&New\tCtrl+N", IDM_FILE_NEW, nullptr },
    { "&Open...\tCtrl+O", IDM_FILE_OPEN, nullptr },
    { "Re&vert\tF5", IDM_FILE_REVERT, nullptr },
    { "&Save\tCtrl+S", IDM_FILE_SAVE, nullptr },
    { "Save &As...\tF6", IDM_FILE_SAVEAS, nullptr },
    { "Save &Copy...\tCtrl+F6", IDM_FILE_SAVECOPY, nullptr },
    { "&Read Only", IDM_FILE_READONLY, nullptr },
    { "&Launch", 0, menuDefLaunch },
    { SEPARATOR, 0, nullptr },
    { "&Encoding", 0, menuDefEncoding },
    { "Line Endin&gs" , 0, menuDefLineEndings},
    { SEPARATOR, 0, nullptr },
    { "Page Se&tup...", IDM_FILE_PAGESETUP, nullptr },
    { "&Print...\tCtrl+P", IDM_FILE_PRINT, nullptr },
    { SEPARATOR, 0, nullptr },
    { "Propert&ies...", IDM_FILE_PROPERTIES, nullptr },
    { "Create &Desktop Link", IDM_FILE_CREATELINK, nullptr },
    { SEPARATOR, 0, nullptr },
    { "&Favorites", 0, menuDefFavorites },
    { "Recent (&History)...\tAlt+H", IDM_FILE_RECENT, nullptr },
    { SEPARATOR, 0, nullptr },
    { "E&xit\tAlt+F4", IDM_FILE_EXIT, nullptr },
    { nullptr, 0, nullptr }
};

static MenuDef menuDefLines [] = {
    { "Move &Up\tCtrl+Shift+Up", IDM_EDIT_MOVELINEUP, nullptr },
    { "&Move Down\tCtrl+Shift+Down", IDM_EDIT_MOVELINEDOWN, nullptr },
    { SEPARATOR, 0, nullptr },
    { "&Duplicate Line\tCtrl+D", IDM_EDIT_DUPLICATELINE, nullptr },
    { "&Cut Line\tCtrl+Shift+X", IDM_EDIT_CUTLINE, nullptr },
    { "C&opy Line\tCtrl+Shift+C", IDM_EDIT_COPYLINE, nullptr },
    { "D&elete Line\tCtrl+Shift+D", IDM_EDIT_DELETELINE, nullptr },
    { SEPARATOR, 0, nullptr },
    { "Column &Wrap...\tCtrl+Shift+W", IDM_EDIT_COLUMNWRAP, nullptr },
    { "&Split Lines\tCtrl+I", IDM_EDIT_SPLITLINES, nullptr },
    { "&Join Lines\tCtrl+J", IDM_EDIT_JOINLINES, nullptr },
    { "Join &Paragraphs\tCtrl+Shift+J", IDM_EDIT_JOINLINESEX, nullptr },
    { nullptr, 0, nullptr }
};

static MenuDef menuDefBlock [] {
    { "&Indent\tTab", IDM_EDIT_INDENT, nullptr },
    { "&Unindent\tShift+Tab", IDM_EDIT_UNINDENT, nullptr },
    { SEPARATOR, 0, nullptr },
    { "&Enclose Selection...\tAlt+Q", IDM_EDIT_ENCLOSESELECTION, nullptr },

    { "&Duplicate Selection\tAlt+D", IDM_EDIT_SELECTIONDUPLICATE, nullptr },

    { SEPARATOR, 0, nullptr },
    { "&Pad With Spaces\tAlt+B", IDM_EDIT_PADWITHSPACES, nullptr },
    { "Strip &First Character\tAlt+Z", IDM_EDIT_STRIP1STCHAR, nullptr },
    { "Strip &Last Character\tAlt+U", IDM_EDIT_STRIPLASTCHAR, nullptr },
    { "Strip &Trailing Blanks\tAlt+W", IDM_EDIT_TRIMLINES, nullptr },
    { "Compress &Whitespace\tAlt+P", IDM_EDIT_COMPRESSWS, nullptr },
    { "Merge &Blank Lines\tAlt+Y", IDM_EDIT_MERGEBLANKLINES, nullptr },
    { "&Remove Blank Lines\tAlt+R", IDM_EDIT_REMOVEBLANKLINES, nullptr },

    { SEPARATOR, 0, nullptr },
    { "&Modify Lines...\tAlt+M", IDM_EDIT_MODIFYLINES, nullptr },
    { "Alig&n Lines...\tAlt+J", IDM_EDIT_ALIGN, nullptr },
    { "S&ort Lines...\tAlt+O", IDM_EDIT_SORTLINES, nullptr },
    { nullptr, 0, nullptr }
};

static MenuDef menuDefConvert [] = { 
    { "&Uppercase\tCtrl+Shift+U", IDM_EDIT_CONVERTUPPERCASE, nullptr },
    { "&Lowercase\tCtrl+U", IDM_EDIT_CONVERTLOWERCASE, nullptr },
    { SEPARATOR, 0, nullptr },
    { "&Invert Case\tCtrl+Alt+U", IDM_EDIT_INVERTCASE, nullptr },
    { "Title &Case\tCtrl+Alt+I", IDM_EDIT_TITLECASE, nullptr },
    { "&Sentence Case\tCtrl+Alt+O", IDM_EDIT_SENTENCECASE, nullptr },
    { SEPARATOR, 0, nullptr },
    { "&Tabify Selection\tCtrl+Shift+T", IDM_EDIT_CONVERTSPACES, nullptr },
    { "U&ntabify Selection\tCtrl+Shift+S", IDM_EDIT_CONVERTTABS, nullptr },
    { SEPARATOR, 0, nullptr },
    { "Ta&bify Indent\tCtrl+Alt+T", IDM_EDIT_CONVERTSPACES2, nullptr },
    { "Untabi&fy Indent\tCtrl+Alt+S", IDM_EDIT_CONVERTTABS2, nullptr },
    { nullptr, 0, nullptr }
};

static MenuDef menuDefInsert [] = {
    { "&HTML/XML Tag...\tAlt+X", IDM_EDIT_INSERT_TAG, nullptr },
    { SEPARATOR, 0, nullptr },
    { "&Encoding Identifier\tCtrl+F8", IDM_EDIT_INSERT_ENCODING, nullptr },
    { SEPARATOR, 0, nullptr },
    { "Time/Date (&Short Form)\tCtrl+F5", IDM_EDIT_INSERT_SHORTDATE, nullptr },
    { "Time/Date (&Long Form)\tCtrl+Shift+F5", IDM_EDIT_INSERT_LONGDATE, nullptr },
    { SEPARATOR, 0, nullptr },
    { "&Filename\tCtrl+F9", IDM_EDIT_INSERT_FILENAME, nullptr },
    { "&Path and Filename\tCtrl+Shift+F9", IDM_EDIT_INSERT_PATHNAME, nullptr },
    { nullptr, 0, nullptr }
};

static MenuDef menuDefSpecial [] = {
    { "Line Comment (&Toggle)\tCtrl+Q", IDM_EDIT_LINECOMMENT, nullptr },
    { "Stream &Comment\tCtrl+Shift+Q", IDM_EDIT_STREAMCOMMENT, nullptr },
    { SEPARATOR, 0, nullptr },
    { "URL &Encode\tCtrl+Shift+E", IDM_EDIT_URLENCODE, nullptr },
    { "URL &Decode\tCtrl+Shift+R", IDM_EDIT_URLDECODE, nullptr },
    { SEPARATOR, 0, nullptr },
    { "Esca&pe C Chars\tCtrl+Alt+E", IDM_EDIT_ESCAPECCHARS, nullptr },
    { "&Unescape C Chars\tCtrl+Alt+R", IDM_EDIT_UNESCAPECCHARS, nullptr },
    { SEPARATOR, 0, nullptr },
    { "C&har To Hex\tCtrl+Alt+X", IDM_EDIT_CHAR2HEX, nullptr },
    { "Hex To Cha&r\tCtrl+Alt+C", IDM_EDIT_HEX2CHAR, nullptr },
    { SEPARATOR, 0, nullptr },
    { "&Find Matching Brace\tCtrl+B", IDM_EDIT_FINDMATCHINGBRACE, nullptr },
    { "&Select To Matching Brace\tCtrl+Shift+B", IDM_EDIT_SELTOMATCHINGBRACE, nullptr },
    { SEPARATOR, 0, nullptr },
    { "Select To Ne&xt\tCtrl+Alt+F2", IDM_EDIT_SELTONEXT, nullptr },
    { "Select To Pre&vious\tCtrl+Alt+Shift+F2", IDM_EDIT_SELTOPREV, nullptr },
    { SEPARATOR, 0, nullptr },
    { "Complete Word\tCtrl+Enter", IDM_EDIT_COMPLETEWORD, nullptr },
    { nullptr, 0, nullptr }
};

#ifdef BOOKMARK_EDITION
static MenuDef menuDefBookmarks [] = {
    { "Toggle\tCtrl+F2", BME_EDIT_BOOKMARKTOGGLE, nullptr },
    { SEPARATOR, 0, nullptr },
    { "Goto Next\tF2", BME_EDIT_BOOKMARKNEXT, nullptr },
    { "Goto Previous\tShift+F2", BME_EDIT_BOOKMARKPREV, nullptr },
    { SEPARATOR, 0, nullptr },
    { "Clear All\tAlt+F2", BME_EDIT_BOOKMARKCLEAR, nullptr },
    { nullptr, 0, nullptr }
};
#endif

static MenuDef menuDefEdit [] = {
    { "&Undo\tCtrl+Z", IDM_EDIT_UNDO, nullptr },
    { "&Redo\tCtrl+Y", IDM_EDIT_REDO, nullptr },
    { SEPARATOR, 0, nullptr },
    { "Cu&t\tCtrl+X", IDM_EDIT_CUT, nullptr },
    { "&Copy\tCtrl+C", IDM_EDIT_COPY, nullptr },
    { "Copy A&ll\tAlt+C", IDM_EDIT_COPYALL, nullptr },
    { "Copy &Add\tCtrl+E", IDM_EDIT_COPYADD, nullptr },
    { "&Paste\tCtrl+V", IDM_EDIT_PASTE, nullptr },
    { "S&wap\tCtrl+K", IDM_EDIT_SWAP, nullptr },
    { "Clear\tDel", IDM_EDIT_CLEAR, nullptr },
    { "Clear Clipboar&d", IDM_EDIT_CLEARCLIPBOARD, nullptr },
    { "&Select All\tCtrl+A", IDM_EDIT_SELECTALL, nullptr },
    { SEPARATOR, 0, nullptr },
    { "L&ines", 0, menuDefLines },
    { "&Block", 0, menuDefBlock },
    { "C&onvert", 0, menuDefConvert },
    { "I&nsert", 0, menuDefInsert },
    { "Spec&ial", 0, menuDefSpecial },
#ifdef BOOKMARK_EDITION
    { "Bookmarks", 0, menuDefBookmarks },
#endif
    { SEPARATOR, 0, nullptr },
    { "&Find...\tCtrl+F", IDM_EDIT_FIND, nullptr },
    { "Sa&ve Find Text\tAlt+F3", IDM_EDIT_SAVEFIND, nullptr },
    { "Find Ne&xt\tF3", IDM_EDIT_FINDNEXT, nullptr },
    { "Find Pre&vious\tShift+F3", IDM_EDIT_FINDPREV, nullptr },
    { "R&eplace...\tCtrl+H", IDM_EDIT_REPLACE, nullptr },
    { "Replace Ne&xt\tF4", IDM_EDIT_REPLACENEXT, nullptr },
    { "&Goto...\tCtrl+G", IDM_EDIT_GOTOLINE, nullptr },
    { nullptr, 0, nullptr }
};

static MenuDef menuDefMarkOccurences[] = {
    { "&Off", IDM_VIEW_MARKOCCURRENCES_OFF, nullptr },
    { "&Blue", IDM_VIEW_MARKOCCURRENCES_BLUE, nullptr },
    { "&Green", IDM_VIEW_MARKOCCURRENCES_GREEN, nullptr },
    { "&Red", IDM_VIEW_MARKOCCURRENCES_RED, nullptr },
    { SEPARATOR, 0, nullptr },
    { "Match &Case", IDM_VIEW_MARKOCCURRENCES_CASE, nullptr },
    { "Match &Whole Words Only", IDM_VIEW_MARKOCCURRENCES_WORD, nullptr },
    { nullptr, 0, nullptr }
};

static MenuDef menuDefView [] = {
    { "&Syntax Scheme...\tF12", IDM_VIEW_SCHEME, nullptr },
    { "&2nd Default Scheme\tShift+F12", IDM_VIEW_USE2NDDEFAULT, nullptr },
    { "&Customize Schemes...\tCtrl+F12", IDM_VIEW_SCHEMECONFIG, nullptr },
    { "&Default Font...\tAlt+F12", IDM_VIEW_FONT, nullptr },
    { SEPARATOR, 0, nullptr },
    { "Word W&rap\tCtrl+W", IDM_VIEW_WORDWRAP, nullptr },
    { "&Long Line Marker\tCtrl+Shift+L", IDM_VIEW_LONGLINEMARKER, nullptr },
    { "Indentation &Guides\tCtrl+Shift+G", IDM_VIEW_SHOWINDENTGUIDES, nullptr },
    { SEPARATOR, 0, nullptr },
    { "Show &Whitespace\tCtrl+Shift+8", IDM_VIEW_SHOWWHITESPACE, nullptr },
    { "Show Line &Endings\tCtrl+Shift+9", IDM_VIEW_SHOWEOLS, nullptr },
    { "Show Wrap S&ymbols\tCtrl+Shift+0", IDM_VIEW_WORDWRAPSYMBOLS, nullptr },
    { SEPARATOR, 0, nullptr },
    { "Visual &Brace Matching\tCtrl+Shift+V", IDM_VIEW_MATCHBRACES, nullptr },
    { "Highlight C&urrent Line\tCtrl+Shift+I", IDM_VIEW_HILITECURRENTLINE, nullptr },
    { "Mar&k Occurrences", 0, menuDefMarkOccurences },
    { SEPARATOR, 0, nullptr },
    { "Line &Numbers\tCtrl+Shift+N", IDM_VIEW_LINENUMBERS, nullptr },
    { "Selection &Margin\tCtrl+Shift+M", IDM_VIEW_MARGIN, nullptr },
    { SEPARATOR, 0, nullptr },
    { "Code &Folding\tCtrl+Shift+Alt+F", IDM_VIEW_FOLDING, nullptr },
    { "&Toggle All Folds\tCtrl+Shift+F", IDM_VIEW_TOGGLEFOLDS, nullptr },
    { SEPARATOR, 0, nullptr },
    { "Zoom &In\tCtrl++", IDM_VIEW_ZOOMIN, nullptr },
    { "Zoom &Out\tCtrl+-", IDM_VIEW_ZOOMOUT, nullptr },
    { "Reset &Zoom\tCtrl+/", IDM_VIEW_RESETZOOM, nullptr },
    { nullptr, 0, nullptr }
};

static MenuDef menuDefWindowTitleDisplay [] = {
    { "Filename &Only", IDM_VIEW_SHOWFILENAMEONLY, nullptr },
    { "Filename and &Directory", IDM_VIEW_SHOWFILENAMEFIRST, nullptr },
    { "Full &Pathname", IDM_VIEW_SHOWFULLPATH, nullptr },
    { "&Text Excerpt\tCtrl+9", IDM_VIEW_SHOWEXCERPT, nullptr },
    { nullptr, 0, nullptr }
};

static MenuDef menuDefEscKeyFunction [] = {
    { "&None", IDM_VIEW_NOESCFUNC, nullptr },
    { "&Minimize velociraptor8", IDM_VIEW_ESCMINIMIZE, nullptr },
    { "E&xit velociraptor8", IDM_VIEW_ESCEXIT, nullptr },
    { nullptr, 0, nullptr }
};

static MenuDef menuDefSettings [] = {
    { "Insert Tabs as &Spaces", IDM_VIEW_TABSASSPACES, nullptr },
    { "&Tab Settings...\tCtrl+T", IDM_VIEW_TABSETTINGS, nullptr },
    { "&Word Wrap Settings...", IDM_VIEW_WORDWRAPSETTINGS, nullptr },
    { "&Long Line Settings...", IDM_VIEW_LONGLINESETTINGS, nullptr },
    { "Auto In&dent Text", IDM_VIEW_AUTOINDENTTEXT, nullptr },
    { "Auto Close &HTML/XML\tCtrl+Shift+H", IDM_VIEW_AUTOCLOSETAGS, nullptr },
    { "Auto Complete Wo&rds", IDM_VIEW_AUTOCOMPLETEWORDS, nullptr },
    { SEPARATOR, 0, nullptr },
    { "&Reuse Window", IDM_VIEW_REUSEWINDOW, nullptr },
    { "Sticky Window &Position", IDM_VIEW_STICKYWINPOS, nullptr },
    { "&Always On Top\tAlt+T", IDM_VIEW_ALWAYSONTOP, nullptr },
    { "Minimi&ze To Tray", IDM_VIEW_MINTOTRAY, nullptr },
    { "Transparent &Mode\tCtrl+0", IDM_VIEW_TRANSPARENT, nullptr },
    { SEPARATOR, 0, nullptr },
    { "Single &File Instance", IDM_VIEW_SINGLEFILEINSTANCE, nullptr },
    { "File &Change Notification...\tAlt+F5", IDM_VIEW_CHANGENOTIFY, nullptr },
    { "Window Title Displa&y", 0, menuDefWindowTitleDisplay },
    { "Esc &Key Function", 0, menuDefEscKeyFunction},
    { "Sa&ve Before Running Tools", IDM_VIEW_SAVEBEFORERUNNINGTOOLS, nullptr },
    { "Remember Recent F&iles", IDM_VIEW_NOSAVERECENT, nullptr },
    { "Remember S&earch Strings", IDM_VIEW_NOSAVEFINDREPL, nullptr },
    { SEPARATOR, 0, nullptr },
    { "Sh&ow Toolbar", IDM_VIEW_TOOLBAR, nullptr },
    { "C&ustomize Toolbar...", IDM_VIEW_CUSTOMIZETB, nullptr },
    { "Sh&ow Statusbar", IDM_VIEW_STATUSBAR, nullptr },
    { SEPARATOR, 0, nullptr },
    { "Save Settings On E&xit", IDM_VIEW_SAVESETTINGS, nullptr },
    { "Save Settings &Now\tF7", IDM_VIEW_SAVESETTINGSNOW, nullptr },
    { nullptr, 0, nullptr }
};

static MenuDef menuDefHelp [] = {
    { "&About...\tF1", IDM_HELP_ABOUT, nullptr },
    { "&Website", IDM_WEBSITE, nullptr },
    { "Install", IDM_INSTALL, nullptr },
    { nullptr, 0, nullptr }
};

static MenuDef menuDefMain [] = {
    { "&File", 0, menuDefFile },
    { "&Edit", 0, menuDefEdit },
    { "&View", 0, menuDefView },
    { "&Settings", 0, menuDefSettings },
    { "&Help", 0, menuDefHelp },
    { nullptr, 0, nullptr }
};

static bool IsMenuSep(const char *s) {
    return str::Eq(s, SEPARATOR);
}

static HMENU AppendToMenu(HMENU m, MenuDef *menuDef) {
    for (int i = 0; menuDef[i].title != nullptr; i++) {
        MenuDef md = menuDef[i];
        if (IsMenuSep(md.title)) {
            CrashIf(md.id != 0);
            CrashIf(md.sub != nullptr);
            AppendMenu(m, MF_SEPARATOR, 0, NULL);
            continue;
        }
        AutoUtf8ToWstr tmp(md.title);
        if (md.sub == nullptr) {
            CrashIf(md.id != 0);
            AppendMenu(m, MF_STRING, (UINT_PTR) md.id, tmp.Get());
        } else {
            auto subMenu = AppendToMenu(CreateMenu(), md.sub);
            AppendMenu(m, MF_POPUP | MF_STRING, (UINT_PTR) subMenu, tmp.Get());
        }
    }
    return m;
}

HMENU BuildMainMenu() {
    return AppendToMenu(CreateMenu(), menuDefMain);
}

static MenuDef menuDefPopup1 [] = {
    { "&Undo", IDM_EDIT_UNDO, nullptr },
    { "&Redo", IDM_EDIT_REDO, nullptr },
    { SEPARATOR, 0, nullptr },
    { "Cu&t", IDM_EDIT_CUT, nullptr },
    { "&Copy", IDM_EDIT_COPY, nullptr },
    { "&Paste", IDM_EDIT_PASTE, nullptr },
    { "Cl&ear", IDM_EDIT_CLEAR, nullptr },
    { SEPARATOR, 0, nullptr },
    { "&Select All", IDM_EDIT_SELECTALL, nullptr },
    { nullptr, 0, nullptr }
};

static MenuDef menuDefPopup2 [] = {
    { "Show &Toolbar", IDM_VIEW_TOOLBAR, nullptr },
    { "&Customize Toolbar...", IDM_VIEW_CUSTOMIZETB, nullptr },
    { "Show &Statusbar", IDM_VIEW_STATUSBAR, nullptr },
    { nullptr, 0, nullptr }
};

static MenuDef menuDefPopup3 [] = {
    { "Open velociraptor8", IDM_TRAY_RESTORE, nullptr },
    { "Exit velociraptor8", IDM_TRAY_EXIT, nullptr },
    { nullptr, 0, nullptr }
};

static MenuDef menuDefPopup [] = {
    { "+", 0, menuDefPopup1 },
    { "+", 0, menuDefPopup2 },
    { "+", 0, menuDefPopup3 },
    { nullptr, 0, nullptr }
};

HMENU BuildPopupMenu() {
    return AppendToMenu(CreateMenu(), menuDefPopup);
}
