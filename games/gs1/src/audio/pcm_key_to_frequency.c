#include "audio_engine_symbols.h"
#include "types.h"

s32 Math_UmulHigh32(s32, s32);
extern u8 Data_080fb830[];
extern u32 Data_080fb8e4[];

s32 Pcm_KeyToFrequency(void *wave, u8 key, u8 pitch)
{
    u32 key_index;
    u32 pitch_scale;
    u32 lower_frequency;
    u32 upper_frequency;

    key_index = key;
    pitch_scale = pitch << 24;
    if (key_index > 178) {
        key_index = 178;
        pitch_scale = 255 << 24;
    }

    lower_frequency = Data_080fb830[key_index];
    lower_frequency =
        Data_080fb8e4[lower_frequency & 15] >> (lower_frequency >> 4);
    upper_frequency = Data_080fb830[key_index + 1];
    upper_frequency =
        Data_080fb8e4[upper_frequency & 15] >> (upper_frequency >> 4);

    return Math_UmulHigh32(*(s32 *)((u8 *)wave + 4),
        lower_frequency
            + Math_UmulHigh32(
                upper_frequency - lower_frequency,
                pitch_scale));
}

void Audio_NoopStub(void)
{
}
