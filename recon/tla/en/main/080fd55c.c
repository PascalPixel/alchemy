// First draft of Func_080fd55c: draws the two packed event values of the
// party state (owner << 10 | item) into a window: a heading (message 0x1013
// when both are set, else 0x100f), then for each the item's quantity, a label
// and the owner's name, or an empty-slot message. Measures each item name
// (result unused). Returns 1. No ☀️ twin; the message names are placeholders
// for the pooled linker symbols. Differ 28 of 140 (draft 140), the same
// difference at both measurements: the native loads the packed value into r2
// before it starts the 0x3ff mask and masks into r3, then adds the pooled
// base in r2; here the value takes r3 and the mask lands in r0 (either
// operand order of the sum compiles the same). Register choice and order.
#include "types.h"
#include "party_state.h"

extern u8 Msg0100f[], Msg01013[], Msg01014[], Msg01015[], Msg01016[],
    Msg01017[];
extern u8 MsgItemName[];

void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void UiText_GetResourceDimensionsFar(s32 message, s32 *left, s32 *top,
                                     s32 *width, s32 *height);
void UiText_DrawQuantity(s32 item, s32 digits);
void *UnitGet(s32 unit);
void UiText_DrawStringAtOffsetFar(s32 text, s32 window, s32 x, s32 y);

s32 Func_080fd55c(s32 window) {
  s32 left;
  s32 top;
  s32 width;
  s32 height;

  if (gPartyState.packedEvents[0] != 0 && gPartyState.packedEvents[1] != 0)
    UiText_DrawCharacterAtOffsetFar((s32)Msg01013, window, 0, -8);
  else
    UiText_DrawCharacterAtOffsetFar((s32)Msg0100f, window, 0, -8);

  UiText_GetResourceDimensionsFar(
      (s32)MsgItemName + (gPartyState.packedEvents[0] & 0x3ff), &left, &top,
      &width, &height);
  if (gPartyState.packedEvents[0] != 0) {
    UiText_DrawQuantity(gPartyState.packedEvents[0] & 0x3ff, 4);
    UiText_DrawCharacterAtOffsetFar((s32)Msg01016, window, 0, 0);
    UiText_DrawStringAtOffsetFar(
        (s32)UnitGet(gPartyState.packedEvents[0] >> 10), window, 80, 0);
  } else {
    UiText_DrawCharacterAtOffsetFar((s32)Msg01014, window, 0, 0);
  }

  UiText_GetResourceDimensionsFar(
      (s32)MsgItemName + (gPartyState.packedEvents[1] & 0x3ff), &left, &top,
      &width, &height);
  if (gPartyState.packedEvents[1] != 0) {
    UiText_DrawQuantity(gPartyState.packedEvents[1] & 0x3ff, 4);
    UiText_DrawCharacterAtOffsetFar((s32)Msg01017, window, 0, 8);
    UiText_DrawStringAtOffsetFar(
        (s32)UnitGet(gPartyState.packedEvents[1] >> 10), window, 80, 8);
  } else {
    UiText_DrawCharacterAtOffsetFar((s32)Msg01015, window, 0, 8);
  }
  return 1;
}
