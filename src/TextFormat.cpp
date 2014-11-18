#include "Common.h"
#include "TextFormat.h"

static bool IsSpace(char c) { return c == ' '; }

static int SkipSpace(const char*& s, int& len) {
    const char* start = s;
    while (len > 0 && IsSpace(*s)) {
        --len;
        ++s;
    }
    return s - start;
}

static int SkipSpace(Slice& slice) { return SkipSpace(slice.s, slice.len); }

static bool IsWs(char c) {
    switch (c) {
        case ' ':
        case '\t':
        case '\n':
        case '\r':
            return true;
    }
    return false;
}

static int SkipWs(const char*& s, int& len) {
    const char* start = s;
    while (len > 0 && IsWs(*s)) {
        --len;
        ++s;
    }
    return s - start;
}

static int SkipWs(Slice& slice) { return SkipWs(slice.s, slice.len); }

static int SkipNonWs(const char*& s, int& len) {
    const char* start = s;
    while (len > 0 && !IsWs(*s)) {
        --len;
        ++s;
    }
    return s - start;
}

static int SkipNonWs(Slice& slice) { return SkipNonWs(slice.s, slice.len); }

static const u16 TYPE_BITS = 2;
static const u16 STYLE_BITS = 16 - TYPE_BITS;
static const u16 STYLE_MASK = ~(3 << STYLE_BITS);

u16 MakeStyleAndType(u16 style, u16 type) {
    CrashIf(type <= TextRun::TypeMax);
    type = type << STYLE_BITS;
    return style | type;
}

u16 ExtractStyle(u16 styleAndType) { return styleAndType & STYLE_MASK; }

u16 ExtractType(u16 styleAndType) {
    u16 type = styleAndType >> STYLE_BITS;
    return type;
}

static void AddRun(FormattedText& f, const char* s, int len, u16 style, u16 type) {
    TextRun r = { 0 };
    r.s.s = s;
    r.s.len = len;
    r.styleAndType = MakeStyleAndType(style, type);
    f.runs.push_back(r);
}

static void BreakIntoTextRuns(FormattedText& f, const TextStyle& initialStyle) {
    f.styles.push_back(initialStyle);
    u16 currStyle = 0;
    int n;
    const char* start;
    Slice s = f.s;
    while (s.len > 0) {
        start = s.s;
        n = SkipWs(s);
        if (n > 0) {
            AddRun(f, start, n, currStyle, TextRun::TypeWs);
        }
        start = s.s;
        n = SkipNonWs(s);
        if (n == 0) {
            break;
        }
        // TODO: needs to parse style tags
        AddRun(f, start, n, currStyle, TextRun::TypeStr);
    }
}

static void MeasureTextRuns(FormattedText& f) {}

static void LayoutTextRuns(FormattedText& f, HDC hdc, int dx) {}

void FormatText(FormattedText& f, Slice& s, HDC hdc, int dx, const TextStyle& initialStyle) {
    str::Len(s); // ensure we have len
    f.s = s;

    BreakIntoTextRuns(f, initialStyle);
    MeasureTextRuns(f);
    LayoutTextRuns(f, hdc, dx);
}

static void DebugDump(FormattedText& f) {
    std::string s;
    for (auto& r : f.runs) {
        char* copy = str::DupN(r.s.s, r.s.len);
        s = str::Format("%d: %s\n", r.s.len, copy);
        OutputDebugStringA(s.c_str());
        free(copy);
    }
}

void TestFormatText() {
    const char* s = "This is a test\nof a {b bold} text.\r\n {i ita{b li}c}";
    Slice slice = { s, 0 };
    FormattedText f;
    TextStyle style;
    style.col = RGB(0xff, 00, 00);
    style.fontName = "Tahoma";
    style.fontSizePt = 12.f;
    FormatText(f, slice, NULL, 1024, style);
    DebugDump(f);
}
