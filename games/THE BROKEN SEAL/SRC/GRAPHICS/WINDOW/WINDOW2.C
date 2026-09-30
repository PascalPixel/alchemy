#include "TYPES.H"
#include "TBS_EDITION.H"
#include "IWRAM_CALL.H"
#include "DMA.H"

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

void UiWindow_ClearTileAttributesInRect(s32 x, s32 y, u32 width, u32 height);
u16 *Memory_FillHalfwordsDma(u16 *destination, s32 value, s32 count);

void UiWindow_DrawFrame(s32 x, s32 y, u32 width, u32 height);

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
    u8 *base = gWindowWork;
    u16 *cursor = (u16 *)((y * 32 + x) * 2 + (u32)base);
    u32 row;

    if (width <= 1 || height <= 1 || width > 30 || height > 30)
        return;
    UiWindow_ClearTileAttributesInRect(x, y, width, height);
    if (base[RENDER_MODE_OFS] != 0)
        *cursor++ = 0xf01c;
    else
        *cursor++ = 0xf010;
    cursor = Memory_FillHalfwordsDma(cursor, 0xf011f011, width - 2);
    if (base[RENDER_MODE_OFS] != 0)
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
    if (base[RENDER_MODE_OFS] != 0)
        *cursor++ = 0xf81c;
    else
        *cursor++ = 0xf013;
    cursor = Memory_FillHalfwordsDma(cursor, 0xf014f014, width - 2);
    if (base[RENDER_MODE_OFS] != 0)
        *cursor = 0xfc1c;
    else
        *cursor = 0xf015;
    base[RENDER_DIRTY_OFS] = 1;
}

/* Fills a window's interior with the text canvas tiles, numbered down each
   column from tile 0x127, so glyphs drawn into the canvas show through.
   The full-width form covers the border columns too. */
void UiWindow_MapTextCanvasTiles(s32 x, s32 y, u32 width, u32 height, s32 full_width)
{
    u8 *base = gWindowWork;
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
    base[RENDER_DIRTY_OFS] = 1;
}
