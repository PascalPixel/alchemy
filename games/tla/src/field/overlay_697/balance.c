#include "types.h"
#include "field_event.h"

extern u8 gSceneState[];

void TrialRoad_Idle(void)
{
}

void TrialRoad_SetBalanceStateReady(void)
{
    u16 *state = (u16 *)gSceneState;
    /* FAKEMATCH: the temporary builds 9 with movs instead of a halfword pool load. */
    u16 ready = 9;

    *state = ready;
}

void TrialRoad_WaitForBalanceState(void)
{
    s16 *state = (s16 *)gSceneState;

    while (*state != 9) {
        Task_Wait(1);
    }
}
