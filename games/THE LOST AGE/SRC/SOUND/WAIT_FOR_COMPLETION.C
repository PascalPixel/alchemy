#include "TYPES.H"

extern u8 gMusicRestoreDelay[];

s32 WaitFrames(s32);

/* Waits up to 300 frames for the pending music restore to finish. */
void AudioCommand_WaitForCompletion(void)
{
    s32 wait_count = 0;

    do {
        if (*(u8 *)gMusicRestoreDelay == 0)
            break;
        WaitFrames(1);
        wait_count++;
    } while (wait_count <= 299);
}
