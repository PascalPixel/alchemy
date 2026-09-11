#include "types.h"
#include "scene.h"
#include "abi/ui/set_bank15_palette_and_clear_render_mode.h"
#include "global_cells.h"
#include "gs1_edition.h"

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
