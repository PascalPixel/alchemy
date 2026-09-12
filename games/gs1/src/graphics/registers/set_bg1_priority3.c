#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "render_input.h"

/* graphics/registers/set_bg1_priority3.c */
/* graphics/registers/set_bg1_priority3.c */
#define REG_BG1CNT (*(volatile u16 *)0x0400000a)

void Graphics_SetBg1Priority3(void)
{
    u32 mask = 4;
    u32 value = REG_BG1CNT;

    mask = -mask;
    value &= mask;
    value |= 3;
    REG_BG1CNT = value;
}

/* graphics/registers/clear_bg1_control_bit2.c */
void Graphics_ClearBg1ControlBit2(void)
{
    u32 mask = 4;
    u32 value = REG_BG1CNT;

    mask = -mask;
    value &= mask;
    REG_BG1CNT = value;
}
