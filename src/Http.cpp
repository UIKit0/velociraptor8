#include "Common.h"
#include <Wininet.h>
#include "Http.h"

// per RFC 1945 10.15 and 3.7, a user agent product token shouldn't contain whitespace
#define USER_AGENT L"BaseHTTP"

// returns ERROR_SUCCESS or an error code
static void HttpGet(const char *urlA, HttpRsp *rspOut) {
    rspOut->error = ERROR_SUCCESS;
    rspOut->httpStatusCode = 0;

    AutoUtf8ToWstr urlW(urlA);

    HINTERNET hReq = NULL;
    HINTERNET hInet = InternetOpenW(USER_AGENT, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (!hInet)
        goto Error;

    DWORD flags = INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_RELOAD;
    hReq = InternetOpenUrlW(hInet, urlW, NULL, 0, flags, 0);
    if (!hReq)
        goto Error;

    for (;;) {
        char buf[1024];
        DWORD dwRead = 0;
        if (!InternetReadFile(hReq, buf, sizeof(buf), &dwRead)) {
            goto Error;
        }
        if (dwRead == 0) {
            break;
        }
        rspOut->data.append((const char*) buf, (size_t) dwRead);
    };

    DWORD headerBuffSize = sizeof(DWORD);
    if (!HttpQueryInfoW(hReq, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &rspOut->httpStatusCode, &headerBuffSize, NULL)) {
        goto Error;
    }

Exit:
    if (hReq)
        InternetCloseHandle(hReq);
    if (hInet)
        InternetCloseHandle(hInet);
    return;

Error:
    rspOut->error = GetLastError();
    if (!rspOut->error)
        rspOut->error = ERROR_GEN_FAILURE;
    goto Exit;
}

void HttpGetAsync(const char *url, const std::function<void(HttpRsp*)>& f) {
    std::thread t([=] {
        std::unique_ptr<HttpRsp> rsp(new HttpRsp);
        HttpGet(url, rsp.get());
        f(rsp.get());
    });
    t.detach();
}
