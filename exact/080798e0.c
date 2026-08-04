#include "types.h"

struct OwnerState {
    u8 unknown_000[0xf8];
    u8 source_f8[0x30];
    u8 record_128;
    u8 use_source_129;
};

struct PresetValues {
    u8 unknown_00[8];
    s32 values[4];
};

struct DigitOffsets {
    u16 first;
    u16 second;
};

extern const struct DigitOffsets Data_08088df8[16];
extern const struct PresetValues Data_08088e38[44];

struct OwnerState *Func_08077394(s32 owner);
u8 *Func_080773d8(s32 record);
s32 Func_080797fc(s32 record, const u8 *source, s32 output[4]);
s32 Func_080022fc(s32 value, s32 divisor);
s32 Func_080022ec(s32 value, s32 divisor);

u32 Func_080798e0(s32 owner, s16 destination[4][2])
{
    struct OwnerState *state = Func_08077394(owner);
    u32 index;
    u32 result;
    s32 values[4];
    s32 i;

    if (state->use_source_129 == 0) {
        index = Func_080773d8(state->record_128)[52];
        if (index > 43)
            index = 0;

        i = 0;
        for (;;) {
            ((s32 *)destination)[i] =
                Data_08088e38[index].values[i];
            i++;
            if (i > 3)
                goto copied;
        }
copied:
        return index;
    }

    Func_080797fc(state->record_128, state->source_f8, values);
    i = 0;
    do {
        s32 value;
        s32 ones;
        s32 tens;

        result = (u32)Data_08088df8;
        value = values[i];
        ones = Func_080022fc(value, 10);
        tens = Func_080022ec(value, 10);

        if (tens > 15)
            tens = 15;
        if (tens < 0)
            tens = 0;

        destination[i][0] = Data_08088df8[tens].first + ones;
        destination[i][1] =
            ((volatile const struct DigitOffsets *)Data_08088df8)[tens].second + ones;
        i++;
    } while (i < 4);
    return result;
}
