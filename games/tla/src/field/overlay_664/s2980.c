#include "ov664.h"

// When the controlled object steps onto a new cell, saves the 1 by 3 cells
// below it at (73, 72) and stamps the cells kept at (72, 72) in their place.
void Func_02002980(void)
{
    struct FieldActor *object = Object_GetById(Func_02005f84());
    s32 x = object->x >> 20;
    s32 y = object->y >> 20;
    s32 z = object->z >> 20;
    s32 key = (x << 16) + z;

    if (key != Data_02006c0c) {
        Data_02006c0c = key;
        Data_020073dc = 1;
        Data_020073e0 = 3;
        Data_020073e4 = x;
        Data_020073e8 = z - y + 64;
        CopyCells(x, Data_020073e8, 1, 3, 73, 72);
        CopyCells(72, 72, Data_020073dc, Data_020073e0, Data_020073e4, Data_020073e8);
    }
}

// As Func_02002980, one row nearer.
void Func_02002a14(void)
{
    struct FieldActor *object = Object_GetById(Func_02005f84());
    s32 x = object->x >> 20;
    s32 y = object->y >> 20;
    s32 z = object->z >> 20;
    s32 key = (x << 16) + z;

    if (key != Data_02006c0c) {
        Data_02006c0c = key;
        Data_020073dc = 1;
        Data_020073e0 = 3;
        Data_020073e4 = x;
        Data_020073e8 = z - y + 63;
        CopyCells(x, Data_020073e8, 1, 3, 73, 72);
        CopyCells(72, 72, Data_020073dc, Data_020073e0, Data_020073e4, Data_020073e8);
    }
}
