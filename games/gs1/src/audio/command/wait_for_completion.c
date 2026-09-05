#include "types.h"

#define AudioCommand_WaitForCompletion Func_080f95a0

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
