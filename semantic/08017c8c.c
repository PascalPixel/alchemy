#include "types.h"

void Func_08018efc(void *, u32, s16, s32, s32);

/*
 * Render an encoded halfword text stream at the requested grid position.
 * Control tokens reset the row or consume their payload halfwords; ordinary
 * glyphs are emitted and advance the row except for the two non-advancing
 * glyph codes.
 */
void Func_08017c8c(
    const u16 *text,
    void *window,
    s16 initial_row,
    s32 column
) {
    u8 *engine;
    const u16 *cursor;
    u16 token;
    s16 row;

    engine = *(u8 **)0x03001E8C;
    row = initial_row;
    if (text == 0) {
        u16 *buffer = (u16 *)(engine + 0xEB0);
        u16 *write_index = (u16 *)(engine + 0x12B2);
        buffer[*write_index] = 0;
        *write_index = (*write_index + 1) & 0x1FF;
        text = buffer;
    }

    cursor = text;
    while ((token = *cursor++) != 0) {
        if (token <= 0x1E) {
            switch (token) {
            case 3:
                row = initial_row;
                column++;
                break;
            case 14:
            case 15:
            case 28:
                cursor += 2;
                break;
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 17:
            case 29:
                cursor++;
                break;
            default:
                break;
            }
        } else {
            Func_08018efc(window, token, row, column, 0);
            if ((u16)(token - 222) > 1) {
                row++;
            }
        }
    }
    engine[0xEA3] = 1;
}
