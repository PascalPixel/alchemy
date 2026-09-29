/* The two page effects the cave's opening runs. */
#include "IMIRU_FUCHIN.H"

/*
 * Imports. Each alias names the call word its site encodes, not a runtime
 * address. Only those used for their return value are typed, and the
 * declarations are old-style because one name is reached with different
 * argument counts.
 */
void SceneState_ApplyValues8And2And1(void)
{
    BattleFx_RunPageEffectForSlot(8, 2, 1);
}

void SceneState_ApplyValues11And62(void)
{
    BattleFx_SetPhaseRequest(0xB, 0x3E);
}
