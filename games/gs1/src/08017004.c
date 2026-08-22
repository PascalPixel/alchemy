#include "types.h"

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

struct InterpolationScratch_08017004 {
    s32 scaled_part;
    s32 scaled_duration;
    s32 result;
};

typedef s32 (*RamScaleFunction_08017004)(s32, s32);

void Func_080170f8(s32, s32, s32, s32);

void Func_08017004(void *window, s32 save_position)
{
    struct InterpolationScratch_08017004 scratch;
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
        (s32)((u32)(frame * WINDOW_WIDTH(window)) << 16);
    scratch.scaled_duration = (s32)((u32)duration << 17);
    scratch.result =
        ((RamScaleFunction_08017004)0x0300013c)(
            scratch.scaled_duration, scratch.scaled_part);
    x = (scratch.result >> 16) + WINDOW_X(window);

    scratch.scaled_part =
        (s32)(((u32)remaining * WINDOW_WIDTH(window)) << 16);
    scratch.result =
        ((RamScaleFunction_08017004)0x0300013c)(
            scratch.scaled_duration, scratch.scaled_part);
    width = scratch.result >> 15;

    scratch.scaled_part =
        (s32)((u32)(frame * WINDOW_HEIGHT(window)) << 16);
    scratch.scaled_duration = (s32)((u32)WINDOW_DURATION(window) << 17);
    scratch.result =
        ((RamScaleFunction_08017004)0x0300013c)(
            scratch.scaled_duration, scratch.scaled_part);
    y = (scratch.result >> 16) + WINDOW_Y(window);

    scratch.scaled_part =
        (s32)(((u32)remaining * WINDOW_HEIGHT(window)) << 16);
    scratch.result =
        ((RamScaleFunction_08017004)0x0300013c)(
            scratch.scaled_duration, scratch.scaled_part);
    height = scratch.result >> 15;

    Func_080170f8(x, y, width, height);
    if (save_position != 0) {
        WINDOW_PREVIOUS_X(window) = x;
        WINDOW_PREVIOUS_Y(window) = y;
        WINDOW_PREVIOUS_WIDTH(window) = width;
        WINDOW_PREVIOUS_HEIGHT(window) = height;
    }
}
