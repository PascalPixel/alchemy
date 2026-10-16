#include "types.h"
#include "party_state.h"

void UiText_DrawNumberAtOffsetFar(s32, s32, s32, s32, s32);
void UiText_DrawCharacterAtOffsetFar(s32, s32, s32, s32);
extern u8 MsgCoinsLabel[];

void UiText_DrawWorkValueWithLabel(s32 work) {
  UiText_DrawNumberAtOffsetFar(gPartyState.coins, 7, work, 8, 0);
  UiText_DrawCharacterAtOffsetFar((s32)MsgCoinsLabel, work, 0x40, 0);
}
