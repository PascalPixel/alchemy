/* Near miss: score 60. ⚓️ loads the comparison byte before narrowing the
   value; this draft narrows first, a load-scheduling difference. */
/*
 * Draft: Script_SetOrCompareComparisonResult does not yet match; 4 bytes differ from +0x22.
 * Links as recon/tla/raw/08025f9c.s.
 */
#include "SCRIPT_OPERANDS.H"

void Script_SetOrCompareComparisonResult(struct ScriptOperands *state, s32 operation, s32 value)
{
    s32 result;

    if (operation == 0) {
        state->comparison_result = value;
    } else if (operation == 1) {
        state->comparison_result =
            (u8)((u32)state->comparison_result + (u32)value);
    } else {
        result = 0;
        if (state->comparison_result == (u8)value)
            result = 1;
        state->comparison_result = result;
    }
}
