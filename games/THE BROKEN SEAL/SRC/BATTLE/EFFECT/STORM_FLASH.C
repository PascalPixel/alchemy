#include "TYPES.H"
#include "DMA.H"
#include "SYSTEM.H"

s32 GameFlag_TestFar(s32);
void Audio_PlayCue(s32);
void BattleFx_ApplyColorToTargetBuffer(void *buffer, s32 mode);

struct FlashTarget {
    u8 unknown_0000[0x1880];
    u8 palette[0x1181];
    u8 step;
    u8 phase;
};

struct FlashWork {
    u8 buffers[0x1f80];
    s16 timer;
    s16 enabled;
    s16 loud;
};

extern u8 *Data_03001ec8[];

/* Counts the storm timer down: at zero it re-arms at a random interval and
   plays the thunder cue, then flashes the target buffer for two steps. */
void BattleFx_UpdateStormFlash(void)
{
    struct FlashWork *work = (struct FlashWork *)Data_03001ec8[0];
    struct FlashTarget *target = (struct FlashTarget *)Data_03001ec8[2];
    s16 *timer = &work->timer;

    if (*timer < 0)
        return;
    if (GameFlag_TestFar(358))
        work->timer = 128;
    switch ((*timer)--) {
    case 0:
        if (work->enabled != 0) {
            u32 a = Random16();
            u32 b = Random16();
            work->timer = ((a * 400) >> 16) - ((b * 100) >> 16) + 150;
            if (work->loud != 0)
                Audio_PlayCue(172);
            else
                Audio_PlayCue(171);
        }
    case 5:
    case 10:
        BattleFx_ApplyColorToTargetBuffer(work, 1);
        Dma_Set(work->buffers + 0x1500, target->palette, 0x840002a0, (volatile u32 *)0x040000d4);
        target->step = 12;
        target->phase = 0;
        break;
    case 1:
    case 6:
    case 11:
        BattleFx_ApplyColorToTargetBuffer(work->buffers + 0xa80, 1);
        target->step = 1;
        target->phase = 0;
        break;
    }
}


