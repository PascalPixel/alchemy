#include "ov670.h"

void Func_02002de0(void);
void Func_0200324c(void);

void Func_0200538c(void)
{
    ObjectDispatch_SetSingleChildFlags(Object_GetById(9), 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(10), 0);
    if (!GameFlag_Test(0x8fb))
        Func_02002de0();
    Func_0200324c();
    if (!GameFlag_Test(0x869))
        GameFlag_SetBit(0x201);
    Func_02005c64(Data_02006324);
    ObjectMotion_SetActionVariant(8, 2);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(11), 0);
    Object_GetById(11)->motion_flags = 0;
    if (GameFlag_Test(0x9ea)) {
        Ov670_ObjSetPos(Object_GetById(11), 208 << 15, 0, 148 << 17);
        Ov670_BlendStart(5, 18, 1, 1, 6, 18);
        WaitFrames(10);
    }
    Object_GetById(gPartyState.current_owner)->priority_flags |= 0x20;
    if (GameFlag_Test(0x86a))
        Func_020005e0();
    if (GameFlag_Test(0x204))
        Func_02000624();
    if (GameFlag_Test(0x205))
        Func_02000668();
    if (GameFlag_Test(0x206))
        Func_020006ac();
    if (GameFlag_Test(0x207))
        Func_020006f0();
    Func_02005b8c(12);
    Func_02005b8c(13);
    Func_02005b8c(14);
    ObjectMotion_SetActionVariant(12, 1);
    ObjectMotion_SetActionVariant(13, 1);
    ObjectMotion_SetActionVariant(14, 1);
}
