#include "types.h"
#include "scene.h"

s32 Audio_PlayCue();

s32 Audio_PlayCueReturnOne(void)
{
    Audio_PlayCue();
    return 1;
}
