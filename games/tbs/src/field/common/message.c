#include "types.h"
#include "system.h"
#include "event_runtime.h"
#include "field.h"

extern struct EventRuntime *gEventWork;
void *ObjectTable_Get(s32 id);

s32 Event_ShowMessageAtPosition();

void Event_ShowSpeakerMessage(s32 speaker)
{
    /* FAKEMATCH: needed to match: the position is never set; as one 8-byte local the native reads it from r6/r7 */
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
