/* Draft: localized label contexts repair four editions, but the Japanese
 * drawing routine still lacks its divider-line call (48 rather than 64 bytes). */
#include "TYPES.H"
#include "PARTY_STATE.H"

void UiText_DrawNumberAtOffsetFar(s32, s32, s32, s32, s32);
void UiText_DrawCharacterAtOffsetFar(s32, s32, s32, s32);
extern u8 MsgCoinsLabel[];

/* Draws the party's coins, seven digits wide, and the coin label into a window. */
void UiText_DrawWorkValueWithLabel(s32 work)
{
    UiText_DrawNumberAtOffsetFar(gPartyState.coins, 7, work, 8, 0);
    UiText_DrawCharacterAtOffsetFar((s32)MsgCoinsLabel, work, 0x40, 0);
}
