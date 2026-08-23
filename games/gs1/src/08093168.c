#include "types.h"

struct State_08093168 {
    u8 padding[472];
    s16 counter;
};

extern struct State_08093168 *Data_03001ebc;
extern s32 UiWork_Create(s32, s32, s32, s32);
extern s32 Func_08015050(s32);
extern void WaitFrames(s32);

void Func_08093168(s32 unused0, s32 unused1, s32 x, s32 y)
{
    s32 originalX = x;
    struct State_08093168 *state = Data_03001ebc;
    s32 adjustedY = y;
    s32 adjustedX = originalX;
    s32 minX = 8;
    s32 minY = 20;
    s32 handle;

    if (adjustedY > 119)
        adjustedY += 32;
    else
        adjustedY -= 32;

    if (originalX < minX)
        adjustedX = minX;
    if (adjustedX > 312)
        adjustedX = 312;
    if (adjustedY < minY)
        adjustedY = minY;
    if (adjustedY > 220)
        adjustedY = 220;

    handle = UiWork_Create(state->counter, adjustedX, adjustedY, 1);
    while (Func_08015050(handle) == 0)
        WaitFrames(1);
    state->counter++;
}
