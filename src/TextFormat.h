
struct TextRun {
    // type in upper 2 bits of styleAndType
    enum {
        TypeWs = 0,
        TypeStr = 1,
        TypeUrl = 2,
        TypeMax = TypeUrl,
    };
    Slice s;
    RECT r;
    u16 styleAndType; // styles within FormattedText
};

struct TextStyle {
    std::string fontName;
    int fontSize;
    COLORREF col;
};

struct FormattedText {
    Slice s;
    RECT r;
    std::vector<TextStyle> styles;
    std::vector<TextRun> runs;
};

u16 MakeStyleAndType(u16 style, u16 type);

void FormatText(FormattedText&, Slice&, HDC, int dx, const TextStyle& initialStyle);
