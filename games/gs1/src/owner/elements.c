#include "runtime_interfaces.h"
#include "preset_table.h"

extern struct PresetValues Data_08088e38[];

struct OwnerBonusValues {
    u8 unknown[2];
    u8 values[148];
};

struct OwnerBonusValues *Func_08078ed8(s32);

s32 Owner_GetDigitValues(s32 record, const u8 *source, s32 output[4])
{
    s32 i;
    s32 *cursor;

    if (record > 7) {
        u32 index;

        index = Owner_GetRecord(record)[52];
        if (index > 43)
            index = 0;

        i = 0;
        cursor = output;
        for (; i <= 3; i++)
            *cursor++ = Data_08088e38[index].digits[i] * 10;
    } else {
        cursor = output;
        source += 36;
        for (i = 3; i >= 0; i--) {
            u32 value = *source;
            source++;
            *cursor++ = value * 10;
        }

        if (record <= 7) {
            for (i = 0; i <= 3; i++) {
                *output += Func_08078ed8(record)->values[144 + i];
                output++;
            }
        }
    }

    return 0;
}

struct OwnerResistanceState {
    u8 unknown[0xf8];
    u8 source[0x30];
    u8 record;
};

void *Owner_GetState(s32);
s32 Owner_GetDigitValues(s32 record, const u8 *source, s32 *output);
s32 FixedPoint_Ratio(s32, s32);

s32 Owner_GetResistanceValue(s32 owner, s32 index)
{
    struct OwnerResistanceState *state = (struct OwnerResistanceState *)Owner_GetState(owner);
    s32 values[4];
    s32 result = 0;

    if (index <= 3) {
        Owner_GetDigitValues(state->record, state->source, values);
        result = FixedPoint_Ratio(values[index], 10);
    }
    return result;
}

struct OwnerElementState {
    u8 unknown[0x128];
    u8 record;
};

struct OwnerElementRecord {
    u8 unknown[0x34];
    u8 value;
};


s32 Owner_GetDefaultElement(struct OwnerElementState *state)
{
    const struct OwnerElementRecord *record =
        (const struct OwnerElementRecord *)Owner_GetRecord(state->record);
    u8 value = record->value;

    if ((u32)value > 43)
        value = 0;
    return Data_08088e38[value].first;
}
