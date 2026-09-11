#include "audio_engine_symbols.h"
#include "types.h"
#include "scene.h"
#include "abi/audio/misc/cgb_key_to_frequency.h"

extern u8 gRom[];
extern s16 gRom2[];
extern u8 gRom3[];

s32 Cgb_KeyToFrequency(
    s32 channel_type_arg,
    s32 key_arg,
    s32 pitch_arg)
{
    u8 channel_type = channel_type_arg;
    u8 key = key_arg;
    u8 pitch = pitch_arg;

    if (channel_type == 4) {
        if (key <= 20) {
            key = 0;
        } else {
            key -= 21;
            if (key > 59)
                key = 59;
        }
        return gRom3[key];
    }

    if (key <= 35) {
        pitch = 0;
        key = 0;
    } else {
        key -= 36;
        if (key > 130) {
            key = 130;
            pitch = 255;
        }
    }

    {
        s32 lower = gRom[key];
        s32 upper;
        lower = gRom2[lower & 15] >> (lower >> 4);
        upper = gRom[key + 1];
        upper = gRom2[upper & 15] >> (upper >> 4);
        return lower + ((pitch *(upper - lower)) >> 8) + 0x800;
    }
}
