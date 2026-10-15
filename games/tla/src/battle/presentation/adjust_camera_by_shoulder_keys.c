#include "types.h"
#include "scene.h"
#include "battle_message.h"
#include "battle_escape.h"
#include "battle_display.h"
#include "battle_target.h"
#include "fixed_math.h"
#include "battle_party.h"
#include "system.h"
#include "ram_buffer.h"

void *UnitGet(s32 owner);
void UiWork_ClearValueNameTablesFar(void);
extern u8 Data_03001ae8[];
s32 BattlePres_ShowDefendMessage(s16 *);
s32 Func_0811ca54(s16 *);
s32 Func_0811c710(struct BattleActionRecord *);
void BattleMotion_SetupEscapeObject(s32);

s32 BattlePres_RunAction(s16 *action)
{
    struct BattlePresentationTransition *transition;
    s32 actor_id;
    s32 battle_mode;
    u8 *actor;

    actor_id = action[0];
    actor = UnitGet(actor_id);
    if (*(s16 *)(actor + 0x38) == 0)
        return -1;

    action[5] = BattleTarget_ReplaceDefeated((const struct BattleActionRecord *)action);
    transition = Ram_HeapSlots->transition_work;
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
        BattlePres_ShowDefendMessage(action);
        break;
    case 2:
        WaitFrames(45);
        Func_0811ca54(action);
        break;
    case 0:
    default: {
        struct BattlePresentationTransition *tr = Ram_HeapSlots->transition_work;
        tr->flag = 0;
        Func_0811ca54(action);
        tr->flag = 0;
        break;
    }
    case 1:
        Func_0811c710((struct BattleActionRecord *)action);
        break;
    }

    UiWork_ResetFreeChannelFar();
    return 0;
}

s32 BattleObject_IsValidId(u32 object_id)
{
    if (object_id <= 7) {
        return 0;
    }
    if (object_id - 128 <= 5) {
        return 0;
    }
    return -1;
}
