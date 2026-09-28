#include "TYPES.H"
#include "SCENE.H"
void BattlePres_SetActorModes(u16 *, s32);
void BattlePresentation_WaitForAdvance(void);
s32 Battle_ResolveTargetAction(void *, s32);
s32 BattleEv_DispatchQueued();

s32 WaitFrames(s32);

s32 BattlePres_RunActorEntries(void *tbl)
{
    s32 i;
    s8 n;

    BattlePres_SetActorModes(0, 0);
    n = FIELD_AT_OFFSET(tbl, s8 *, 1);
    if (n == 0) {
        BattlePresentation_WaitForAdvance();
    } else {
        i = 0;
        if (i < (s32)n) {
            do {
                Battle_ResolveTargetAction(tbl, i);
                BattleEv_DispatchQueued();
                i += 1;
            } while (i < (s32)FIELD_AT_OFFSET(tbl, s8 *, 1));
        }
    }
    return WaitFrames(1);
}
