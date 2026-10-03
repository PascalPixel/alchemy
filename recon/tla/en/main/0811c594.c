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
s32 BattlePresentation_RunPairedUnitTransition(struct BattleActionRecord *);
void BattleMotion_SetupEscapeObject(s32);

s32 BattlePres_RunAction(s16 *action)
{
    struct BattlePresentationTransition *transition;
    s32 actor_id;
    s32 battle_mode;
    u8 *actor;

    actor_id = action[0];
    actor = Owner_GetStateFar(actor_id);
    if (*(s16 *)(actor + 0x38) == 0)
        return -1;

    action[5] = BattleTarget_ReplaceDefeated((const struct BattleActionRecord *)action);
    transition = gTransitionWork;
    if (action[0] > 4)
        battle_mode = -0x2000;
    else
        battle_mode = 0x2000;
    transition->target_yaw = battle_mode;
    transition->frames = 60;
    UiWork_ClearValueNameTablesFar();

    switch (action[3]) {
    case 99:
        UiText_ShowMessageAndWaitCoreFar((s32)&MsgPartyFlees);
        if (BattleEscape_PlayRun((struct BattleActionRecord *)action)!= 0)
            return 1;
        break;
    case 3:
        WaitFrames(45);
        BattlePres_ShowMessageWhenField38Positive(action);
        break;
    case 2:
        WaitFrames(45);
        BattlePres_RunUnitAction(action);
        break;
    case 0:
    default: {
        struct BattlePresentationTransition *tr = gTransitionWork;
        tr->flag = 0;
        BattlePres_RunUnitAction(action);
        tr->flag = 0;
        break;
    }
    case 1:
        BattlePresentation_RunPairedUnitTransition((struct BattleActionRecord *)action);
        break;
    }

    UiWork_ResetFreeChannelFar();
    return 0;
}
