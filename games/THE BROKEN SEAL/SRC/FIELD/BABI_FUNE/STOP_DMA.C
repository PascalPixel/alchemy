#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SCROLL.H"
#include "IO_REG.H"

/* Remove the two hblank scroll callbacks and stop DMA 0. */
void SceneEffect_LoadTablesAndStopDma0(void)
{
    volatile u16 *reg;

    Engine_TaskRemoveCallback(DisplayScroll_ArmHBlankDma);
    Engine_TaskRemoveCallback(DisplayScroll_BuildAndSwapHBlankPage);
    reg = REG_DMA0;
    reg[5] = 0xC5FF & reg[5];
    reg[5] = 0x7FFF & reg[5];
    reg[5];
}
