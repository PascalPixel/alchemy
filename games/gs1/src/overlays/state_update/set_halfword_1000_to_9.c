#include "types.h"

#define SceneState_SetHalfword1000To9 Func_0200322c

extern u16 Data_02001000;

/* Complete eight-byte state setter plus its sole four-byte pool word. */
void SceneState_SetHalfword1000To9(void)
{
    u16 *p = &Data_02001000;
    u16 v = 9;
    *p = v;
}
