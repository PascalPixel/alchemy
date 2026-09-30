#include "TYPES.H"
#include "GLOBAL_CELLS.H"

extern u8 gEventWork[];
extern volatile u32 gKeysHeld;

/* The wait calls it with the action still in r0, as the ROM does, so it is
   declared without a prototype. */
s32 UiText_OpenMessageAtObject();

void Battle_WaitMode0(s32 arg0);
void BattleEv_RunWait(s32 action, s32 flag);
extern u8 Data_03001ebc[];
extern u8 gGameState;
s32 Inventory_PromptAndSetObjectMode(void *, s32);

void BattleEv_RunWait(s32 action, s32 flag)
{
    u8 *runtime = *(u8 **)gEventWork;
    s32 wait_token = UiText_OpenMessageAtObject();
    s32 resolved_action;
    u32 frames = 0;

    WaitFrames(1);
    resolved_action = ObjectTable_ReadActiveValue(action);
    if (action <= 7) {
        s32 masked_action = action & 0x0fff;

        if (BattleAction_FindDescriptor(masked_action) == 0) {
            resolved_action = masked_action;
        }
    }
    UiWork_FinalizeEntityMatchingLocalizedIdFar(resolved_action);

    if (*(s32 *)(runtime + 0x1cc) == 0) {
        while (UiWork_IsIdleFar(wait_token) == 0) {
            WaitFrames(1);
            frames++;
            if (frames > 600 ||
                ((gKeysHeld & 4) && (gKeysHeld & 0x100) &&
                 (gKeysHeld & 0x200) && (gKeysHeld & 1))) {
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
    u8 *runtime;
    u8 *global_table;

    UiText_OpenMessageAtObject(object_id);
    global_table = &gGameState;
    result = Inventory_PromptAndSetObjectMode(*(void **)(global_table + 500), 0);
    if (result == 0) {
        BattleEv_RunWait(object_id, action_id);
        runtime = *(u8 **)((u32)&Data_03001ebc);
        *(u16 *)(runtime + 472) += 1;
    } else {
        runtime = *(u8 **)((u32)&Data_03001ebc);
        *(u16 *)(runtime + 472) += 1;
        BattleEv_RunWait(object_id, action_id);
    }
    return result;
}

void BattleEventRuntime_EmptyCallback(void)
{
}
