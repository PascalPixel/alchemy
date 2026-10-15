#include "airs_rock_inside.h"

extern const u8 Data_02006084[];

void Func_02003dd4(void)
{
    Func_02005dac(26, 17, 1, 1, 8, 19);
    Func_02005dac(26, 17, 1, 1, 24, 19);
    Call6((void (*)())Func_02005d9c, 90, 17, 72, 19, 1, 2);
    Call6((void (*)())Func_02005d9c, 90, 17, 88, 19, 1, 2);
}

void Func_02003e28(void)
{
    Func_02005dac(25, 17, 1, 1, 8, 19);
    Func_02005dac(25, 17, 1, 1, 24, 19);
    Call6((void (*)())Func_02005d9c, 89, 17, 72, 19, 1, 2);
    Call6((void (*)())Func_02005d9c, 89, 17, 88, 19, 1, 2);
}

void Func_02003e7c(struct FieldActor *source)
{
    struct FieldActor *object = (struct FieldActor *)Value4((s32 (*)())Engine_ObjectCreate, 0x2a0, source->x, source->y, source->z);

    WORD_AT(object, 28) = 0x6666;
    WORD_AT(object, 24) = 0x6666;
    object->motion_flags = 0;
    Object_SetPartAttribute(object, 2);
    Object_SetSpritePriority(object, 1);
    Func_02005d5c(object, 1);
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Animation_SetStateFlags(object, 1);
    Func_02005d6c(object, Data_02006084);
}
