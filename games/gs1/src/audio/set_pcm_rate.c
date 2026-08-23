#include "audio_engine_symbols.h"
#include "types.h"

struct PcmRateState {
    u8 padding00[8];
    u8 rate;
    u8 padding09[2];
    u8 dma_period;
    u8 padding0c[4];
    u32 samples_per_vblank;
    s32 frequency;
    s32 half_period;
};

extern u16 Data_080fb914[];

s32 Func_080022ec(s32 numerator, s32 denominator);
void AudioEngine_EnablePcmDma(void);

void AudioEngine_SetPcmRate(u32 mode_bits)
{
    struct PcmRateState *audio =
        *(struct PcmRateState **)0x03007ff0;
    u16 samples_per_vblank;
    s32 frequency;
    s32 zero;
    volatile u16 *timer;

    mode_bits = (mode_bits & 0x000f0000) >> 16;
    zero = 0;
    audio->rate = mode_bits;
    samples_per_vblank = Data_080fb914[mode_bits - 1];
    audio->samples_per_vblank = samples_per_vblank;
    audio->dma_period = Func_080022ec(0x630, samples_per_vblank);
    frequency = Func_080022ec(
        0x91d1b * samples_per_vblank + 0x1388,
        0x2710);
    audio->frequency = frequency;
    audio->half_period = (Func_080022ec(0x01000000, frequency) + 1) >> 1;
    *(volatile u16 *)0x04000102 = zero;
    timer = (volatile u16 *)0x04000100;
    *timer = -Func_080022ec(0x44940, samples_per_vblank);
    AudioEngine_EnablePcmDma();
    while (*(volatile u8 *)0x04000006 == 0x9f) {
    }
    while (*(volatile u8 *)0x04000006 != 0x9f) {
    }
    *(volatile u16 *)0x04000102 = 0x80;
}
