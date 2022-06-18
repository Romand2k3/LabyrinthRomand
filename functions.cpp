#include <GLFW/glfw3.h>
#include <windows.h>
#include <iostream>
#include <math.h>
#include "functions.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "calculate.h"

///объявление кнопок и их параметров
TButton btn[] = {
        {"start", {0,0, 100,0, 100,30, 0, 30,}, FALSE},
        {"stop", {0,40, 100,40, 100,70, 0, 70,}, FALSE},
        {"quit", {0,80, 100,80, 100,110, 0, 110,}, FALSE}
};

int btnCnt = sizeof (btn) / sizeof (btn[0]);

///объявление карты, пирамидок и растений и их параметров
#define mapW 200 ///ширина камеры
#define mapH 200 ///длина камеры
#define pyraN 100 ///количество пирамидок
#define hillN 20 ///количество холмов

#define travaN 1000 ///количество травы
#define treeN 200 ///количество деревьев
#define mushN 400 ///количество грибов
#define flowerN 500 ///количество цветов

TPosition map[mapW][mapH]; ///массив координат карты
TPosition normalforlight[mapW][mapH]; ///массив координат нормалей
TTexure maptex[mapW][mapH]; ///массив координат для текстур

TPyramid pyramids[pyraN];///масиив пирамидок

TPlant trava[travaN];///массив травы
TPlant tree[treeN];///массив деревьев
TPlant mush[mushN];///массив грибов
TPlant flower[flowerN];///массив цветов

GLuint mapIndex[mapW-1][mapH-1][6];///массив индексов для карты
int mapIndexNum = sizeof(mapIndex) / sizeof(GLuint);///количесвто индексов карты

float pyramid[] = {0,0, 3, 1,1,0, 1,-1,0, -1,-1,0,
                   -1,1,0, 1,1,0};///форма одной пирамидки

float plant[] = {-0.5,0,0, 0.5,0,0, 0.5,0,1, -0.5,0,1,
                 0,-0.5,0, 0,0.5,0, 0,0.5,1, 0,-0.5,1};///форма одного растения
float plantUV[] = {0,1, 1,1, 1,0, 0,0, 0,1, 1,1, 1,0, 0,0};///грани одного растения
GLuint plantIndex[] = {0,1,2, 2,3,0, 4,5,6, 6,7,4};///массив индексов одного растения
int plantIndexNum = sizeof(plantIndex) / sizeof(GLuint);///количество индексов одного растения

GLuint
tex_trava_zelen, /// текстура травы зеленой
tex_bereza, /// текстура березы
tex_elka, /// текстура  елки
tex_trava_syhaya, /// текстура травы сухой
tex_good_flower, /// текстура хорошего цветка
tex_bad_flower, /// текстура плохого цветка
tex_red_mush, /// текстура красного гриба
tex_green_mush, /// текстура зеденого гриба
tex_blue_mush, /// текстура синего гриба
tex_zemla;/// текстура земли


/**функция изменения размеров окна
* @param width - ширина окна
* @param height - высота окна
 */
void WindowResize(int width, int height){

    glViewport(0, 0, width, height);
    float XandYDiff = width / (float) height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-XandYDiff, XandYDiff, -1, 1, 2, 800);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/**функция применения параметров камеры
 */
void Camera_Apply(){

    glRotatef(-camera.Xrot, 1,0,0);
    glRotatef(-camera.Zrot, 0,0,1);
    glTranslatef(-camera.x, -camera.y, -camera.z);
}

/**функция изменения положения камеры в зависимости от движений мышки
* @param xAngle - угол по горизонтали
* @param zAngle - угол по вертикали
 */
void Camera_Rotating(float xAngle, float zAngle){

    camera.Zrot += zAngle;
    if (camera.Zrot < 0 ) camera.Zrot +=360;
    if (camera.Zrot > 360 ) camera.Zrot -=360;
    camera.Xrot += xAngle;
    if (camera.Xrot < 0 ) camera.Xrot = 0 ;
    if (camera.Xrot > 180 ) camera.Xrot = 180;
}

/**функция расчета для перемещения камеры по оси z отсносительно ландшафта
* @param x - координата по x
* @param y - координата по y
 */
float OnFoot(float x, float y){

    if (!OnMap(x,y, mapW, mapH)) return 0;
    int INTx = (int)x;
    int INTy = (int)y;

    ///не мой код
    float height1 = ( (1-(x-INTx))*map[INTx][INTy].z + (x-INTx)*map[INTx+1][INTy].z);
    float height2 = ((1-(x-INTx))*map[INTx][INTy+1].z + (x-INTx)*map[INTx+1][INTy+1].z );
    return (1-(y-INTy)) * height1 + (y-INTy) * height2;
    ///конец не моего кода
}

/**фукнция отображения кнопки
* @param btn - кнопка
 */
void Button_Show(TButton btn){

    glEnableClientState(GL_VERTEX_ARRAY);
    if (btn.light) glColor3f(1,0,0);
    else glColor3f(0,1,0);
    glVertexPointer(2, GL_FLOAT, 0, btn.vert);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
}

/**фукнция проверки нахождения мыши на кнопке
* @param x - координата кнопки по x
* @param y - координата кнопки по y
* @param btn - кнопка
 */
bool InButton(int x, int y, TButton btn){

    return (x > btn.vert[0]) && (x < btn.vert[4]) &&
           (y > btn.vert[1]) && (y < btn.vert[5]);
}

/**функция отображения меню
* @param width - ширина окна
* @param height - высотка окна
 */
void Show_Menu(int width, int height){

    glPushMatrix();
    glLoadIdentity();
    glOrtho(0,width, height,0,-1,1);
    for (int i = 0; i < btnCnt; i++)
        Button_Show(btn[i]);
    glPopMatrix();
}

/**функция создания холомов на карте
* @param x - координата холма по x
* @param y - коордианта холма по y
* @param radius - радиус холма
* @param height - высота холма
*/
void MakeSomeHill(int x, int y, int radius, int height){

    for (int i = x-radius; i <= x+radius; i++)
        for (int j = y-radius; j<= y+radius; j++){
            if (OnMap(i,j, mapW, mapH)){
                ///не мой способ создания холма
                float lenght = sqrt(pow(x-i,2) + pow(y-j,2));
                if (lenght < radius){
                    lenght = lenght / radius * M_PI_2;
                    map[i][j].z += cos(lenght) * height;
                    ///конец не моего кода
                }
            }
        }
}

/**
 * функция создания нормалей на карте
*/
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

/**функция получения текстур из файлов и их обработки
* @param file -  путь к файлу
* @param texture - текстура полученная
*/
void Get_Texture(char *file, GLuint &texture){

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int width, height, n;
    unsigned char *data = stbi_load(file, &width, &height, &n, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                 n==4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}

/**функция инициализации карты
 */
void Map_Init(){


     ///получение текстур
    Get_Texture("textures/zemla.png", tex_zemla);
    Get_Texture("textures/good_flower.png", tex_good_flower);
    Get_Texture("textures/bad_flower.png", tex_bad_flower);
    Get_Texture("textures/bereza.png", tex_bereza);
    Get_Texture("textures/elka.png", tex_elka);
    Get_Texture("textures/trava_syhaya.png", tex_trava_syhaya);
    Get_Texture("textures/trava_zelen.png", tex_trava_zelen);
    Get_Texture("textures/red_mush.png", tex_red_mush);
    Get_Texture("textures/green_mush.png", tex_green_mush);
    Get_Texture("textures/blue_mush.png", tex_blue_mush);

    ///инициализация пола/земли
    for (int i = 0; i < mapW; i++)
        for (int j = 0; j < mapH; j++)
        {
            maptex[i][j].a = i;
            maptex[i][j].b = j;

            map[i][j].x = i;
            map[i][j].y = j;
            map[i][j].z = (rand() % 10) * 0.05;
        }

    ///инициализация позиции индексов пола/земли
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
    ///инициализация холмов
    for (int i = 0; i<hillN; i++){
        MakeSomeHill(rand() % mapW, rand() % mapH, 10 + rand() % 50, rand() % 10);
    }

    ///получение нормалей для света
    for (int i = 0; i < mapW; i++)
        for (int j = 0; j < mapH; j++) {
            Make_Normals(map[i][j], map[i + 1][j], map[i][j + 1], &normalforlight[i][j]);
        }

    ///инициализация пирадмидок
    for (int i = 0; i < pyraN; i++){
        float dc = (rand() %20) *0.01;
        float ds = (rand() %120) *0.01;
        pyramids[i].clr.r = 0.85;
        pyramids[i].clr.g = 0.77 + dc;
        pyramids[i].clr.b = 0.65 + dc;

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

    ///инициализация травы
    for (int i = 0; i < travaN; i++){
        if (i % 2 == 0) trava[i].tex = tex_trava_syhaya;
        if (i % 2 == 1) trava[i].tex = tex_trava_zelen;

        trava[i].scl = 2 + (rand()% 5 ) *0.1;

        trava[i].pos.x = 1 + rand() % mapW - 1;
        trava[i].pos.y = 1 + rand() % mapH - 1;
        trava[i].pos.z = map[(int)trava[i].pos.x][(int)trava[i].pos.y].z;
    }

    ///инициализация деревьев
    for (int i = 0; i < treeN; i++){
        if (i % 2 == 0) tree[i].tex = tex_bereza;
        if (i % 2 == 1) tree[i].tex = tex_elka;

        tree[i].scl = 15 + (rand()% 5 ) *0.1;

        tree[i].pos.x = 1 + rand() % mapW - 1;
        tree[i].pos.y = 1 + rand() % mapH - 1;
        tree[i].pos.z = map[(int)tree[i].pos.x][(int)tree[i].pos.y].z;
    }

    ///инициализация цветов
    for (int i = 0; i < flowerN; i++){
        if (i % 2 == 0) flower[i].tex = tex_good_flower;
        if (i % 2 == 1) flower[i].tex = tex_bad_flower;

        flower[i].scl = 2 + (rand()% 5 ) *0.1;

        flower[i].pos.x = 1 + rand() % mapW - 1;
        flower[i].pos.y = 1 + rand() % mapH - 1;
        flower[i].pos.z = map[(int)flower[i].pos.x][(int)flower[i].pos.y].z;
    }

    ///инициализация грибов
    for (int i = 0; i < mushN; i++){
        if (i % 3 == 0) mush[i].tex = tex_red_mush;
        if (i % 3 == 1) mush[i].tex = tex_green_mush;
        if (i % 3 == 2) mush[i].tex = tex_blue_mush;

        mush[i].scl = 2 + (rand()% 5 ) *0.1;

        mush[i].pos.x = 1 + rand() % mapW - 1;
        mush[i].pos.y = 1 + rand() % mapH - 1;
        mush[i].pos.z = map[(int)mush[i].pos.x][(int)mush[i].pos.y].z;
    }


}


/**
 *    инициализация самой игры со всеми ее параметрами
 *    @param hwnd - дексриптор окна
 */
void Game_Init(HWND hwnd){

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);

    Map_Init();

    RECT rect;
    GetClientRect(hwnd, &rect);
    WindowResize(rect.right, rect.bottom);
}


/**функция обработки движений игрока
 * @param hwnd - дескриптор окна
 */
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

    for (int i; i<pyraN; i++){
        if (PyraHit(camera.x, camera.y, pyramids[i].pos.x,pyramids[i].pos.y)){
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

/**функция рисования мира
 */
void Game_Show(){

    glClearColor((178.f / 255.f), 211.f / 255.f, 230.f / 255.f, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glPushMatrix();
    Camera_Apply();

    ///настройка света
    GLfloat sunpoint[] = {1,0,2,0};
    glLightfv(GL_LIGHT0, GL_POSITION, sunpoint);

    ///рисование пола/земли
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, map);
        glTexCoordPointer(2, GL_FLOAT, 0, maptex);
        glColor3f(0.6, 0.6, 0.6);
        glNormalPointer(GL_FLOAT, 0, normalforlight);
        glBindTexture(GL_TEXTURE_2D, tex_zemla);
        glDrawElements(GL_TRIANGLES, mapIndexNum, GL_UNSIGNED_INT, mapIndex);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);


    ///рисование пирамидок
    glEnableClientState(GL_VERTEX_ARRAY);
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

    ///рисование травы
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.99);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &plant);
        glTexCoordPointer(2, GL_FLOAT, 0, plantUV);
        glColor3f(0.75, 0.75, 0.75);
        for (int i=0; i < travaN; i++) {
            glBindTexture(GL_TEXTURE_2D, trava[i].tex);
            glPushMatrix();
            glTranslatef(trava[i].pos.x, trava[i].pos.y, trava[i].pos.z);
            glScalef(trava[i].scl, trava[i].scl, trava[i].scl);
            glDrawElements(GL_TRIANGLES, plantIndexNum, GL_UNSIGNED_INT, plantIndex);
            glPopMatrix();
        }
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    ///рисование деревьеьв
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.99);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &plant);
    glTexCoordPointer(2, GL_FLOAT, 0, plantUV);
    glColor3f(0.75, 0.75, 0.75);
    for (int i=0; i < treeN; i++) {
        glBindTexture(GL_TEXTURE_2D, tree[i].tex);
        glPushMatrix();
        glTranslatef(tree[i].pos.x, tree[i].pos.y, tree[i].pos.z);
        glScalef(tree[i].scl, tree[i].scl, tree[i].scl);
        glDrawElements(GL_TRIANGLES, plantIndexNum, GL_UNSIGNED_INT, plantIndex);
        glPopMatrix();
    }
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    ///рисование цветов
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.99);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &plant);
    glTexCoordPointer(2, GL_FLOAT, 0, plantUV);
    glColor3f(0.75, 0.75, 0.75);
    for (int i=0; i < flowerN; i++) {
        glBindTexture(GL_TEXTURE_2D, flower[i].tex);
        glPushMatrix();
        glTranslatef(flower[i].pos.x, flower[i].pos.y, flower[i].pos.z);
        glScalef(flower[i].scl, flower[i].scl, flower[i].scl);
        glDrawElements(GL_TRIANGLES, plantIndexNum, GL_UNSIGNED_INT, plantIndex);
        glPopMatrix();
    }
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    ///рисование грибов
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.99);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &plant);
    glTexCoordPointer(2, GL_FLOAT, 0, plantUV);
    glColor3f(0.75, 0.75, 0.75);
    for (int i=0; i < mushN; i++) {
        glBindTexture(GL_TEXTURE_2D, mush[i].tex);
        glPushMatrix();
        glTranslatef(mush[i].pos.x, mush[i].pos.y, mush[i].pos.z);
        glScalef(mush[i].scl, mush[i].scl, mush[i].scl);
        glDrawElements(GL_TRIANGLES, plantIndexNum, GL_UNSIGNED_INT, plantIndex);
        glPopMatrix();
    }
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();
}


/**
 *     функция обработки действий с окном
 */
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    switch (uMsg)
    {
        case WM_CLOSE:
            ///выход из окна
            PostQuitMessage(0);
            break;

        case WM_LBUTTONDOWN:
            ///нажатие левой кнопки мыши
            for (int i = 0; i < btnCnt; i++)
                if (InButton(LOWORD(lParam), HIWORD(lParam), btn[i])) {
                    std::cout << btn[i].name << '\n';
                    if (strcmp(btn[i].name, "quit")==0)
                        PostQuitMessage(0);
                }
            break;

        case WM_MOUSEMOVE:
            ///движение мыши
            for (int i = 0; i < btnCnt; i++)
                btn[i].light =  InButton(LOWORD(lParam), HIWORD(lParam), btn[i]);
            break;

        case WM_SIZE:
            ///изменение размера окна
            WindowResize(LOWORD(lParam), HIWORD(lParam));
            break;

        case WM_SETCURSOR:
        ///прятание курсора
            ShowCursor(FALSE);
            break;

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

/**
 *     стандартная инициализауия OpenGL
 */
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

/**
 *     стандартное отключение OpenGL
 */
void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC){

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hWnd, hDC);
}


