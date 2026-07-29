typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

void Func_08018850(s32, s32 *, s32 *, s32);
void Func_08018a50(s32, s32 *, s32 *, s32);

/*
 * Measure a text box when requested, convert its pixel dimensions to tile
 * dimensions, and clamp its origin so the box remains on the 30x20 screen.
 * A narrow-display mode reserves one extra column and adds two columns to the
 * measured width.
 */
void Func_0801868c(
    s32 text,
    s32 *column,
    s32 *row,
    s32 *width,
    s32 *height,
    s32 measurement_context,
    s32 flags
) {
    void *engine;
    s32 screen_columns;
    s32 box_width;
    s32 box_height;
    s32 adjusted;

    engine = *(void **)0x03001E8C;
    screen_columns = 30;
    box_width = *width;
    box_height = *height;

    if ((flags & 2) == 0) {
        if (flags & 1) {
            Func_08018a50(text, width, height, measurement_context);
        } else {
            Func_08018850(text, width, height, measurement_context);
        }
    }

    if (*width == 0 && *height == 0) {
        return;
    }

    if ((flags & 2) == 0) {
        *width = ((u32)*width + 19) >> 3;
        *height = ((u32)*height + 15) >> 3;
        if (*((u8 *)engine + 0xEA4) != 0) {
            *width += 2;
            screen_columns = 29;
        }
    }

    box_width = *width;
    box_height = *height;

    adjusted = *column + box_width;
    if (adjusted > screen_columns) {
        *column -= adjusted - screen_columns;
        if (*column < 0) {
            *column = 0;
        }
    }

    adjusted = *row + box_height;
    if (adjusted > 20) {
        *row -= adjusted - 20;
        if (*row < 0) {
            *row = 0;
        }
    }

    if (*column < 0) {
        *column = 0;
    }
    if (*row < 0) {
        *row = 0;
    }
    if ((u32)*column > (u32)(screen_columns - box_width)) {
        *column = screen_columns - box_width;
    }
    if ((u32)*row > (u32)(20 - box_height)) {
        *row = 20 - box_height;
    }
}
