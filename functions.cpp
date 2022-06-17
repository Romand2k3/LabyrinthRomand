#include <GLFW/glfw3.h>
#include <windows.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <math.h>
#include "functions.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



TButton btn[] = {
        {"start", {0,0, 100,0, 100,30, 0, 30,}, FALSE},
        {"stop", {0,40, 100,40, 100,70, 0, 70,}, FALSE},
        {"quit", {0,80, 100,80, 100,110, 0, 110,}, FALSE}
};

int btnCnt = sizeof (btn) / sizeof (btn[0]);

#define mapW 200
#define mapH 200
#define pyraN 200
#define hillN 20
TPosition map[mapW][mapH];
TPosition normalforlight[mapW][mapH];
TColor mapcolor[mapW][mapH];
TPyramid pyramids[pyraN];
GLuint mapIndex[mapW-1][mapH-1][6];
int mapIndexNum = sizeof(mapIndex) / sizeof(GLuint);

float pyramid[] = {0,0, 3, 1,1,0, 1,-1,0, -1,-1,0, -1,1,0, 1,1,0};


void WindowResize(int width, int height){
    glViewport(0, 0, width, height);
    float XandYDiff = width / (float) height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-XandYDiff, XandYDiff, -1, 1, 2, 800);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void Camera_Apply(){
    glRotatef(-camera.Xrot, 1,0,0);
    glRotatef(-camera.Zrot, 0,0,1);
    glTranslatef(-camera.x, -camera.y, -camera.z);
}

void Camera_Rotating(float xAngle, float zAngle){
    camera.Zrot += zAngle;
    if (camera.Zrot < 0 ) camera.Zrot +=360;
    if (camera.Zrot > 360 ) camera.Zrot -=360;
    camera.Xrot += xAngle;
    if (camera.Xrot < 0 ) camera.Xrot = 0 ;
    if (camera.Xrot > 180 ) camera.Xrot = 180;
}

bool OnMap(float x, float y){
    return (x >= 0) && (x < mapW) && (y >= 0) && (y <=mapW );
}

float OnFoot(float x, float y){
    if (!OnMap(x,y)) return 0;
    int INTx = (int)x;
    int INTy = (int)y;
    ///не мой код
    float height1 = ( (1-(x-INTx))*map[INTx][INTy].z + (x-INTx)*map[INTx+1][INTy].z);
    float height2 = ((1-(x-INTx))*map[INTx][INTy+1].z + (x-INTx)*map[INTx+1][INTy+1].z );
    return (1-(y-INTy)) * height1 + (y-INTy) * height2;
    ///конец не моего кода
}

void TButton_Show(TButton btn){
    glEnableClientState(GL_VERTEX_ARRAY);
    if (btn.hover) glColor3f(1,0,0);
    else glColor3f(0,1,0);
    glVertexPointer(2, GL_FLOAT, 0, btn.vert);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
}
bool PointInButton(int x, int y, TButton btn){
    return (x > btn.vert[0]) && (x < btn.vert[4]) &&
           (y > btn.vert[1]) && (y < btn.vert[5]);
}
void Show_Menu(int width, int height){
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0,width, height,0,-1,1);
    for (int i = 0; i < btnCnt; i++)
        TButton_Show(btn[i]);
    glPopMatrix();
}

void MakeSomeHill(int x, int y, int radius, int height){
    for (int i = x-radius; i <= x+radius; i++)
        for (int j = y-radius; j<= y+radius; j++){
            if (OnMap(i,j)){
                ///не мой способ создания холма
                float lenght = sqrt(pow(x-i,2) + pow(y-j,2));
                if (lenght < radius){
                    lenght = lenght / radius * M_PI_2;
                    map[i][j].z += cos(lenght) * height;
                    ///конец не меого способа
                }
            }
        }
}

void Make_Normals(TPosition a, TPosition b, TPosition c, TPosition *n){
    ///не мой код, загуглил расчет нормалей
    float temp;
    TPosition v1, v2;
    v1.x = a.x - b.x;
    v1.y = a.y - b.y;
    v1.z = a.z - b.z;
    v2.x = b.x - c.x;
    v2.y = b.y - c.y;
    v2.z = b.z - c.z;
    n->x = (v1.y * v2.z - v1.z * v2.y);
    n->y = (v1.z * v2.x - v1.x * v2.z);
    n->z = (v1.x * v2.y - v1.y * v2.x);
    temp = sqrt(pow(n->x , 2) + pow(n->y , 2) + pow(n->z , 2));
    n->x /= temp;
    n->y /= temp;
    n->z /= temp;
    ///конец не моего кода
}


void Map_Init(){
    for (int i = 0; i < mapW; i++)
        for (int j = 0; j < mapH; j++)
        {
            float dc = (rand() %20) *0.01;
            mapcolor[i][j].r = 0.30 + dc;
            mapcolor[i][j].g = 0.5 + dc;
            mapcolor[i][j].b = 0.15 + dc;
            map[i][j].x = i;
            map[i][j].y = j;
            map[i][j].z = (rand() % 10) * 0.05;
        }

    for (int i = 0; i < mapW-1; i++) {
        int position = i * mapH;
        for (int j = 0; j < mapH-1; j++) {
            mapIndex[i][j][0] = position;
            mapIndex[i][j][1] = position + 1 ;
            mapIndex[i][j][2] = position + 1 + mapH;

            mapIndex[i][j][3] = position + 1 + mapH;
            mapIndex[i][j][4] = position + mapH;
            mapIndex[i][j][5] = position;

            position++;
        }
    }

    std::srand(time(nullptr));
    for (int i = 0; i<hillN; i++){
        MakeSomeHill(rand() % mapW, rand() % mapH, 10 + rand() % 50, rand() % 10);
    }

    for (int i = 0; i < mapW; i++)
        for (int j = 0; j < mapH; j++) {
            Make_Normals(map[i][j], map[i + 1][j], map[i][j + 1], &normalforlight[i][j]);
        }

    for (int i = 0; i < pyraN; i++){
        float dc = (rand() %20) *0.01;
        float ds = (rand() %120) *0.01;
        pyramids[i].clr.r = 0.95;
        pyramids[i].clr.g = 0.87 + dc;
        pyramids[i].clr.b = 0.75 + dc;

        pyramids[i].scl.x = 0.2 + ds;
        pyramids[i].scl.y = 0.2 + ds;
        pyramids[i].scl.z = 0.6 + ds;


        pyramids[i].pos.x = 1 + rand() % mapW - 1;
        pyramids[i].pos.y = 1 + rand() % mapH - 1;
        if (pyramids[i].pos.x-1 >= 5 && pyramids[i].pos.x+1 <=5
            && pyramids[i].pos.y-1 >= 6 && pyramids[i].pos.y+1 <=5) {
            pyramids[i].pos.x +=2;
            pyramids[i].pos.y +=2;
        }
        pyramids[i].pos.z = map[(int)pyramids[i].pos.x][(int)pyramids[i].pos.y].z - 0.6;
    }
    for (int i = 0; i < pyraN; i++) {
        std::cout << pyramids[i].pos.x<<'\n';
        std::cout << pyramids[i].pos.y<<'\n';
        std::cout << pyramids[i].pos.z<<'\n';
    }
}

void Game_Init(HWND hwnd){
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);

    Map_Init();

    RECT rect;
    GetClientRect(hwnd, &rect);
    WindowResize(rect.right, rect.bottom);
}

void Player_Move(HWND hwnd){
    if (GetForegroundWindow() != hwnd) return;

    float ugol = -camera.Zrot / 180 * M_PI;
    float speed = 0;
    if (GetKeyState('W') < 0) speed = 0.1;
    if (GetKeyState('S') < 0) speed = -0.1;
    if (GetKeyState('W') < 0  && GetKeyState(VK_LSHIFT) < 0) speed = 0.3;

    if (GetKeyState('A') < 0) {speed = 0.1; ugol -= M_PI*0.5;}
    if (GetKeyState('D') < 0) {speed = 0.1; ugol += M_PI*0.5;}

    if (speed != 0) {
        camera.x += sin(ugol)*speed;
        camera.y += cos(ugol)*speed;
    }

    POINT cursor;
    static POINT based = {400, 300};
    GetCursorPos(&cursor);
    Camera_Rotating((based.y - cursor.y) / 5.0, (based.x - cursor.x) / 5.0);
    SetCursorPos(based.x,based.y);
    camera.z = OnFoot(camera.x, camera.y) + 5;

    std::cout<<camera.x<<" "<<camera.y<<'\n';
    for (int i; i<pyraN; i++){
        if ((int)camera.x >= pyramids[i].pos.x-1 && (int)camera.x <= pyramids[i].pos.x+1
            && (int)camera.y >= pyramids[i].pos.y-1 && (int)camera.y <= pyramids[i].pos.y+1){
            camera.x = 5;
            camera.y = 5;
            camera.Xrot = 70;
            camera.Zrot = -40;
        }
    }

    if (camera.x > mapW || camera.x < 0 || camera.y > mapH || camera.y < 0) {
        for (int i; i <10; i++) camera.z = -i;
        camera.x = 5;
        camera.y = 5;
        camera.Xrot = 70;
        camera.Zrot = -40;
        camera.z = 5;
    }

}


void Game_Show(){
    glClearColor((178.f / 255.f), 211.f / 255.f, 230.f / 255.f, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glPushMatrix();
    Camera_Apply();

    GLfloat sunpoint[] = {1,0,2,0};
    glLightfv(GL_LIGHT0, GL_POSITION, sunpoint);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, map);
    glColorPointer(3, GL_FLOAT, 0, mapcolor);
    glNormalPointer(GL_FLOAT, 0, normalforlight);
    glDrawElements(GL_TRIANGLES, mapIndexNum, GL_UNSIGNED_INT, mapIndex);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);


    glVertexPointer(3, GL_FLOAT, 0, &pyramid);
    for (int i = 0; i < pyraN; i++){
        glPushMatrix();
        glTranslatef(pyramids[i].pos.x, pyramids[i].pos.y, pyramids[i].pos.z);
        glScalef(pyramids[i].scl.x, pyramids[i].scl.y, pyramids[i].scl.z);
        glColor3f(pyramids[i].clr.r, pyramids[i].clr.g, pyramids[i].clr.b);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
        glPopMatrix();
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glNormal3f(0,0,10);
    glPopMatrix();
}



LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
            break;

        case WM_LBUTTONDOWN:
            for (int i = 0; i < btnCnt; i++)
                if (PointInButton(LOWORD(lParam), HIWORD(lParam), btn[i])) {
                    std::cout << btn[i].name << '\n';
                    if (strcmp(btn[i].name, "quit")==0)
                        PostQuitMessage(0);
                }
            break;

        case WM_MOUSEMOVE:
            for (int i = 0; i < btnCnt; i++)
                btn[i].hover =  PointInButton(LOWORD(lParam), HIWORD(lParam), btn[i]);
            break;

        case WM_SIZE:
            WindowResize(LOWORD(lParam), HIWORD(lParam));
            break;

//        case WM_SETCURSOR:
//            ShowCursor(FALSE);
//            break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                    break;
            }
        }
            break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void EnableOpenGL(HWND hWnd, HDC* hDC, HGLRC* hRC){

    PIXELFORMATDESCRIPTOR pdf;

    int iFormat;

/* get the device context (DC) */
    *hDC = GetDC(hWnd);

/* set the pixel format for the DC */
    ZeroMemory (&pdf, sizeof (pdf));
    pdf.nSize = sizeof (pdf);
    pdf.nVersion = 1;
    pdf.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pdf.iPixelType = PFD_TYPE_RGBA;
    pdf.cColorBits = 24;
    pdf.cDepthBits = 16;
    pdf.iLayerType = PFD_MAIN_PLANE;
    iFormat = ChoosePixelFormat (*hDC, &pdf);
    SetPixelFormat (*hDC, iFormat, &pdf);

/* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);

}


void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC){

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hWnd, hDC);
}


