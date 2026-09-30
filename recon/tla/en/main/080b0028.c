/*
 * Draft: Owner_GetResistanceValue does not yet match; 5 halfwords differ from ☀️'s C, first at +0x12 (movs r2, #165).
 * Links as recon/tla/raw/080affac.s.
 */
#include "TYPES.H"

struct OwnerResistanceState {
    u8 unknown[0xf8];
    u8 source[0x30];
    u8 record;
};

void *Owner_GetState(s32);
s32 Owner_GetDigitValues(s32 record, const u8 *source, s32 *output);

void *Owner_GetState(s32 owner);
s32 Owner_GetDigitValues(s32 record, const u8 *source, s32 output[4]);

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
