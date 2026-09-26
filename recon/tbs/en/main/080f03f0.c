/* Draft: complete 328-byte owner. Candidate 332 bytes; 77 aligned
   halfword edits remain in I/O literal materialization, register allocation
   and pool placement. Recovered BG3, blend control and alpha-loop values. */
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

struct DisplayScrollIo {
    u16 control;
    u8 reserved_02[10];
    u16 bg2_control;
    u16 bg3_control;
    u8 reserved_10[0x40];
    u16 blend_control;
    u16 blend_alpha;
};

/* Crossfade consecutive images between BG2 and BG3, then restore the menu. */
s32 Func_080f03f0(void)
{
    u32 frame;
    s32 block;
    s32 base;
    s32 alpha;
    const u32 *table;
    struct DisplayScrollIo *io = (void *)0x04000000;

    Data_03001d18 = 0;
    Data_03001f58 = 0;
    Data_03001ac4 = 0;
    Data_03001d08 = 0;
    Scheduler_ResetTaskTable();
    Scheduler_AddOrUpdateCallback(DisplayScroll_StepPositionEveryFourFrames, 1152);
    io->control = 64;
    DisplayScroll_BuildHblankWordTable((void *)0x06007800);
    DisplayScroll_BuildHblankWordTable((void *)0x0600f800);
    Graphics_ClearCharacterBlockAndPalette(0);
    Graphics_ClearCharacterBlockAndPalette(1);
    io->bg2_control = 0x1f8a;
    io->bg3_control = 0x0f83;
    io->control = 0x1c40;
    io->blend_control = 0x2844;
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
                io->blend_alpha = (alpha << 8) | (16 - alpha);
            else
                io->blend_alpha = base | alpha;
            WaitFrames(4);
            alpha++;
            base -= 256;
        } while (alpha <= 16);
        WaitFrames(267);
        frame++;
        table++;
    } while (frame <= 32);
    io->blend_control = 0;
    io->control = 0x1040;
    Ui_LoadWindowGraphics();
    Bg0_ClearTilemap();
    Data_03001d18 = 1;
    return 0;
}
