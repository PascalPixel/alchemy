#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"
extern u8 Data_03001e8c[];

extern void Scheduler_RemoveCallback(s32);
extern void BattleFx_ArmBg0HBlankDma(void);
extern s32 PaletteGlow_UpdateFar(s32, s32);
extern u8 gGameState[];

void Ui_SetBank15PaletteAndClearRenderMode(void)
{
    void *work;

    work = *(void **)((u32)&Data_03001e8c);
    Scheduler_RemoveCallback((s32)BattleFx_ArmBg0HBlankDma);
    *(volatile s16 *)0x050001E2 = 0x7FFF;
    *(s16 *)0x050001E6 = 0;
    *(volatile s16 *)0x050001F6 = 0x294A;
    *(volatile s16 *)0x050001F8 = 0x5294;
    PaletteGlow_UpdateFar(gGameState[0x205], gGameState[0x206]);
    *((u8 *)work + RENDER_MODE_OFS) = 0;
}
