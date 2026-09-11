#include "types.h"
#include "scene.h"
#include "abi/object/motion/act/set_mode_and_wait_animation.h"

s32 Motion_WaitForAnimationChange(s32);

void Motion_SetModeAndWaitAnimation(s32 arg0)
{
    Obj_Check();
    Motion_WaitForAnimationChange(arg0);
}
