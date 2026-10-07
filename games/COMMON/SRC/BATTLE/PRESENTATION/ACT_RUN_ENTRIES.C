#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "BTLEV.H"
#include "BTLDISP.H"
#include "BTLTYPE.H"
#include "SYSTEM.H"
void BattlePresentation_WaitForAdvance(void);

s32 BattlePres_RunActorEntries(struct BtlPlan *plan, s32 unused)
{
    s32 i;

    BattlePres_SetActorModes(0, 0);
    if (plan->target_count == 0) {
        BattlePresentation_WaitForAdvance();
    } else {
        for (i = 0; i < plan->target_count; i++) {
            BtlTgtExec(plan, i);
            BattleEv_DispatchQueued();
        }
    }
    WaitFrames(1);
}
