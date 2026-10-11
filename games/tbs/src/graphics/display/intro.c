#define OWN_Graphics_TransformSmallPalette
#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "types.h"
#include "callback_scheduler.h"
#include "dma.h"
#include "edition.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "map_scroll.h"
#include "ram_buffer.h"
#include "resource.h"
#include "resource_ids.h"
#include "palbuf.h"


s32 Graphics_TransformSmallPalette(); // no prototype was in scope: the call loads r0 last

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
    s32 rise;
    union {
        volatile u32 objects[120][2];
        struct {
            u8 unknown_18[0x68];
            struct IntroSprite sprites[8];
        } prompt;
    } area;
};

s32 Resource_DecodeByteLz(const void *source, void *destination);

extern u8 gDebugPaused;

// 六列のスプライトの x
const u8 Title_IntroSpriteColumns[] = { 96, 136, 140, 160, 168, 184 };

// プロンプトの明るさ（60 フレームで一巡）
const u8 Title_PromptBlendLevels[60] = {
	1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
};

#define ENTRY(i, word) (work->area.objects[i][word])

#define WRAP256(v) \
    while ((v) > 255) \
        (v) -= 256; \
    while ((v) < 0) \
        (v) += 256

#define PLACE(column, half) \
    x = Title_IntroSpriteColumns[column] - (half); \
    y = (top - base) * ((x + (half)) - 104) / 80 + base - (half); \
    WRAP256(y)

#define QUAD(i, size, shape, tile) \
    ENTRY(i, 0) = ((x + 4) << 16) | y | ((shape) + 0x00002400); \
    ENTRY((i) + 1, 0) = ((x + 4 + (size)) << 16) | y | ((shape) + 0x10002400); \
    ENTRY((i) + 2, 0) = ((x + 4) << 16) | (u8)(y + (size)) | ((shape) + 0x20002400); \
    ENTRY((i) + 3, 0) = ((x + 4 + (size)) << 16) | (u8)(y + (size)) | ((shape) + 0x30002400); \
    ENTRY(i, 1) = (tile); \
    ENTRY((i) + 1, 1) = (tile); \
    ENTRY((i) + 2, 1) = (tile); \
    ENTRY((i) + 3, 1) = (tile)

void Title_PlaceIntroSprites(void)
{
    struct IntroWork *work;
    u32 n;
    s32 base;
    s32 top;
    s32 x;
    s32 y;
    s32 line;

    work = Ram_WorkSlot[43];
    n = 0;
    if (gDebugPaused == 0) {
        if ((++work->tick & 3) == 0)
            work->rise++;
    }
    base = 48 - gBgScroll[1].y;
    top = 144 - work->rise;
    if (work->frame < 0x118) {
        if ((work->tick & 1) == 0) {
            PLACE(0, 16);
            QUAD(n, 16, 0x40000000, 232);
            n += 4;
            PLACE(2, 16);
            ENTRY(n, 0) = ((x + 4) << 16) | y | 0x80002400;
            ENTRY(n, 1) = 128;
            n++;
            PLACE(4, 32);
            QUAD(n, 32, 0x80000000, 192);
            n += 4;
        } else {
            PLACE(1, 16);
            QUAD(n, 16, 0x40000000, 232);
            n += 4;
            PLACE(3, 16);
            QUAD(n, 16, 0x40000000, 224);
            n += 4;
            PLACE(5, 32);
            QUAD(n, 32, 0x80000000, 160);
            n += 4;
        }
    }
    for (; n <= 119; n++)
        ENTRY(n, 0) = 0x400020a0;
    REG_BLDCNT = 0x3f50;
    REG_BLDALPHA = 0x0e0e;
    Dma_Set(&ENTRY(0, 0), OAM, ((n * 8) >> 2) | 0x84000000, REG_DMA3);
    line = 32 - gBgScroll[1].y;
    WRAP256(line);
    ENTRY(12, 0) = line | 0xc05c2000;
    ENTRY(12, 1) = 0x800;
    Dma_Set(&ENTRY(12, 0), (u8 *)OAM + n * 8, 0x84000002, REG_DMA3);
    Dma_Set(&ENTRY(0, 0), OAM, 0x84000008, REG_DMA3);
}

void Title_LoadIntroBackgrounds(void)
{
    struct IntroWork *work;
    u8 *data;
    u16 *map;
    s32 x;
    s32 y;
    u16 tile;
    s32 blank;

    work = Ram_WorkSlot[43];
    REG_DISPCNT = 0;
    data = Resource_GetTableEntry((s32)&ResourceId_IntroGraphicsA);
    Dma_Set(data, (void *)OBJ_PLTT, 0x84000080, REG_DMA3);
    OBJ_PLTT[0] = 0;
    data += 0x200;
    Resource_DecodeByteLz(data, Ram_MapCellBuffer);
    Dma_Set(Ram_MapCellBuffer, (void *)0x06010000, 0x80000f00, REG_DMA3);
    data = Resource_GetTableEntry((s32)&ResourceId_IntroGraphicsC);
    Dma_Set(data, (void *)BG_PLTT, 0x84000080, REG_DMA3);
    BG_PLTT[0] = 0;
    data += 0x200;
    Resource_DecodeByteLz(data, Ram_MapCellBuffer);
    Dma_Set(Ram_MapCellBuffer + 0x2940, BG_CHAR_BLOCK(0), 0x80002760, REG_DMA3);
    Dma_Set(Ram_MapCellBuffer + 0xa140, (void *)0x06004ec0, 0x80004ec0, REG_DMA3);

    blank = 0x1ff;
    map = (u16 *)BG_SCREEN_BLOCK(30);
    tile = 0x267;
    for (y = 0; y <= 10; y++) {
        for (x = 0; x < 30; x++)
            *map++ = tile++;
        *map++ = blank;
        *map++ = blank;
    }
    tile = 0x13b;
    for (y = 11; y <= 31; y++) {
        for (x = 0; x < 30; x++)
            *map++ = tile++;
        *map++ = blank;
        *map++ = blank;
    }
    map = (u16 *)BG_SCREEN_BLOCK(31);
    tile = 300;
    for (y = 0; y <= 10; y++) {
        for (x = 0; x < 30; x++)
            *map++ = tile++;
        *map++ = blank;
        *map++ = blank;
    }
    tile = 0;
    for (y = 11; y <= 31; y++) {
        for (x = 0; x < 30; x++)
            *map++ = tile++;
        *map++ = blank;
        *map++ = blank;
    }

    REG_BG1CNT = 0x1f43;
    REG_BG2CNT = 0x1e81;
    REG_WIN0H = 0xf0;
    REG_WIN0V = 0x9f;
    REG_WIN1H = 0xf0;
    REG_WIN1V = 0x9f;
    REG_WININ = 0x1616;
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
    work->rise = 0;
    work->state = 0;
    Dma_Set(gBgScroll, REG_BG0HOFS, 0x84000004, REG_DMA3);
    REG_BLDCNT = 0x3fbf;
    REG_BLDALPHA = 0x1010;
    REG_BLDCNT = 0x3f44;
}

#if defined(TBS_EDITION_DE)
#define PROMPT_TILES 8
#else
#define PROMPT_TILES 5
#endif
#if defined(TBS_EDITION_DE)
#define PROMPT_ROWS 2
#define PROMPT_SPRITES 3
#define PROMPT_X 80
#define PROMPT_Y 116
#elif !EDITION_INTERNATIONAL
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

extern u8 gOamCopyEnabled;
extern volatile u8 gCpuLoadDisplayEnabled;

s32 Title_ShowIntro(s32 prompt)
{
    struct IntroWork *work;
    s32 result;
    u8 saved;
    u16 zero;
    u8 *data;
    u16 *map;
    u32 i;
    u16 tile;
    u32 j;
    u32 limit;
    s32 blank;

    result = 0;
    blank = 0x1ff;
    saved = gCpuLoadDisplayEnabled;
    work = Runtime_AllocateHeapBlock(43, 224);
    Bg0_ClearTilemap();
    Resource_InitializeTable();
    WaitFrames(1);
    Scheduler_ResetTaskTable();
    gOamCopyEnabled = result;
    gCpuLoadDisplayEnabled = result;
    Title_LoadIntroBackgrounds();
    TitlePalette_InitializeBuffers();
    Graphics_TransformSmallPalette(2, 0);
    {
        volatile u16 *ime;
        struct IoWriteQueue *q;
        u32 old;
        s32 count;

        q = &gIoWriteQueue;
        /* FAKEMATCH: needed to match: removing this one-pass block changes instruction scheduling. */
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
            *destination++ = (u32)&REG_DISPCNT;
            *destination = 0x20000;
        }
        *ime = old;
    }
    Graphics_UpdatePaletteInterpolation(60);
    Scheduler_AddOrUpdateCallback(Title_PlaceIntroSprites, 0x480);

    for (;;) {
        s32 frame;
        s32 next;
        s32 tick;

        frame = work->frame;
        if (frame > 20 && frame < 239 && (gKeyState & (KEY_A | KEY_START)) != 0) {
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
                        0x800001e0, REG_DMA3);
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
                            0x800003c0, REG_DMA3);
                    } else {
                        zero = 0;
                        Dma_Set(&zero, (u8 *)0x06004ec0 + (160 - work->front_rows * 8) % 160 * 240, 0x810003c0,
                            REG_DMA3);
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
        } else if (next == 281) {
            work->state = 2;
        } else if (next == 289) {
            work->state = 0;
        } else if (next == 280) {
            Blend_SetBrightenTarget16(1);
            Scheduler_RemoveCallback(Title_PlaceIntroSprites);
            gOamCopyEnabled = 1;
            WaitFrames(1);
            REG_BG2CNT = 0x681;
            REG_DISPCNT = 0x1440;
            gBgScroll[2].y = 0;
            data = Resource_GetTableEntry((s32)&ResourceId_IntroGraphicsB);
            Dma_Set(data, (void *)BG_PLTT, 0x84000078, REG_DMA3);
            BG_PLTT[0] = 0;
            data += 0x200;
            Resource_DecodeByteLz(data, Ram_MapCellBuffer);
            Dma_Set(Ram_MapCellBuffer, BG_CHAR_BLOCK(1), 0x80004b00, REG_DMA3);
            map = (u16 *)BG_SCREEN_BLOCK(6);
            tile = 0x100;
            i = 0;
            break;
        }
        WaitFrames(1);
    }
    for (; i < 20; i++) {
        for (j = 0; j < 30; j++)
            *map++ = tile++;
        *map++ = blank;
        *map++ = blank;
    }

    Ui_LoadWindowGraphics();
    Bg0_ClearTilemap();
    if (prompt != 0) {
        u8 *buffer = Runtime_AllocateBlock(14, 0x400);
        struct IntroSprite *sprite = work->area.prompt.sprites;

        Resource_DecodeByteLz(Title_PromptTiles, buffer);
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
    REG_DISPCNT = 0x1540;
    if (prompt != 0)
        limit = 3600;
    else
        limit = 300;
    for (i = 0; i < limit; i++) {
        if (prompt != 0) {
            struct IntroSprite *sprite = work->area.prompt.sprites;
            u32 level;
            s32 x;
#if defined(TBS_EDITION_DE)
            u32 row;
            s32 y;

            for (row = 0, y = PROMPT_Y; row < PROMPT_ROWS; y += 8, row++) {
                for (j = 0, x = PROMPT_X; j < PROMPT_SPRITES; j++) {
                    sprite->x = x;
                    sprite->y = y;
                    Runtime_PushSlotEntry(sprite, 0);
                    x += 32;
                    sprite++;
                }
            }
#else

            for (j = 0, x = PROMPT_X; j < PROMPT_SPRITES; j++) {
                sprite->x = x;
                sprite->y = PROMPT_Y;
                Runtime_PushSlotEntry(sprite, 0);
                x += 32;
                sprite++;
            }
#endif
            level = Title_PromptBlendLevels[i % 60];
            REG_BLDCNT = 0x2f50;
            REG_BLDALPHA = (16 - level) * 256 + level;
        }
        if ((gKeyState & (KEY_A | KEY_START)) != 0) {
            result = 1;
            break;
        }
        WaitFrames(1);
    }
    gCpuLoadDisplayEnabled = saved;
    Runtime_ReleaseHeapBlock(43);
    REG_BLDCNT = 0;
    REG_BLDALPHA = 0;
    WaitFrames(1);
    return result;
}

void AudioCommand_PlayFar(s32 cue);

void Title_NoOpPalette(void)
{
}

s32 Title_ShowSplashScreen(s32 mode)
{
    s32 result;
    u8 *data;
    u16 *map;
    u32 x;
    u32 y;
    u16 tile;
    s32 blank;
    s32 resource;

    AudioCommand_PlayFar(110);
    gOamCopyEnabled = 1;
    resource = (s32)&ResourceId_NintendoLogo;
    blank = 0x1ff;
    Scheduler_ResetTaskTable();
    Blend_SetDarkenTarget16(1);
    Bg0_ClearTilemap();
    WaitFrames(1);
    REG_BG2CNT = 0x681;
    REG_DISPCNT = 0x1440;
    result = 0;
    gBgScroll[2].y = result;
    data = Resource_GetTableEntry(resource);
    Dma_Set(data, (void *)BG_PLTT, 0x84000070, REG_DMA3);
    Resource_DecodeType01(data + 448, Ram_MapCellBuffer);
    Dma_Set(Ram_MapCellBuffer, BG_CHAR_BLOCK(1), 0x84002580, REG_DMA3);
    tile = 256;
    map = (u16 *)BG_SCREEN_BLOCK(6);
    for (y = 0; y < 20; y++) {
        for (x = 0; x < 30; x++)
            *map++ = tile++;
        *map++ = blank;
        *map++ = blank;
    }
    for (y = 0; y < 4; y++) {
        gBgScroll[y].y = 0;
        gBgScroll[y].x = 0;
    }
    Dma_Set(gBgScroll, REG_BG0HOFS, 0x84000004, REG_DMA3);
    Ui_LoadWindowGraphics();
    Bg0_ClearTilemap();
    REG_DISPCNT = 0x1540;
    if (mode == 0) {
        Blend_SetDarkenTarget0(1);
        Blend_WaitForTransition();
        for (y = 0; y < 120; y++) {
            if (gKeyState & (KEY_A | KEY_START)) {
                result = -1;
                break;
            }
            WaitFrames(1);
        }
        return result;
    }
    for (y = 0; y < 60; y++) {
        if (gKeyState & (KEY_A | KEY_START)) {
            result = -1;
            break;
        }
        WaitFrames(1);
    }
    if (result != 0)
        Blend_SetDarkenTarget0(8);
    else
        Blend_SetDarkenTarget0(60);
    Blend_WaitForTransition();
    if (result == 0) {
        for (y = 0; y < 180; y++) {
            if (gKeyState & (KEY_A | KEY_START)) {
                result = -1;
                break;
            }
            WaitFrames(1);
        }
    }
    if (result != 0)
        Blend_SetDarkenTarget16(8);
    else
        Blend_SetDarkenTarget16(60);
    Blend_WaitForTransition();
    return result;
}

#ifndef MODERN // modern GCC keeps the headers' volatile declaration
#endif

s32 Title_ShowAnimatedSplash(void)
{
    s32 result;
    u32 i;
    u8 *buffer;
    s32 resource;

    gOamCopyEnabled = 1;
    result = 0;
    resource = (s32)&ResourceId_CamelotLogo;
    Scheduler_ResetTaskTable();
    Blend_SetDarkenTarget16(1);
    Bg0_ClearTilemap();
    WaitFrames(1);
    REG_BG2CNT = 0x685;
    REG_DISPCNT = 0x1440;
    gBgScroll[2].y = result;
    /* FAKEMATCH: needed to match: i carries the decode destination before it counts; a pointer of its own is coalesced with buffer (31 differ). */
    i = (u32)Ram_MapCellBuffer;
    Resource_DecodeType01(Resource_GetTableEntry(resource), (u8 *)i);
    buffer = (u8 *)i;
    Dma_Set(buffer, (void *)BG_PLTT, DMA_ENABLE32 | DMA_32BIT | 0x1c0 / 4, REG_DMA3);
    buffer += 0x1c0;
    Dma_Set(buffer, BG_SCREEN_BLOCK(6), DMA_ENABLE32 | DMA_32BIT | 0x800 / 4, REG_DMA3);
    buffer += 0x800;
    Dma_Set(buffer, BG_CHAR_BLOCK(1), DMA_ENABLE32 | DMA_32BIT | 0x4000 / 4, REG_DMA3);
    buffer += 0x4000;
    for (i = 0; i < 4; i++) {
        gBgScroll[i].y = 0;
        gBgScroll[i].x = 0;
    }
    Dma_Set(gBgScroll, REG_BG0HOFS, DMA_ENABLE32 | DMA_32BIT | 4, REG_DMA3);
    Ui_LoadWindowGraphics();
    Bg0_ClearTilemap();
    Blend_SetBrightenTarget0(1);
    Blend_WaitForTransition();
    REG_DISPCNT = 0x1540;
    for (i = 0; i < 120; i++) {
        Dma_Set(((gFrameCount >> 3) & 3) * 0x400 + buffer, BG_CHAR_BLOCK(1) + 0x100, DMA_ENABLE32 | DMA_32BIT | 0x340 / 4, REG_DMA3);
        if (gKeyState & (KEY_A | KEY_START))
            break;
        WaitFrames(1);
    }
    return result;
}

void Graphics_NoOpColor(void)
{
}
