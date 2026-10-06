#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TEXT_RENDER_RUNTIME.H"
#include "TYPES.H"
#include "WINDOW.H"

#define UiText_DrawStringAtOffset ((void (*)(u8 *, s32, s32, s32))UiText_DrawStringAtOffset)

void UiText_DrawStringInWindow(u8 *s, s32 arg1, u32 arg2, u32 arg3);
s32 Link_CreateCountdownLabelWindow(void)
{
    s32 handle = UiWindow_Create(0, 0, 6, 4, 6);

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || \
    defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    UiText_DrawStringAtOffset(Link_TimeLabelString, handle, 0, 0);
#else
    UiText_DrawStringInWindow(Link_TimeLabelString, handle, 0, 0);
#endif
    return handle;
}
