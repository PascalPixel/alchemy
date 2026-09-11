#include "scene.h"
#include "abi/battle/event_runtime/schedule_phase.h"
#include "runtime_1e74.h"

s32 ScheduleCallbackAfterFrames(s32, s32);

s32 BattleEventRuntime_SchedulePhase(s32 parameter)
{
    struct Runtime1e74 *runtime;

    runtime = Runtime1e74_Get();
    runtime->value_7fc = 0;
    runtime->value_804 = 0;
    runtime->value_808 = parameter;
    runtime->phase = 2;
    runtime->flag_655 = 0;
    return ScheduleCallbackAfterFrames((s32)Battle_Run, 0xC80);
}
