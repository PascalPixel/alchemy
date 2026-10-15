#include "local.h"

void Func_02004cd8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02004d40(void);
void Func_02004d48(void);
void Func_02004e28(s32 a);
void Func_02004f30(s32 cue);

void Func_020023e8(void)
{
    s32 width = 1;
    s32 height = 2;

    Func_02004d40();
    Func_02004e90(0);
    Func_02004f30(188);
    Func_02004cd8(72, 67, 15, 19, width, height);
    Battle_WaitMode0(8);
    Func_02004cd8(73, 67, 15, 19, width, height);
    Battle_WaitMode0(8);
    Func_02004cd8(74, 67, 15, 19, width, height);
    Battle_WaitMode0(8);
    Func_02004cd8(75, 67, 15, 19, width, height);
    Battle_WaitMode0(15);
    Object_GetById(gPartyState.currentOwner)->motionFlags = 0;
    Object_GetById(gPartyState.currentOwner)->speedLimit =
        Object_GetById(gPartyState.currentOwner)->acceleration = 0x8000;
    Func_02004f30(123);
    ObjectMotion_ResetAndSetPositionInMode2_(gPartyState.currentOwner, 248, 328);
    Battle_WaitMode0(5);
    Func_02004e28(1);
    Func_02004d48();
}
