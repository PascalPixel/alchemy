#include "TYPES.H"

void Engine_TaskRemoveCallback(void *callback);
void DisplayScroll_ArmHBlankDma(void);
void DisplayScroll_BuildAndSwapHBlankPage(void);

/* Remove the two hblank scroll callbacks and stop DMA 0. */
void SceneEffect_LoadTablesAndStopDma0(void)
{
    volatile u16 *reg;

    Engine_TaskRemoveCallback(DisplayScroll_ArmHBlankDma);
    Engine_TaskRemoveCallback(DisplayScroll_BuildAndSwapHBlankPage);
    reg = (volatile u16 *)0x040000B0;
    reg[5] = 0xC5FF & reg[5];
    reg[5] = 0x7FFF & reg[5];
    reg[5];
}
