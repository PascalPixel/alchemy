#include "audio_engine_symbols.h"
#include "types.h"

extern u8 Data_080fb92c[];
extern s16 Data_080fb9b0[];
extern u8 Data_080fb9c8[];

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
        return Data_080fb9c8[key];
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
        s32 lower = Data_080fb92c[key];
        s32 upper;
        lower = Data_080fb9b0[lower & 15] >> (lower >> 4);
        upper = Data_080fb92c[key + 1];
        upper = Data_080fb9b0[upper & 15] >> (upper >> 4);
        return lower + ((pitch * (upper - lower)) >> 8) + 0x800;
    }
}
