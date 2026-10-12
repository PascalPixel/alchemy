#include "ov6a9.h"

s32 Func_02000c38(struct FieldActor *object, s16 *position);

void Func_02000cdc(void)
{
    struct FieldActor *leader = Object_GetById(gPartyState.current_owner);
    s32 position[3];
    s32 x;

    /* FAKEMATCH: the one-pass block loads x before the offset is built (differ 2 without it). */
    do {
        x = leader->x;
    } while (0);
    x += 0x200000;
    position[0] = x;
    position[1] = leader->y;
    position[2] = leader->z;
    Func_02000c38(leader, (s16 *)position);
}
