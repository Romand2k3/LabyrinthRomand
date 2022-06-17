#pragma once
#ifndef LABYRINTHROMAND_FUNCTIONS_H
#define LABYRINTHROMAND_FUNCTIONS_H
#include <windows.h>

typedef struct {
    char name[20];
    float vert[8];
    bool hover;
} TButton;

typedef struct {
    float r,g,b;
} TColor;

typedef struct {
    float a,b;
} TTexure;

typedef struct {
    float x,y,z;
} TScale;

typedef struct {
    float x,y,z;
} TPosition;

typedef struct {
    TColor clr;
    TPosition pos;
} TCell;

typedef struct {
    TColor clr;
    TScale scl;
    TPosition pos;
} TPyramid;

typedef struct {
    GLuint tex;
    int scl;
    TPosition pos;
} TPlant;

struct {
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
