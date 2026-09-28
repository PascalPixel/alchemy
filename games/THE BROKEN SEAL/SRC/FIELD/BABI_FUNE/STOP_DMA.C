#include "TYPES.H"

void Engine_TaskRemoveCallback(void *callback);
void Engine_ArmScrollDma(void);
void Engine_BuildScrollPage(void);

/* Remove the two hblank scroll callbacks and stop DMA 0. */
void SceneEffect_LoadTablesAndStopDma0(void)
{
    volatile u16 *reg;

    Engine_TaskRemoveCallback(Engine_ArmScrollDma);
    Engine_TaskRemoveCallback(Engine_BuildScrollPage);
    reg = (volatile u16 *)0x040000B0;
    reg[5] = 0xC5FF & reg[5];
    reg[5] = 0x7FFF & reg[5];
    reg[5];
}
