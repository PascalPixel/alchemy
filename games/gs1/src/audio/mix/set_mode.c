#include "audio_engine_symbols.h"
#include "types.h"

void AudioEngine_SetPcmRate(u32);
void AudioEngine_DisablePcmDma(void);

void AudioEngine_SetMode(u32 mode)
{
    u8 *audio = *(u8 **)0x03007FF0;
    u32 ident = *(u32 *)audio;
    u32 mode_bits;

    if (ident != 0x68736D53) {
        return;
    }

    *(u32 *)audio = ident + 1;

    mode_bits = mode & 0xFF;
    if (mode_bits != 0) {
        audio[5] = mode_bits & 0x7F;
    }

    mode_bits = mode & 0xF00;
    if (mode_bits != 0) {
        u8 *channel;

        audio[6] = mode_bits >> 8;
        mode_bits = 12;
        channel = audio + 0x50;
        do {
            *channel = 0;
            mode_bits--;
            channel += 0x40;
        } while (mode_bits != 0);
    }

    mode_bits = mode & 0xF000;
    if (mode_bits != 0) {
        audio[7] = mode_bits >> 12;
    }

    mode_bits = mode & 0xB00000;
    if (mode_bits != 0) {
        mode_bits = (mode_bits & 0x300000) >> 14;
        *(volatile u8 *)0x04000089 =
            (*(volatile u8 *)0x04000089 & 0x3F) | mode_bits;
    }

    mode_bits = mode & 0xF0000;
    if (mode_bits != 0) {
        AudioEngine_DisablePcmDma();
        AudioEngine_SetPcmRate(mode_bits);
    }

    *(u32 *)audio = 0x68736D53;
}
