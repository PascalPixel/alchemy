#include "TYPES.H"
#include "SYSTEM.H"
#include "RAM_BUFFER.H"

/* The event work's message counter: ☀️ keeps it at 0x1d8 and names its
   twin Event_ShowValue1d8AtPosition. */
struct EventCounterWork {
    u8 unknown_000[0x1c4];
    s16 counter;
};

extern s32 UiText_OpenMessageWindowFar(s32, s32, s32, s32);
extern s32 UiWork_IsIdleFar(s32);

/* Open the counter's message window near a point, kept on screen, and wait
   for it before counting on. */
void Event_ShowCounterAtPosition(s32 unused0, s32 unused1, s32 x, s32 y)
{
    s32 x0 = x;
    struct EventCounterWork *state = Ram_HeapSlots->event_work;
    s32 py = y;
    s32 px = x0;
    s32 min_x = 8;
    s32 min_y = 20;
    s32 ret;

    if (py > 119)
        py += 32;
    else
        py -= 32;

    if (x0 < min_x)
        px = min_x;
    if (px > 312)
        px = 312;
    if (py < min_y)
        py = min_y;
    if (py > 220)
        py = 220;

    ret = UiText_OpenMessageWindowFar(state->counter, px, py, 1);
    while (UiWork_IsIdleFar(ret) == 0)
        WaitFrames(1);
    state->counter++;
}
