/* NONMATCHING: resource_3bc at 0x02008a84 (80 bytes with its pool),
 * ColossoLogRollingStage_WaitForSceneEventTask, between
 * FIELD/KOROSSEO_MARUTA/LOG_ROLLING.C and ACTIVE_ACTOR.C, stays listing.
 *
 * Remaining difference: the reference tests the scene task's status through
 * one register before the wait loop and copies the address to a saved
 * register for the loop, as a literal address does. With the status named,
 * the compiler shares one register and merges the first test into the loop
 * (72 bytes); a volatile first read does not change that.
 */
#include "SITES.H"

void ColossoLogRollingStage_WaitForSceneEventTask(void)
{
    s32 *status;
    s32 value;

    Func_0200561a_wait_for_scene_event_task(28);
    Func_02005448_wait_for_scene_event_task(0x361);
    Func_020052d6_wait_for_scene_event_task(10);
    value = *(volatile s32 *)&gColossoSceneTaskStatus;
    if (value != 1 && value != 3) {
        status = &gColossoSceneTaskStatus;
        do {
            Func_020052ea_wait_for_scene_event_task(1);
            value = *status;
        } while (value != 1 && value != 3);
    }
    Func_020052fa_wait_for_scene_event_task(1);
    Func_02005310_wait_for_scene_event_task((s32)ColossoLogRollingStage_SceneTask);
}
