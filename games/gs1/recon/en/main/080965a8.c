#include "types.h"
#include "fixed_math.h"

extern s32 Data_02000240[];

void Func_080965a8(s32 arg)
{
    s32 id;
    void *ctx;
    s32 base;
    s32 type;
    void *children[8];
    void **children_ptr;
    void **slot;
    s32 prev;
    s32 acc;
    s32 count;
    void *child;
    s32 last;
    struct
    {
        s32 x;
        s32 y;
        s32 z;
    } pos;
    void *work;
    s8 *state;

    id = arg;
    ctx = Object_GetById(id);
    if (ctx == ((void *)0))
    {
        return;
    }
    Func_080958a8();
    base = (*((s32 *)0x03001F30));
    Func_080b0048(0x00204084);
    WaitFrames(30);
    (*((u8 *)(((s8 *)ctx) + 91))) = 0;
    Audio_PlayCue(173);
    Func_080925cc(id, 1);
    Audio_PlayCue(175);
    Func_080925cc(id, 1);
    WaitFrames(20);
    Audio_PlayCue(152);
    Func_08092560(id, 3, 14);
    Audio_PlayCue(152);
    Func_08092560(id, 5, 16);
    Audio_PlayCue(152);
    Func_08092560(id, 7, 18);
    WaitFrames(20);
    type = (*((s16 *)(*((s32 *)((*((s32 *)(((s8 *)ctx) + 80))) + 40)))));
    children_ptr = children;
    slot = children_ptr;
    prev = (s32)ctx;
    acc = 0;
    count = 7;
    do
    {
        child = (void *)Func_080090c8(type, (*((s32 *)(((s8 *)ctx) + 8))), (*((s32 *)(((s8 *)ctx) + 12))), (*((s32 *)(((s8 *)ctx) + 16))));
        *slot = child;
        slot++;
        if (child != ((void *)0))
        {
            (*((s32 *)(((s8 *)child) + 28))) = 0xF000;
            (*((s32 *)(((s8 *)child) + 24))) = 0xF000;
            (*((u8 *)(((s8 *)child) + 85))) = 0;
            (*((u8 *)(((s8 *)child) + 35))) = 2;
            (*((u8 *)(((s8 *)child) + 90))) |= 1;
            (*((void **)(((s8 *)child) + 108))) = (void *)0x08096575;
            (*((u16 *)(((s8 *)child) + 6))) = (*((u16 *)(((s8 *)ctx) + 6)));
            Func_08009240(child, 9);
            Func_080091e0(child, 0);
            acc = (s32)Func_08096c48((void *)(*((s32 *)(((s8 *)child) + 80))), (void *)acc);
            (*((s32 *)(((s8 *)child) + 104))) = prev;
            prev = (s32)child;
        }
        count--;
    }
    while (count >= 0);
    last = (*((u8 *)(((s8 *)acc) + 28)));
    Audio_PlayCue(153);
    (*((u8 *)(((s8 *)ctx) + 85))) = 0;
    count = 14;
    do
    {
        (*((s32 *)(((s8 *)ctx) + 12))) += 0x80000;
        WaitFrames(1);
        count--;
    }
    while (count >= 0);
    Func_080090d0(ctx);
    slot = children_ptr;
    count = 7;
    do
    {
        child = *slot;
        slot++;
        count--;
        Func_080090d0(child);
    }
    while (count >= 0);
    if (last != 96)
    {
        Func_08003f3c(last);
    }
    WaitFrames(10);
    ObjectMotion_ArmCallback(Data_02000240[125], 0x4000, 0);
    WaitFrames(20);
    Object_SetMode(Object_GetById(Data_02000240[125]), 28);
    WaitFrames(20);
    pos.x = (*((s32 *)(((s8 *)ctx) + 8)));
    pos.y = (*((s32 *)(((s8 *)ctx) + 12)));
    pos.z = (*((s32 *)(((s8 *)ctx) + 16)));
    NormalizeVector(&pos);
    work = (void *)(base + 0x58);
    count = 23;
    do
    {
        Func_0809ba90(work, 240, pos.x, pos.z);
        EffectSlot_SetCallback(work, (void *)0x0809641d);
        EffectSlot_SetObjectMode(work, 7);
        Func_08009248((*((s32 *)work)), 9);
        count--;
        WaitFrames(1);
        work += 0x48;
    }
    while (count >= 0);
    WaitFrames(120);
    state = ((s8 *)base) + 152;
    count = 23;
    do
    {
        if (state[5] != 0)
        {
            state[0] = 2;
        }
        count--;
        state += 72;
    }
    while (count >= 0);
    WaitFrames(50);
    Func_080b0050();
    BattleEffect_ClearActiveSlotsAndScheduleUpdates();
}
