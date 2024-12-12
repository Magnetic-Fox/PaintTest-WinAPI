#include <windows.h>

#define WINCLASS    "TestWindow"

ATOM registerMainWindowClass(WNDCLASS*);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);

ATOM registerMainWindowClass(HINSTANCE hInstance, WNDCLASS *wc) {
    wc->style=          0;
    wc->lpfnWndProc=    MainWndProc;
    wc->cbClsExtra=     0;
    wc->cbWndExtra=     0;
    wc->hInstance=      hInstance;
    wc->hIcon=          LoadIcon(NULL, IDI_ASTERISK);
    wc->hCursor=        LoadCursor(NULL, IDC_ARROW);
    wc->hbrBackground=  (HBRUSH)(COLOR_WINDOW + 1);
    wc->lpszMenuName=   NULL;
    wc->lpszClassName=  WINCLASS;
    return RegisterClass(wc);
}

void paintSomething(HWND hwnd, HDC hdc) {
    POINT oldPoint;
    HPEN oldPen, redPen=CreatePen(PS_SOLID, 1, RGB(255,0,255));
    oldPen=(HPEN)SelectObject(hdc,redPen);
    MoveToEx(hdc,10,10,&oldPoint);
    LineTo(hdc,121,10);
    SelectObject(hdc,oldPen);
    DeleteObject(redPen);
    Rectangle(hdc,20,20,300,300);
    return;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = { 0 };
    MSG msg;

    HWND hwnd;
    
    if(!registerMainWindowClass(hInstance,&wc)) {
        MessageBox(NULL,"Could not create window class","Error",MB_ICONSTOP | MB_OK);
        return 1;
    }

    hwnd=CreateWindow(WINCLASS, "Paint test", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                        NULL, NULL, hInstance, NULL);

    if(hwnd==NULL) {
        MessageBox(NULL,"Could not create window","Error",MB_ICONSTOP | MB_OK);
        return 1;
    }

    ShowWindow(hwnd,SW_SHOWNORMAL);

    // HDC hdc=GetDC(hwnd);
    // paintSomething(hwnd,hdc);
    // ReleaseDC(hwnd,hdc);

    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_PAINT:
            PAINTSTRUCT ps;
            HDC hdc=BeginPaint(hwnd,&ps);
            paintSomething(hwnd,hdc);
            EndPaint(hwnd,&ps);
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd,msg,wParam,lParam);
    }
    return 0;
}
