#include "types.h"

#define Event_ShowValue1d8AtPosition Func_08093168

struct State_08093168 {
    u8 padding[472];
    s16 counter;
};

extern struct State_08093168 *Data_03001ebc;
extern s32 UiWork_Create(s32, s32, s32, s32);
extern s32 Func_08015050(s32);
extern void WaitFrames(s32);

void Event_ShowValue1d8AtPosition(s32 unused0, s32 unused1, s32 x, s32 y)
{
    s32 x0 = x;
    struct State_08093168 *state = Data_03001ebc;
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

    ret = UiWork_Create(state->counter, px, py, 1);
    while (Func_08015050(ret) == 0)
        WaitFrames(1);
    state->counter++;
}
