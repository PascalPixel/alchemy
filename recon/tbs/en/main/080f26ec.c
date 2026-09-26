/* Draft, not exact (2026-09-25): 1156 of 1152 bytes, 400 differing
   halfwords, 214 differing instruction lines; the block layout matches.
   The title intro: two backgrounds scroll up a row every eight lines, the
   credits background swaps to the logo on frame 0x118, then the optional
   sprite banner and the fade loop, which A or Start (mask 9) cut short and
   report as 1.
   Layout facts that closed: the logo tilemap loop's initialisers sit in the
   0x118 branch before the break and the loop itself after the frame loop
   (`for (; i < 20; i++)`), so the break jumps straight to its test; the
   300/3600 limit is an if/else, which gives loop.c the barrier it moves the
   key-exit block to; the scroll block is a volatile u16 pointer; the logo
   row's inner loop is a goto loop.
   Remaining, register allocation only: reload picks r0 where this C picks
   r1 (the result zero, the r8 base copies, the sl copy), so the key-cut
   state store follows the frame store and the 0x119/0x121 tails cross-jump;
   the reference rematerialises 0x10000 and loads 0x1ff twice in the logo
   loop (its hoisted constants were spilled); the palette clear stores r1
   (0x05000000) itself where this C loads a pool zero. */
#include "TYPES.H"
#include "DMA.H"
#include "CALLBACK_SCHEDULER.H"
#include "IO_WRITE_QUEUE.H"
#include "RESOURCE.H"

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
extern u32 Data_03001c94;
extern u8 Value_00000000;
extern u8 Value_00000016;
extern u8 Value_00001440;
extern u8 Value_00001540;
extern u8 Data_03001d18;
extern u8 Data_03001f58;
extern const u8 Data_080f39b1[];
extern volatile u16 Data_04000208;
#define REG_IME Data_04000208

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
s32 Math_Mod(s32, s32);
u32 Math_ModU(u32, u32);
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
    Data_03001d18 = result;
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
        if ((u32)(frame - 21) <= 217 && (Data_03001c94 & 9) != 0) {
            work->state = 1;
            work->frame = 239;
            frame = 239;
        }
        next = frame + 1;
        work->frame = next;
        if (next <= 278) {
            tick = work->tick;
            if (Math_Mod(tick, 3) == 0) {
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
                        row = Math_Mod(160 - work->rows_b * 8, 160);
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
            Data_03001d18 = 1;
            WaitFrames(1);
            *(u16 *)0x0400000c = 0x681;
            *(u16 *)0x04000000 = (u16)(u32)&Value_00001440;
            scroll[5] = (u16)(u32)&Value_00000000;
            {
                u8 *palette = GetResource((s32)&Value_00000016);

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
    *(volatile u16 *)0x04000000 = (u16)(u32)&Value_00001540;
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
            phase = Math_ModU(i, 60);
            *(volatile u16 *)0x04000050 = 0x2f50;
            level = Data_080f39b1[phase];
            *(volatile u16 *)0x04000052 = ((16 - level) << 8) + level;
        }
        if ((Data_03001c94 & 9) != 0) {
            result = 1;
            break;
        }
        WaitFrames(1);
    }
    Data_03001f58 = saved;
    Runtime_ReleaseHeapBlock(43);
    *(volatile u16 *)0x04000050 = (u16)(u32)&Value_00000000;
    *(volatile u16 *)0x04000052 = (u16)(u32)&Value_00000000;
    WaitFrames(1);
    return result;
}
