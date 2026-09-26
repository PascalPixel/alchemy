/* Not-yet-C, complete 328-byte owner. Separate I/O ports and an s32 inline
 * store keep the reference's word pools: 332 bytes, 75 aligned halfword edits.
 * Volatile ports retain that count, but DISP CNT is rematerialized across
 * calls instead of held in r5; nested-loop scalars still get different
 * registers. Computing block after the resource call grows to 336 bytes and
 * 81 edits. Three structural hypotheses stopped; no adoption or byte credit. */
#include "TYPES.H"
#include "SYSTEM.H"

extern u8 Data_03001d18;
extern u8 Data_03001f58;
extern u8 Data_03001ac4;
extern u8 Data_03001d08;
extern const u32 Data_080f0a5c[];
void Scheduler_ResetTaskTable(void);
s32 Scheduler_AddOrUpdateCallback(void *callback, s32 priority);
void DisplayScroll_StepPositionEveryFourFrames(void);
void DisplayScroll_BuildHblankWordTable(void *tiles);
void Graphics_ClearCharacterBlockAndPalette(s32 block);
void Graphics_LoadCharacterBlockAndPalette(u32 resource, s32 block);
void DisplayScroll_InitObjectTable(void);
void Ui_LoadWindowGraphics(void);
void Bg0_ClearTilemap(void);

static __inline__ void WriteHalf(volatile u16 *port, s32 value)
{
    *port = value;
}

/* Crossfade consecutive images between BG2 and BG3, then restore the menu. */
s32 Func_080f03f0(void)
{
    u32 frame;
    s32 block;
    s32 base;
    s32 alpha;
    const u32 *table;
    volatile u16 *display = (volatile u16 *)0x04000000;
    volatile u16 *backgrounds = (volatile u16 *)0x0400000c;
    volatile u16 *blend = (volatile u16 *)0x04000050;

    Data_03001d18 = 0;
    Data_03001f58 = 0;
    Data_03001ac4 = 0;
    Data_03001d08 = 0;
    Scheduler_ResetTaskTable();
    Scheduler_AddOrUpdateCallback(DisplayScroll_StepPositionEveryFourFrames, 1152);
    WriteHalf(display, 64);
    DisplayScroll_BuildHblankWordTable((void *)0x06007800);
    DisplayScroll_BuildHblankWordTable((void *)0x0600f800);
    Graphics_ClearCharacterBlockAndPalette(0);
    Graphics_ClearCharacterBlockAndPalette(1);
    WriteHalf(backgrounds, 0x1f8a);
    WriteHalf(backgrounds + 1, 0x0f83);
    WriteHalf(display, 0x1c40);
    WriteHalf(blend, 0x2844);
    DisplayScroll_InitObjectTable();
    WaitFrames(300);
    frame = 0;
    table = Data_080f0a5c;
    do {
        block = frame & 1;
        Graphics_LoadCharacterBlockAndPalette(*table, block ^ 1);
        base = 0xf00;
        alpha = 1;
        do {
            if (block != 0)
                WriteHalf(blend + 1, (alpha << 8) | (16 - alpha));
            else
                WriteHalf(blend + 1, base | alpha);
            WaitFrames(4);
            alpha++;
            base -= 256;
        } while (alpha <= 16);
        WaitFrames(267);
        frame++;
        table++;
    } while (frame <= 32);
    WriteHalf(blend, 0);
    WriteHalf(display, 0x1040);
    Ui_LoadWindowGraphics();
    Bg0_ClearTilemap();
    Data_03001d18 = 1;
    return 0;
}
