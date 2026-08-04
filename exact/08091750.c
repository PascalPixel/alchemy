#include "event_runtime.h"

extern struct EventValueWork Data_02000240;

void Func_08004278(u32);
void Func_080772f0(void);
void Func_0809335c(s32 value, s32 enabled);

void Func_08091750(void)
{
    Func_08004278(0x080915ed);
    Func_0809335c(Data_02000240.value, 1);
    Func_080772f0();
}
