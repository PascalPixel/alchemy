#include "types.h"
#include "global_cells.h"
#include "gs1_edition.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void ScheduleCallbackAfterFrames(s32, s32);
void Func_08097868(void);

void Func_08097a7c(void) {
    volatile s16 *palette;

    M2C_FIELD(*(void **)ADDR_03001E8C, s8, RENDER_MODE_OFS) = 1;
    palette = (s16 *)0x050001E2;
    *palette = 0x739C;
    palette += 2;
    *palette = 0x739C;
    palette += 1;
    *palette = 0x739C;
    palette += 1;
    *palette = 0x739C;
    palette += 1;
    *palette = 0x739C;
    palette += 1;
    *palette = 0x739C;
    palette += 1;
    *palette = 0x739C;
    palette += 1;
    *palette = 0x739C;
    palette += 1;
    *palette = 0x739C;
    palette += 1;
    *palette = 0x739C;
    palette += 1;
    *palette = 0x739C;
    {
        s32 n = 0x480;
        ScheduleCallbackAfterFrames((s32)Func_08097868, n);
    }
}
