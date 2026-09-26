/* Draft, not exact (2026-09-26): candidate=484 reference=484, 27 differing
   halfwords. The title-background recipe keeps the signed tile wrap explicit
   and starts blank's lifetime before Scheduler_ResetTaskTable; mode, result
   and blank now occupy the reference's r8, r6 and r7. Reusing the outer row
   counter for scroll clearing and frame waits fixes its r5 assignment and
   extends the exact prefix to 120 halfwords. Residual: key value/mask use
   r2/r3 instead of r3/r2, and frame increments precede the wait argument.
   Splitting the three key locals and incrementing inside the call argument
   changed no bytes; reusing the inner counter left 33 differing halfwords.
   The key waits remain goto loops because the reference reloads key state
   and mask each pass. No exact-C credit. */
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
    s32 tile;
    s32 blank;
    s32 resource;
    u32 keys;

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
        keys = gKeyState & 9;
        y = 0;
        goto check_start;
    wait_start:
        y++;
        WaitFrames(1);
        if (y > 119)
            goto done;
        keys = gKeyState & 9;
    check_start:
        if (keys == 0)
            goto wait_start;
        result = -1;
        goto done;
    }
    keys = gKeyState & 9;
    y = 0;
    goto check_fade_in;
wait_fade_in:
    y++;
    WaitFrames(1);
    if (y > 59)
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
        y = 0;
        goto check_hold;
    wait_hold:
        y++;
        WaitFrames(1);
        if (y > 179)
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
