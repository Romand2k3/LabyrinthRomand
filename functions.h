#pragma once
#ifndef LABYRINTHROMAND_FUNCTIONS_H
#define LABYRINTHROMAND_FUNCTIONS_H
#include <windows.h>
#include <GLFW/glfw3.h>


typedef struct {
    ///структура для кнопок
    char name[20];
    float vert[8];
    bool light;
} TButton;

typedef struct {
    ///структура для цвета
    float r,g,b;
} TColor;

typedef struct {
    ///структура для поцизии текстуры
    float a,b;
} TTexure;

typedef struct {
    ///структура для размера отдельно по x,y,z
    float x,y,z;
} TScale;

typedef struct {
    ///структура дял позиции
    float x,y,z;
} TPosition;

typedef struct {
    ///структура для пирамидок
    TColor clr;
    TScale scl;
    TPosition pos;
} TPyramid;

typedef struct {
    ///структура для растений
    GLuint tex;
    int scl;
    TPosition pos;
} TPlant;

struct {
    ///структура для модуля камеры игрока
    float x,y,z;
    float Xrot, Zrot;
} camera = {5,5,5,70,-40};




LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);


    void EnableOpenGL(HWND hwnd, HDC *, HGLRC *);

    void DisableOpenGL(HWND, HDC, HGLRC);

    void Game_Init(HWND hwnd);

    void Player_Move(HWND hwnd);

    void Game_Show();

    void WindowResize(int width, int height);



#endif //LABYRINTHROMAND_FUNCTIONS_H
