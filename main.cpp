#include <GLFW/glfw3.h> ///библиотека для 3D графики
#include <windows.h> ///библиотека для обработки окна windows
#include "functions.h" ///заголовочный файл функции


/**
 * основная функция
 */
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{

    WNDCLASSEX wcex;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    bool bQuit = FALSE;
    HWND hwnd;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "Labyrinth";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
        return 0;

    ///настройка дескриптора окна
    hwnd = CreateWindowEx(0,
                          "Labyrinth",
                          "Labyrinth Romand",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          1600,
                          900,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);
    EnableOpenGL(hwnd, &hDC, &hRC);
    Game_Init(hwnd);

    while (!bQuit)
    ///главный цикл обработки
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            ///обработка закрытия окна
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            ///анимация здесь
            Player_Move(hwnd);
            Game_Show();
            SwapBuffers(hDC);
            Sleep(1);
        }
    }
    ///shutdown OPENGL
    DisableOpenGL(hwnd, hDC, hRC);

    DestroyWindow(hwnd);

    return msg.wParam;
}






