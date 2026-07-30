typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

extern u8 *Data_03001e8c;
extern const u16 Data_08032224[];

s32 Func_080022ec(s32 value, s32 divisor);

s32 Func_08018850(s32 start, s32 *width, s32 *height, s16 *spacing)
{
    u8 *text_state = Data_03001e8c;
    s16 line_widths[4];
    s16 word_counts[4];
    s32 current_width = 0;
    s32 words = 0;
    s32 line = 0;
    s32 total_height = 15;
    s32 max_width = 0;
    s32 result = 0;
    s32 cursor = start;

    for (;;) {
        u16 token = *(u16 *)(text_state + 0xeb0 + cursor * 2);
        cursor = (cursor + 1) & 0x1ff;

        if (token > 0x1f) {
            if (token == 0x20) {
                current_width += 5;
                words++;
            } else {
                s32 glyph_width = Data_08032224[(token - 0x20) * 16];
                u16 mode = *(u16 *)(text_state + 0xeac);

                if (mode == 1 || mode == 5)
                    glyph_width++;
                current_width += glyph_width;
            }
            continue;
        }

        switch (token) {
        case 3:
            word_counts[line] = words + 1;
            line_widths[line] = current_width;
            if (max_width < current_width)
                max_width = current_width;
            if (line <= 2)
                line++;
            words = 0;
            current_width = 0;
            total_height += 15;
            continue;

        case 14:
        case 28:
            cursor = (cursor + 1) & 0x1ff;
            /* fall through */
        case 8:
        case 10:
        case 15:
        case 17:
            cursor = (cursor + 1) & 0x1ff;
            continue;

        case 9:
            *(u16 *)(text_state + 0xeac) =
                *(u16 *)(text_state + 0xeb0 + cursor * 2);
            cursor = (cursor + 1) & 0x1ff;
            continue;

        default:
            goto finished;
        }
    }

finished:
    word_counts[line] = words + 1;
    line_widths[line] = current_width;
    if (max_width < current_width)
        max_width = current_width;
    if (text_state[0xea4] != 0)
        max_width += 2;

    *width = max_width;
    *height = total_height;

    if (spacing != 0) {
        s32 i;

        for (i = 0; i <= line; i++) {
            if (word_counts[i] <= 1) {
                spacing[i] = 0;
            } else {
                s32 available =
                    ((((u32)(max_width + 0x13) >> 3) * 8) - 0x10) -
                    line_widths[i] - 4;

                if (available < 0)
                    available = 0;
                result = Func_080022ec(
                    available << 8, word_counts[i] - 1);
                if ((u32)result > 0xc00)
                    result = 0x200;
                spacing[i] = result;
            }
        }
    }
    return result;
}
