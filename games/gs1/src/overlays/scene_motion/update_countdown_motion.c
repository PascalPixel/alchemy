#include "resource_3af_motion.h"

#define SceneEffect_UpdateCountdownMotion Func_02000030
#define BuildMotionCountdown Func_020042e6
#define GetMotionCountdownInput Func_020042f8

s32 BuildMotionCountdown(s32, s16);
s32 GetMotionCountdownInput(void);

s32 SceneEffect_UpdateCountdownMotion(struct Resource3afMotion *motion) {
    switch (motion->countdown) {
    case 6:
        motion->component_a += (s32) 0xFFFFC000;
        motion->component_b += 0x2000;
        break;
    case 4:
        motion->component_a += 0x2000;
        /* The load at 0x0200006A owns the Thumb-like data word at 0x020000AC. */
        motion->component_b += (s32) 0xF856F001;
        break;
    case 2:
        motion->component_a += 0x1000;
        motion->component_b += (s32) 0xFFFFF800;
        break;
    case 0:
        motion->component_a = 0x10000;
        motion->component_b = 0x10000;
        motion->countdown =
            (s16) (BuildMotionCountdown(GetMotionCountdownInput(), 90) + 60);
        break;
    }
    motion->countdown--;
    return 1;
}
