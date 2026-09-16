#include "TYPES.H"

extern const u8 Sound_CgbPitchCodes[];
extern const s16 Sound_CgbFrequencySteps[];
extern const u8 Sound_NoisePitchCodes[];

s32 Cgb_KeyToFrequency(u8 kind, u8 key, u8 fine)
{
    s32 low;
    s32 high;

    if (kind == 4) {
        if (key <= 20)
            key = 0;
        else {
            key -= 21;
            if (key > 59)
                key = 59;
        }
        return Sound_NoisePitchCodes[key];
    }

    if (key <= 35) {
        fine = 0;
        key = 0;
    } else {
        key -= 36;
        if (key > 130) {
            key = 130;
            fine = 255;
        }
    }
    low = Sound_CgbPitchCodes[key];
    low = Sound_CgbFrequencySteps[low & 15] >> (low >> 4);
    high = Sound_CgbPitchCodes[key + 1];
    high = Sound_CgbFrequencySteps[high & 15] >> (high >> 4);
    return low + ((fine * (high - low)) >> 8) + 2048;
}
