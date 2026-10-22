// First draft of Func_080fd55c: draws the two packed event values of the
// party state (owner << 10 | item) into a window: a heading (message 0x1013
// when both are set, else 0x100f), then for each the item's quantity, a label
// and the owner's name, or an empty-slot message. Measures each item name
// (result unused). Returns 1. No ☀️ twin; the message names are placeholders
// for the pooled linker symbols. Differ 10 of 140 (2026-10-19, from 28 with
// the item name's address in a local): at both measurements the native loads
// the packed value before it starts the 0x3ff mask; here the mask's first
// move comes first. Three minutes of permute found nothing nearer.
#include "types.h"
#include "party_state.h"

extern u8 Msg0100f[], Msg01013[], Msg01014[], Msg01015[], Msg01016[],
    Msg01017[];
extern u8 MsgItemName[];

void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void UiText_GetResourceDimensionsFar(u8 *message, s32 *left, s32 *top,
                                     s32 *width, s32 *height);
void UiText_DrawQuantity(s32 item, s32 digits);
void *Unit_Get(s32 unit);
void UiText_DrawStringAtOffsetFar(s32 text, s32 window, s32 x, s32 y);

s32 Func_080fd55c(s32 window) {
  s32 left;
  s32 top;
  s32 width;
  s32 height;
  u8 *name;

  if (gPartyState.packedEvents[0] != 0 && gPartyState.packedEvents[1] != 0)
    UiText_DrawCharacterAtOffsetFar((s32)Msg01013, window, 0, -8);
  else
    UiText_DrawCharacterAtOffsetFar((s32)Msg0100f, window, 0, -8);

  name = &MsgItemName[gPartyState.packedEvents[0] & 0x3ff];
  UiText_GetResourceDimensionsFar(name, &left, &top, &width, &height);
  if (gPartyState.packedEvents[0] != 0) {
    UiText_DrawQuantity(gPartyState.packedEvents[0] & 0x3ff, 4);
    UiText_DrawCharacterAtOffsetFar((s32)Msg01016, window, 0, 0);
    UiText_DrawStringAtOffsetFar(
        (s32)Unit_Get(gPartyState.packedEvents[0] >> 10), window, 80, 0);
  } else {
    UiText_DrawCharacterAtOffsetFar((s32)Msg01014, window, 0, 0);
  }

  name = &MsgItemName[gPartyState.packedEvents[1] & 0x3ff];
  UiText_GetResourceDimensionsFar(name, &left, &top, &width, &height);
  if (gPartyState.packedEvents[1] != 0) {
    UiText_DrawQuantity(gPartyState.packedEvents[1] & 0x3ff, 4);
    UiText_DrawCharacterAtOffsetFar((s32)Msg01017, window, 0, 8);
    UiText_DrawStringAtOffsetFar(
        (s32)Unit_Get(gPartyState.packedEvents[1] >> 10), window, 80, 8);
  } else {
    UiText_DrawCharacterAtOffsetFar((s32)Msg01015, window, 0, 8);
  }
  return 1;
}
