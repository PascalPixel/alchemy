#include "types.h"
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

extern struct BattlePresentationTransition *Data_03001f00;

u8 *Runtime_GetObject(s32);
void Func_08015118(void);
void UiText_ShowMessageAndWait(s32);
void WaitFrames(s32);
s32 Func_080b8888(s16 *);
s32 Func_080b8c1c(s16 *);
s32 Func_080b88d0(s16 *);
void Func_08015220(void);

s32 BattlePresentation_RunAction(s16 *action)
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
    transition = Data_03001f00;
    if (action[0] > 4)
        battle_mode = -0x2000;
    else
        battle_mode = 0x2000;
    transition->battle_value = battle_mode;
    transition->timer = 60;
    Func_08015118();

    switch (action[3]) {
    case 99:
        UiText_ShowMessageAndWait((s32)&Value_00000843);
        if (BattleEscape_PlayRun(action) != 0)
            return 1;
        break;
    case 3:
        WaitFrames(45);
        Func_080b8888(action);
        break;
    case 2:
        WaitFrames(45);
        Func_080b8c1c(action);
        break;
    case 0:
    default: {
        struct BattlePresentationTransition *tr = Data_03001f00;
        tr->flag = 0;
        Func_080b8c1c(action);
        tr->flag = 0;
        break;
    }
    case 1:
        Func_080b88d0(action);
        break;
    }

    Func_08015220();
    return 0;
}
