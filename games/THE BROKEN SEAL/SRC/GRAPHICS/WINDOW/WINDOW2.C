#include "TYPES.H"
#include "TBS_EDITION.H"
#include "IWRAM_CALL.H"
#include "DMA.H"
#include "WINDOW.H"

void UiWindow_UpdateInterpolatedGeometry(void *window, s32 save_position);
void UiWindow_EraseBorderRect(s32 x, s32 y, u32 width, u32 height);
void UiWork_DrawByAttributes(struct UiWindow *window);

/* The opaque interpolation boundary reads the window as scalar halfwords. */
#define WINDOW_HALF(window, type, field) \
    (*(type *)((u8 *)(window) + (u32)&(((struct UiWindow *)0)->field)))

struct UiWindowInterpolationScratch {
    s32 scaled_part;
    s32 scaled_duration;
    s32 result;
};

void UiWindow_ClearTileAttributesInRect(s32 x, s32 y, u32 width, u32 height);
u16 *Memory_FillHalfwordsDma(u16 *destination, s32 value, s32 count);

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
            work->unknown_00 = 0;
            work->unknown_04 = 0;
            work->width = 0;
            work->height = 0;
            work->x = 0;
            work->y = 0;
            work->unknown_10 = 0;
            work->unknown_12 = 0;
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
    /* FAKEMATCH: the existing opaque halfword window view keeps geometry
       reads ahead of scratch stores; direct typed fields change their
       measured native scheduling at the same 192-byte extent. */
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
        (s32)((u32)(frame *WINDOW_HALF(window, u16, width)) << 16);
    scratch.scaled_duration = (s32)((u32)duration << 17);
    scratch.result =
        Iwram_RatioMulQ14(
            scratch.scaled_duration, scratch.scaled_part);
    x = (scratch.result >> 16) + WINDOW_HALF(window, u16, x);

    scratch.scaled_part =
        (s32)(((u32)remaining *WINDOW_HALF(window, u16, width)) << 16);
    scratch.result =
        Iwram_RatioMulQ14(
            scratch.scaled_duration, scratch.scaled_part);
    width = scratch.result >> 15;

    scratch.scaled_part =
        (s32)((u32)(frame *WINDOW_HALF(window, u16, height)) << 16);
    scratch.scaled_duration = (s32)((u32)WINDOW_HALF(window, s16, duration) << 17);
    scratch.result =
        Iwram_RatioMulQ14(
            scratch.scaled_duration, scratch.scaled_part);
    y = (scratch.result >> 16) + WINDOW_HALF(window, u16, y);

    scratch.scaled_part =
        (s32)(((u32)remaining *WINDOW_HALF(window, u16, height)) << 16);
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

u16 *Memory_FillHalfwordsDma(u16 *destination, s32 value, s32 count)
{
    volatile u16 fill;
    if (count > 0) {
        fill = value;
        Dma_Set(&fill, destination, count | 0x81000000, (volatile u32 *)0x040000d4);
        destination += count;
    }
    return destination;
}

/* Draws a window frame into the text canvas: corners, edges and a blank
   interior. The alternate frame style (byte RENDER_MODE_OFS) uses the flipped corner
   tiles of the second border set. */
void UiWindow_DrawFrame(s32 x, s32 y, u32 width, u32 height)
{
    u8 *base = gWindowWork[0];
    u16 *cursor = (u16 *)((y * 32 + x) * 2 + (u32)base);
    u32 row;

    if (width <= 1 || height <= 1 || width > 30 || height > 30)
        return;
    UiWindow_ClearTileAttributesInRect(x, y, width, height);
    if (((struct UiRenderWork *)base)->mode != 0)
        *cursor++ = 0xf01c;
    else
        *cursor++ = 0xf010;
    cursor = Memory_FillHalfwordsDma(cursor, 0xf011f011, width - 2);
    if (((struct UiRenderWork *)base)->mode != 0)
        *cursor++ = 0xf41c;
    else
        *cursor++ = 0xf012;
    cursor += 32 - width;
    for (row = 1; row < height - 1; row++) {
        *cursor++ = 0xf016;
        if (width != 2)
            cursor = Memory_FillHalfwordsDma(cursor, 0xf020f020, width - 2);
        *cursor++ = 0xf017;
        cursor += 32 - width;
    }
    if (((struct UiRenderWork *)base)->mode != 0)
        *cursor++ = 0xf81c;
    else
        *cursor++ = 0xf013;
    cursor = Memory_FillHalfwordsDma(cursor, 0xf014f014, width - 2);
    if (((struct UiRenderWork *)base)->mode != 0)
        *cursor = 0xfc1c;
    else
        *cursor = 0xf015;
    ((struct UiRenderWork *)base)->dirty = 1;
}

/* Fills a window's interior with the text canvas tiles, numbered down each
   column from tile 0x127, so glyphs drawn into the canvas show through.
   The full-width form covers the border columns too. */
void UiWindow_MapTextCanvasTiles(s32 x, s32 y, u32 width, u32 height, s32 full_width)
{
    u8 *base = gWindowWork[0];
    u16 *cursor = (u16 *)((y * 32 + x) * 2 + (u32)base);
    u32 row;
    u32 col;

    if (width <= 1 || height <= 1 || width > 30 || height > 30)
        return;
    cursor += 32;
    if (full_width == 0) {
        for (row = 1; row < height - 1; row++) {
            cursor++;
            for (col = 1; col < width - 1; col++)
                *cursor++ = ((0x127 + row + (col - 1) * (height - 2)) & 0xfff) | 0xf000;
            cursor++;
            cursor += 32 - width;
        }
    } else {
        for (row = 1; row < height - 1; row++) {
            for (col = 0; col < width; col++)
                *cursor++ = ((0x127 + row + col * (height - 2)) & 0xfff) | 0xf000;
            cursor += 32 - width;
        }
    }
    ((struct UiRenderWork *)base)->dirty = 1;
}
