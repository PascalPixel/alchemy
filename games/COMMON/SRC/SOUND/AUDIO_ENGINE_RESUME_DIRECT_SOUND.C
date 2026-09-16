#include "TYPES.H"

#define SOUND_LOCK 0x68736D53
#define SOUND_WORK (*(struct SoundWork **)0x03007FF0)

struct SoundWork {
    u32 lock;
    volatile u8 transfer_countdown;
};

void AudioEngine_ResumeDirectSound(void)
{
    struct SoundWork *work = SOUND_WORK;
    u32 lock = work->lock;

    if (lock == SOUND_LOCK)
        return;
    *(u16 *)0x040000C6 = 0xB600;
    *(u16 *)0x040000D2 = 0xB600;
    work->transfer_countdown = 0;
    work->lock = lock - 10;
}
