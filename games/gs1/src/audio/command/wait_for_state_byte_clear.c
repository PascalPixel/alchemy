#include "types.h"
#include "scene.h"
#include "abi/audio/command/wait_for_state_byte_clear.h"

s32 WaitFrames(s32);

void AudioCommand_WaitForStateByteClear(void)
{
    while (Audio_Check() != 0) {
        WaitFrames(1);
    }
}
