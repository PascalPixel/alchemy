#include "TYPES.H"
#include "FIELD_EVENT.H"

/* The logs' balance state, the first halfword of the scene's saved words:
 * 9 once the competitors stand balanced, which the stage start waits for. */

extern u8 gSceneState[];

/* A stage hook this stage leaves empty. */
void ColossoLogRollingStage_Idle(void)
{
}

void ColossoLogRollingStage_SetBalanceStateReady(void)
{
    u16 *state = (u16 *)gSceneState;
    /* FAKEMATCH: the forced temporary builds 9 with movs; stored directly,
     * the halfword constant is loaded from the pool. */
    u16 ready = 9;

    *state = ready;
}

void ColossoLogRollingStage_WaitForBalanceState(void)
{
    s16 *state = (s16 *)gSceneState;

    while (*state != 9) {
        Task_Wait(1);
    }
}
