/* Field events: record the speaker, show the next event message in a window and wait for it, then advance the message. */
#include "TYPES.H"
#include "SYSTEM.H"

struct EventWork {
    u8 unknown_000[0x1cc];
    s32 silent;
    u8 unknown_1d0[8];
    s16 message;
    u8 unknown_1da[0x1a];
    s32 speaker;
    s32 window;
};

extern struct EventWork *gEventWork;
void *ObjectTable_Get(s32 id);
s32 UiText_OpenMessageWindowFar(s32 message, s32 x, s32 y, s32 mode);
s32 UiWork_IsIdleFar(s32 window);

s32 Event_ShowValue1d8AtPosition();

void Event_ShowSpeakerMessage(s32 speaker)
{
    register s32 x asm("r6"); /* FAKEMATCH: the unset x lives in r6 */
    s32 y;
    register struct EventWork *state asm("r8") = gEventWork; /* FAKEMATCH: keeps the work in r8 */

    speaker &= 0xfff;
    ObjectTable_Get(speaker);
    state->speaker = speaker;
    if (state->silent == 0) {
        s32 py; s32 px = x; s32 min_x = 8; s32 min_y = 20; asm("" : "=l"(py) : "0"(y)); /* FAKEMATCH: copies y before the test */ if (py > 119) py += 32; else py -= 32; if (px < min_x) px = min_x; if (px > 312) px = 312; if (py < min_y) py = min_y; if (py > 220) py = 220;
        { s32 win = UiText_OpenMessageWindowFar(state->message, px, py, 1);
        state->window = win;
        while (UiWork_IsIdleFar(win) == 0)
            WaitFrames(1);
        }
    }
    { s16 *m = &state->message; (*m)++; }
}

void ObjectTable_CallRefreshHook(void)
{
    Event_ShowValue1d8AtPosition();
}
