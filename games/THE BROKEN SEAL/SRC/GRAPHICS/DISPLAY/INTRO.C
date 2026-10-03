#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "DMA.H"
#include "EDITION.H"
#include "IO_REG.H"
#include "IO_WRITE_QUEUE.H"
#include "MAP_SCROLL.H"
#include "RAM_BUFFER.H"
#include "RESOURCE.H"
#include "RESOURCE_IDS.H"
#include "SYSTEM.H"
#include "PALBUF.H"

/* One sprite of the "press start" prompt, as the slot list takes it. */
struct IntroSprite {
    u32 unknown_0;
    u8 y;
    u8 unknown_5;
    u16 x : 9;
    u16 unknown_6 : 7;
    s32 tile;
};

/* The title intro's work block, heap slot 43. */
struct IntroWork {
    s32 back_rows;
    s32 front_rows;
    s32 frame;
    s32 tick;
    s32 state;
    s32 unknown_14;
    u8 unknown_18[0x68];
    struct IntroSprite sprites[8];
};

s32 Resource_DecodeByteLz(const void *source, void *destination);

/* Title intro: load both scrolling pictures and lay out their tilemaps.
   Each map row is thirty running tiles and two blank ones; the rows below
   the first eleven start a second run. Both layers start 96 lines down,
   behind a full-screen window, and the work block's counters are cleared. */
void Title_LoadIntroBackgrounds(void)
{
    struct IntroWork *work;
    u8 *data;
    u16 *map;
    s32 x;
    s32 y;
    s32 tile;
    s32 blank;

    work = Ram_WorkSlot[43];
    *(volatile u16 *)0x04000000 = 0;
    data = Resource_GetTableEntry((s32)&ResourceId_IntroGraphicsA);
    Dma_Set(data, (void *)0x05000200, 0x84000080, (volatile u32 *)0x040000d4);
    *(u16 *)0x05000200 = 0;
    data += 0x200;
    Resource_DecodeByteLz(data, Ram_MapCellBuffer);
    Dma_Set(Ram_MapCellBuffer, (void *)0x06010000, 0x80000f00, (volatile u32 *)0x040000d4);
    data = Resource_GetTableEntry((s32)&ResourceId_IntroGraphicsC);
    Dma_Set(data, (void *)0x05000000, 0x84000080, (volatile u32 *)0x040000d4);
    *(u16 *)0x05000000 = 0;
    data += 0x200;
    Resource_DecodeByteLz(data, Ram_MapCellBuffer);
    Dma_Set(Ram_MapCellBuffer + 0x2940, (void *)0x06000000, 0x80002760, (volatile u32 *)0x040000d4);
    Dma_Set(Ram_MapCellBuffer + 0xa140, (void *)0x06004ec0, 0x80004ec0, (volatile u32 *)0x040000d4);

    /* FAKEMATCH: goto loops and a tile counted in its high half keep the
       increment's constant inside each strip, where the loop pass would
       hoist it. */
    blank = 0x1ff;
    map = (u16 *)0x0600f000;
    tile = 0x267;
    y = 0;
front_top:
    x = 29;
front_top_cell:
    {
        s32 old = tile;

        tile = ((old << 16) + 0x10000) >> 16;
        *map++ = old;
    }
    if (--x >= 0)
        goto front_top_cell;
    *map++ = blank;
    *map++ = blank;
    if (++y <= 10)
        goto front_top;
    tile = 0x13b;
    y = 11;
front_rest:
    x = 29;
front_rest_cell:
    {
        s32 old = tile;

        tile = ((old << 16) + 0x10000) >> 16;
        *map++ = old;
    }
    if (--x >= 0)
        goto front_rest_cell;
    *map++ = blank;
    *map++ = blank;
    if (++y <= 31)
        goto front_rest;
    map = (u16 *)0x0600f800;
    tile = 300;
    y = 0;
back_top:
    x = 29;
back_top_cell:
    {
        s32 old = tile;

        tile = ((old << 16) + 0x10000) >> 16;
        *map++ = old;
    }
    if (--x >= 0)
        goto back_top_cell;
    *map++ = blank;
    *map++ = blank;
    if (++y <= 10)
        goto back_top;
    tile = 0;
    y = 11;
back_rest:
    x = 29;
back_rest_cell:
    {
        s32 old = tile;

        tile = ((old << 16) + 0x10000) >> 16;
        *map++ = old;
    }
    if (--x >= 0)
        goto back_rest_cell;
    *map++ = blank;
    *map++ = blank;
    if (++y <= 31)
        goto back_rest;

    *(volatile u16 *)0x0400000a = 0x1f43;
    *(volatile u16 *)0x0400000c = 0x1e81;
    *(volatile u16 *)0x04000040 = 0xf0;
    *(volatile u16 *)0x04000044 = 0x9f;
    *(volatile u16 *)0x04000042 = 0xf0;
    *(volatile u16 *)0x04000046 = 0x9f;
    *(volatile u16 *)0x04000048 = 0x1616;
    for (y = 0; y < 4; y++) {
        gBgScroll[y].y = 0;
        gBgScroll[y].x = 0;
    }
    gBgScroll[1].y = 96;
    gBgScroll[2].y = 96;
    work->frame = 0;
    work->back_rows = 0;
    work->front_rows = 0;
    work->tick = 0;
    work->unknown_14 = 0;
    work->state = 0;
    Dma_Set(gBgScroll, (void *)0x04000010, 0x84000004, (volatile u32 *)0x040000d4);
    *(volatile u16 *)0x04000050 = 0x3fbf;
    *(volatile u16 *)0x04000052 = 0x1010;
    *(volatile u16 *)0x04000050 = 0x3f44;
}

/* The German edition keeps its own version in its scaffold: its prompt is
   eight tiles drawn as two rows of three sprites, at lines 116 and 124,
   and the obvious branch for it compiles four bytes short. */
#if !defined(TBS_EDITION_DE)

/* The prompt is five tiles. Japanese and French draw five sprites from
   column 56, the Japanese ones two lines lower; the others draw three
   from column 80. */
#define PROMPT_TILES 5
#if !EDITION_INTERNATIONAL
#define PROMPT_SPRITES 5
#define PROMPT_X 56
#define PROMPT_Y 126
#elif defined(TBS_EDITION_FR)
#define PROMPT_SPRITES 5
#define PROMPT_X 56
#define PROMPT_Y 124
#else
#define PROMPT_SPRITES 3
#define PROMPT_X 80
#define PROMPT_Y 124
#endif

extern volatile u32 gKeyState;
extern u8 gOamCopyEnabled;
extern volatile u8 Data_03001f58;
extern const u8 Title_PromptTiles[];
extern const u8 Title_PromptBlendLevels[];

void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void Bg0_ClearTilemap(void);
void Resource_InitializeTable(void);
void Func_080f2028(void);
void Blend_SetBrightenTarget16(s32 frames);
void Blend_SetBrightenTarget0(s32 frames);
void Blend_WaitForTransition(void);
void Ui_LoadWindowGraphics(void);
s32 VramBlock_LoadCached(s32 entry, s32 size, const void *source);
void Runtime_PushSlotEntry(void *entry, s32 mode);

/* Title intro: both pictures scroll up, the back one a line every third
   tick and the front one every second, each loading a new row of tiles
   every eight lines; A or START skips ahead to frame 239. On frame 0x118
   the logo picture replaces them. With a prompt, its sprites then pulse
   for up to a minute, otherwise the logo holds for five seconds; returns 1
   when A or START ended the wait. */
s32 Title_ShowIntro(s32 prompt)
{
    struct IntroWork *work;
    s32 result;
    u8 saved;
    u16 zero;
    u8 *data;
    u16 *map;
    u32 i;
    s32 tile;
    u32 j;
    u32 limit;
    s32 blank;

    result = 0;
    blank = 0x1ff;
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
        /* FAKEMATCH: removing this one-pass block changes instruction scheduling. */
        do {
            ime = &REG_IME;
            old = *ime;
        } while (0);
        *ime = (u16)ime;
        count = q->count;
        if (count <= 31) {
            u32 *destination = q->entries[count];
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
                s32 y;

                gBgScroll[1].y--;
                y = gBgScroll[1].y;
                if ((y & 7) == 0) {
                    Dma_Set(Ram_MapCellBuffer + 0x2580 - work->back_rows * 960, (u8 *)0x06004b00 - work->back_rows * 960,
                        0x800001e0, (volatile u32 *)0x040000d4);
                    work->back_rows++;
                    tick = work->tick;
                }
            }
            if ((tick & 1) == 0) {
                s32 y;

                gBgScroll[2].y--;
                y = gBgScroll[2].y;
                if ((y & 7) == 0) {
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
            gBgScroll[2].y = 0;
            data = Resource_GetTableEntry((s32)&ResourceId_IntroGraphicsB);
            Dma_Set(data, (void *)0x05000000, 0x84000078, (volatile u32 *)0x040000d4);
            *(u16 *)0x05000000 = 0;
            data += 0x200;
            Resource_DecodeByteLz(data, Ram_MapCellBuffer);
            Dma_Set(Ram_MapCellBuffer, (void *)0x06004000, 0x80004b00, (volatile u32 *)0x040000d4);
            map = (u16 *)0x06003000;
            tile = 0x100;
            i = 0;
            break;
        }
        WaitFrames(1);
    }
    /* FAKEMATCH: the row is a goto loop, as in the background set-up; a for
       loop frees a register and moves the parameter off the stack. */
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
        *map++ = blank;
        *map++ = blank;
    }

    Ui_LoadWindowGraphics();
    Bg0_ClearTilemap();
    if (prompt != 0) {
        u8 *buffer = Runtime_AllocateBlock(14, 0x400);
        struct IntroSprite *sprite;

        Resource_DecodeByteLz(Title_PromptTiles, buffer);
        sprite = work->sprites;
        for (i = 0; i < PROMPT_TILES; i++) {
            s32 block = VramBlock_LoadCached(Resource_FindFreeEntry(), 128, buffer + (i << 8) / 2);
            u32 *out = (u32 *)sprite;

            *out++ = 0;
            sprite++;
            *out++ = 0x40004000;
            *out = block;
        }
        Runtime_ReleaseHeapBlock(14);
    }
    Blend_SetBrightenTarget0(30);
    Blend_WaitForTransition();
    *(volatile u16 *)0x04000000 = 0x1540;
    if (prompt != 0)
        limit = 3600;
    else
        limit = 300;
    for (i = 0; i < limit; i++) {
        if (prompt != 0) {
            struct IntroSprite *sprite = work->sprites;
            u32 level;
            s32 x;
            u32 inverse;

            for (j = 0, x = PROMPT_X; j < PROMPT_SPRITES; j++) {
                sprite->x = x;
                sprite->y = PROMPT_Y;
                Runtime_PushSlotEntry(sprite, 0);
                x += 32;
                sprite++;
            }
            level = Title_PromptBlendLevels[i % 60];
            *(volatile u16 *)0x04000050 = 0x2f50;
            inverse = 16 - level;
            *(volatile u16 *)0x04000052 = (inverse << 8) + level;
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

#endif
