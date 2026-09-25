/* Draft, not exact (2026-09-25): 1156 of 1152 bytes, 451 differing halfwords.
   Hand-written from the assembly: the title intro. Two backgrounds scroll up
   one row every eight lines, the credits background swaps to the logo on
   frame 0x118, then the optional sprite banner and the fade loop, which the
   A or Start keys (mask 9) cut short and report as 1.
   Residual: the scroll base must be a constant volatile u16 pointer (a
   volatile struct or symbol array adds a dummy halfword read before each
   store). Remaining: the frame 239 store precedes the state store; the
   0x119/0x121 tails cross-jump; the palette clear stores a pool zero where
   the reference stores r1 (0x05000000) itself; the 0x100 tile strip
   hoists 0x10000 where the reference rematerialises it; the two 0x1ff
   stores share one load; pool placement after the frame loop differs. */
#include "TYPES.H"
#include "DMA.H"
#include "IO_WRITE_QUEUE.H"

struct TitleSprite {
    u32 head;
    u8 y;
    u8 attr0;
    u16 x : 9;
    u16 attr1 : 7;
    u32 tile;
};

struct TitleWork {
    s32 upper;
    s32 lower;
    s32 frame;
    s32 tick;
    s32 state;
    u8 pad[108];
    struct TitleSprite sprites[5];
};

struct BgScroll {
    volatile u16 x;
    volatile u16 y;
};

extern volatile u32 Data_03001c94;
extern u8 Data_03001d18;
extern u8 Data_03001f58;
extern volatile u16 Data_04000208;
extern u8 Value_00000000;
extern u8 Value_00000016;
extern u8 Value_00001440;
extern u8 Value_00001540;
extern const u8 Data_080f38bc[];
extern const u8 Data_080f39b1[];

void *Runtime_AllocateHeapBlock(s32 slot, u32 size);
void *Runtime_AllocateBlock(s32 slot, u32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
void Bg0_ClearTilemap(void);
void Resource_InitializeTable(void);
void WaitFrames(s32 frames);
void Scheduler_ResetTaskTable(void);
void Func_080f24a0(void);
void TitlePalette_InitializeBuffers(void);
s32 Graphics_TransformSmallPalette(s32 a, s32 b);
void Graphics_UpdatePaletteInterpolation(s32 frames);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Scheduler_RemoveCallback(void (*callback)(void));
void Func_080f2028(void);
s32 Math_Mod(s32 a, s32 b);
u32 Math_ModU(u32 a, u32 b);
void Blend_SetBrightenTarget16(s32 frames);
void Blend_SetBrightenTarget0(s32 frames);
void Blend_WaitForTransition(void);
void *Resource_GetTableEntry(s32 index);
void Resource_DecodeByteLz(const void *source, void *destination);
void Ui_LoadWindowGraphics(void);
s32 Resource_FindFreeEntry(void);
u32 VramBlock_LoadCached(s32 entry, s32 size, const void *source);
void Runtime_PushSlotEntry(void *entry, s32 priority);

s32 Func_080f26ec(s32 logo)
{
    struct TitleWork *work;
    volatile u16 *scroll;
    struct TitleSprite *sprite;
    struct IoWriteQueue *q;
    volatile u16 *ime;
    u32 saved_ime;
    s32 count;
    s32 result;
    s32 saved;
    s32 frame;
    s32 tick;
    u32 i;
    u32 j;
    u32 limit;
    s32 x;
    u16 fill;
    u8 *resource;
    u16 *map;
    s16 tile;
    u8 *buffer;
    u32 level;

    result = 0;
    saved = Data_03001f58;
    work = Runtime_AllocateHeapBlock(43, 224);
    Bg0_ClearTilemap();
    Resource_InitializeTable();
    WaitFrames(1);
    Scheduler_ResetTaskTable();
    Data_03001d18 = result;
    Data_03001f58 = result;
    Func_080f24a0();
    TitlePalette_InitializeBuffers();
    Graphics_TransformSmallPalette(2, 0);

    q = &gIoWriteQueue;
    do {
        ime = &Data_04000208;
        saved_ime = *ime;
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
    *ime = saved_ime;

    Graphics_UpdatePaletteInterpolation(60);
    Scheduler_AddOrUpdateCallback(Func_080f2028, 0x480);
    scroll = (volatile u16 *)0x03001ad0;

    for (;;) {
        if ((u32)(work->frame - 21) <= 217 && (Data_03001c94 & 9) != 0) {
            work->frame = 239;
            work->state = 1;
        }
        work->frame++;
        if (work->frame <= 278) {
            tick = work->tick;
            if (Math_Mod(tick, 3) == 0) {
                scroll[3] = scroll[3] + 0xffff;
                if ((scroll[3] & 7) == 0) {
                    Dma_Set((void *)(0x02012580 - work->upper * 960),
                        (void *)(0x06004b00 - work->upper * 960), 0x800001e0,
                        (volatile u32 *)0x040000d4);
                    work->upper++;
                    tick = work->tick;
                }
            }
            if ((tick & 1) == 0) {
                scroll[5] = scroll[5] + 0xffff;
                if ((scroll[5] & 7) == 0) {
                    s32 y = work->lower * 8;
                    if (y <= 24) {
                        Dma_Set((void *)(0x020199c0 - work->lower * 1920),
                            (void *)(0x0600e4c0 - work->lower * 1920), 0x800003c0,
                            (volatile u32 *)0x040000d4);
                    } else {
                        fill = 0;
                        Dma_Set(&fill, (void *)(Math_Mod(160 - y, 160) * 240 + 0x06004ec0),
                            0x810003c0, (volatile u32 *)0x040000d4);
                        map = (u16 *)0x0600f6c0;
                        for (i = 0; i < 5; i++) {
                            for (j = 0; j < 30; j++) {
                                *map++ = 0x13b;
                            }
                            *map++ = 0x13b;
                            *map++ = 0x13b;
                        }
                    }
                    work->lower++;
                }
            }
            if (work->frame == 239) {
                work->state = 1;
            }
        } else if (work->frame == 0x119) {
            work->state = 2;
        } else if (work->frame == 0x121) {
            work->state = 0;
        } else if (work->frame == 0x118) {
            break;
        }
        WaitFrames(1);
    }

    Blend_SetBrightenTarget16(1);
    Scheduler_RemoveCallback(Func_080f2028);
    Data_03001d18 = 1;
    WaitFrames(1);
    *(volatile u16 *)0x0400000c = 0x681;
    *(volatile u16 *)0x04000000 = (u16)(u32)&Value_00001440;
    scroll[5] = (u16)(u32)&Value_00000000;
    resource = Resource_GetTableEntry((s32)&Value_00000016);
    Dma_Set(resource, (void *)0x05000000, 0x84000078, (volatile u32 *)0x040000d4);
    *(u16 *)0x05000000 = 0;
    Resource_DecodeByteLz(resource + 0x200, (void *)0x02010000);
    Dma_Set((void *)0x02010000, (void *)0x06004000, 0x80004b00, (volatile u32 *)0x040000d4);
    tile = 0x100;
    map = (u16 *)0x06003000;
    for (i = 0; i < 20; i++) {
        for (j = 0; j < 30; j++) {
            *map++ = tile;
            tile++;
        }
        *map++ = 0x1ff;
        *map++ = 0x1ff;
    }
    Ui_LoadWindowGraphics();
    Bg0_ClearTilemap();

    if (logo != 0) {
        buffer = Runtime_AllocateBlock(14, 0x400);
        Resource_DecodeByteLz(Data_080f38bc, buffer);
        sprite = work->sprites;
        for (i = 0; i < 5; i++) {
            u32 tile_index = VramBlock_LoadCached(Resource_FindFreeEntry(), 128, buffer + (i << 8) / 2);
            u32 *out = (u32 *)sprite;
            *out++ = 0;
            *out++ = 0x40004000;
            *out = tile_index;
            sprite++;
        }
        Runtime_ReleaseHeapBlock(14);
    }

    Blend_SetBrightenTarget0(30);
    Blend_WaitForTransition();
    *(volatile u16 *)0x04000000 = (u16)(u32)&Value_00001540;
    limit = 300;
    if (logo != 0) {
        limit = 3600;
    }
    for (i = 0; i < limit; i++) {
        if (logo != 0) {
            sprite = work->sprites;
            x = 80;
            for (j = 0; j < 3; j++) {
                sprite->x = x;
                sprite->y = 124;
                Runtime_PushSlotEntry(sprite, 0);
                x += 32;
                sprite++;
            }
            level = Data_080f39b1[Math_ModU(i, 60)];
            *(volatile u16 *)0x04000050 = 0x2f50;
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
