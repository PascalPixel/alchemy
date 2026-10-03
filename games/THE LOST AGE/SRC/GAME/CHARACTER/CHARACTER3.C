#include "TYPES.H"
#include "OWNER_STATE.H"

s32 Owner_GetDigitValues(s32 record, const u8 *source, s32 *output);

s32 Owner_GetResistanceValue(s32 owner, s32 index)
{
    struct OwnerInventoryState *state = Owner_GetState(owner);
    s32 values[4];
    s32 result = 0;

    if (index <= 3) {
        Owner_GetDigitValues(state->class_id, (const u8 *)&((struct OwnerDjinnState *)state)->flags.banks.owned, values);
        result = values[index] / 10;
    }
    return result;
}
