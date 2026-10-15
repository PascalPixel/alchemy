#include "scene.h"

void Func_0200157c(void)
{
    if (Object_GetById(64))
        Func_02004090(64, 0, 0);
}

void Func_02001594(void)
{
    Func_02003ff0(64, 64, 72, 32, 1, 1);
    Func_02004000(64, 64, 1, 1, 8, 32);
    Func_02004000(64, 64, 1, 1, 8, 96);
}

void Func_020015d4(s32 mode)
{
    if (mode == 1)
    {
        Func_02001594();
        GameFlag_SetBit(0x88d);
    }
}

void Func_020015ec(s32 show)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov661Room8)
        Func_02001650(show, 11, 2);
    else if (scene == (s32)&SceneId_Ov661Room12)
        Func_02001650(show, 9, 7);
}

void Func_02001628(void)
{
    Func_020015ec(1);
    GameFlag_SetBit(0x20d);
}

void Func_0200163c(void)
{
    Func_020015ec(0);
    GameFlag_ClearBit(0x20d);
}

void Func_02001650(s32 show, s32 first, u32 count)
{
    u32 i;

    for (i = 0; i < count;)
    {
        u8 *flags = &Object_GetById(first + i)->priorityFlags;
        u32 next = i + 1;

        if (show)
            *flags &= ~0x10;
        else
            /* FAKEMATCH: the native ties the flag or to the constant's register. */
            *flags = *flags + 0x10 - (*flags & 0x10);
        i = next;
    }
}
