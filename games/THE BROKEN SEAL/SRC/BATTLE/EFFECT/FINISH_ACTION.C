#include "EVENT_RUNTIME.H"

extern struct EventValueWork Data_02000240;

void ScheduleCallback(u32);
void Func_080772f0(void);
void Func_080915ec(void);
void Func_0809335c(s32 value, s32 enabled);
#define Object_AttachWorkTargetToObject Func_0809335c

void BattleFx_FinishAction(void)
{
    ScheduleCallback((u32)Func_080915ec);
    Object_AttachWorkTargetToObject(Data_02000240.value, 1);
    Func_080772f0();
}
