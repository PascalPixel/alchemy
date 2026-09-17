#include "TYPES.H"

s32 Audio_UmulHigh32(s32, s32);
extern u8 Sound_PcmPitchCodes[];
extern u32 Sound_PcmFrequencySteps[];

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

    lower_frequency = Sound_PcmPitchCodes[key_index];
    lower_frequency =
        Sound_PcmFrequencySteps[lower_frequency & 15] >> (lower_frequency >> 4);
    upper_frequency = Sound_PcmPitchCodes[key_index + 1];
    upper_frequency =
        Sound_PcmFrequencySteps[upper_frequency & 15] >> (upper_frequency >> 4);

    return Audio_UmulHigh32(*(s32 *)((u8 *)wave + 4),
        lower_frequency
            + Audio_UmulHigh32(
                upper_frequency - lower_frequency,
                pitch_scale));
}

void Audio_NoopStub(void)
{
}
