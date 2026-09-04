#include "types.h"

#define ObjectMotion_SetModeAndWaitAnimation Func_08092548

s32 Func_080924d4();
s32 ObjectMotion_WaitForAnimationChange(s32);

void ObjectMotion_SetModeAndWaitAnimation(s32 arg0) {
    Func_080924d4();
    ObjectMotion_WaitForAnimationChange(arg0);
}
