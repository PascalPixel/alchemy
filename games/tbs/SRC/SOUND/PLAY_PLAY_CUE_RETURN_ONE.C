#include "TYPES.H"
#include "SCENE.H"

s32 Audio_PlayCue();

s32 Audio_PlayCueReturnOne(void)
{
    Audio_PlayCue();
    return 1;
}
