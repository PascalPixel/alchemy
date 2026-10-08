#include "audio_engine.h"
#include "system.h"

void AudioCommand_WaitForCompletion(void)
{
    s32 wait_count = 0;

    do {
        if (gMusicRestoreDelay == 0)
            break;
        WaitFrames(1);
        wait_count++;
    } while (wait_count <= 299);
}
