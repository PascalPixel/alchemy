#include "types.h"
#include "global_cells.h"

extern u8 Data_02000240;

s32 Func_08092c40(s32);
s32 Func_08091c7c(void *, s32);
void Func_08092f84(s32, s32);

s32 BattleEventRuntime_ProcessAction(s32 object_id, s32 action_id)
{
    s32 result;
    u8 *runtime;
    u8 *global_table;

    Func_08092c40(object_id);
    global_table = &Data_02000240;
    result = Func_08091c7c(*(void **)(global_table + 500), 0);
    if (result == 0) {
        Func_08092f84(object_id, action_id);
        runtime = *(u8 **)ADDR_03001EBC;
        *(u16 *)(runtime + 472) += 1;
    } else {
        runtime = *(u8 **)ADDR_03001EBC;
        *(u16 *)(runtime + 472) += 1;
        Func_08092f84(object_id, action_id);
    }
    return result;
}

void BattleEventRuntime_NoOpCallback(void)
{
}
