#include "script.h"

void Script_SetOrCompareAddress(struct ScriptOperands *state, s32 operation,
                                s32 value) {
  s8 result;

  if (operation == 0) {
    state->scriptAddress = value;
    return;
  }
  if (operation == 1) {
    state->scriptAddress += value * 4;
    return;
  }
  result = 0;
  if (state->scriptAddress == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void Script_SetOrCompareCursor(struct ScriptOperands *state, s32 operation,
                               s32 value) {
  if (operation == 0) {
    state->cursor = value;
  } else if (operation == 1) {
    state->cursor = (s16)state->cursor + value;
  } else {
    state->comparisonResult = (s16)state->cursor == (s16)value;
  }
}

void Script_SetOrCompareUnsignedHalfword(struct ScriptOperands *state,
                                         s32 operation, s32 value) {
  s32 result;

  if (operation == 0) {
    state->unsignedHalfword = value;
  } else if (operation == 1) {
    state->unsignedHalfword = (u16)((u32)state->unsignedHalfword + (u32)value);
  } else {
    u32 current = state->unsignedHalfword;

    /* FAKEMATCH: ⚓️ loads the halfword before it starts truncating the value;
     * without the barrier the shift is scheduled first (differ 2) */
    asm volatile("");
    result = 0;
    if (current == (u16)value)
      result = 1;
    state->comparisonResult = result;
  }
}
