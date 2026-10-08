#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "edition.h"
#include "types.h"
#include "window.h"
#include "callback_scheduler.h"
#include "tbs_edition.h"
#include "btlwork.h"


s32 UiWork_IsIdle(struct UiWindow *window)
{
#if !EDITION_INTERNATIONAL
    if (window == NULL)
        return 1;
#endif
    if (window->flags == 0 && window->duration == 0)
        return 1;
    return 0;
}

void UiWork_ResetCounters(void)
{
    struct UiRenderWork *state = (struct UiRenderWork *)gWindowWork[0];

    state->colour = 15;
    state->line_spacing = 10;
    state->unknown_before_count = 9;
    state->outline = 0;
    state->unknown_after_line_spacing = 1;
}

void UiWork_InitCountersWithResourceAndScheduleRefresh(void)
{
    struct UiRenderWork *state = (struct UiRenderWork *)gWindowWork[0];
    s32 size;

    state->glyph_tiles = VramBlock_LoadCached(95, 128 << 6, 0);
    state->unknown_before_count = 9;
    state->line_spacing = 10;
    state->outline = 0;
    state->colour = 15;
    state->count = 0;
    size = 200;
    size <<= 4;
    Scheduler_AddOrUpdateCallback((void *)UiWork_ProcessAll, size);
}

void UiWork_InitCountersAndScheduleRefresh(s32 initialize)
{
    struct UiRenderWork *state = (struct UiRenderWork *)gWindowWork[0];
    s32 size;

    if (initialize != 0)
        state->glyph_tiles = VramBlock_LoadCached(95, 128 << 6, 0);
    state->unknown_before_count = 9;
    state->line_spacing = 10;
    state->outline = 0;
    state->colour = 15;
    state->count = 0;
    size = 200;
    size <<= 4;
    Scheduler_AddOrUpdateCallback(UiWork_ProcessAll, size);
}

void UiWork_FinalizeSharedSlot(void)
{
    struct BattleDisplayWork *display = gBattleDisplayWork;
    struct UiWindow *window = display->window;

    if (window != NULL) {
        UiWork_Finalize(window, 1);
        display->window = NULL;
    }
}
