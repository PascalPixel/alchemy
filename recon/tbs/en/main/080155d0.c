/* 080155d0: ARM, compiled C (push starts at r5: -fcall-used-r4). Score in ARM mode: listing 198, draft 206, differ 366 */
#include "TYPES.H"

struct UiTextWindow {
    u8 unk0[10];
    u16 width;
    u8 unkC[10];
    u16 flags;
};

struct UiTextGlyph {
    u16 width;
    u16 rows[12];
};

struct UiTextWideGlyph {
    u16 width;
    u16 unk2[3];
    u16 rows[12];
};

extern u8 UiSprite_CursorFrameTiles[];
extern struct UiTextGlyph UiText_Glyphs[];

static __inline__ void UiText_PlotPixel(u32 width, u32 px, u32 ty, u32 sub, u32 color)
{
    u16 *dst = (u16 *)((0x06002500 + ((width - 2) * (px >> 3) + ty) * 32) | ((sub | ((px & 7) >> 2)) << 1));
    u32 mask = 15 << ((px & 3) * 4);

    *dst = (*dst & ~mask) | (mask & color);
}

s32 UiText_DrawGlyphArm(struct UiTextWindow *win, u32 ch, u32 x, u32 y, u8 *font)
{
    u32 b;
    u32 width;
    u32 nrow;
    s32 bits;
    u16 *rows;
    u32 idx;
    u32 pass;
    u32 code;
    u32 row;
    s32 ret;
    u16 *next;
    u32 col;

    idx = ch - 32;
    width = win->width;
    if (idx > 111)
        return 1;
    if (ch < 256 || (font != 0 && ch - 222 < 2)) {
        if (font != 0) {
            u32 c3 = 0x3333;
            u32 c1 = 0x1111;
            u8 *src = font + ch * 32;
            s32 w = (UiSprite_CursorFrameTiles + 0x800)[idx];
            u32 px;
            u32 sub;
            u32 color;
            u32 ty;

            row = 0;
            if (y < 32) {
                do {
                    nrow = row + 1;
                    ty = (y + row) >> 3;
                    sub = ((y + row) & 7) * 2;
                    for (col = 0; col < 8; col++) {
                        b >>= 4;
                        if (!(col & 1))
                            b = *src++;
                        if ((b & 13) == 1) {
                            color = (b & 15) == 1 ? c1 : c3;
                            px = x + col;
                            if (px >> 3 < 30)
                                UiText_PlotPixel(width, px, ty, sub, color);
                        }
                    }
                } while ((row = nrow) < 8 && y + row < 32);
            }
            return w;
        }
        code = ch - 222;
        rows = UiText_Glyphs[idx].rows;
        ret = UiText_Glyphs[idx].width;
    } else {
        code = ch - 222;
        rows = ((struct UiTextWideGlyph *)UiText_Glyphs)[ch - 32].rows;
        ret = ((struct UiTextWideGlyph *)UiText_Glyphs)[ch - 32].width;
    }
    if (code < 2) {
        x -= 3;
        y -= 2;
        ret = 0;
    }
    for (pass = 0; pass < 4; pass++) {
        u32 ty;
        u32 color;
        u32 px;
        u32 sub;

        color = pass == 3 ? 0x1111 : 0x5555;
        if (win->flags & 64) {
            if (pass < 2) {
                if (pass == 1)
                    x += 2;
                else
                    x--;
            } else if (pass == 2) {
                x--;
                y++;
            } else {
                y--;
            }
        }
        next = rows + 1;
        bits = rows[0] << 16;
        row = 0;
        if (y < 32) {
            do {
                nrow = row + 1;
                ty = (y + row) >> 3;
                sub = ((y + row) & 7) * 2;
                for (col = 0; col < 14; col++) {
                    if (bits < 0) {
                        px = x + col;
                        UiText_PlotPixel(width, px, ty, sub, color);
                    }
                    if ((bits <<= 1) == 0)
                        break;
                }
                if ((row = nrow) >= 12)
                    break;
                bits = *next++ << 16;
            } while (y + row < 32);
        }
    }
    return ret;
}
