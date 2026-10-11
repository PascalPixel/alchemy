#include "ov697.h"

static __inline__ void Map_CopyCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02005818(a, b, c, d, e, f);
}

void Func_0200106c(void)
{
    Func_02000e40(16, -1, 0, 0);
}

void Func_02001080(void)
{
    Func_02000e40(16, 1, 0, 0);
}

void Func_02001090(void)
{
    Func_02000e40(17, -1, 0, 0);
}

void Func_020010a4(void)
{
    Func_02000e40(17, 1, 0, 0);
}

void Func_020010b4(void)
{
    Func_02000e40(18, 0, -1, 1);
}

void Func_020010c8(void)
{
    Func_02000e40(18, 0, 1, 1);
}

void Func_020010d8(void)
{
    struct PartyState *party = &gPartyState;

    /* FAKEMATCH: loads the party base before it builds the field offset. */
    __asm__("");
    if (party->render_mode == 2)
        Func_020059c8();
    else
        Func_02000e40(18, 0, 1, 1);
}

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

        /* FAKEMATCH: loads the leader height before it builds the threshold. */
        __asm__("");
        if (y < 0x200000)
            leader->y = 0x200000;
    }
}

void Func_020011c0(void)
{
    Object_GetById(gPartyState.current_owner)->terrain_id = 2;
}

void Func_020011dc(void)
{
    Object_GetById(gPartyState.current_owner)->terrain_id = 0;
}

void Func_020011f8(void)
{
    Map_CopyCells(12, 21, 1, 1, 19, 21);
    GameFlag_SetBit(0x374);
}
