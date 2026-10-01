/* Draft, not exact (2026-10-01, slice-11): score 765, 35 instructions off of
   1152 bytes; block layout, stack slots, r8-r11 and every pool word match.
   Remaining: (1) both scroll tests tie the AND result to the constant 7
   where the reference ties it to the re-read cell (ands r3, r2 with the
   operands swapped), which also moves the zero for the row clear; (2) the
   logo loop: the reference rebuilds 0x10000 in r0 and loads 0x1ff twice per
   row (spilled loop constants), here they sit in registers because the inner
   loop is a goto loop; natural for loops free a register and move the
   parameter off the stack, so they score worse (2391); (3) 16 for the blend
   weight is a pool halfword here, movs in the reference; computing it in an
   int moves the 0x04000052 address out of r11; (4) the sprite record store
   has the pointer in r3 and the values in r2, the reference the reverse.
   What closed: Data_03001f58 is volatile (its load is a pseudo, not a
   reload), the scroll cells are block-local volatile u16 pointers so the
   base is a hoisted invariant after the constant 1, Resource_DecodeByteLz
   returns s32, and the next frame is its own variable. */
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "DMA.H"
#include "IO_REG.H"
#include "IO_WRITE_QUEUE.H"
#include "MAP_SCROLL.H"
#include "RAM_BUFFER.H"
#include "RESOURCE.H"
#include "RESOURCE_IDS.H"
#include "SYSTEM.H"

struct IntroSprite {
    u32 unknown_0;
    u8 y;
    u8 unknown_5;
    u16 x : 9;
    u16 unknown_6 : 7;
    s32 tile;
};

struct IntroWork {
    s32 back_rows;
    s32 front_rows;
    s32 frame;
    s32 tick;
    s32 state;
    s32 unknown_14;
    u8 unknown_18[0x68];
    struct IntroSprite sprites[5];
};

extern volatile u32 gKeyState;
extern u8 gOamCopyEnabled;
extern volatile u8 Data_03001f58;
extern const u8 Data_080f38bc[];
extern const u8 Data_080f39b1[];
extern char ResourceId_IntroGraphicsB;

void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void Bg0_ClearTilemap(void);
void Resource_InitializeTable(void);
void Title_LoadIntroBackgrounds(void);
void Func_080f2028(void);
void TitlePalette_InitializeBuffers(void);
s32 Graphics_TransformSmallPalette(s32, s32);
void Graphics_UpdatePaletteInterpolation(s32);
void Blend_SetBrightenTarget16(s32);
void Blend_SetBrightenTarget0(s32);
void Blend_WaitForTransition(void);
s32 Resource_DecodeByteLz(const void *source, void *destination);
void Ui_LoadWindowGraphics(void);
s32 VramBlock_LoadCached(s32 entry, s32 size, const void *source);
void Runtime_PushSlotEntry(void *entry, s32 mode);

s32 Func_080f26ec(s32 sprites)
{
    struct IntroWork *work;
    s32 result;
    u8 saved;
    u16 zero;
    u8 *data;
    u16 *map;
    s32 tile;
    u32 i;
    u32 j;
    u32 limit;

    result = 0;
    saved = Data_03001f58;
    work = Runtime_AllocateHeapBlock(43, 224);
    Bg0_ClearTilemap();
    Resource_InitializeTable();
    WaitFrames(1);
    Scheduler_ResetTaskTable();
    gOamCopyEnabled = result;
    Data_03001f58 = result;
    Title_LoadIntroBackgrounds();
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
    Scheduler_AddOrUpdateCallback((s32)Func_080f2028, 0x480);

    for (;;) {
        s32 frame;
        s32 next;
        s32 tick;

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
            if (tick % 3 == 0) {
                volatile u16 *scroll = (volatile u16 *)gBgScroll;

                scroll[3]--;
                if ((scroll[3] & 7) == 0) {
                    Dma_Set(Ram_MapCellBuffer + 0x2580 - work->back_rows * 960, (u8 *)0x06004b00 - work->back_rows * 960,
                        0x800001e0, (volatile u32 *)0x040000d4);
                    work->back_rows++;
                    tick = work->tick;
                }
            }
            if ((tick & 1) == 0) {
                volatile u16 *scroll = (volatile u16 *)gBgScroll;

                scroll[5]--;
                if ((scroll[5] & 7) == 0) {
                    if (work->front_rows * 8 <= 24) {
                        Dma_Set(Ram_MapCellBuffer + 0x99c0 - work->front_rows * 1920, (u8 *)0x0600e4c0 - work->front_rows * 1920,
                            0x800003c0, (volatile u32 *)0x040000d4);
                    } else {
                        zero = 0;
                        Dma_Set(&zero, (u8 *)0x06004ec0 + (160 - work->front_rows * 8) % 160 * 240, 0x810003c0,
                            (volatile u32 *)0x040000d4);
                        map = (u16 *)0x0600f6c0;
                        for (i = 0; i < 5; i++) {
                            for (j = 0; j < 30; j++)
                                *map++ = 0x13b;
                            *map++ = 0x13b;
                            *map++ = 0x13b;
                        }
                    }
                    work->front_rows++;
                }
            }
            if (work->frame == 239)
                work->state = 1;
        } else if (next == 0x119) {
            work->state = 2;
        } else if (next == 0x121) {
            work->state = 0;
        } else if (next == 0x118) {
            Blend_SetBrightenTarget16(1);
            Scheduler_RemoveCallback((u32)Func_080f2028);
            gOamCopyEnabled = 1;
            WaitFrames(1);
            *(volatile u16 *)0x0400000c = 0x681;
            *(volatile u16 *)0x04000000 = 0x1440;
            {
                volatile u16 *scroll = (volatile u16 *)gBgScroll;

                scroll[5] = 0;
            }
            data = Resource_GetTableEntry((s32)&ResourceId_IntroGraphicsB);
            Dma_Set(data, (void *)0x05000000, 0x84000078, (volatile u32 *)0x040000d4);
            *(u16 *)0x05000000 = 0;
            data += 0x200;
            Resource_DecodeByteLz(data, Ram_MapCellBuffer);
            Dma_Set(Ram_MapCellBuffer, (void *)0x06004000, 0x80004b00, (volatile u32 *)0x040000d4);
            tile = 0x100;
            map = (u16 *)0x06003000;
            i = 0;
            break;
        }
        WaitFrames(1);
    }
    for (; i < 20; i++) {
        j = 0;
    cell:
        {
            s32 old = tile;

            tile = ((old << 16) + 0x10000) >> 16;
            *map++ = old;
        }
        if (++j <= 29)
            goto cell;
        *map++ = 0x1ff;
        *map++ = 0x1ff;
    }

    Ui_LoadWindowGraphics();
    Bg0_ClearTilemap();
    if (sprites != 0) {
        u8 *buffer = Runtime_AllocateBlock(14, 0x400);
        struct IntroSprite *sprite;

        Resource_DecodeByteLz(Data_080f38bc, buffer);
        sprite = work->sprites;
        for (i = 0; i < 5; i++) {
            s32 block = VramBlock_LoadCached(Resource_FindFreeEntry(), 128, buffer + (i << 8) / 2);
            u32 *out = (u32 *)sprite;

            *out++ = 0;
            *out++ = 0x40004000;
            *out = block;
            sprite++;
        }
        Runtime_ReleaseHeapBlock(14);
    }
    Blend_SetBrightenTarget0(30);
    Blend_WaitForTransition();
    *(volatile u16 *)0x04000000 = 0x1540;
    if (sprites != 0)
        limit = 3600;
    else
        limit = 300;
    for (i = 0; i < limit; i++) {
        if (sprites != 0) {
            struct IntroSprite *sprite = work->sprites;
            s32 x;
            u32 level;

            for (j = 0, x = 80; j < 3; j++) {
                sprite->x = x;
                sprite->y = 124;
                Runtime_PushSlotEntry(sprite, 0);
                x += 32;
                sprite++;
            }
            level = Data_080f39b1[i % 60];
            *(volatile u16 *)0x04000050 = 0x2f50;
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
