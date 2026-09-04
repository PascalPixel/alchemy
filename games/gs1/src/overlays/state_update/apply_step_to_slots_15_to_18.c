#include "types.h"

#define SceneState_ApplyStepToSlots15To18 Func_02003050

extern void Func_02007f06(s32);

void SceneState_ApplyStepToSlots15To18(void)
{
    u32 i;

    i = 15;
    do {
        Func_02007f06(i);
        i++;
    } while (i <= 18);
}
