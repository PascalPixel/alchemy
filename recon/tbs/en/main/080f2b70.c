/* Draft, not exact (2026-09-24): candidate=512 reference=484, 243 differing
   halfwords. Hand-written from the assembly; the key waits are goto loops as
   the reference reloads the key state and mask each pass. Residual: the
   reference keeps mode in r8, the result in r6 and the 0x1ff blank tile in r7
   from before the palette copy, so the tilemap loop rebuilds its 0x10000 step
   each pass; here the step is hoisted and mode and result go to high
   registers. */
/* Title: show the splash picture, fade it in and wait for A or START (or
   time out), then fade it out. Returns -1 when a button cut it short. */
#include "TYPES.H"
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
void WaitFrames(s32 frames);
u8 *Resource_GetTableEntry(s32 index);
s32 Resource_DecodeType01(const void *source, void *destination);

s32 Title_ShowSplashScreen(s32 mode)
{
    s32 result;
    u8 *data;
    u16 *map;
    u32 x;
    u32 y;
    s16 tile;
    u32 i;
    s32 resource;
    u32 keys;

    Audio_PlayCue(110);
    Data_03001d18 = 1;
    resource = (s32)Value_00000018;
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
    for (y = 0; y < 20; y++) {
        for (x = 0; x < 30; x++)
            *map++ = tile++;
        *map++ = 0x1ff;
        *map++ = 0x1ff;
    }
    for (i = 0; i < 4; i++) {
        gBgScroll[i].y = 0;
        gBgScroll[i].x = 0;
    }
    Dma_Set(gBgScroll, (void *)0x04000010, 0x84000004, (volatile u32 *)0x040000d4);
    Ui_LoadWindowGraphics();
    Bg0_ClearTilemap();
    *(volatile u16 *)0x04000000 = 0x1540;
    if (mode == 0) {
        Blend_SetDarkenTarget0(1);
        Blend_WaitForTransition();
        keys = gKeyState & 9;
        i = 0;
        goto check_start;
    wait_start:
        i++;
        WaitFrames(1);
        if (i > 119)
            goto done;
        keys = gKeyState & 9;
    check_start:
        if (keys == 0)
            goto wait_start;
        result = -1;
        goto done;
    }
    keys = gKeyState & 9;
    i = 0;
    goto check_fade_in;
wait_fade_in:
    i++;
    WaitFrames(1);
    if (i > 59)
        goto fade_in_done;
    keys = gKeyState & 9;
check_fade_in:
    if (keys == 0)
        goto wait_fade_in;
    result = -1;
fade_in_done:
    if (result != 0)
        Blend_SetDarkenTarget0(8);
    else
        Blend_SetDarkenTarget0(60);
    Blend_WaitForTransition();
    if (result == 0) {
        keys = gKeyState & 9;
        i = 0;
        goto check_hold;
    wait_hold:
        i++;
        WaitFrames(1);
        if (i > 179)
            goto hold_done;
        keys = gKeyState & 9;
    check_hold:
        if (keys == 0)
            goto wait_hold;
        result = -1;
    }
hold_done:
    if (result != 0)
        Blend_SetDarkenTarget16(8);
    else
        Blend_SetDarkenTarget16(60);
    Blend_WaitForTransition();
done:
    return result;
}
