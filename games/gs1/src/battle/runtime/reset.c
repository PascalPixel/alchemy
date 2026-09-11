#include "scene.h"
#include "abi/battle/runtime/reset.h"
#include "battle_effect_runtime.h"

void Battle_InitializeRenderObject(void);

void ScheduleCallbackAfterFrames(const void *, s32);
void Battle_UpdateModeFromShoulderButtons(void);
u32 GameFlag_Clear(s32);

void Battle_Reset(void)
{
    struct BattleRuntime *runtime = gWork;

    Battle_Run();
    Battle_InitializeRenderObject();
    if (runtime->unknown_cb6 != 0) {
        Battle_Run2();
    }
    {
        s32 zero = 0;
        runtime->unknown_cc2 = zero;
        runtime->unknown_cc4 = zero;
        runtime->unknown_1c8 = 0x10;
        runtime->mode_1cc = zero;
        runtime->unknown_1da = 0xFFFF;
        runtime->unknown_1dc = -1;
        runtime->unknown_1de = -1;
        ScheduleCallbackAfterFrames((const void *)Battle_UpdateModeFromShoulderButtons, 0xC80);
        GameFlag_Clear(0x132);
        runtime->object_id = gCell.object_id;
        runtime->unknown_1f8 = zero;
    }
}
