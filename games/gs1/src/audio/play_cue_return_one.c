#include "types.h"

#define Audio_PlayCueReturnOne Func_080a2438

s32 Audio_PlayCue();

s32 Audio_PlayCueReturnOne(void) {
    Audio_PlayCue();
    return 1;
}
