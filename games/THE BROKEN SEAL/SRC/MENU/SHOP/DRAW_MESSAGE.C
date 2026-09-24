#include "FAR_RUNTIME.H"
#include "SHOP.H"

s32 UiText_DrawCharacterAtOffsetFar(s32, s32, s32, s32);

void Shop_DrawMsg(s32 window, s32 message)
{
    if (window != 0) {
        RenderOutput_RedrawSavedRectFar(window);
        UiText_DrawCharacterAtOffsetFar(message, window, 0, 0);
    }
}
