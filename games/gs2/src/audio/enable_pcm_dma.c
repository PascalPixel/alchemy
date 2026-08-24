#include "audio_engine_symbols.h"
#include "types.h"

struct PcmDmaState {
    u32 ident;
    volatile u8 dma_counter;
};

void AudioEngine_EnablePcmDma(void)
{
    struct PcmDmaState *audio =
        *(struct PcmDmaState * volatile *)0x03007ff0;
    u32 ident = audio->ident;

    if (ident != 0x68736d53) {
        volatile u16 *dma_control = (volatile u16 *)0x040000c6;

        *dma_control = 0xb600;
        dma_control += 6;
        *dma_control = 0xb600;
        audio->dma_counter = 0;
        audio->ident = ident - 10;
    }
}
