#include "TYPES.H"
#include "EVENT_RUNTIME.H"
#include "SYSTEM.H"

extern struct EventRuntime *gEventWork;
s32 UiText_OpenMessageWindowFar(s32, s32, s32, s32);
s32 UiWork_IsIdleFar(s32);

void Event_ShowValue1d8AtPosition(s32 unused0, s32 unused1, s32 x, s32 y)
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
