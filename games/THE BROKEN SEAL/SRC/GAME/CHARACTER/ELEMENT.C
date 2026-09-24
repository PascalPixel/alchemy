#include "TYPES.H"

extern s32 Data_08088db8[];

s32 Owner_LookupFourColumnTable(s32 row, s32 column)
{
    return Data_08088db8[(row * 4) + column];
}

#include "RUNTIME_INTERFACES.H"
#include "PRESET_TABLE.H"
#include "FIXED_MATH.H"

extern struct PresetValues Enemy_ElementPresetTable[];

struct OwnerBonusValues {
    u8 unknown[2];
    u8 values[148];
};

struct OwnerBonusValues *Owner_GetRecordStride180(s32);

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
            *cursor++ = Enemy_ElementPresetTable[index].digits[i] * 10;
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
                *output += Owner_GetRecordStride180(record)->values[144 + i];
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

s32 Owner_GetResistanceValue(s32 owner, s32 index)
{
    struct OwnerResistanceState *state = (struct OwnerResistanceState *)Owner_GetState(owner);
    s32 values[4];
    s32 result = 0;

    if (index <= 3) {
        Owner_GetDigitValues(state->record, state->source, values);
        result = Math_Div(values[index], 10);
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
    return Enemy_ElementPresetTable[value].first;
}

#include "TYPES.H"

struct OwnerDigitState {
    u8 unknown_000[0xf8];
    u8 source_f8[0x30];
    u8 record_128;
    u8 use_source_129;
};

#include "PRESET_TABLE.H"
#include "FIXED_MATH.H"

extern struct PresetValues Enemy_ElementPresetTable[];

struct DigitOffsets {
    u16 first;
    u16 second;
};

extern const struct DigitOffsets Element_PowerResistByLevel[16];

void *Owner_GetState(s32 owner);
const u8 *Owner_GetRecord(s32 record);
s32 Owner_GetDigitValues(s32 record, const u8 *source, s32 output[4]);

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
