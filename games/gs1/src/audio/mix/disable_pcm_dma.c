#include "audio_engine_symbols.h"
#include "types.h"

void Func_08006864(s32 *, u8 *, void *);

void AudioEngine_DisablePcmDma(void)
{
    s32 *audio;
    s32 ident;
    s32 zero;

    ident = *(audio = *(s32 **)0x03007ff0);
    if ((u32)(ident + 0x978c92ad) <= 1) {
        *audio = ident + 10;
        if (*(u32 *)0x040000c4 & 0x02000000)
            *(u32 *)0x040000c4 = 0x84400004;
        if (*(u32 *)0x040000d0 & 0x02000000)
            *(u32 *)0x040000d0 = 0x84400004;
        {
            volatile u16 *dma_control = (volatile u16 *)0x040000c6;
            *dma_control = 0x400;
            dma_control += 6;
            *dma_control = 0x400;
        }
        zero = 0;
        Func_08006864(&zero, (u8 *)audio + 848, (void *)0x05000318);
    }
}
