#include "types.h"
#include "fixed_math.h"

extern s32 Data_02000240[];

void BattleEffect_FinishHeavyImpact(s32 arg)
{
    s8 *head;
    struct {
        s32 x;
        s32 y;
        s32 z;
    } pos;
    s32 value;
    s8 *zptr;
    s32 base;
    s32 id;
    s32 count;
    void *ctx;
    void *work;

    id = arg;
    ctx = Object_GetById();
    /* The single-pass block preserves the first call's argument schedule. */
    do
    {
        if (ctx == ((void *)0))
        {
            return;
        }
        Func_080958a8();
        base = (*((s32 *)0x03001F30));
        Func_080b0048(0x20118C);
        Audio_PlayCue(0xAD);
        Func_080925cc(id, 1);
    }
    while (0);
    Audio_PlayCue(0xAE);
    Func_080925cc(id, 1);
    /* A second boundary gives the third repeated call its observed order. */
    do
    {
        Audio_PlayCue(0xAF);
        Func_080925cc(id, 1);
        WaitFrames(0x14);
        Audio_PlayCue(0x8C);
        (*((s32 *)(((s8 *)ctx) + 0x6C))) = 0x0809592D;
        WaitFrames(0x28);
        Audio_PlayCue(0x99);
        Func_08092560(id, 0xC, 0x16);
        pos.x = ((s32)(*((s32 *)(((s8 *)ctx) + 8))));
        pos.y = ((s32)(*((s32 *)(((s8 *)ctx) + 0xC))));
        pos.z = ((s32)(*((s32 *)(zptr = (((s8 *)ctx) + 0x10)))));
        NormalizeVector(&pos);
        Object_Destroy(ctx);
        Audio_PlayCue(0xA4);
        work = (base + 0x58);
        count = 0x17;
        do
        {
            Func_0809ba90(work, 0x11C, pos.x, pos.z);
            Func_0809ba7c(work, 0x08095939);
            /* This boundary keeps the work pointer ahead of the constant. */
            do
            {
                Func_0809ba70(work, 7);
            }
            while (0);
            Func_08009248(*((s32 *)(head = (((s8 *)work) + 0))), ((u32)(Random16() * 7)) >> 0x10);
            value = (Func_080022f4(Random16(), 3) + 0x10000);
            (*((s32 *)(((s8 *)work) + 0x2C))) = value;
            (*((s32 *)(((s8 *)work) + 0x28))) = value;
            count = (count - 1);
            WaitFrames(1);
            work += 0x48;
        }
        while (count >= 0);
    }
    while (0);
    WaitFrames(0x3C);
    ObjectMotion_ArmCallback(Data_02000240[125], 0x4000, 0);
    WaitFrames(0x14);
    Object_SetMode(Object_GetById(Data_02000240[125]), 0x1C);
    WaitFrames(0x28);
    Audio_PlayCue(0xA4);
    WaitFrames(0x64);
    Func_080b0050();
    Func_080958e4();
}
