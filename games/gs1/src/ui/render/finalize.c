#include "types.h"

struct Work {
    s32 unknown00;
    s32 unknown04;
    u16 width;
    u16 height;
    u16 x;
    u16 y;
    u16 unknown10;
    u16 unknown12;
    u16 state;
    u16 flags;
    u16 frame;
    u16 duration;
    u16 previous_x;
    u16 previous_y;
    u16 previous_width;
    u16 previous_height;
};

void RenderOutput_PrepareForRedraw(void *work);
void Func_08016178(u16 x, u16 y, u16 width, u16 height);

void UiWork_Finalize(struct Work *work, s32 release) {
    u16 zero;

    if (work == 0)
        return;

    RenderOutput_PrepareForRedraw(work);
    work->previous_x = work->x;
    work->previous_y = work->y;
    work->previous_width = work->width;
    zero = 0;
    work->flags = zero;
    work->previous_height = work->height;

    if (release != 0) {
        Func_08016178(work->x, work->y, work->width, work->height);
        work->unknown00 = zero;
        work->unknown04 = zero;
        work->width = zero;
        work->height = zero;
        work->x = zero;
        work->y = zero;
        work->unknown10 = zero;
        work->unknown12 = zero;
        work->state = zero;
        work->flags = zero;
        work->frame = zero;
        work->duration = zero;
        work->previous_x = zero;
        work->previous_y = zero;
        work->previous_width = zero;
        work->previous_height = zero;
    } else {
        work->frame = release;
        work->duration = 4;
    }
}
