#include "TYPES.H"
#include "GLOBAL_CELLS.H"
extern u8 Data_03001ebc[];

extern u8 gGameState;

s32 UiText_OpenMessageAtObject(s32);
s32 Inventory_PromptAndSetObjectMode(void *, s32);
void BattleEv_RunWait(s32, s32);

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
