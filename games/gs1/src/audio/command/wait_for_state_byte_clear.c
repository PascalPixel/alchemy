#include "types.h"
#include "scene.h"

void WaitFrames(s32);

void AudioCommand_WaitForStateByteClear(void)
{
    while (Audio_Check() != 0) {
        WaitFrames(1);
    }
}
