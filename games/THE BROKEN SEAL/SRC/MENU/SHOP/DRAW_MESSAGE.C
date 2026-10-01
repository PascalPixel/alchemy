#include "EDITION.H"
#include "FAR_RUNTIME.H"
#include "SHOP.H"
#include "SYSTEM.H"

s32 UiText_DrawCharacterAtOffsetFar(s32, s32, s32, s32);
#if !EDITION_INTERNATIONAL
void RenderOutput_ClearListFar(s32);
void UiText_DrawMessageAt(s32 message, s32 window, s32 x, s32 y);
#endif

void Shop_DrawMsg(s32 window, s32 message)
{
    if (window != 0) {
#if EDITION_INTERNATIONAL
        RenderOutput_RedrawSavedRectFar(window);
        UiText_DrawCharacterAtOffsetFar(message, window, 0, 0);
#else
        /* The Japanese shop clears the window and waits a frame first. */
        RenderOutput_ClearListFar(window);
        WaitFrames(1);
        UiText_DrawMessageAt(message, window, 0, 0);
#endif
    }
}
