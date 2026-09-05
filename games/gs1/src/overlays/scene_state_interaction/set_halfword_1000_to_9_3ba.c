#include "types.h"

#define SceneState_SetHalfword1000To9 Func_02002f94

extern u16 Data_02001000;

void SceneState_SetHalfword1000To9(void)
{
    u16 *p = &Data_02001000;
    s32 v = 9;

    *p = v;
}
