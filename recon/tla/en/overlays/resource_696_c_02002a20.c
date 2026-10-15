/* Draft, differ 516 of 556: the portrait mode task, ☀️'s Colosso_UpdateModeTask (scene/common/colosso_task.h) with overlay 696's variables, the French four-graphic case and a transform of u32 bitfields (the native writes scale and angle by read-modify-write). The logic and every constant agree; the allocation does not: the native keeps both sprite pointers in stack slots (frame 24, this form 20 with the write pointer in sl), the timer in r5 and the script address in r4, loads each target's address before its duration test, rematerializes y and the flags inside the case 1 and 3 loops and builds scale << 16 and &work before the move step. Tried: the attribute inline in the loops (520), the transform filled before the move step (559). */
#include "../../../../games/tla/src/scene/shaman_village/shaman_village.h"
#include "affine.h"
#include "vram_tab.h"
#include "io_reg.h"
#include "io_write_queue.h"

struct Sprite {
    u32 words[3];
};

struct ModeTransform {
    u32 scaleX : 16;
    u32 scaleY : 16;
    u32 angle : 16;
    u32 unk06 : 16;
};

extern u32 Data_020064d0[];
extern s16 Data_02005158[];
extern s16 Data_020064bc;
extern s16 *Data_020064c0;
extern s32 Data_020064a0;
extern s16 Data_02006504;
extern s16 Data_0200649c;
extern s16 Data_02006500;
extern s16 Data_020064a8;
extern s16 Data_020064a4;
extern s16 Data_02006498;
extern s16 Data_02006490;
extern s16 Data_02006508;
extern s16 Data_020064b4;
extern s16 Data_020064b8;
extern s16 Data_020064c4;
extern s16 Data_020064ac;
extern s16 Data_020064b0;
extern s16 Data_02006494;

void Resource_ResetEntry(s32 entry);
void Func_020047cc(void *entry, s32 slot);

void Func_02002a20(void)
{
    u32 *write = Data_020064d0;
    struct Sprite *sprite = (struct Sprite *)write;
    s32 tile = ResourceTableEntries[Data_02005158[0]].offset >> 5;
    u32 flags;
    s32 scale, blend, pos;
    s32 matrix, i, x, y, left;
    struct ModeTransform work;

commands:
    if (Data_020064bc == 0) {
        switch (*Data_020064c0++) {
        case 0x4000:
            Data_020064a0 = *Data_020064c0++ << 8;
            Data_02006504 = *Data_020064c0++;
            Data_0200649c = 0;
            break;
        case 0x3000:
            Data_02006500 = Data_02006504;
            Data_02006504 = *Data_020064c0++;
            Data_0200649c = *Data_020064c0++;
            Data_020064a8 = 0;
            break;
        case 0x1000:
            Data_02006498 = Data_020064a4;
            Data_020064a4 = *Data_020064c0++;
            Data_02006490 = *Data_020064c0++;
            Data_02006508 = 0;
            break;
        case 0x2000:
            Data_020064b8 = Data_020064b4;
            Data_020064b4 = *Data_020064c0++;
            Data_020064c4 = *Data_020064c0++;
            Data_020064ac = 0;
            break;
        case 0x7fff:
            Data_020064bc = *Data_020064c0++;
            break;
        case -1:
            Scheduler_RemoveCallbackFar(Func_02002a20);
            Resource_ResetEntry(Data_02005158[0]);
            return;
        }
        goto commands;
    }
    Data_020064bc--;
    if (Data_02006490 == 0) {
        scale = Data_020064a4;
    } else {
        s32 duration, start, progress, target;
        duration = Data_02006490;
        start = Data_02006498;
        target = Data_020064a4;
        progress = ++Data_02006508;
        scale = start + (target - start) * progress / duration;
        if (progress >= duration)
            Data_02006490 = 0;
    }
    if (Data_020064c4 == 0) {
        blend = Data_020064b4;
    } else {
        s32 duration, start, progress, target;
        duration = Data_020064c4;
        start = Data_020064b8;
        target = Data_020064b4;
        progress = ++Data_020064ac;
        blend = start + (target - start) * progress / duration;
        if (progress >= duration)
            Data_020064c4 = 0;
    }
    if (Data_0200649c == 0) {
        pos = Data_02006504;
    } else {
        s32 duration, start, progress, target;
        duration = Data_0200649c;
        start = Data_02006500;
        target = Data_02006504;
        progress = ++Data_020064a8;
        pos = start + (target - start) * progress / duration;
        if (progress >= duration)
            Data_0200649c = 0;
    }
    work.angle = 0;
    work.scaleX = scale;
    work.scaleY = scale;
    matrix = (s16)AffineMatrix_BuildForEffect((struct AffineTransform *)&work);
    Data_020064a0 += pos;
    pos = Data_020064a0 / 256;
    switch (Data_020064b0) {
    case 1:
    {
        u32 attr;
        i = 0;
        y = 56;
        flags = 0x80004000;
        attr = matrix << 25;
        for (; i < 4; i++) {
            x = pos + scale * (i * 32 - 48) / 256;
            left = x + 88;
            if ((u32)(x + 152) < 304) {
                x = left & 511;
                *write++ = 0;
                *write++ = (x << 16) | y | flags | attr | 0x700;
                *write++ = 0xf400 | tile;
                Func_020047cc(sprite++, 236);
            }
            tile += 8;
        }
        break;
    }
    case 3:
    {
        u32 attr;
        i = 0;
        y = 48;
        flags = 0x80004000;
        attr = matrix << 25;
        for (; i < 2; i++) {
            x = pos + scale * (i * 32 - 16) / 256;
            left = x + 88;
            if ((u32)(x + 152) < 304) {
                x = left & 511;
                *write++ = 0;
                *write++ = (x << 16) | y | flags | attr | 0x700;
                *write++ = 0xf400 | (tile + Data_02006494);
                Func_020047cc(sprite++, 236);
            }
            tile += 8;
        }
        break;
    }
#if defined(TLA_EDITION_FR)
    case 4:
    {
        u32 attr;
        i = 0;
        y = 64;
        flags = 0x80004000;
        attr = matrix << 25;
        for (; i < 2; i++) {
            x = pos + scale * (i * 32 - 16) / 256;
            left = x + 88;
            if ((u32)(x + 152) < 304) {
                x = left & 511;
                *write++ = 0;
                *write++ = (x << 16) | y | flags | attr | 0x700;
                *write++ = 0xf400 | (tile + Data_02006494);
                Func_020047cc(sprite++, 236);
            }
            tile += 8;
        }
        break;
    }
#else
    case 4:
        y = 48;
        flags = 0xc0004000;
        left = pos + 56;
        if ((u32)(pos + 120) < 304) {
            x = left & 511;
            *write++ = 0;
            *write++ = (x << 16) | y | flags | (matrix << 25) | 0x700;
            *write++ = 0xf400 | (tile + Data_02006494);
            Func_020047cc(sprite, 236);
        }
        break;
#endif
    case 2:
        y = 48;
        flags = 0x80000000;
        left = pos + 88;
        if ((u32)(pos + 152) < 304) {
            x = left & 511;
            *write++ = 0;
            *write++ = (x << 16) | y | flags | (matrix << 25) | 0x700;
            *write++ = 0xf400 | (tile + Data_02006494);
            Func_020047cc(sprite, 236);
        }
        break;
    }
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, 0x3f00, 0x20000);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, ((16 - blend) << 8) | blend, 0x20000);
}
