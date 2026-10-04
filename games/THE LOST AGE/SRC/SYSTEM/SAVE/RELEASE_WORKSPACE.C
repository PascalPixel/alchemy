#include "TYPES.H"
#include "RUNTIME_INTERFACES.H"
#include "IRQ.H"

/* Removes the save's serial interrupt handler and frees its workspace. */
void SaveState_ReleaseWorkspace(void)
{
    Runtime_SetIrqHandler(5, 0, NULL);
    Runtime_ReleaseHeapBlock(204);
}
