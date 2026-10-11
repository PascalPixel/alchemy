#include "types.h"
#include "system.h"
#include "event_runtime.h"
#include "field.h"

void Event_ShowMessageAtPosition(s32 unused0, s32 unused1, s32 x, s32 y);

extern struct EventRuntime *gEventWork;
#define ObjectTable_Get ((void *(*)(s32 id))ObjectTable_Get)
#define Event_ShowMessageAtPosition ((s32 (*)())Event_ShowMessageAtPosition)

void Event_ShowSpeakerMessage(s32 speaker)
{
    // 位置は未初期化のまま読まれる (original bug: the position is never set)
    struct { s32 x, y; } pos;
    struct EventRuntime *state = gEventWork;

    speaker &= 0xfff;
    ObjectTable_Get(speaker);
    state->speaker = speaker;
    if (state->message_busy == 0) {
        s32 px = pos.x;
        s32 py = pos.y;
        s32 win;

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
        win = UiText_OpenMessageWindowFar(state->message, px, py, 1);
        state->message_window = win;
        while (UiWork_IsIdleFar(win) == 0)
            WaitFrames(1);
    }
    state->message++;
}

void ObjectTable_CallRefreshHook(void)
{
    Event_ShowMessageAtPosition();
}

#undef ObjectTable_Get
#undef Event_ShowMessageAtPosition

void Event_ShowMessageAtPosition(s32 unused0, s32 unused1, s32 x, s32 y)
{
    struct EventRuntime *state = gEventWork;
    s32 py = y;
    s32 px = x;
    s32 min_x = 8;
    s32 min_y = 20;
    s32 ret;

    if (py > 119)
        py += 32;
    else
        py -= 32;

    if (x < min_x)
        px = min_x;
    if (px > 312)
        px = 312;
    if (py < min_y)
        py = min_y;
    if (py > 220)
        py = 220;

    ret = UiText_OpenMessageWindowFar(state->message, px, py, 1);
    while (UiWork_IsIdleFar(ret) == 0)
        WaitFrames(1);
    state->message++;
}
