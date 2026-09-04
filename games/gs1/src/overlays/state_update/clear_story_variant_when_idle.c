#include "types.h"

#define SceneState_ClearStoryVariantWhenIdle Func_02000674

extern s16 *Data_0200add0;
extern s32 Func_02002238(void);

/* Clear the active story variant once the scene controller is idle. */
void SceneState_ClearStoryVariantWhenIdle(void)
{
    if (Func_02002238() == 0)
        *Data_0200add0 = -1;
}
