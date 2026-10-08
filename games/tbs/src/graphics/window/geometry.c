#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "tbs_edition.h"
#include "iwram_call.h"
#include "dma.h"
#include "window.h"

#define WINDOW_HALF(window, type, field) \
    (*(type *)((u8 *)(window) + (u32)&(((struct UiWindow *)0)->field)))

struct UiWindowInterpolationScratch {
    s32 scaled_part;
    s32 scaled_duration;
    s32 result;
};

void UiWindow_DrawFrame(s32 x, s32 y, u32 width, u32 height);

void UiWork_ProcessDirectWork(void)
{
    struct UiRenderWork *state = (struct UiRenderWork *)gWindowWork[0];
    struct UiWindow *work = state->windows;
    s32 index = 0;
    u8 dirty;

loop:
    if (work->flags != 0) {
        if ((s16)work->frame != 0) {
            UiWindow_UpdateInterpolatedGeometry(work, 0);
            work->frame = (s16)work->frame - 1;
        } else if (work->duration != 0) {
            UiWork_DrawByAttributes(work);
        }
    } else if (work->duration != 0) {
        if ((s16)work->frame != work->duration) {
            UiWindow_EraseBorderRect((s16)work->previous_x, (s16)work->previous_y,
                                     (s16)work->previous_width,
                                     (s16)work->previous_height);
            UiWindow_UpdateInterpolatedGeometry(work, 1);
            work->frame++;
            dirty = 1;
            state->dirty = dirty;
        } else {
            UiWindow_EraseBorderRect((s16)work->previous_x, (s16)work->previous_y,
                                     (s16)work->previous_width,
                                     (s16)work->previous_height);
            work->output.head = NULL;
            work->output.tail_link = NULL;
            work->width = 0;
            work->height = 0;
            work->x = 0;
            work->y = 0;
            work->unknown_10 = 0;
            work->effect = 0;
            work->state = 0;
            work->flags = 0;
            work->frame = 0;
            work->duration = 0;
            work->previous_x = 0;
            work->previous_y = 0;
            work->previous_width = 0;
            work->previous_height = 0;
            state->dirty = 1;
        }
    }
    index++;
    work++;
    if (index != WINDOW_COUNT)
        goto loop;
}

void UiWindow_UpdateInterpolatedGeometry(void *window, s32 save_position)
{
    /* FAKEMATCH: needed to match: the halfword window view keeps geometry reads before scratch stores; direct fields reorder them. */
    struct UiWindowInterpolationScratch scratch;
    s32 frame;
    s32 duration;
    s32 remaining;
    s32 x;
    s32 y;
    s32 width;
    s32 height;

    frame = WINDOW_HALF(window, s16, frame);
    duration = WINDOW_HALF(window, s16, duration);
    remaining = duration - frame;
    scratch.scaled_part =
        (frame *WINDOW_HALF(window, u16, width)) << 16;
    scratch.scaled_duration = duration << 17;
    scratch.result =
        Iwram_RatioMulQ14(
            scratch.scaled_duration, scratch.scaled_part);
    x = (scratch.result >> 16) + WINDOW_HALF(window, u16, x);

    scratch.scaled_part =
        (remaining *WINDOW_HALF(window, u16, width)) << 16;
    scratch.result =
        Iwram_RatioMulQ14(
            scratch.scaled_duration, scratch.scaled_part);
    width = scratch.result >> 15;

    scratch.scaled_part =
        (frame *WINDOW_HALF(window, u16, height)) << 16;
    scratch.scaled_duration = WINDOW_HALF(window, s16, duration) << 17;
    scratch.result =
        Iwram_RatioMulQ14(
            scratch.scaled_duration, scratch.scaled_part);
    y = (scratch.result >> 16) + WINDOW_HALF(window, u16, y);

    scratch.scaled_part =
        (remaining *WINDOW_HALF(window, u16, height)) << 16;
    scratch.result =
        Iwram_RatioMulQ14(
            scratch.scaled_duration, scratch.scaled_part);
    height = scratch.result >> 15;

    UiWindow_DrawFrame(x, y, width, height);
    if (save_position != 0) {
        WINDOW_HALF(window, u16, previous_x) = x;
        WINDOW_HALF(window, u16, previous_y) = y;
        WINDOW_HALF(window, u16, previous_width) = width;
        WINDOW_HALF(window, u16, previous_height) = height;
    }
}

