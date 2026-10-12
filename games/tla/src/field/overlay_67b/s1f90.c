#include "local.h"

void Func_02002998(void);
void Func_020029a0(void);
void Func_02002a48(s32 a);
void Func_02002a70(s32 cue);
void Func_02002a18(s32 a);
void Func_02001d30(struct ObjectRuntime *object);

void Func_02001f90(s32 arg0)
{
    struct ObjectRuntime *object = Object_GetById(gPartyState.current_owner);
    s32 i;
    s32 angle;

    Func_02002998();
    Func_02002a48(0);
    ObjectMotion_SetActionVariant(gPartyState.current_owner, 1);
    object->motion_flags = 0;
    /* FAKEMATCH: keeps the flag store before the next address is formed (differ 2 without) */
    __asm__("");
    object->priority_flags |= 0x20;
    Func_02002a70(215);
    Func_02002910(object, 18);
    Func_02002a70(612);
    for (i = 0; i < 60; i++) {
        if (i == 30)
            Event_FinishScreenTransition();
        object->y -= 0x4000;
        angle = object->angle;
        /* FAKEMATCH: keeps the angle load before its step is built (differ 2 without) */
        __asm__("" : : "r"(angle));
        angle += 0x1000;
        object->angle = angle;
        if ((i & 7) == 0)
            Func_02001d30(Object_GetById(15));
        WaitFrames(1);
    }
    Func_020029a0();
    Func_02002a18(arg0);
}
