#include "MURA.H"

/* Returns the scene's message table. */
u8 *SceneData_GetMessageTable(void)
{
    return Placement_Messages;
}

void SceneState_SetValues9_3_0(void)
{
    BattleFx_RunPageEffectForSlot(9, 3, 0);
}
