#include "airs_rock_inside.h"

s32 Func_02000690(u32 layer, s32 x, s32 y);
void Func_02005efc(s32 countdown);
void ObjectMotion_ResetAndSetPositionInMode2(s32 actor, s32 x, s32 z);

// When layer 1's cell at the leader's x and z - 1 reads 30, runs the four
// steps 65, 66, 67 and 62 and moves the leader to (312, 792).
void Func_02003170(void)
{
    struct FieldActor *leader = Object_GetById(gPartyState.currentOwner);
    s32 width;
    s32 height;

    if (Func_02000690(1, leader->x >> 20, (leader->z >> 20) - 1) >> 8 != 30)
        return;
    Func_02005e14();
    Func_02005f2c(0);
    Func_02005f94(188);
    width = 1;
    height = 2;
    Func_02005d9c(65, 76, 19, 92, width, height);
    Battle_WaitMode0(8);
    Func_02005d9c(66, 76, 19, 92, width, height);
    Battle_WaitMode0(8);
    Func_02005d9c(67, 76, 19, 92, width, height);
    Battle_WaitMode0(8);
    Func_02005d9c(62, 76, 19, 92, width, height);
    Battle_WaitMode0(15);
    Object_GetById(gPartyState.currentOwner)->motionFlags = 0;
    Object_GetById(gPartyState.currentOwner)->speed = Object_GetById(gPartyState.currentOwner)->acceleration = 0x8000;
    Func_02005f94(123);
    ObjectMotion_ResetAndSetPositionInMode2(gPartyState.currentOwner, 312, 792);
    Battle_WaitMode0(5);
    Func_02005efc(15);
    Func_02005e1c();
}
