#include "types.h"
#include "gs1_edition.h"

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
    s16 frame;
    s16 duration;
    s16 previous_x;
    s16 previous_y;
    s16 previous_width;
    s16 previous_height;
};

extern u8 *Data_03001e8c;

void UiWindow_UpdateInterpolatedGeometry(void *window, s32 save_position);
void UiWindow_EraseBorderRect(s32 x, s32 y, u32 width, u32 height);
void Func_08016230(void *arg0);

void UiWork_ProcessDirectWork(void)
{
    u8 *base = Data_03001e8c;
    struct Work *work = (struct Work *)(base + 0x500);
    s32 index = 0;
    u8 dirty;

loop:
    if (work->flags != 0) {
        if (work->frame != 0) {
            UiWindow_UpdateInterpolatedGeometry(work, 0);
            work->frame--;
        } else if (work->duration != 0) {
            Func_08016230(work);
        }
    } else if (work->duration != 0) {
        if (work->frame != work->duration) {
            UiWindow_EraseBorderRect(work->previous_x, work->previous_y,
                                     work->previous_width,
                                     work->previous_height);
            UiWindow_UpdateInterpolatedGeometry(work, 1);
            work->frame++;
            dirty = 1;
            base[RENDER_DIRTY_OFS] = dirty;
        } else {
            UiWindow_EraseBorderRect(work->previous_x, work->previous_y,
                                     work->previous_width,
                                     work->previous_height);
            work->unknown00 = 0;
            work->unknown04 = 0;
            work->width = 0;
            work->height = 0;
            work->x = 0;
            work->y = 0;
            work->unknown10 = 0;
            work->unknown12 = 0;
            work->state = 0;
            work->flags = 0;
            work->frame = 0;
            work->duration = 0;
            work->previous_x = 0;
            work->previous_y = 0;
            work->previous_width = 0;
            work->previous_height = 0;
            base[RENDER_DIRTY_OFS] = 1;
        }
    }
    index++;
    work++;
    if (index != 8)
        goto loop;
}
