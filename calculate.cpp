/**
 * проверка столкновения с пирамидой
 * @param x, y - позиция объекта
 * @param mapX, mapY - позиция на карте
 */
bool OnMap(float x, float y, int mapX, int mapY){
    ///функция проверки находится ли объект на карте
    return (x >= 0) && (x < mapX) && (y >= 0) && (y <=mapY );
}

/**
 * проверка столкновения с пирамидой
 * @param cameraX, cameraY - позиция камеры
 * @param PyraX, PyraY - позиция пирамиды
 */
bool PyraHit(float cameraX, float cameraY, int PyraX, int PyraY){

    return (int)cameraX >= PyraX-1 && (int)cameraX <= PyraX+1
           && (int)cameraY >= PyraY-1 && (int)cameraY <= PyraY+1;
}
