#include "types.h"
#include "global_cells.h"
#include "gs1_edition.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void ScheduleCallbackAfterFrames(s32, s32);
void Func_08097868(void);

void Ui_FillBank15PaletteGrey(void) {
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
    {
        s32 n = 0x480;
        ScheduleCallbackAfterFrames((s32)Func_08097868, n);
    }
}
