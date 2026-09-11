#include "types.h"
#include "scene.h"
#include "abi/audio/command/wait_for_completion.h"

s32 WaitFrames(s32);

void AudioCommand_WaitForCompletion(void)
{
    s32 wait_count = 0;

    do {
        if (*(u8 *)0x02003000 == 0)
            break;
        WaitFrames(1);
        wait_count++;
    } while (wait_count <= 299);
}
