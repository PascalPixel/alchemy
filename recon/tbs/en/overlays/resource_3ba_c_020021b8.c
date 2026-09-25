/* NONMATCHING: 1264 bytes, candidate 1264, 268 differing halfwords, 158
 * halfword edits (2026-09-25). CommandInterpolationRenderer_Update, meant
 * for FIELD/KOROSSEO_KAWA/F_021B8.C as a single-overlay unit binding its
 * names at their runtime addresses (an import veneer's listing offset plus
 * 0x8000). Remaining: Rebuilt command loop, three signed interpolation
 * channels, affine parameter bitfields, sprite records and IO queue from
 * disassembly. Verified helper prototypes and every literal. Complete size
 * matches; remaining register lifetimes, zero-load width, and instruction
 * scheduling differ.
 * WALL: Register lifetimes across interpolation and sprite emission;
 * preserve the separate per-case shifted affine index. */
#include "TYPES.H"
#include "IO_WRITE_QUEUE.H"

struct Sprite { u32 words[3]; };
struct SpriteTile { u16 pad, base; };
struct SpriteTransform { unsigned x : 16; unsigned y : 16; unsigned angle : 16; unsigned pad : 16; };
/* FAKEMATCH: a halfword zero aggregate keeps the interior literal pools. */
struct Half { u16 value; };
extern struct SpriteTile Data_03001b10[];
extern s16 Data_0200c57c, Data_0200c79c, Data_0200c7f8, Data_0200c76c;
extern s16 Data_0200c7f0, Data_0200c77c, Data_0200c778, Data_0200c768;
extern s16 Data_0200c754, Data_0200c7fc, Data_0200c794, Data_0200c798;
extern s16 Data_0200c7a8, Data_0200c784, Data_0200c790, Data_0200c764;
extern s32 Data_0200c770;
extern s16 *Data_0200c7a0;
extern u32 Data_0200c7c0[];
extern s32 Main_080000d8(void (*fn)(void));
extern void Main_080001b8(s32 slot);
extern s32 Local_02003b00(s32 left, s32 right);
extern s32 Main_080001e0(struct SpriteTransform *work);
extern void Main_080001e8(void *sprite, s32 priority);

void CommandInterpolationRenderer_Update(void)
{
    u32 *write = Data_0200c7c0;
    struct Sprite *sprite = (struct Sprite *)write;
    s32 tile = Data_03001b10[Data_0200c57c].base >> 5;
    s32 scale, blend, pos, duration, start, progress;
    s32 matrix, i, x, y, left;
    u32 flags;
    struct SpriteTransform work;
    struct IoWriteQueue *queue;
    volatile u16 *ime;
    s32 saved, cnt;
    u32 *entry;

commands:
    if (Data_0200c79c != 0)
        goto render;
    {
        switch (*Data_0200c7a0++) {
        case 0x4000:
            Data_0200c770 = *Data_0200c7a0++ << 8;
            Data_0200c7f8 = *Data_0200c7a0++;
            Data_0200c76c = 0;
            break;
        case 0x3000:
            Data_0200c7f0 = Data_0200c7f8;
            Data_0200c7f8 = *Data_0200c7a0++;
            Data_0200c76c = *Data_0200c7a0++;
            Data_0200c77c = 0;
            break;
        case 0x1000:
            Data_0200c768 = Data_0200c778;
            Data_0200c778 = *Data_0200c7a0++;
            Data_0200c754 = *Data_0200c7a0++;
            Data_0200c7fc = 0;
            break;
        case 0x2000:
            Data_0200c798 = Data_0200c794;
            Data_0200c794 = *Data_0200c7a0++;
            Data_0200c7a8 = *Data_0200c7a0++;
            Data_0200c784 = 0;
            break;
        case 0x7fff:
            Data_0200c79c = *Data_0200c7a0++;
            break;
        case -1:
            Main_080000d8(CommandInterpolationRenderer_Update);
            Main_080001b8(Data_0200c57c);
            return;
        }
    }
    goto commands;
render:
    Data_0200c79c--;
    duration = Data_0200c754;
    if (duration == 0) {
        scale = Data_0200c778;
    } else {
        struct Half zero = { 0 };
        start = Data_0200c768;
        x = Data_0200c778;
        progress = ++Data_0200c7fc;
        scale = start + Local_02003b00((x - start) * progress, duration);
        if (progress >= duration)
            Data_0200c754 = zero.value;
    }
    duration = Data_0200c7a8;
    if (duration == 0) {
        blend = Data_0200c794;
    } else {
        struct Half zero = { 0 };
        start = Data_0200c798;
        x = Data_0200c794;
        progress = ++Data_0200c784;
        blend = start + Local_02003b00((x - start) * progress, duration);
        if (progress >= duration)
            Data_0200c7a8 = zero.value;
    }
    duration = Data_0200c76c;
    if (duration == 0) {
        pos = Data_0200c7f8;
    } else {
        struct Half zero = { 0 };
        start = Data_0200c7f0;
        x = Data_0200c7f8;
        progress = ++Data_0200c77c;
        pos = start + Local_02003b00((x - start) * progress, duration);
        if (progress >= duration)
            Data_0200c76c = zero.value;
    }
    work.angle = 0;
    work.x = scale;
    work.y = scale;
    matrix = (s16)Main_080001e0(&work);
    Data_0200c770 += pos;
    pos = Data_0200c770 / 256;
    switch (Data_0200c790) {
    case 1: {
        u32 attr = matrix << 25;
        y = 56;
        flags = 0x80004000;
        for (i = 0; i < 4; i++) {
            x = pos + scale * (i * 32 - 48) / 256;
            left = x + 88;
            if ((u32)(x + 152) < 304) {
                x = left & 511;
                *write++ = 0;
                *write++ = (x << 16) | y | flags | attr | 0x700;
                *write++ = 0xf400 | tile;
                Main_080001e8(sprite++, 236);
            }
            tile += 8;
        }
        break;
    }
    case 3: {
        u32 attr = matrix << 25;
        y = 48;
        flags = 0x80004000;
        for (i = 0; i < 2; i++) {
            x = pos + scale * (i * 32 - 16) / 256;
            left = x + 88;
            if ((u32)(x + 152) < 304) {
                x = left & 511;
                *write++ = 0;
                *write++ = (x << 16) | y | flags | attr | 0x700;
                *write++ = 0xf400 | (tile + Data_0200c764);
                Main_080001e8(sprite++, 236);
            }
            tile += 8;
        }
        break;
    }
    case 4:
        y = 48;
        flags = 0xc0004000;
        if ((u32)(pos + 120) < 304) {
            x = (pos + 56) & 511;
            *write++ = 0;
            *write++ = (x << 16) | y | flags | (matrix << 25) | 0x700;
            *write++ = 0xf400 | (tile + Data_0200c764);
            Main_080001e8(sprite, 236);
        }
        break;
    case 2:
        y = 48;
        flags = 0x80000000;
        if ((u32)(pos + 152) < 304) {
            x = (pos + 88) & 511;
            *write++ = 0;
            *write++ = (x << 16) | y | flags | (matrix << 25) | 0x700;
            *write++ = 0xf400 | (tile + Data_0200c764);
            Main_080001e8(sprite, 236);
        }
        break;
    }
    queue = &gIoWriteQueue;
    ime = (volatile u16 *)0x04000208;
    saved = *ime;
    *ime = (u16)(u32)ime;
    cnt = queue->count;
    if (cnt < 32) {
        queue->count = cnt + 1;
        entry = queue->entries[cnt];
        *entry++ = 0x3f00;
        *entry++ = 0x04000050;
        *entry = 0x20000;
    }
    *ime = saved;
    saved = *ime;
    *ime = (u16)(u32)ime;
    cnt = queue->count;
    if (cnt < 32) {
        queue->count = cnt + 1;
        entry = queue->entries[cnt];
        *entry++ = ((16 - blend) << 8) | blend;
        *entry++ = 0x04000052;
        *entry = 0x20000;
    }
    *ime = saved;
}
