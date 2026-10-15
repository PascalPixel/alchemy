#include "garoh.h"
#include "call.h"

extern u8 MsgGaroh12[];

void Field_DispatchTypeHandler(s32 type);
void Motion_CamBounds(s32 x, s32 a, s32 z, s32 b);
void ObjectMotion_CommitPositionAndActivate(s32 id, s32 dx, s32 dz);
void Motion_SetVarCbAndRefresh(s32 id, s32 value);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 frames);
void Func_0200520c(s32 id, s32 x, s32 z);
void Func_02005214();
void Func_02005274(s32 id, s32 angle);
void Func_0200528c(s32 id, s32 a, s32 b);
void Func_020052b4(void);
void Func_02005304(s32 a, s32 b);
void Func_0200530c(s32 id, s32 a);
void Func_02005314(void);
void Func_0200531c(void);

void Func_02000808(void)
{
    s32 flag;
    s32 speed;

    void Func_020007a0(s32 id)
    {
        Func_02005304(144, 1);
        Func_0200530c(id, 0);
        Field_DispatchTypeHandler(1);
        Func_02005314();
        Func_0200531c();
    }

    void Func_020007d4(s32 id)
    {
        Func_02005304(144, 1);
        Func_0200530c(id, 0);
        Field_DispatchTypeHandler(3);
        Func_02005314();
        Func_0200531c();
    }

    flag = Value1((s32 (*)())GameFlag_Test, 0x320);
    if (flag == 0) {
        Call1((void (*)())GameFlag_SetBit, 0x320);
        Func_020051ac();
        Func_02005324(0);
        ObjectDispatch_SetSingleChildFlags(Object_GetById(22), 0);
        speed = 0x4000;
        Call4(Func_02005214, 22, 180 << 17, 199 << 16, speed);
        Motion_CamBounds(0x01670000, -1, 220 << 16, 1);
        Func_020052b4();
        Func_020007a0(22);
        Battle_WaitMode0(30);
        Call4(Func_02005214, 21, 180 << 17, 199 << 16, speed);
        ObjectMotion_CommitPositionAndActivate(21, 0, 16);
        Func_020007d4(22);
        Func_0200524c((s32)MsgGaroh12);
        Call2((void (*)())Func_02005274, 21, 160 << 7);
        Motion_SetVarCbAndRefresh(21, 2);
        Battle_WaitMode0(20);
        Call3((void (*)())ObjectMotion_ArmCallback, 4, 224 << 8, 0);
        Func_0200525c(21, 0, 10);
        Func_0200528c(21, 0x102, 50);
        Object_GetById(21)->action_flags &= ~1;
        ObjectMotion_ArmCallback(21, speed, 0);
        Battle_WaitMode0(30);
        Func_020007a0(22);
        Battle_WaitMode0(20);
        Call3((void (*)())ObjectMotion_CommitPositionAndActivate, 21, 0, -16);
        Object_GetById(21)->motion_flags = flag;
        Call3((void (*)())ObjectMotion_CommitPositionAndActivate, 21, 0, -16);
        Func_0200520c(21, 0, 0);
        Battle_WaitMode0(10);
        Func_020007d4(22);
        Battle_WaitMode0(60);
        Func_0200520c(22, 0, 0);
        Func_020051b4();
    }
}
