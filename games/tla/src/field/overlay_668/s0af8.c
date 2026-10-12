#include "types.h"
#include "call.h"

void Scheduler_AddOrUpdateCallback();
void Func_02000a98(void);
void Func_02000a44(void);

void Func_02000af8(void)
{
    Value2((s32 (*)())Scheduler_AddOrUpdateCallback, (s32)Func_02000a98, 0x480);
    Value2((s32 (*)())Scheduler_AddOrUpdateCallback, (s32)Func_02000a44, 0xc80);
}
