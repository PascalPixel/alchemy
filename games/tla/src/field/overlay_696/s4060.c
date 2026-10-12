#include "ov696.h"

void Func_02003d30(void);
void Func_02003f78(void);

void Func_02004060(void)
{
    if (GameFlag_Test(0x94c)) {
        Ov696_MapCopy(47, 19, 38, 19, 9, 11);
        Ov696_BlendStart(1, 5, 9, 1, 6, 29);
        Object_SetPartAttribute(Object_GetById(14), 6);
    } else if (!GameFlag_Test(0x109)) {
        s32 angle = 0x4000;

        Ov696_ObjPut(15, 0xa80000, 0x2380000, angle);
        Ov696_ObjPut(17, 0xd80000, 0x2180000, angle);
        Ov696_ObjPut(16, 0x780000, 0x2180000, angle);
        WaitFrames(1);
    }
    Object_SetModeById(8, 2);
    Object_SetModeById(9, 2);
    Object_SetModeById(10, 2);
    Object_SetModeById(11, 2);
    Object_SetModeById(12, 2);
    Object_SetModeById(13, 2);
    if (!GameFlag_Test(0x109)) {
        s32 entrance = gPartyState.entrance;

        if (entrance == 11)
            Func_02003d30();
        else if (entrance == 9)
            Func_02003f78();
    }
    if (GameFlag_Test(0x9fe)) {
        Func_02004934(15, 0, 0);
        Func_02004934(17, 0, 0);
        Func_02004934(16, 0, 0);
        Ov696_BlendStart(10, 12, 3, 1, 10, 13);
        Ov696_BlendStart(15, 12, 3, 1, 15, 13);
        Ov696_BlendStart(9, 20, 3, 1, 9, 21);
    }
}

void Func_020041b8(void)
{
    struct Ov696Object *object;

    Object_SetModeById(8, 2);
    Object_SetModeById(9, 2);
    Object_SetModeById(10, 2);
    Object_SetModeById(11, 2);
    Object_SetModeById(12, 2);
    Object_SetModeById(13, 2);
    Object_GetById(14)->motion_flags = 0;
    if (GameFlag_Test(0x94e)) {
        Object_SetPositionAndResetMotion(Object_GetById(14), 0x2080000, 0x200000, 0xb80000);
        WaitFrames(1);
        Ov696_BlendStart(30, 11, 1, 1, 32, 11);
        WaitFrames(10);
    }
    if (GameFlag_Test(0x9e1)) {
        Ov696_MapCopy(64, 0, 76, 2, 3, 3);
        Ov696_BlendStart(12, 5, 3, 1, 12, 6);
    }
    if (GameFlag_Test(0x94d))
        Ov696_MapCopy(48, 40, 101, 11, 1, 2);
    else
        Func_02004934(17, 0, 0);
    object = Object_GetById(16);
    object->collision_flags = 8;
    object->scale_x = 0xcccc;
    object->scale_y = 0xcccc;
    ObjectDispatch_SetSingleChildFlags(Object_GetById(16), 0);
}
