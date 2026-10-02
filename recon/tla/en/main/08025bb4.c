/*
 * Draft: Script_SetOrCompareUnsignedHalfword does not yet match; 4 bytes differ from +0x18.
 * Links as recon/tla/raw/08025bb4.s.
 */
#include "SCRIPT.H"

void Script_SetOrCompareUnsignedHalfword(struct ScriptOperands *state, s32 operation, s32 value)
{
    s32 result;

    if (operation == 0) {
        state->unsigned_halfword = value;
    } else if (operation == 1) {
        state->unsigned_halfword =
            (u16)((u32)state->unsigned_halfword + (u32)value);
    } else {
        result = 0;
        if (state->unsigned_halfword == (u16)value)
            result = 1;
        state->comparison_result = result;
    }
}
