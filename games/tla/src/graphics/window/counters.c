#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "resource.h"
#include "callback_scheduler.h"
#include "uiflow.h"

void UiWork_ProcessAll(void);

void UiWork_InitCountersWithResourceAndScheduleRefresh(void)
{
    struct UiRenderWork *state = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;

    state->glyphTiles = VramBlock_LoadCached(95, 128 << 6, 0);
    state->glyphSpacing = 9;
    state->lineSpacing = 10;
    state->outline = 0;
    state->colour = 15;
    state->count = 0;
    Scheduler_AddOrUpdateCallback(UiWork_ProcessAll, 0x480);
}

void UiWork_ProcessAll(void);
void UiWork_ProcessAllOrChannels(void);

void UiWork_InitCountersAndScheduleRefresh(s32 initialize)
{
    struct UiRenderWork *state = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;

    if (initialize != 0)
        state->glyphTiles = VramBlock_LoadCached(95, 128 << 6, 0);
    state->glyphSpacing = 9;
    state->lineSpacing = 10;
    state->outline = 0;
    state->colour = 15;
    state->count = 0;
    if (initialize != 0)
        Scheduler_AddOrUpdateCallback(UiWork_ProcessAll, 0x480);
    else
        Scheduler_AddOrUpdateCallback(UiWork_ProcessAllOrChannels, 0x480);
}

struct UiMessageSlot
{
    struct UiWindow *window;
    s32              result;
    s32              cursor;
};

void UiMessage_Close(void)
{
    struct UiMessageSlot *slot = Ram_HeapSlots->unk84[4];

    if (slot->window != NULL)
    {
        UiWork_Finalize(slot->window, 1);
        slot->window = NULL;
    }
}
