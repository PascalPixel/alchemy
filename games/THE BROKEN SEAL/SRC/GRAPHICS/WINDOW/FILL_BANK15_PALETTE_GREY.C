#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"
extern u8 Data_03001e8c[];

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Scheduler_AddOrUpdateCallback(s32, s32);
void BattleFx_ArmBg0HBlankDma(void);

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
