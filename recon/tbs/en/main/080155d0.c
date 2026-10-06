/* 080155d0: ARM, compiled C (push starts at r5: -fcall-used-r4). Analysis-only ARM study; no route admitted, no credit.
 * agbcc_arm control (library setting -O2 -mthumb-interwork -fomit-frame-pointer -fcall-used-r4):
 *   listing 196, draft 198, common 90, differ 214
 * agscc control (alchemy diff --arm): listing 196, draft 221, common 17, differ 383
 * Trials, differ as agbcc_arm / agscc --arm:
 *   previous draft (inline plot function, do-while rows): 314 / 357
 *   plot as a block with its own VRAM pointer; inlining had folded the base into the add,
 *     native holds it in a register hoisted before each row loop: 285 / 369
 *   packed 26-byte glyph struct (ARM pads it to 28 otherwise): 286 / 370
 *   glyph width into a temporary, ret set in both arms (native branches, no conditional store): 283 / 369
 *   glyph rows as a while loop that breaks at row 12 before loading the next row: 245 / 385
 *   font rows as for (row < 8 && y + row < 32): 226 / 383
 *   (px >> 3) * (width - 2), as native mla's operand order: 214 / 383
 *   rejected: literal colour constants instead of two locals 221; width read before rows 217
 * Remaining: win is spilled and reloaded for its width (native reads r0 before the spill), frame
 * 48 against 44, the wide glyph offset is folded into the symbol (native recomputes ch - 32),
 * the narrow path loads UiText_Glyphs + 2 (native keeps the symbol and adds 2), spill order of
 * the colour constants, and register choice throughout. agscc ARM adds an APCS frame native lacks. */
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
} __attribute__((packed, aligned(2)));

struct UiTextWideGlyph {
    u16 width;
    u16 unk2[3];
    u16 rows[12];
} __attribute__((packed, aligned(2)));

extern u8 UiSprite_CursorFrameTiles[];
extern struct UiTextGlyph UiText_Glyphs[];

#define UiText_PlotPixel(width, px, ty, sub, color) \
{ \
    u8 *vram = (u8 *)0x06002500; \
    u16 *dst = (u16 *)((u32)(vram + ((px >> 3) * (width - 2) + ty) * 32) | ((sub | ((px & 7) >> 2)) << 1)); \
    u32 mask = 15 << ((px & 3) * 4); \
 \
    *dst = (*dst & ~mask) | (mask & color); \
}

s32 UiText_DrawGlyphArm(struct UiTextWindow *win, u32 ch, u32 x, u32 y, u8 *font)
{
    u32 b;
    u32 width;
    u32 gw;
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

            for (row = 0; row < 8 && y + row < 32; row++) {
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
            }
            return w;
        }
        code = ch - 222;
        rows = UiText_Glyphs[idx].rows;
        gw = UiText_Glyphs[idx].width;
    } else {
        code = ch - 222;
        rows = ((struct UiTextWideGlyph *)UiText_Glyphs)[ch - 32].rows;
        gw = ((struct UiTextWideGlyph *)UiText_Glyphs)[ch - 32].width;
    }
    if (code < 2) {
        x -= 3;
        y -= 2;
        ret = 0;
    } else {
        ret = gw;
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
        while (y + row < 32) {
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
            if (++row >= 12)
                break;
            bits = *next++ << 16;
        }
    }
    return ret;
}
