#include "audio_engine_symbols.h"
#include "types.h"

typedef void (*DisableFunction)(u8);

extern u8 *Data_03007ff0;

void AudioEngine_StopAllChannels(void)
{
    u8 *audio = Data_03007ff0;
    u32 ident = *(u32 *)audio;
    s32 remaining;
    u8 *channel;

    if (ident != 0x68736d53)
        return;

    *(u32 *)audio = ident + 1;

    remaining = 12;
    channel = audio + 80;
    do {
        *channel = 0;
        remaining--;
        channel += 64;
    } while (remaining > 0);

    channel = *(u8 **)(audio + 28);
    if (channel != 0) {
        remaining = 1;
        do {
            u8 channel_id = (u8)remaining;
            DisableFunction disable = *(DisableFunction *)(audio + 44);

            disable(channel_id);
            *channel = 0;
            remaining++;
            channel += 64;
        } while (remaining <= 4);
    }

    *(u32 *)audio = 0x68736d53;
}
