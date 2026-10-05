#include "GLOBAL.H"
#include "TYPES.H"
#include "WINDOW.H"

void UiText_DrawStringInWindow(u8 *s, s32 arg1, u32 arg2, u32 arg3);
void UiText_DrawStringAtOffset(u8 *s, s32 window, s32 x, s32 y);
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
