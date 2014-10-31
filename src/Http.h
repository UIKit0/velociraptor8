
struct HttpRsp {
    std::string data;
    DWORD httpStatusCode;
    DWORD error; // ERROR_SUCCESS if no error
};

bool IsHttpRspOk(const HttpRsp*);

void HttpGetAsync(const char *url, const std::function<void(HttpRsp*)>&);

