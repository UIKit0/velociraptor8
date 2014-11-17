
#include "Common.h"
#include "UITask.h"
#include "WinUtil.h"

// TODO(kjk): can I avoid this struct wrapper?
struct UITask {
    UITask(const std::function<void()> f) : f(f) {}
    const std::function<void()> f;
};

namespace uitask {

static HWND gTaskDispatchHwnd = NULL;

#define WM_EXECUTE_TASK (WM_USER + 1)

static LRESULT CALLBACK WndProcTaskDispatch(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    UITask *task;
    if (WM_EXECUTE_TASK == msg) {
        task = (UITask *)lParam;
        task->f();
        delete task;
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Initialize() {
    const WCHAR *uiTaskClassName = L"UITask_Wnd_Class";
    const WCHAR *uiTaskWindowName = L"UITask Dispatch Window";

    WNDCLASSEX wcex;
    FillWndClassEx(wcex, uiTaskClassName, WndProcTaskDispatch);
    RegisterClassEx(&wcex);

    CrashIf(gTaskDispatchHwnd);
    gTaskDispatchHwnd = CreateWindow(uiTaskClassName, uiTaskWindowName, WS_OVERLAPPED, 0, 0, 0, 0,
                                     HWND_MESSAGE, NULL, GetModuleHandle(NULL), NULL);
}

void DrainQueue() {
    CrashIf(!gTaskDispatchHwnd);
    MSG msg;
    while (PeekMessage(&msg, gTaskDispatchHwnd, WM_EXECUTE_TASK, WM_EXECUTE_TASK, PM_REMOVE)) {
        DispatchMessage(&msg);
    }
}

void Destroy() {
    DrainQueue();
    DestroyWindow(gTaskDispatchHwnd);
    gTaskDispatchHwnd = NULL;
}

void Post(const std::function<void()> &f) {
    auto task = new UITask(f);
    PostMessage(gTaskDispatchHwnd, WM_EXECUTE_TASK, 0, (LPARAM)task);
}
}
