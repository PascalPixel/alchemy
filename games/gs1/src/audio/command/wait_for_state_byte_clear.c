#include "types.h"
#include "scene.h"

s32 WaitFrames(s32);

void AudioCommand_WaitForStateByteClear(void)
{
    while (Audio_Check() != 0) {
        WaitFrames(1);
    }
}
