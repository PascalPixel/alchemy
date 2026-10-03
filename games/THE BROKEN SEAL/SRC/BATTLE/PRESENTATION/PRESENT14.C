#include "TYPES.H"
#include "IO_REG.H"
#include "GLOBAL_CELLS.H"
#include "EVENT_RUNTIME.H"
#include "GAME_STATE.H"

extern u8 gEventWork[];
extern volatile u32 gKeysHeld;

struct UiWindow;
struct UiWindow *UiText_OpenMessageAtObject(s32 actor_and_flags);
s32 UiWork_IsIdleFar(struct UiWindow *window);

void Battle_WaitMode0(s32 mode);
void BattleEv_RunWait(s32 action, s32 flag);
extern u8 Data_03001ebc[];
s32 Inventory_PromptAndSetObjectMode(s32 actor, s32 force);

void BattleEv_RunWait(s32 action, s32 flag)
{
    struct EventRuntime *runtime = *(struct EventRuntime **)gEventWork;
    struct UiWindow *window = UiText_OpenMessageAtObject(action);
    s32 message_id;
    u32 frames = 0;

    WaitFrames(1);
    message_id = ObjectTable_ReadActiveValue(action);
    if (action <= 7) {
        s32 masked_action = action & 0x0fff;

        if (BattleAction_FindDescriptor(masked_action) == 0) {
            message_id = masked_action;
        }
    }
    UiWork_FinalizeEntityMatchingLocalizedIdFar(message_id);

    if (runtime->message_busy == 0) {
        while (UiWork_IsIdleFar(window) == 0) {
            WaitFrames(1);
            frames++;
            if (frames > 600 ||
                ((gKeysHeld & KEY_SELECT) && (gKeysHeld & KEY_R) &&
                 (gKeysHeld & KEY_L) && (gKeysHeld & KEY_A))) {
                UiWork_FinalizePendingCoreFar();
            }
        }
    }

    WaitFrames(1);
}

void BattlePres_RunActionThenWaitIfModeZero(s32 first, s32 second, s32 value)
{
    BattleEv_RunWait(first, second);
    Battle_WaitMode0(value);
}

s32 BattleEventRuntime_ProcessAction(s32 object_id, s32 action_id)
{
    s32 result;
    struct EventRuntime *runtime;
    struct GameState *save;

    UiText_OpenMessageAtObject(object_id);
    save = &gGameState;
    result = Inventory_PromptAndSetObjectMode(save->selected_actor, 0);
    if (result == 0) {
        BattleEv_RunWait(object_id, action_id);
        runtime = *(struct EventRuntime **)Data_03001ebc;
        *(u16 *)&runtime->message += 1;
    } else {
        runtime = *(struct EventRuntime **)Data_03001ebc;
        *(u16 *)&runtime->message += 1;
        BattleEv_RunWait(object_id, action_id);
    }
    return result;
}

void BattleEventRuntime_EmptyCallback(void)
{
}
