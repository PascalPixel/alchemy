#include "scene.h"

void Func_020021d4(void)
{
    ((struct SceneEventWork *)Ram_HeapSlots->event_work)->unknown_16a = 0;
    Data_02004808 = 0;
}

s32 Func_020021f0(struct FieldActor *object)
{
    s32 x = object->target_x;

    /* FAKEMATCH: loads the target before it builds the constant. */
    __asm__("");
    if (x == ACTOR_NO_TARGET && object->target_z == x)
        return 1;
    return 0;
}
