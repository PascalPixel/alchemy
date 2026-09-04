#include "event_runtime.h"

#define BattleEffect_FinishAction Func_08091750

extern struct EventValueWork Data_02000240;

void ScheduleCallback(u32);
void Func_080772f0(void);
void BattleRuntime_UpdateModeFromShoulderButtons(void);
void Func_0809335c(s32 value, s32 enabled);

void BattleEffect_FinishAction(void)
{
    ScheduleCallback((u32)BattleRuntime_UpdateModeFromShoulderButtons);
    Func_0809335c(Data_02000240.value, 1);
    Func_080772f0();
}
