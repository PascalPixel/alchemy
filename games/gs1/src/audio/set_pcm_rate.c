#include "audio_engine_symbols.h"
#include "types.h"

struct PcmRateState {
    u8 padding00[8];
    u8 rate;
    u8 padding09[2];
    u8 dma_period;
    u8 padding0c[4];
    u32 pcm_samples_per_vblank;
    s32 frequency;
    s32 half_period;
};

extern u16 Data_080fb914[];

s32 FixedPoint_Ratio(s32 numerator, s32 denominator);
void AudioEngine_EnablePcmDma(void);

void AudioEngine_SetPcmRate(u32 mode_bits)
{
    struct PcmRateState *audio =
        *(struct PcmRateState **)0x03007ff0;
    u16 pcm_samples_per_vblank;
    s32 frequency;
    s32 zero;
    volatile u16 *timer;

    mode_bits = (mode_bits & 0x000f0000) >> 16;
    zero = 0;
    audio->rate = mode_bits;
    pcm_samples_per_vblank = Data_080fb914[mode_bits - 1];
    audio->pcm_samples_per_vblank = pcm_samples_per_vblank;
    audio->dma_period = FixedPoint_Ratio(0x630, pcm_samples_per_vblank);
    frequency = FixedPoint_Ratio(
        0x91d1b * pcm_samples_per_vblank + 0x1388,
        0x2710);
    audio->frequency = frequency;
    audio->half_period = (FixedPoint_Ratio(0x01000000, frequency) + 1) >> 1;
    *(volatile u16 *)0x04000102 = zero;
    timer = (volatile u16 *)0x04000100;
    *timer = -FixedPoint_Ratio(0x44940, pcm_samples_per_vblank);
    AudioEngine_EnablePcmDma();
    while (*(volatile u8 *)0x04000006 == 0x9f) {
    }
    while (*(volatile u8 *)0x04000006 != 0x9f) {
    }
    *(volatile u16 *)0x04000102 = 0x80;
}
