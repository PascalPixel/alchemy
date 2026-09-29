#include "TYPES.H"
#include "SYSTEM.H"
#include "DMA.H"

struct BgScroll {
    s16 x;
    s16 y;
};

extern u8 Data_03001d18;
extern struct BgScroll gBgScroll[4];
extern volatile u32 gKeyState;
extern u8 Value_00000018[];

void Audio_PlayCue(s32 cue);
void Scheduler_ResetTaskTable(void);
void Blend_SetDarkenTarget16(s32 frames);
void Blend_SetDarkenTarget0(s32 frames);
void Blend_WaitForTransition(void);
void Bg0_ClearTilemap(void);
void Ui_LoadWindowGraphics(void);
u8 *Resource_GetTableEntry(s32 index);
s32 Resource_DecodeType01(const void *source, void *destination);

/* Title: show the splash picture, fade it in and wait for A or START (or
   time out), then fade it out. Mode 0 only waits for two seconds after the
   fade-in. Returns -1 when a button cut it short. */
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
    Data_03001d18 = 1;
    resource = (s32)Value_00000018;
    blank = 0x1ff;
    Scheduler_ResetTaskTable();
    Blend_SetDarkenTarget16(1);
    Bg0_ClearTilemap();
    WaitFrames(1);
    *(volatile u16 *)0x0400000c = 0x681;
    *(volatile u16 *)0x04000000 = 0x1440;
    result = 0;
    gBgScroll[2].y = result;
    data = Resource_GetTableEntry(resource);
    Dma_Set(data, (void *)0x05000000, 0x84000070, (volatile u32 *)0x040000d4);
    Resource_DecodeType01(data + 448, (void *)0x02010000);
    Dma_Set((void *)0x02010000, (void *)0x06004000, 0x84002580, (volatile u32 *)0x040000d4);
    tile = 256;
    map = (u16 *)0x06003000;
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
    Dma_Set(gBgScroll, (void *)0x04000010, 0x84000004, (volatile u32 *)0x040000d4);
    Ui_LoadWindowGraphics();
    Bg0_ClearTilemap();
    *(volatile u16 *)0x04000000 = 0x1540;
    if (mode == 0) {
        Blend_SetDarkenTarget0(1);
        Blend_WaitForTransition();
        for (y = 0; y < 120; y++) {
            if (gKeyState & 9) {
                result = -1;
                break;
            }
            WaitFrames(1);
        }
        return result;
    }
    for (y = 0; y < 60; y++) {
        if (gKeyState & 9) {
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
            if (gKeyState & 9) {
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
