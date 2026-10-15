#include "local.h"

void Func_02001328(void)
{
    Func_02003d24_(70, 8, 7, 6, 77, 8);
    Func_02003d1c_(38, 17, 7, 6, 13, 9);
    GameFlag_ClearBit(0x349);
}

void Func_02001364(struct ObjectRuntime *object)
{
    struct ObjectRuntime *child = Engine_ObjectCreate_(0x2a0, object->x, object->y, object->z);

    child->scale_y = 0x6666;
    child->scale_x = 0x6666;
    child->motion_flags = 0;
    Object_SetPartAttribute(child, 2);
    Object_SetSpritePriority(child, 1);
    Func_02003cc4(child, 1);
    ObjectDispatch_SetSingleChildFlags(child, 0);
    Animation_SetStateFlags(child, 1);
    Func_02003ccc(child, Data_02003fd8);
}
