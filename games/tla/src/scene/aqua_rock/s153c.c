#include "local.h"

void Func_02002998(void);
void Func_020029a0(void);
void Func_02002a48(s32 a);
void Func_02001d30(struct ObjectRuntime *object);
void ObjectMotion_SetPositionAndCommit(s32 id, s32 x, s32 z);

void Func_0200153c(void)
{
    struct ObjectRuntime *object = Object_GetById(gPartyState.current_owner);
    s32 saved = object->motion_flags;
    s32 i;
    s32 angle;

    Func_02002998();
    Func_02002a48(0);
    Event_StartScreenTransition();
    ObjectDispatch_SetSingleChildFlags(object, 0);
    object->motion_flags = 0;
    object->y = -0x400000;
    ObjectMotion_SetSpeedParameters(gPartyState.current_owner, 0x20000, 0x10000);
    for (i = 23; i >= 0; i--) {
        object->y += 0x20000;
        angle = object->angle;
        /* FAKEMATCH: keeps the angle load before its step is built (differ 2 without) */
        __asm__("" : : "r"(angle));
        angle += 0x2000;
        object->angle = angle;
        WaitFrames(1);
    }
    Func_02001d30(object);
    object->motion_flags = saved;
    object->velocity_y = 0xc0000;
    ObjectDispatch_SetSingleChildFlags(object, 1);
    ObjectMotion_SetPositionAndCommit(gPartyState.current_owner, 680, 912);
    Func_020029a0();
}
