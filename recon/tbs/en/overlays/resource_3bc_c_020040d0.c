/* NONMATCHING: 964 bytes, candidate 880, 469 differing halfwords (2026-09-24).
 * Structure follows the ROM: seven OAM entries written through a cursor, one
 * register call per entry. The ROM spills the cursor to sp+12 and holds the
 * shape bits (0x8000, 0x40000000) and 0xe400 in r9/sl across calls; this
 * build keeps the cursor in r7 and rematerialises the constants. */
#include "TYPES.H"
#include "DMA.H"

#define FieldScene_RunScene3bcSequenceB Func_020040d0

struct HudBlock {
    u16 base;
    u16 offset;
};

struct HudWork {
    u32 oam[18][3];
    s16 block;
    s16 level;
    s16 shown;
    s16 marker_b;
    s16 marker_a;
    s16 pad226;
    s16 pad228;
    s16 count;
    s32 cam_x;
    s32 cam_z;
};

struct HudEntry {
    u32 a0;
    u32 a1;
    u32 a2;
};

struct HudObject {
    u8 pad00[8];
    s32 x;
    u8 pad0c[4];
    s32 z;
};

extern struct HudBlock Data_03001b10[];

s32 Func_020089f4(s32 size);
void Func_02008a0e(const void *src, s32 dst);
void Func_02008a1a(s32 buf);
void Func_02008a34(s32 block, s32 size, s32 buf);
void Func_02008a46(s32 block);
void Func_02008abc(void *e, s32 pri);
s32 Func_02008ad4(s32 flag);
void Func_02008afe(void *e, s32 pri);
void Func_02008b3e(void *e, s32 pri);
void Func_02008b6e(void *e, s32 pri);
s32 Func_02008bac(s32 a, s32 b);
void Func_02008bb4(void *e, s32 pri);
s32 Func_02008bc2(s32 a, s32 b);
void Func_02008c0a(void *e, s32 pri);
s32 Func_02008c34(s32 a, s32 b);
s32 Func_02008c4a(s32 a, s32 b);
void Func_02008ca6(void *e, s32 pri);
void Func_02008d28(void *e, s32 pri);
struct HudObject *Func_02008eca(s32 id);
struct HudObject *Func_02008f52(s32 id);

void FieldScene_RunScene3bcSequenceB(void)
{
    struct HudWork *work = *(struct HudWork **)0x03001f3c;
    u32 *oam = (u32 *)work;
    struct HudWork *hud = work;
    struct HudEntry *ent = (struct HudEntry *)work;
    s32 tile;
    s32 count;
    s32 y;
    s32 x;
    u32 i;
    s32 buf;
    u32 shape;
    struct HudObject *obj;

    tile = Data_03001b10[work->block].offset >> 5;
    count = work->count;
    if (work->shown != 0) {
        work->level = 2;
    } else if (Func_02008ad4(0x106) != 0) {
        if (work->level > 0) {
            work->level--;
        }
    } else if (work->level <= 1) {
        if (++work->level == 1) {
            Dma_Set((const void *)0x0200cd60, (void *)0x050003c0, 0x80000010, (volatile u32 *)0x040000d4);
            buf = Func_020089f4(0x200);
            Func_02008a0e((const void *)0x0200cd80, buf);
            Func_02008a34(work->block, 0x200, buf);
            Func_02008a1a(buf);
        }
    }
    if (work->level == 0) {
        Func_02008a46(((struct HudWork *)oam)->block);
        return;
    }
    y = (work->level * 6 - 8) & 0xff;
    x = count << 4;
    shape = 0x8000;
    *oam++ = 0;
    *oam++ = ((104 - x) << 16) | y | shape;
    *oam++ = tile | 0xe400;
    Func_02008abc(ent++, 255);
    shape = 0x40000000;
    for (i = 0; i < count; i++) {
        *oam++ = 0;
        *oam++ = ((96 - (i << 4)) << 16) | y | shape;
        *oam++ = (tile + 2) | 0xe400;
        Func_02008afe(ent++, 255);
    }
    shape = 0x8000;
    *oam++ = 0;
    *oam++ = (112 << 16) | y | shape;
    *oam++ = (tile + 6) | 0xe400;
    Func_02008b3e(ent++, 255);
    *oam++ = 0;
    *oam++ = (120 << 16) | y | shape | 0x10000000;
    *oam++ = (tile + 6) | 0xe400;
    Func_02008b6e(ent++, 255);
    shape = 0x40000000;
    for (i = 0; i < count; i++) {
        *oam++ = 0;
        *oam++ = ((128 + (i << 4)) << 16) | y | shape | 0x10000000;
        *oam++ = (tile + 2) | 0xe400;
        Func_02008bb4(ent++, 255);
    }
    shape = 0x8000;
    *oam++ = 0;
    *oam++ = ((x + 128) << 16) | y | shape | 0x10000000;
    *oam++ = tile | 0xe400;
    Func_02008c0a(ent++, 255);
    if ((*(u32 *)0x03001e40 & 15) > 4) {
        shape = 0x40000000;
        obj = Func_02008eca(hud->marker_a);
        if (obj != 0) {
            x = Func_02008bac(obj->x - hud->cam_x, 0xe0000) + 112;
            y = (Func_02008bc2(obj->z - hud->cam_z, 0xe0000) + hud->level * 6 - 4) & 0xff;
            *oam++ = 0;
            *oam++ = (x << 16) | y | shape;
            *oam++ = (tile + 12) | 0xe400;
            Func_02008ca6(ent++, 255);
        }
        obj = Func_02008f52(hud->marker_b);
        if (obj != 0) {
            x = Func_02008c34(obj->x - hud->cam_x, 0xe0000) + 112;
            y = (Func_02008c4a(obj->z - hud->cam_z, 0xe0000) + hud->level * 6 - 4) & 0xff;
            *oam++ = 0;
            *oam++ = (x << 16) | y | shape;
            *oam++ = (tile + 8) | 0xe400;
            Func_02008d28(ent++, 255);
        }
    }
}
