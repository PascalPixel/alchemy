#include "audio_engine.h"
#include "io_reg.h"
#include "iwram_call.h"

extern u8 Data_02003a90[];

s32 __divsi3(s32 numerator, s32 denominator);
void AudioEngine_ResumeDirectSound(void);

static __inline__ s32 Sound_ClearWords(void *destination, s32 size)
{
    return Iwram_ClearWords(destination, size);
}

// Sets direct sound to a fixed 31536 Hz output with 528 samples a frame,
// keeping mode's rate nibble, clears the two PCM buffers and points DMA 1
// and 2 at them, then restarts timer 0 at line 159.
void AudioEngine_SetFixedPcmRate(u32 mode)
{
    struct SoundWork *work = SOUND_WORK;
    u32 frame;
    u8 *buffer;
    u32 control;
    volatile u8 *vcount;

    mode = (mode >> 16) & 15;
    work->transferPeriod = 3;
    work->outputRate = 31536;
    work->rate = mode;
    frame = 528;
    work->frameSamples = frame;
    work->rateStep = 266;
    /* FAKEMATCH: control holds both timer 0 control values, so 0 is built after the last work store into r2 and 0x80 by movs rather than as halfword pool constants (written directly, differ 22 with 0x80 and 2 with 0). */
    control = 0;
    /* FAKEMATCH: one-pass blocks around the timer stores and the second DMA source keep each store where the native has it among its neighbours' loads and constants (differ 8, 6 and 6 without each). */
    do
    {
        REG_TM0CNT_H = control;
    } while (0);
    do
    {
        REG_TM0CNT_L = __divsi3(-140448, frame);
    } while (0);
    buffer = Data_02003a90;
    /* FAKEMATCH: the clear goes through the inline wrapper Sound_ClearWords, as ☀️'s IWRAM call layer, so the entry's pool load comes after the size's shift (differ 2 called directly). */
    Sound_ClearWords(buffer, 0x18c0);
    AudioEngine_ResumeDirectSound();
    REG_DMA1SAD = (u32)buffer;
    buffer += 0xc60;
    do
    {
        REG_DMA2SAD = (u32)buffer;
    } while (0);
    vcount = (volatile u8 *)0x04000006;
    while (*vcount == 159)
        ;
    while (*vcount != 159)
        ;
    control = 0x80;
    REG_TM0CNT_H = control;
}
