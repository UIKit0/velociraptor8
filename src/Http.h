
struct HttpRsp {
    std::string data;
    DWORD httpStatusCode;
    DWORD error; // ERROR_SUCCESS if no error
};

void HttpGetAsync(const char *url, const std::function<void(HttpRsp*)>&);

