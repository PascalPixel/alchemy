/* Near miss: score 120 beyond the trailing pad. ⚓️ loads the halfword (ldrh
   r2, [r0, #32]) before sign-extending the value; this draft extends first. */
/*
 * Draft: Script_SetOrCompareHalfword20 does not yet match; 4 bytes differ from +0x18.
 * Links as recon/tla/raw/08025c5c.s.
 */
#include "SCRIPT_OPERANDS.H"

void Script_SetOrCompareHalfword20(struct ScriptOperands *state, s32 operation, s32 value)
{
  s8 result;
  if (operation == 0)
  {
    state->halfword_20 = value;
    return;
  }
  if (operation == 1)
  {
    state->halfword_20 = (u16)((u32)state->halfword_20 + (u32)value);
    return;
  }
  result = 0;
  if (state->halfword_20 == (s16)value)
  {
    result = 1;
  }
  state->comparison_result = result;
}
