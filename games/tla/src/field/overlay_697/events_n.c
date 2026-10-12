#include "ov697.h"

void Func_02001104(s32 id, s32 a, s32 frame)
{
    if (frame == 30) {
        struct ObjectRuntime *object = Object_GetById(id);
        s32 height;

        object->x = object->action << 16;
        object->z = (s16)object->unknown_66 << 16;
        height = Map_GetTerrainHeight(0, object->x, object->z);
        object->terrain_height = height;
        object->y = height;
        Func_0200078c(id);
        GameFlag_SetBit(id + 0x353);
    }
}

void Func_02001148(s32 a, s32 b)
{
    Func_02001104(16, a, b);
}

void Func_02001158(s32 a, s32 b)
{
    Func_02001104(17, a, b);
}

void Func_02001168(s32 a, s32 b)
{
    Func_02001104(18, a, b);
}

void Func_02001178(void)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.current_owner);
    struct ObjectRuntime *block = Object_GetById(15);

    if (block->x >> 20 == leader->x >> 20 && block->z >> 20 == leader->z >> 20) {
        s32 y = leader->y;

        /* FAKEMATCH: loads the leader height before it builds the threshold (differ 2 without it, as in Func_020007b4). */
        do { } while (0);
        if (y < 0x200000)
            leader->y = 0x200000;
    }
}
