#include <iostream>
#include <math.h>
bool OnMap(float x, float y, int mapX, int mapY){
    ///функция проверки находится ли объект на карте
    return (x >= 0) && (x < mapX) && (y >= 0) && (y <=mapY );
}
