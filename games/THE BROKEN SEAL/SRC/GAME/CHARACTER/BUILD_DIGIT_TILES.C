#include "TYPES.H"

struct OwnerDigitState {
    u8 unknown_000[0xf8];
    u8 source_f8[0x30];
    u8 record_128;
    u8 use_source_129;
};

#include "PRESET_TABLE.H"

extern struct PresetValues Enemy_ElementPresetTable[];

struct DigitOffsets {
    u16 first;
    u16 second;
};

extern const struct DigitOffsets Element_PowerResistByLevel[16];

void *Owner_GetState(s32 owner);
const u8 *Owner_GetRecord(s32 record);
s32 Owner_GetDigitValues(s32 record, const u8 *source, s32 output[4]);
s32 Math_Mod(s32 value, s32 divisor);
s32 Math_Div(s32 value, s32 divisor);

u32 Owner_BuildDigitTiles(s32 owner, s16 destination[4][2])
{
    struct OwnerDigitState *state = (struct OwnerDigitState *)Owner_GetState(owner);
    u32 index;
    u32 result;
    s32 values[4];
    s32 i;

    if (state->use_source_129 == 0) {
        index = Owner_GetRecord(state->record_128)[52];
        if (index > 43)
            index = 0;

        i = 0;
        for (;;) {
            ((s32 *)destination)[i] =
                Enemy_ElementPresetTable[index].values[i];
            i++;
            if (i > 3)
                goto copied;
        }
copied:
        return index;
    }

    Owner_GetDigitValues(state->record_128, state->source_f8, values);
    i = 0;
    do {
        s32 value;
        s32 ones;
        s32 tens;

        result = (u32)Element_PowerResistByLevel;
        value = values[i];
        ones = Math_Mod(value, 10);
        tens = Math_Div(value, 10);

        if (tens > 15)
            tens = 15;
        if (tens < 0)
            tens = 0;

        destination[i][0] = Element_PowerResistByLevel[tens].first + ones;
        destination[i][1] =
            ((volatile const struct DigitOffsets *)Element_PowerResistByLevel)[tens].second + ones;
        i++;
    } while (i < 4);
    return result;
}
