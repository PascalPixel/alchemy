#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "TLA_EDITION.H"
#include "PARTY_STATE.H"
#include "text/MSG_IDS.H"

void UiText_DrawNumberAtOffsetFar(s32, s32, s32, s32, s32);
void UiText_DrawCharacterAtOffsetFar(s32, s32, s32, s32);
#if !EDITION_INTERNATIONAL
#endif

void UiText_DrawWorkValueWithLabel(s32 work)
{
    UiText_DrawNumberAtOffsetFar(gPartyState.coins, 7, work, 8, 0);
    UiText_DrawCharacterAtOffsetFar((s32)MsgCoinsLabel, work, 0x40, 0);
#if !EDITION_INTERNATIONAL
    UiWindow_DrawDividerLineFar(work, 12, 0, 12, 2);
#endif
}
