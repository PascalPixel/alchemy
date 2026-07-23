typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;

struct State_08093168 {
    u8 padding[472];
    s16 counter;
};

extern struct State_08093168 *Data_03001ebc;
extern s32 Func_08015038(s32, s32, s32, s32);
extern s32 Func_08015050(s32);
extern void Func_080030f8(s32);

void Func_08093168(s32 unused0, s32 unused1, s32 x, s32 y)
{
    s32 originalX = x;
    struct State_08093168 *state = Data_03001ebc;
    s32 adjustedY = y;
    s32 adjustedX = originalX;
    s32 handle;

    if (adjustedY > 119)
        adjustedY += 32;
    else
        adjustedY -= 32;
    if (8 <= originalX)
        goto x_ok;
    adjustedX = 8;
x_ok:
    if (adjustedX > 312)
        adjustedX = 312;
    if (20 <= adjustedY)
        goto y_ok;
    adjustedY = 20;
y_ok:
    if (adjustedY > 220)
        adjustedY = 220;

    handle = Func_08015038(state->counter, adjustedX, adjustedY, 1);
    while (Func_08015050(handle) == 0)
        Func_080030f8(1);
    state->counter++;
}
