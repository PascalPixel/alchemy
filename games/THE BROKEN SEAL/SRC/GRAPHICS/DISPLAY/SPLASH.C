#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "RESOURCE.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "DMA.H"
#include "RESOURCE_IDS.H"
#include "RAM_BUFFER.H"
#include "MAP_SCROLL.H"

#define gKeyState (*(volatile u32 *)&gKeyState)

extern u8 gOamCopyEnabled;

void Audio_PlayCue(s32 cue);

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
    s32 tile;
    s32 blank;
    s32 resource;

    Audio_PlayCue(110);
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
    y = 0;
col:
    {
        x = 0;
row:
        {
            s32 old = tile;

            /* FAKEMATCH: keep the signed tile wrap in the high half. */
            tile = ((old << 16) + 0x10000) >> 16;
            *map++ = old;
        }
        if (++x <= 29)
            goto row;
        *map++ = blank;
        *map++ = blank;
    }
    if (++y <= 19)
        goto col;
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
extern u32 gFrameCount;
#endif

s32 Title_ShowAnimatedSplash(void)
{
    s32 result;
    u32 i;
    /* FAKEMATCH: separate the post-decode buffer carrier from the resource-number carrier. */
    register u8 *buffer __asm__("r6");
    /* FAKEMATCH: keep the decode destination carrier distinct until the buffer copy. */
    register u8 *decode __asm__("r5");
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
    decode = Ram_MapCellBuffer;
    Resource_DecodeType01(Resource_GetTableEntry(resource), decode);
    buffer = decode;
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
        /* FAKEMATCH: the unsigned address sum retains the DMA source operand order. */
        Dma_Set((void *)((((gFrameCount >> 3) & 3) << 10) + (u32)buffer), BG_CHAR_BLOCK(1) + 0x100, DMA_ENABLE32 | DMA_32BIT | 0x340 / 4, REG_DMA3);
        if (gKeyState & (KEY_A | KEY_START))
            break;
        WaitFrames(1);
    }
    return result;
}
