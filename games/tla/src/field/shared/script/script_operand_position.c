#include "script.h"

void Script_SetOrCompareX(struct ScriptOperands *state, s32 operation,
                          s32 value) {
  s8 result;

  if (operation == 0) {
    state->word08 = value;
    return;
  }
  if (operation == 1) {
    state->word08 += value;
    return;
  }
  result = 0;
  if (state->word08 == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void Script_SetOrCompareY(struct ScriptOperands *state, s32 operation,
                          s32 value) {
  s8 result;

  if (operation == 0) {
    state->word0c = value;
    return;
  }
  if (operation == 1) {
    state->word0c += value;
    return;
  }
  result = 0;
  if (state->word0c == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void Script_SetOrCompareZ(struct ScriptOperands *state, s32 operation,
                          s32 value) {
  s8 result;

  if (operation == 0) {
    state->word10 = value;
    return;
  }
  if (operation == 1) {
    state->word10 += value;
    return;
  }
  result = 0;
  if (state->word10 == value) {
    result = 1;
  }
  state->comparisonResult = result;
}

void Script_SetOrCompareRadius(struct ScriptOperands *state, s32 operation,
                               s32 value) {
  s8 result;
  u32 current;

  if (operation == 0) {
    state->halfword20 = value;
    return;
  }
  if (operation == 1) {
    state->halfword20 += value;
    return;
  }
  current = state->halfword20;
  /* FAKEMATCH: ⚓️ loads the halfword before extending the value */
  asm volatile("" : "+l"(current));
  result = 0;
  if (current == (u32)(s16)value)
    result = 1;
  state->comparisonResult = result;
}
