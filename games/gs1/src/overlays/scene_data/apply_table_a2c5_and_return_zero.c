#include "types.h"

#define SceneData_ApplyTableA2c5AndReturnZero Func_02002304

extern void Func_02007c96(s32);
extern u8 Data_0200a2c5[];

s32 SceneData_ApplyTableA2c5AndReturnZero(void)
{
    Func_02007c96((s32)Data_0200a2c5);
    return 0;
}
