#include "types.h"

struct Work {
    u8 padding00[8];
    u16 width;
    u16 height;
    u16 x;
    u16 y;
    u16 padding10;
    u16 pending;
};

struct Slot {
    struct Work *work;
    u16 field04;
    u16 field06;
    u16 values[4];
    u16 field10;
    u16 field12;
    u16 field14;
};

void Func_080170f8(s32 x, s32 y, s32 width, s32 height);
void Func_08016178(s32 x, s32 y, s32 width, s32 height);

void Func_08019854(struct Slot *channel)
{
    struct Work *work = channel->work;
    s32 x = work->x;
    s32 y = work->y;
    s32 width = work->width;
    s32 pending = work->pending;
    s32 height = work->height;

    if (pending != 4)
        return;

    Func_080170f8(x - 1, y - 1, width + 2, height + 2);
    channel->field14--;
    if (channel->field14 != 0)
        return;

    channel->work->pending = 0;
    Func_08016178(x - 1, y - 1, width + 2, height + 2);
    Func_080170f8(x, y, width, height);
}
