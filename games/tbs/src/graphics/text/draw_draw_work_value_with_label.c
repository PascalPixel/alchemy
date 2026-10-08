#include "edition.h"
#include "types.h"
#include "scene.h"
#include "m7_interfaces.h"
#include "tbs_edition.h"
#include "game_state.h"
#include "ui.h"
#include "text/msg_ids.h"
#include "graphics.h"

void UiText_DrawWorkValueWithLabel(s32 work)
{
    UiText_DrawNumberAtOffsetFar(gGameState.coins, 7, work, 8, 0);
    UiText_DrawCharacterAtOffsetFar((s32)MsgCoinsLabel, work, 0x40, 0);
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
