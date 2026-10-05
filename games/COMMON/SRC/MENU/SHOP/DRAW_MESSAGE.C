#include "PROTO.H"
#include "EDITION.H"
#include "FAR_RUNTIME.H"
#include "SYSTEM.H"

void Shop_DrawMsg(s32 window, s32 message);
s32 UiText_DrawCharacterAtOffsetFar(s32, s32, s32, s32);
#if !EDITION_INTERNATIONAL
void RenderOutput_ClearListFar(s32);
#endif

void Shop_DrawMsg(s32 window, s32 message)
{
    if (window != 0) {
#if EDITION_INTERNATIONAL
        RenderOutput_RedrawSavedRectFar(window);
        UiText_DrawCharacterAtOffsetFar(message, window, 0, 0);
#else
        RenderOutput_ClearListFar(window);
        WaitFrames(1);
        UiText_DrawResourceFar(message, window, 0, 0);
#endif
    }
}
