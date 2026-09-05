#include "types.h"
#include "global_cells.h"
#include "gs1_edition.h"

extern void ScheduleCallback(s32);
extern void Func_08097868(void);
extern s32 Func_08015360(s32, s32);
extern u8 Data_02000240[];

void Ui_SetBank15PaletteAndClearRenderMode(void) {
    void *work;

    work = *(void **)ADDR_03001E8C;
    ScheduleCallback((s32)Func_08097868);
    *(volatile s16 *)0x050001E2 = 0x7FFF;
    *(s16 *)0x050001E6 = 0;
    *(volatile s16 *)0x050001F6 = 0x294A;
    *(volatile s16 *)0x050001F8 = 0x5294;
    Func_08015360(Data_02000240[0x205], Data_02000240[0x206]);
    *((u8 *)work + RENDER_MODE_OFS) = 0;
}
