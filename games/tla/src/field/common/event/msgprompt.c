#include "types.h"
#include "ram_buffer.h"
#include "eventwrk.h"
#include "object_runtime.h"

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

s32 UiText_OpenMessageWindowFar(s32 message, s32 x, s32 y, s32 mode);
s32 UiWork_IsIdleFar(s32 window);
void WaitFrames(s32 frames);

// Opens the next message for the speaker and waits until its window is idle.
void Func_080d40dc(s32 speaker)
{
    // BUG: the position is never set; the window is placed from whatever the
    // two registers hold.
    struct {
        s32 x, y;
    } pos;
    struct EventWork *work = Ram_HeapSlots->event_work;

    speaker &= 0xfff;
    ObjectTable_Get(speaker);
    work->current_owner = speaker;
    if (work->wait_mode == 0) {
        s32 px = pos.x;
        s32 py = pos.y;
        s32 window;

        if (py > 119)
            py += 32;
        else
            py -= 32;
        px = px < 8 ? 8 : px;
        if (px > 312)
            px = 312;
        py = py < 20 ? 20 : py;
        if (py > 220)
            py = 220;
        window = UiText_OpenMessageWindowFar((s16)work->message_id, px, py, 1);
        work->object_status = window;
        while (UiWork_IsIdleFar(window) == 0)
            WaitFrames(1);
    }
    work->message_id++;
}
