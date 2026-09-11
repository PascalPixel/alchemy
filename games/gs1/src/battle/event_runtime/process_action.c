#include "types.h"
#include "scene.h"
#include "abi/battle/event_runtime/process_action.h"
#include "global_cells.h"

extern u8 gCell;

s32 BattleEventRuntime_ProcessAction(s32 object_id, s32 action_id)
{
    s32 result;
    u8 *runtime;
    u8 *global_table;

    Battle_Check(object_id);
    global_table = &gCell;
    result = Battle_Apply(*(void **)(global_table + 500), 0);
    if (result == 0) {
        Battle_Apply2(object_id, action_id);
        runtime = *(u8 **)ADDR_03001EBC;
        *(u16 *)(runtime + 472) += 1;
    } else {
        runtime = *(u8 **)ADDR_03001EBC;
        *(u16 *)(runtime + 472) += 1;
        Battle_Apply2(object_id, action_id);
    }
    return result;
}

void BattleEventRuntime_NoOpCallback(void)
{
}
