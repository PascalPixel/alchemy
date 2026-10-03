#include "RESOURCE.H"
#include "EDITION.H"
#include "TYPES.H"
#include "WINDOW.H"
#include "CALLBACK_SCHEDULER.H"
#include "TBS_EDITION.H"
#include "BATTLE_WORK.H"


void UiWork_ProcessAll(void);


s32 UiWork_IsIdle(struct UiWindow *window)
{
#if !EDITION_INTERNATIONAL
    /* The Japanese check counts a missing window as idle. */
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
    Scheduler_AddOrUpdateCallback((s32)((void *)UiWork_ProcessAll), size);
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
    Scheduler_AddOrUpdateCallback((s32)(UiWork_ProcessAll), size);
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
