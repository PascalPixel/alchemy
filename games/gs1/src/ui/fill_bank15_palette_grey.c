#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "gs1_edition.h"

/* ui/fill_bank15_palette_grey.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void ScheduleCallbackAfterFrames(s32, s32);

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
    {
        s32 n = 0x480;
        ScheduleCallbackAfterFrames((s32)Ui_Run, n);
    }
}

/* ui/set_bank15_palette_and_clear_render_mode.c */
extern void ScheduleCallback(s32);

extern u8 gCell[];

void Ui_SetBank15PaletteAndClearRenderMode(void)
{
    void *work;

    work = *(void **)ADDR_03001E8C;
    ScheduleCallback((s32)Ui_Run);
    *(volatile s16 *)0x050001E2 = 0x7FFF;
    *(s16 *)0x050001E6 = 0;
    *(volatile s16 *)0x050001F6 = 0x294A;
    *(volatile s16 *)0x050001F8 = 0x5294;
    Ui_Apply(gCell[0x205], gCell[0x206]);
    *((u8 *)work + RENDER_MODE_OFS) = 0;
}
