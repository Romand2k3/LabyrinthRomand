#pragma once
#ifndef LABYRINTHROMAND_FUNCTIONS_H
#define LABYRINTHROMAND_FUNCTIONS_H
#include <windows.h>
#include <GLFW/glfw3.h>



/**
 * структура для кнопок
 * name - надпись на кнопке
 * vert - массив вертикалей
 * light - подсветка кнопки
 */
typedef struct {
    char name[20];
    float vert[8];
    bool light;
} TButton;


/**
 * структура для цвета
 * r,g,b - цвета в РГБ
 */
typedef struct {
    float r,g,b;
} TColor;


/**
 * структура для поцизии текстуры
 * a,b - позиции вершин для текстурирования
 */
typedef struct {
    float a,b;
} TTexure;


/**
 * структура для размера отдельно по x,y,z
 * x,y,z - оси размеров
 */
typedef struct {
    float x,y,z;
} TScale;

/**
 * структура дял позиции
 * x,y,z - координаты позиции
 */
typedef struct {
    float x,y,z;
} TPosition;

/**
 * структура для пирамидок
 * clr - цвет пирамиды
 * scl - размер пирамиды
 * pos - позиция пирамиды
 */
typedef struct {
    TColor clr;
    TScale scl;
    TPosition pos;
} TPyramid;


/**
 * структура для растений
 * tex - индекс текстуры
 * scl - размер объекта
 * pos - позиция объекта
 */
typedef struct {
    GLuint tex;
    int scl;
    TPosition pos;
} TPlant;


/**
 * структура камеры
 * x,y,z - позиция камеры
 * Xrot, Zrot - угол наклона камеры
 */
struct {
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
