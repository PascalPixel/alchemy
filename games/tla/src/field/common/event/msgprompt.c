#include "types.h"
#include "ram_buffer.h"
#include "eventwrk.h"

void Func_080d3fb0(s32 object, s32 message);
void EventRuntime_Wait(s32 frames);

void EventRuntime_ShowMessageThenWait(s32 object, s32 message, s32 frames)
{
    Func_080d3fb0(object, message);
    EventRuntime_Wait(frames);
}

void Func_080d3fb0(s32 object, s32 message);

void EventRuntime_ShowMessageAndWait(s32 object, s32 message)
{
    Func_080d3fb0(object, message);
}

void *UiText_OpenMessageAtObject(s32 object, s32 message);
s32 EventRuntime_GetControlledOwner(void);
s32 Inventory_PromptAndSetObjectMode(s32 owner, s32 mode);
void Func_080d3fb0(s32 object, s32 message);

s32 EventRuntime_PromptInventoryMessage(s32 object, s32 message)
{
    s32 result;

    UiText_OpenMessageAtObject(object, message);
    result = Inventory_PromptAndSetObjectMode(EventRuntime_GetControlledOwner(), 0);
    if (result == 0) {
        Func_080d3fb0(object, message);
        ((struct EventWork *)Ram_HeapSlots->event_work)->message_id++;
    } else {
        ((struct EventWork *)Ram_HeapSlots->event_work)->message_id++;
        Func_080d3fb0(object, message);
    }
    return result;
}

void EventRuntime_Noop(void)
{
}
