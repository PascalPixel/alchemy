/* Draft, differ 16 of 73: register choice only. The native keeps the object in r7 and the counter
 * in r6; here the object outranks the counter (8 references over 52 insns against 7 over 36) and
 * takes r6. Tried: count-up loops, an induction expression for the speed, other counter types
 * (31 to 34), inline wrappers on the calls before the loop (16), a register pin on the object (13,
 * but the counter then shares r7), three minutes of permute (nothing better). */
#include "../../../../games/tla/src/field/overlay_678/local.h"

void Func_0200065c(void)
{
    struct ObjectRuntime *object = Object_GetById(8);
    s32 speed;
    s32 i;

    Func_02002898();
    Func_02002968(0);
    ObjectMotion_SetAngleToward(gPartyState.current_owner, 8, 0);
    Func_02002908(gPartyState.current_owner, 256, 30);
    Motion_CamBounds(0x1980000, -1, 0x1380000, 1);
    Func_02002928();
    Func_02002910_(8, 257);
    speed = 0x8000;
    for (i = 2; i >= 0; i--) {
        object->velocity_y = 0x20000;
        Func_020029b0(152);
        object->speed_limit = speed;
        object->acceleration = speed / 2;
        ObjectMotion_CommitPositionAndActivate(8, 0, 8);
        speed += 0x4000;
    }
    Func_020029b0(310);
    object->velocity_y = 0x40000;
    ObjectMotion_CommitPositionAndActivate(8, 0, 8);
    Battle_WaitMode0(60);
    GameFlag_SetBit(0x310);
    Func_020028a0();
}
