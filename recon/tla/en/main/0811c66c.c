#include "TYPES.H"
#include "SCENE.H"
#include "BATTLE_MSG.H"
#include "BATTLE_ESCAPE.H"
#include "BATTLE_PRESENTATION.H"
#include "BATTLE_TARGET.H"
#include "FIXED_MATH.H"
#include "BATTLE_PARTY.H"
#include "SYSTEM.H"
void UiWork_ClearValueNameTablesFar(void);
extern u8 Data_03001ae8[];
s32 BattlePres_ShowMessageWhenField38Positive(s16 *);
s32 BattlePres_RunUnitAction(s16 *);
s32 BattlePresentation_RunPairedUnitTransition(s16 *);
void BattleMotion_SetupEscapeObject(s32);

s32 BattleEscape_PlayRun(s16 *action)
{
    s16 party_members[14];
    s32 party_size;
    s32 animated;
    s32 member_slot;

    (void)action;
    if (((u32)(Random16() << 4) >> 16) != 0) {
        party_size = BattleParty_ListLivingUnits(
            BATTLE_SIDE_PARTY,
            party_members);
        animated = 0;
        if (party_size != 0) {
            member_slot = 0;
            do {
                BattleMotion_SetupEscapeObject(party_members[member_slot]);
                animated++;
                WaitFrames(8);
                member_slot++;
            } while (animated != party_size);
        }
        WaitFrames(22);
        return 1;
    }
    UiText_ShowMessageAndWaitCoreFar((s32)&MsgCannotEscape);
    return 0;
}
