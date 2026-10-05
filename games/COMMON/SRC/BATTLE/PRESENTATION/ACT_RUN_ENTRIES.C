#include "TYPES.H"
#include "BATTLE_EVENT.H"
#include "BATTLE_PRESENTATION.H"
#include "BATTLE_TYPES.H"
#include "SYSTEM.H"
void BattlePres_SetActorModes(u16 *, s32);
void BattlePresentation_WaitForAdvance(void);
s32 Battle_ResolveTargetAction(struct BattlePlan *plan, s32 slot);

/* Resolve each target action and dispatch its queued presentation events. */
s32 BattlePres_RunActorEntries(struct BattlePlan *plan, s32 unused)
{
    s32 i;

    BattlePres_SetActorModes(0, 0);
    if (plan->target_count == 0) {
        BattlePresentation_WaitForAdvance();
    } else {
        for (i = 0; i < plan->target_count; i++) {
            Battle_ResolveTargetAction(plan, i);
            BattleEv_DispatchQueued();
        }
    }
    WaitFrames(1);
}
