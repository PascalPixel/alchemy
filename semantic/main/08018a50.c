#include "layout_guard.h"
#include "types.h"

enum {
    TEXT_RING_SIZE_08018A50 = 512,
    TEXT_COLUMN_LIMIT_08018A50 = 16,
    TEXT_SEGMENT_LIMIT_08018A50 = 4,
};

typedef struct GlyphMetrics_08018a50 {
    u16 width;
    u8 padding02[0x1e];
} GlyphMetrics_08018a50;

typedef struct TextRuntime_08018a50 {
    u8 padding000[0xea4];
    u8 narrow_display;
    u8 paddingea5[7];
    u16 render_mode;
    u8 paddingeae[2];
    u16 scratch_text[TEXT_RING_SIZE_08018A50];
} TextRuntime_08018a50;

LAYOUT_SIZE_GUARD(
    GlyphMetrics08018a50_Size,
    GlyphMetrics_08018a50,
    0x20);
LAYOUT_OFFSET_GUARD(
    TextRuntime08018a50_NarrowDisplay,
    TextRuntime_08018a50,
    narrow_display,
    0xea4);
LAYOUT_OFFSET_GUARD(
    TextRuntime08018a50_RenderMode,
    TextRuntime_08018a50,
    render_mode,
    0xeac);
LAYOUT_OFFSET_GUARD(
    TextRuntime08018a50_ScratchText,
    TextRuntime_08018a50,
    scratch_text,
    0xeb0);

extern TextRuntime_08018a50 *Data_03001e8c;

s32 Func_080022ec(s32, s32);

static void FinishSegment_08018a50(
    s16 line_widths[TEXT_SEGMENT_LIMIT_08018A50],
    s16 glyph_counts[TEXT_SEGMENT_LIMIT_08018A50],
    s32 segment,
    s32 width,
    s32 glyph_count)
{
    glyph_counts[segment] = glyph_count + 1;
    line_widths[segment] = width;
}

/* Measure the multi-column scratch-text stream and optional segment spacing. */
void Func_08018a50(
    s32 start,
    u32 *width,
    u32 *height,
    s16 *spacing)
{
    TextRuntime_08018a50 *runtime = Data_03001e8c;
    const GlyphMetrics_08018a50 *metrics =
        (const GlyphMetrics_08018a50 *)0x08032224;
    s16 line_widths[TEXT_SEGMENT_LIMIT_08018A50];
    s16 glyph_counts[TEXT_SEGMENT_LIMIT_08018A50];
    u32 column_heights[TEXT_COLUMN_LIMIT_08018A50];
    u32 max_width = 0;
    s32 segment = 0;
    s32 current_width = 0;
    s32 glyph_count = 0;
    s32 column = 0;
    s32 cursor = start;
    s32 index;

    for (index = 0; index < TEXT_COLUMN_LIMIT_08018A50; index++)
        column_heights[index] = 0x0f;

    for (;;) {
        u16 token = runtime->scratch_text[cursor];

        cursor = (cursor + 1) & 0x01ff;
        if (token > 0x1f) {
            if (token == 0x20) {
                current_width += 5;
                glyph_count++;
            } else {
                s32 glyph_width = metrics[token - 0x20].width;

                if (runtime->render_mode == 1 ||
                    runtime->render_mode == 5) {
                    glyph_width++;
                }
                current_width += glyph_width;
            }
            continue;
        }

        switch (token) {
        case 0:
            FinishSegment_08018a50(
                line_widths,
                glyph_counts,
                segment,
                current_width,
                glyph_count);
            if (column == 0 && max_width < (u32)current_width)
                max_width = current_width;
            column++;
            goto finished;

        case 1:
            FinishSegment_08018a50(
                line_widths,
                glyph_counts,
                segment,
                current_width,
                glyph_count);
            if (column == 0 && max_width < (u32)current_width)
                max_width = current_width;
            column++;
            continue;

        case 3:
            FinishSegment_08018a50(
                line_widths,
                glyph_counts,
                segment,
                current_width,
                glyph_count);
            if (column == 0 && max_width < (u32)current_width)
                max_width = current_width;
            if (segment <= 2)
                segment++;
            current_width = 0;
            glyph_count = 0;
            column_heights[column] += 0x0f;
            continue;

        case 14:
        case 28:
            cursor = (cursor + 1) & 0x01ff;
            /* Fall through: these commands carry two operands. */
        case 8:
        case 10:
        case 15:
        case 17:
            cursor = (cursor + 1) & 0x01ff;
            continue;

        case 9:
            runtime->render_mode = runtime->scratch_text[cursor];
            cursor = (cursor + 1) & 0x01ff;
            continue;

        default:
            continue;
        }
    }

finished:
    if (runtime->narrow_display != 0)
        max_width += 2;

    *height = column_heights[0];
    for (index = 1; index < column; index++) {
        if (*height < column_heights[index])
            *height = column_heights[index];
    }
    *width = max_width;

    if (spacing != 0) {
        s32 available_width =
            (((max_width + 0x13) >> 3) * 8) - 0x10;

        for (index = 0; index <= segment; index++) {
            if (glyph_counts[index] <= 1) {
                spacing[index] = 0;
            } else {
                s32 available =
                    available_width - line_widths[index] - 4;

                if (available < 0)
                    available = 0;
                spacing[index] = (s16)Func_080022ec(
                    available << 8,
                    glyph_counts[index] - 1);
            }
        }
    }
}
