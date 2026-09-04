#include "types.h"

#define SceneState_SetValues8_3_4 Func_02000cc0

/* Open the three-by-four presentation block used by this resource. */
extern void Func_02002a16(s32, s32, s32);
void SceneState_SetValues8_3_4(void)
{
    Func_02002a16(8, 3, 4);
}
