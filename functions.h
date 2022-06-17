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


struct {
    float x,y,z;
    float Xrot, Zrot;
} camera = {5,5,5,70,-40};



LRESULT CALLBACK WindowProc (HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL (HWND, HDC, HGLRC);

void Game_Init(HWND hwnd);
void Player_Move(HWND hwnd);
void Game_Show();


#endif //LABYRINTHROMAND_FUNCTIONS_H
