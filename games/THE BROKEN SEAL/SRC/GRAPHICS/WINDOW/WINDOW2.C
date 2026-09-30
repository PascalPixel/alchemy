#include "TYPES.H"
#include "TBS_EDITION.H"
#include "IWRAM_CALL.H"

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

extern u8 *gWindowWork;
void UiWindow_UpdateInterpolatedGeometry(void *window, s32 save_position);
void UiWindow_EraseBorderRect(s32 x, s32 y, u32 width, u32 height);
void UiWork_DrawByAttributes(void *arg0);

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define WINDOW_WIDTH(window) FIELD(window, u16, 0x08)
#define WINDOW_HEIGHT(window) FIELD(window, u16, 0x0a)
#define WINDOW_X(window) FIELD(window, u16, 0x0c)
#define WINDOW_Y(window) FIELD(window, u16, 0x0e)
#define WINDOW_FRAME(window) FIELD(window, s16, 0x18)
#define WINDOW_DURATION(window) FIELD(window, s16, 0x1a)
#define WINDOW_PREVIOUS_X(window) FIELD(window, u16, 0x1c)
#define WINDOW_PREVIOUS_Y(window) FIELD(window, u16, 0x1e)
#define WINDOW_PREVIOUS_WIDTH(window) FIELD(window, u16, 0x20)
#define WINDOW_PREVIOUS_HEIGHT(window) FIELD(window, u16, 0x22)

struct UiWindowInterpolationScratch {
    s32 scaled_part;
    s32 scaled_duration;
    s32 result;
};

void UiWindow_DrawFrame(s32, s32, s32, s32);

void UiWork_ProcessDirectWork(void)
{
    u8 *base = gWindowWork;
    struct Work *work = (struct Work *)(base + 0x500);
    s32 index = 0;
    u8 dirty;

loop:
    if (work->flags != 0) {
        if (work->frame != 0) {
            UiWindow_UpdateInterpolatedGeometry(work, 0);
            work->frame--;
        } else if (work->duration != 0) {
            UiWork_DrawByAttributes(work);
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
    if (index != WINDOW_COUNT)
        goto loop;
}

void UiWindow_UpdateInterpolatedGeometry(void *window, s32 save_position)
{
    struct UiWindowInterpolationScratch scratch;
    s32 frame;
    s32 duration;
    s32 remaining;
    s32 x;
    s32 y;
    s32 width;
    s32 height;

    frame = WINDOW_FRAME(window);
    duration = WINDOW_DURATION(window);
    remaining = duration - frame;
    scratch.scaled_part =
        (s32)((u32)(frame *WINDOW_WIDTH(window)) << 16);
    scratch.scaled_duration = (s32)((u32)duration << 17);
    scratch.result =
        Iwram_RatioMulQ14(
            scratch.scaled_duration, scratch.scaled_part);
    x = (scratch.result >> 16) + WINDOW_X(window);

    scratch.scaled_part =
        (s32)(((u32)remaining *WINDOW_WIDTH(window)) << 16);
    scratch.result =
        Iwram_RatioMulQ14(
            scratch.scaled_duration, scratch.scaled_part);
    width = scratch.result >> 15;

    scratch.scaled_part =
        (s32)((u32)(frame *WINDOW_HEIGHT(window)) << 16);
    scratch.scaled_duration = (s32)((u32)WINDOW_DURATION(window) << 17);
    scratch.result =
        Iwram_RatioMulQ14(
            scratch.scaled_duration, scratch.scaled_part);
    y = (scratch.result >> 16) + WINDOW_Y(window);

    scratch.scaled_part =
        (s32)(((u32)remaining *WINDOW_HEIGHT(window)) << 16);
    scratch.result =
        Iwram_RatioMulQ14(
            scratch.scaled_duration, scratch.scaled_part);
    height = scratch.result >> 15;

    UiWindow_DrawFrame(x, y, width, height);
    if (save_position != 0) {
        WINDOW_PREVIOUS_X(window) = x;
        WINDOW_PREVIOUS_Y(window) = y;
        WINDOW_PREVIOUS_WIDTH(window) = width;
        WINDOW_PREVIOUS_HEIGHT(window) = height;
    }
}
