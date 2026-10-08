#include "kabe.h"
#include "scene_ids.h"

void Func_02001250(s32 show, s32 first, u32 count);

void Func_02001200(s32 show)
{
    if (gPartyState.scene == (s32)&SceneId_Kabe6)
        Func_02001250(show, 9, 3);
}

void Func_02001228(void)
{
    Func_02001200(1);
    GameFlag_SetBit(0x208);
}

void Func_0200123c(void)
{
    Func_02001200(0);
    GameFlag_ClearBit(0x208);
}

void Func_02001250(s32 show, s32 first, u32 count)
{
    u32 i;

    for (i = 0; i < count;) {
        u8 *flags = &Object_GetById(first + i)->priority_flags;
        u32 next = i + 1;

        if (show)
            *flags &= ~0x10;
        else
            /* FAKEMATCH: the native ties the flag or to the constant's register. */
            *flags = *flags + 0x10 - (*flags & 0x10);
        i = next;
    }
}

void Func_02001290(void)
{
    ObjectMotion_SetActionVariant(gPartyState.current_owner, 1);
    GameFlag_SetBit(0x20a);
}

void Func_020012b4(void)
{
    ObjectMotion_SetActionVariant(gPartyState.current_owner, 2);
    Object_GetById(gPartyState.current_owner)->priority_flags |= 1;
    GameFlag_ClearBit(0x20a);
}
