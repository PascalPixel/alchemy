#include "TYPES.H"
#include "OWNER_STATE.H"

s32 Math_Div(s32, s32);
s32 Owner_GetDigitValues(s32 record, const u8 *source, s32 *output);

/* ⚓️ keeps the class as a halfword at 0x14a; ☀️ as a byte at 0x128. */
struct OwnerResistanceState {
    u8 unknown[0xf8];
    u8 source[0x52];
    u16 record;
};

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
