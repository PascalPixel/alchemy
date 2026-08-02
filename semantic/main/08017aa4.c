#include "layout_guard.h"
#include "types.h"

typedef struct TextObject_08017aa4 {
    u8 padding00[0x16];
    u16 flags;
} TextObject_08017aa4;

typedef struct GlyphMetrics_08017aa4 {
    u16 width;
    u8 padding02[0x1e];
} GlyphMetrics_08017aa4;

typedef struct TextRuntime_08017aa4 {
    u8 padding0000[0xea8];
    u16 vertical_offset;
    u8 padding0eaa[2];
    u16 render_mode;
    u16 text_parameter;
    u16 scratch_text[512];
    u16 horizontal_offset;
    u16 scratch_write_index;
} TextRuntime_08017aa4;

LAYOUT_OFFSET_GUARD(
    TextObject08017aa4_Flags,
    TextObject_08017aa4,
    flags,
    0x16);
LAYOUT_SIZE_GUARD(
    GlyphMetrics08017aa4_Size,
    GlyphMetrics_08017aa4,
    0x20);
LAYOUT_OFFSET_GUARD(
    TextRuntime08017aa4_VerticalOffset,
    TextRuntime_08017aa4,
    vertical_offset,
    0xea8);
LAYOUT_OFFSET_GUARD(
    TextRuntime08017aa4_RenderMode,
    TextRuntime_08017aa4,
    render_mode,
    0xeac);
LAYOUT_OFFSET_GUARD(
    TextRuntime08017aa4_TextParameter,
    TextRuntime_08017aa4,
    text_parameter,
    0xeae);
LAYOUT_OFFSET_GUARD(
    TextRuntime08017aa4_ScratchText,
    TextRuntime_08017aa4,
    scratch_text,
    0xeb0);
LAYOUT_OFFSET_GUARD(
    TextRuntime08017aa4_HorizontalOffset,
    TextRuntime_08017aa4,
    horizontal_offset,
    0x12b0);
LAYOUT_OFFSET_GUARD(
    TextRuntime08017aa4_ScratchWriteIndex,
    TextRuntime_08017aa4,
    scratch_write_index,
    0x12b2);

extern TextRuntime_08017aa4 *Data_03001e8c;

void Func_080173ac(void);
s32 Func_08018cac(void *, u32, s32, s32, s32);

static s32 CanPackNextGlyph_08017aa4(
    u16 current,
    u16 next,
    const GlyphMetrics_08017aa4 *metrics)
{
    u32 combined_width;

    if (current <= 0x20 || next <= 0x20)
        return 0;

    combined_width =
        metrics[current - 0x20].width + metrics[next - 0x20].width;
    return (combined_width << 16) <= 0x000f0000;
}

/*
 * Interpret a zero-terminated field-text stream and emit its visible glyphs.
 * Control tokens update renderer state, advance over their operands, or start
 * a new fifteen-pixel line.  When packing is enabled, two narrow neighboring
 * glyphs are passed to the renderer as one encoded pair.
 */
void Func_08017aa4(
    const u16 *text,
    TextObject_08017aa4 *object,
    s32 x,
    s32 y)
{
    TextRuntime_08017aa4 *runtime = Data_03001e8c;
    const GlyphMetrics_08017aa4 *metrics =
        (const GlyphMetrics_08017aa4 *)0x08032224;
    s16 line_start = (s16)x;

    if (text == 0) {
        runtime->scratch_text[runtime->scratch_write_index] = 0;
        text = runtime->scratch_text;
        runtime->scratch_write_index =
            (runtime->scratch_write_index + 1) & 0x01ff;
    }

    for (;;) {
        u16 character = *text++;

        if (character > 0xff)
            character = 0x40;
        if (character == 0)
            return;

        if (character <= 0x1e) {
            switch (character) {
            case 3:
                x = line_start;
                y += 15;
                break;
            case 7:
                Func_080173ac();
                break;
            case 8:
                runtime->text_parameter = *text++;
                break;
            case 9:
                runtime->render_mode = *text++;
                break;
            case 10:
                runtime->vertical_offset = *text++;
                break;
            case 11:
            case 12:
            case 17:
            case 29:
                text++;
                break;
            case 14:
            case 15:
            case 28:
                text += 2;
                break;
            default:
                break;
            }
            continue;
        }

        if ((object->flags & 8) == 0 &&
            CanPackNextGlyph_08017aa4(character, *text, metrics)) {
            character |= *text++ << 8;
        }

        x += Func_08018cac(object, character, (s16)x, y, 0);
    }
}
