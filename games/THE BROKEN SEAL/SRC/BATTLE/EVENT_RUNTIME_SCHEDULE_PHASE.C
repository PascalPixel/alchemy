#include "RUNTIME_1E74.H"

s32 ScheduleCallbackAfterFrames(s32, s32);
void Func_080bd898(void);
#define BattleEvent_Playback Func_080bd898

s32 BattleEventRuntime_SchedulePhase(s32 parameter)
{
    struct Runtime1e74 *runtime;

    runtime = Runtime1e74_Get();
    runtime->value_7fc = 0;
    runtime->value_804 = 0;
    runtime->value_808 = parameter;
    runtime->phase = 2;
    runtime->flag_655 = 0;
    return ScheduleCallbackAfterFrames((s32)Func_080bd898, 0xC80);
}
