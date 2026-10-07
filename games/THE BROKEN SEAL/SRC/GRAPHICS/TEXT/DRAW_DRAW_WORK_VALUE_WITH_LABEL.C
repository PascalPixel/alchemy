#include "EDITION.H"
#include "TYPES.H"
#include "SCENE.H"
#include "M7_INTERFACES.H"
#include "TBS_EDITION.H"
#include "GAME_STATE.H"
#include "UI.H"
#include "text/MSG_IDS.H"
#include "GRAPHICS.H"

void UiText_DrawWorkValueWithLabel(s32 work)
{
    UiText_DrawNumberAtOffsetFar(gGameState.coins, 7, work, 8, 0);
    UiText_DrawCharacterAtOffsetFar((s32)MsgCoinsLabel, work, 64, 0);
#if !EDITION_INTERNATIONAL
    UiWindow_DrawDividerLineFar(work, 12, 0, 12, 2);
#endif
}

void UiWindow_SetBounds(struct RenderInput *window, s32 x, s32 y,
    s32 width, s32 height)
{
    if (window != NULL) {
        window->width = width;
        window->x = x;
        window->height = height;
        window->y = y;
    }
}
