#include "types.h"
#include "scene.h"
#include "global_cells.h"

/* battle/presentation/act/run_wait.c */
extern volatile u32 gIw;

void BattleEv_RunWait(s32 action)
{
    u8 *runtime = *(u8 **)0x03001ebc;
    s32 wait_token = Battle_Run();
    s32 resolved_action;
    u32 frames = 0;

    Battle_unk2_4(1);
    resolved_action = Battle_unk3_4(action);
    if (action <= 7) {
        s32 masked_action = action & 0x0fff;

        if (Battle_unk4_4(masked_action) == 0) {
            resolved_action = masked_action;
        }
    }
    Battle_unk5_2(resolved_action);

    if (*(s32 *)(runtime + 0x1cc) == 0) {
        while (Battle_unk6_2(wait_token) == 0) {
            Battle_unk2_4(1);
            frames++;
            if (frames > 600 ||
                ((gIw & 4) && (gIw & 0x100) &&
                 (gIw & 0x200) && (gIw & 1))) {
                Battle_unk7_2();
            }
        }
    }

    Battle_unk2_4(1);
}

/* battle/presentation/act/run_wait_mode0.c */
void Battle_WaitMode0(s32 arg0);
/* 入力r0/r1をそのまま渡すため、引数型は意図的に省略する。 */

void BattlePres_RunActionThenWaitIfModeZero(s32 first, s32 second, s32 value)
{
    Battle_Check();
    Battle_WaitMode0(value);
}

/* battle/event_runtime/process_action.c */
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
