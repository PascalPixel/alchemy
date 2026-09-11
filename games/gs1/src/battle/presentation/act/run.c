#include "types.h"
#include "scene.h"
#include "abi/battle/presentation/act/run.h"
#include "battle_msg.h"
#include "battle_escape.h"
#include "battle_presentation.h"
#include "battle_target.h"

struct BattlePresentationTransition {
    s32 battle_value;
    s32 timer;
    u8 reserved08[12];
    s32 flag;
};

extern struct BattlePresentationTransition *gIw;

u8 *Runtime_GetObject(s32);

void UiText_ShowMessageAndWait(s32);
void WaitFrames(s32);

s32 BattlePres_RunAction(s16 *action)
{
    struct BattlePresentationTransition *transition;
    s32 actor_id;
    s32 battle_mode;
    u8 *actor;

    actor_id = action[0];
    actor = Runtime_GetObject(actor_id);
    if (*(s16 *)(actor + 0x38) == 0)
        return -1;

    action[5] = BattleTarget_ReplaceDefeated((u8 *)action);
    transition = gIw;
    if (action[0] > 4)
        battle_mode = -0x2000;
    else
        battle_mode = 0x2000;
    transition->battle_value = battle_mode;
    transition->timer = 60;
    Battle_Run();

    switch (action[3]) {
    case 99:
        UiText_ShowMessageAndWait((s32)&Value_00000843);
        if (BattleEscape_PlayRun(action)!= 0)
            return 1;
        break;
    case 3:
        WaitFrames(45);
        Battle_Check(action);
        break;
    case 2:
        WaitFrames(45);
        Battle_Check2(action);
        break;
    case 0:
    default: {
        struct BattlePresentationTransition *tr = gIw;
        tr->flag = 0;
        Battle_Check2(action);
        tr->flag = 0;
        break;
    }
    case 1:
        Battle_Check3(action);
        break;
    }

    Battle_Run2();
    return 0;
}
