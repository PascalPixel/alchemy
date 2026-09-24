#include "TYPES.H"
#include "GLOBAL_CELLS.H"

extern u8 gGameState;
#define PARTY_STATE gGameState

s32 Func_08092c40(s32);
s32 Inventory_PromptAndSetObjectMode(void *, s32);
void BattleEv_RunWait(s32, s32);

s32 BattleEventRuntime_ProcessAction(s32 object_id, s32 action_id)
{
    s32 result;
    u8 *runtime;
    u8 *global_table;

    Func_08092c40(object_id);
    global_table = &PARTY_STATE;
    result = Inventory_PromptAndSetObjectMode(*(void **)(global_table + 500), 0);
    if (result == 0) {
        BattleEv_RunWait(object_id, action_id);
        runtime = *(u8 **)ADDR_03001EBC;
        *(u16 *)(runtime + 472) += 1;
    } else {
        runtime = *(u8 **)ADDR_03001EBC;
        *(u16 *)(runtime + 472) += 1;
        BattleEv_RunWait(object_id, action_id);
    }
    return result;
}

void Func_080930b8(void)
{
}
