
#define MAX_COL 8
#define MAX_ROWS 64

struct CellInfo {
    RECT r;
};

struct Table {
    int nRows;
    int nCols;
    CellInfo cells[MAX_COL * MAX_ROWS];
};