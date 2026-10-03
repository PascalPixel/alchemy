#include "EDITION.H"
#include "TYPES.H"
#include "SCENE.H"
#include "M7_INTERFACES.H"
#include "TBS_EDITION.H"
#include "GAME_STATE.H"

/* ui/text/draw/draw_work_value_with_label.c */

extern void UiText_DrawCharacterAtOffsetFar(s32, s32, s32, s32);
void UiWindow_DrawDividerLineFar(s32 window, s32 x1, s32 y1, s32 x2, s32 y2);
extern u8 MsgCoinsLabel[];

void UiText_DrawWorkValueWithLabel(s32 work)
{
    UiText_DrawNumberAtOffsetFar(gGameState.coins, 7, work, 8, 0);
    UiText_DrawCharacterAtOffsetFar((s32)MsgCoinsLabel, work, 0x40, 0);
#if !EDITION_INTERNATIONAL
    /* The Japanese window closes the coin count off with a short rule. */
    UiWindow_DrawDividerLineFar(work, 12, 0, 12, 2);
#endif
}

/* ui/window/set_bounds.c */
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
