/* NONMATCHING: current namespace/scalar repair, measured 2026-10-01.
 * Source hypothesis: Use the maintained REG_IME hardware definition and the physically owned Resource_GetTableEntry callee.
 * EN: 1152/1152 complete linked bytes, 878 differing byte positions, first +0x10, including the complete literal pool.
 * All six ordinary TBS targets compile; text extents in bytes:
 * JA 1152; EN 1152; DE 1152; ES 1152; FR 1152; IT 1152.
 * The other five editions have no complete linked proof in this attempt.
 * Existing raw numeric callbacks/data pointers and provisional field views
 * still need owning source declarations before adoption. This is an
 * uncredited S4 draft; compiler options and production routing are unchanged.
 */
#include "TYPES.H"
#include "DMA.H"
#include "CALLBACK_SCHEDULER.H"
#include "IO_WRITE_QUEUE.H"
#include "RESOURCE.H"
#include "IO_REG.H"

struct TitleSprite {
    s32 unk00;
    u8 y;
    u8 unk05;
    u16 x : 9;
    u16 unk06_9 : 7;
    s32 tile;
};

struct TitleWork {
    s32 rows_a;
    s32 rows_b;
    s32 frame;
    s32 tick;
    s32 state;
    u8 padding14[0x6c];
    struct TitleSprite sprites[5];
};

struct Scroll {
    u16 unk00;
    u16 unk02;
    u16 unk04;
    u16 unk06;
    u16 unk08;
    u16 unk0a;
};

extern volatile struct Scroll gBgScroll;
extern u32 gKeyState;
extern u8 gOamCopyEnabled;
extern u8 Data_03001f58;
extern const u8 Data_080f39b1[];

void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void *Runtime_AllocateBlock(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
void Bg0_ClearTilemap(void);
void Resource_InitializeTable(void);
void WaitFrames(s32 frames);
void Scheduler_ResetTaskTable(void);
void Unnamed_080f24a0(void);
void TitlePalette_InitializeBuffers(void);
s32 Graphics_TransformSmallPalette(s32, s32);
void Graphics_UpdatePaletteInterpolation(s32);
s32 __modsi3(s32, s32);
u32 __umodsi3(u32, u32);
void Blend_SetBrightenTarget16(s32);
void Blend_SetBrightenTarget0(s32);
void Blend_WaitForTransition(void);

void Resource_DecodeByteLz(const void *source, void *destination);
void Ui_LoadWindowGraphics(void);
s32 VramBlock_LoadCached(s32 entry, s32 size, const void *source);
void Runtime_PushSlotEntry(void *entry, s32 mode);

s32 Func_080f26ec(s32 sprites)
{
    struct TitleWork *work;
    s32 frame;
    s32 next;
    s32 tick;
    u32 i;
    u32 j;
    u32 limit;
    s32 result;
    u8 saved;
    u16 zero;
    volatile u16 *scroll;
    u16 *tile;
    s16 t;

    result = 0;
    saved = Data_03001f58;
    work = Runtime_AllocateHeapBlock(43, 224);
    Bg0_ClearTilemap();
    Resource_InitializeTable();
    WaitFrames(1);
    Scheduler_ResetTaskTable();
    gOamCopyEnabled = result;
    Data_03001f58 = result;
    Unnamed_080f24a0();
    TitlePalette_InitializeBuffers();
    Graphics_TransformSmallPalette(2, 0);
    {
        volatile u16 *ime;
        struct IoWriteQueue *q;
        u32 old;
        s32 count;

        q = &gIoWriteQueue;
        do {
            ime = &REG_IME;
            old = *ime;
        } while (0);
        *ime = (u16)ime;
        count = q->count;
        if (count <= 31) {
            u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);
            *(u16 *)&q->count = count + 1;
            *destination++ = 0xf740;
            *destination++ = 0x04000000;
            *destination = 0x20000;
        }
        *ime = old;
    }
    Graphics_UpdatePaletteInterpolation(60);
    Scheduler_AddOrUpdateCallback(0x080f2029, 0x480);

    scroll = (volatile u16 *)0x03001ad0;
    for (;;) {
        frame = work->frame;
        if ((u32)(frame - 21) <= 217 && (gKeyState & 9) != 0) {
            work->state = 1;
            work->frame = 239;
            frame = 239;
        }
        next = frame + 1;
        work->frame = next;
        if (next <= 278) {
            tick = work->tick;
            if (__modsi3(tick, 3) == 0) {
                scroll[3] = scroll[3] + 0xffff;
                if ((scroll[3] & 7) == 0) {
                    Dma_Set((u8 *)0x02012580 - work->rows_a * 960, (u8 *)0x06004b00 - work->rows_a * 960,
                        0x800001e0, (volatile u32 *)0x040000d4);
                    work->rows_a++;
                    tick = work->tick;
                }
            }
            if ((tick & 1) == 0) {
                scroll[5] = scroll[5] + 0xffff;
                if ((scroll[5] & 7) == 0) {
                    if (work->rows_b * 8 <= 24) {
                        Dma_Set((u8 *)0x020199c0 - work->rows_b * 1920, (u8 *)0x0600e4c0 - work->rows_b * 1920,
                            0x800003c0, (volatile u32 *)0x040000d4);
                    } else {
                        u16 *tile;
                        s32 row;

                        zero = 0;
                        row = __modsi3(160 - work->rows_b * 8, 160);
                        Dma_Set(&zero, (u8 *)0x06004ec0 + row * 240, 0x810003c0, (volatile u32 *)0x040000d4);
                        tile = (u16 *)0x0600f6c0;
                        for (i = 0; i < 5; i++) {
                            for (j = 0; j < 30; j++) {
                                *tile++ = 0x13b;
                            }
                            *tile++ = 0x13b;
                            *tile++ = 0x13b;
                        }
                    }
                    work->rows_b++;
                }
            }
            if (work->frame == 239) {
                work->state = 1;
            }
        } else if (next == 0x119) {
            work->state = 2;
        } else if (next == 0x121) {
            work->state = 0;
        } else if (next == 0x118) {
            Blend_SetBrightenTarget16(1);
            Scheduler_RemoveCallback(0x080f2029);
            gOamCopyEnabled = 1;
            WaitFrames(1);
            *(u16 *)0x0400000c = 0x681;
            *(u16 *)0x04000000 = (u16)0x1440;
            scroll[5] = 0;
            {
                u8 *palette = Resource_GetTableEntry(0x16);

                Dma_Set(palette, (void *)0x05000000, 0x84000078, (volatile u32 *)0x040000d4);
                *(volatile u16 *)0x05000000 = 0;
                Resource_DecodeByteLz(palette + 0x200, (void *)0x02010000);
                Dma_Set((void *)0x02010000, (void *)0x06004000, 0x80004b00, (volatile u32 *)0x040000d4);
                t = 0x100;
                tile = (u16 *)0x06003000;
                i = 0;
            }
            break;
        }
        WaitFrames(1);
    }
    for (; i < 20; i++) {
        j = 0;
    column:
        j++;
        *tile++ = t++;
        if (j <= 29) goto column;
        *tile++ = 0x1ff;
        *tile++ = 0x1ff;
    }

    Ui_LoadWindowGraphics();
    Bg0_ClearTilemap();
    if (sprites != 0) {
        u8 *buffer = Runtime_AllocateBlock(14, 0x400);
        struct TitleSprite *sprite;
        s32 tile;

        Resource_DecodeByteLz((const void *)0x080f38bc, buffer);
        sprite = work->sprites;
        for (i = 0; i < 5; i++) {
            tile = VramBlock_LoadCached(Resource_FindFreeEntry(), 128, buffer + (i << 8) / 2);
            {
                u32 *out = (u32 *)sprite;

                *out++ = 0;
                *out++ = 0x40004000;
                *out = tile;
            }
            sprite++;
        }
        Runtime_ReleaseHeapBlock(14);
    }
    Blend_SetBrightenTarget0(30);
    Blend_WaitForTransition();
    *(volatile u16 *)0x04000000 = (u16)0x1540;
    if (sprites != 0) {
        limit = 3600;
    } else {
        limit = 300;
    }
    for (i = 0; i < limit; i++) {
        if (sprites != 0) {
            struct TitleSprite *sprite = work->sprites;
            s32 x = 80;
            u32 phase;
            u32 level;

            for (j = 0; j < 3; j++) {
                sprite->x = x;
                sprite->y = 124;
                Runtime_PushSlotEntry(sprite, 0);
                x += 32;
                sprite++;
            }
            phase = __umodsi3(i, 60);
            *(volatile u16 *)0x04000050 = 0x2f50;
            level = Data_080f39b1[phase];
            *(volatile u16 *)0x04000052 = ((16 - level) << 8) + level;
        }
        if ((gKeyState & 9) != 0) {
            result = 1;
            break;
        }
        WaitFrames(1);
    }
    Data_03001f58 = saved;
    Runtime_ReleaseHeapBlock(43);
    *(volatile u16 *)0x04000050 = 0;
    *(volatile u16 *)0x04000052 = 0;
    WaitFrames(1);
    return result;
}
