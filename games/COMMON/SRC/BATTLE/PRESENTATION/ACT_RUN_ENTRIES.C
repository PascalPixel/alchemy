#include "TYPES.H"
#include "BATTLE_EVENT.H"
#include "BATTLE_PRESENTATION.H"
#include "BATTLE_TYPES.H"
#include "SYSTEM.H"
void BattlePres_SetActorModes(u16 *, s32);
void BattlePresentation_WaitForAdvance(void);
s32 Battle_ResolveTargetAction(struct BattlePlan *plan, s32 slot);

s32 BattlePres_RunActorEntries(struct BattlePlan *plan, s32 unused)
{
    /* FAKEMATCH: a void owner makes the interworking epilogue pop into r0;
       this result type keeps the native pop into r1. Callers discard it. */
    s32 i;
    s8 n;

    BattlePres_SetActorModes(0, 0);
    n = plan->target_count;
    if (n == 0) {
        BattlePresentation_WaitForAdvance();
    } else {
        i = 0;
        if (i < (s32)n) {
            do {
                Battle_ResolveTargetAction(plan, i);
                BattleEv_DispatchQueued();
                i += 1;
            } while (i < (s32)plan->target_count);
        }
    }
    WaitFrames(1);
}
