#include "TYPES.H"
#include "GLOBAL_CELLS.H"

void Battle_WaitMode0(s32 arg0);

/* 入力r0/r1をそのまま渡すため、引数型は意図的に省略する。 */
s32 BattleEv_RunWait();

extern u8 Data_03001ebc[];
extern u8 gGameState;
s32 UiText_OpenMessageAtObject(s32);
s32 Inventory_PromptAndSetObjectMode(void *, s32);

void BattlePres_RunActionThenWaitIfModeZero(s32 first, s32 second, s32 value)
{
    BattleEv_RunWait();
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
