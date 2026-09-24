#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 ScheduleCallbackAfterFrames(s32, s32);
void BattleFx_ArmBg0HBlankDma(void);

void Ui_FillBank15PaletteGrey(void)
{
    volatile s16 *p;

    FIELD_AT_OFFSET(*(void **)ADDR_03001E8C, s8, RENDER_MODE_OFS) = 1;
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
    ScheduleCallbackAfterFrames((s32)BattleFx_ArmBg0HBlankDma, 0x480);
}
