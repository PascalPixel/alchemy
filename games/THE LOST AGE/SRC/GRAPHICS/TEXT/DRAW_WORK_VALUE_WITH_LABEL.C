#include "TYPES.H"
#include "TLA_EDITION.H"
#include "PARTY_STATE.H"

void UiText_DrawNumberAtOffsetFar(s32, s32, s32, s32, s32);
void UiText_DrawCharacterAtOffsetFar(s32, s32, s32, s32);
#if defined(TLA_EDITION_JA)
void UiWindow_DrawDividerLineFar(s32, s32, s32, s32, s32);
#endif
extern u8 MsgCoinsLabel[];

/* Draws the party's coins, seven digits wide, and the coin label into a window. */
void UiText_DrawWorkValueWithLabel(s32 work)
{
    UiText_DrawNumberAtOffsetFar(gPartyState.coins, 7, work, 8, 0);
    UiText_DrawCharacterAtOffsetFar((s32)MsgCoinsLabel, work, 0x40, 0);
#if defined(TLA_EDITION_JA)
    UiWindow_DrawDividerLineFar(work, 12, 0, 12, 2);
#endif
}
