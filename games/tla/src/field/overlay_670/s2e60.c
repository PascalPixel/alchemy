#include "ov670.h"

void Func_02002e60(void)
{
    struct Ov670Object *leader = Object_GetById(gPartyState.current_owner);

    do { } while (0); /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ loads the leader's height before it builds the limit (2 differ without it). */
    if (leader->y > 0x100000) {
        Func_02005b0c(29, 0, 0);
        ObjectMotion_SetActionVariant(30, 2);
    } else {
        Ov670_ObjSetPos(Object_GetById(29), 0x780000, 0, 0xa80000);
        ObjectMotion_SetActionVariant(30, 3);
    }
}

void Func_02002eb0(void)
{
    if (Data_02007950 != 0) {
        struct Ov670Object *object = Object_GetById(27);

        Object_SetPositionAndResetMotion(Data_02007950, object->x, object->y + 0xc0000, object->z);
    }
}
