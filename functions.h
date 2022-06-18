#pragma once
#ifndef LABYRINTHROMAND_FUNCTIONS_H
#define LABYRINTHROMAND_FUNCTIONS_H
#include <windows.h>
#include <GLFW/glfw3.h>


typedef struct {
    ///структура для кнопок
    ///name - надпись на кнопке
    ///vert - массив вертикалей
    ///light - подсветка кнопки
    char name[20];
    float vert[8];
    bool light;
} TButton;

typedef struct {
    ///структура для цвета
    ///r,g,b - цвета в РГБ
    float r,g,b;
} TColor;

typedef struct {
    ///структура для поцизии текстуры
    ///a,b - позиции вершин для текстурирования
    float a,b;
} TTexure;

typedef struct {
    ///структура для размера отдельно по x,y,z
    ///x,y,z - оси размеров
    float x,y,z;
} TScale;

typedef struct {
    ///структура дял позиции
    ///x,y,z - координаты позиции
    float x,y,z;
} TPosition;

typedef struct {
    ///структура для пирамидок
    ///clr - цвет пирамиды
    ///scl - размер пирамиды
    ///pos - позиция пирамиды
    TColor clr;
    TScale scl;
    TPosition pos;
} TPyramid;

typedef struct {
    ///структура для растений
    ///tex - индекс текстуры
    ///scl - размер объекта
    ///pos - позиция объекта
    GLuint tex;
    int scl;
    TPosition pos;
} TPlant;

struct {
    ///структура для модуля камеры игрока
    ///x,y,z - координаты камеры
    ///Xrot, Zrot - углы поворота камеры по горизонтали и вертикали
    float x,y,z;
    float Xrot, Zrot;
} camera = {5,5,5,70,-40};///инициализация камеры

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);


    void EnableOpenGL(HWND hwnd, HDC *, HGLRC *);

    void DisableOpenGL(HWND, HDC, HGLRC);

    void Game_Init(HWND hwnd);

    void Player_Move(HWND hwnd);

    void Game_Show();

    void WindowResize(int width, int height);



#endif //LABYRINTHROMAND_FUNCTIONS_H
