#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"

extern u8 Data_03001e8c[];
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))
s32 Scheduler_AddOrUpdateCallback(s32, s32);
void BattleFx_ArmBg0HBlankDma(void);

extern void Scheduler_RemoveCallback(s32);
extern void BattleFx_ArmBg0HBlankDma(void);
extern s32 PaletteGlow_UpdateFar(s32, s32);
extern u8 gGameState[];

void Ui_FillBank15PaletteGrey(void)
{
    volatile s16 *p;

    FIELD_AT_OFFSET(*(void **)((u32)&Data_03001e8c), s8, RENDER_MODE_OFS) = 1;
    p = (s16 *)0x050001E2;
    *p = 0x739C;
    p += 2;
    *p = 0x739C;
    p += 1;
    *p = 0x739C;
    p += 1;
    *p = 0x739C;
    p += 1;
    *p = 0x739C;
    p += 1;
    *p = 0x739C;
    p += 1;
    *p = 0x739C;
    p += 1;
    *p = 0x739C;
    p += 1;
    *p = 0x739C;
    p += 1;
    *p = 0x739C;
    p += 1;
    *p = 0x739C;
    Scheduler_AddOrUpdateCallback((s32)BattleFx_ArmBg0HBlankDma, 0x480);
}

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
