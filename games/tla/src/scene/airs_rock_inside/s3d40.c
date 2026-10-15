#include "airs_rock_inside.h"

// As Func_02002980 with a 1 by 2 block: saves the cells 64 columns across
// from the controlled object at (73, 72) and stamps the cells kept at
// (70, 74) in their place.
void Func_02003d40(void)
{
    struct FieldActor *object = Object_GetById(Func_02005f84());
    s32                x = object->x >> 20;
    s32                y = object->y >> 20;
    s32                z = object->z >> 20;
    s32                key = (x << 16) + z;

    if (key != gData_02006c0c)
    {
        gData_02006c0c = key;
        gData_020073dc = 1;
        gData_020073e0 = 2;
        gData_020073e4 = x + 64;
        gData_020073e8 = z - y;
        CopyCells(gData_020073e4, gData_020073e8, 1, 2, 73, 72);
        CopyCells(70, 74, gData_020073dc, gData_020073e0, gData_020073e4, gData_020073e8);
    }
}
