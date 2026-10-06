/* 080155d0: ARM, compiled C (push starts at r5: -fcall-used-r4); draft for the camelot_arm route (CAMELOT_ARM_SOURCES, K1).
 * alchemy diff --camelot-arm: listing 196, draft 197, common 114, differ 165 (stock agbcc_arm 164).
 * agbcc_arm control (library setting -O2 -mthumb-interwork -fomit-frame-pointer -fcall-used-r4):
 *   listing 196, draft 198, common 115, differ 164
 * agscc control (alchemy diff --arm): listing 196, draft 223, common 24, differ 371
 * Trials, differ as agbcc_arm / agscc --arm:
 *   previous draft (inline plot function, do-while rows): 314 / 357
 *   plot as a block with its own VRAM pointer; inlining had folded the base into the add,
 *     native holds it in a register hoisted before each row loop: 285 / 369
 *   packed 26-byte glyph struct (ARM pads it to 28 otherwise): 286 / 370
 *   glyph width into a temporary, ret set in both arms (native branches, no conditional store): 283 / 369
 *   glyph rows as a while loop that breaks at row 12 before loading the next row: 245 / 385
 *   font rows as for (row < 8 && y + row < 32): 226 / 383
 *   (px >> 3) * (width - 2), as native mla's operand order: 214 / 383
 *   rows, ret, width declared first and src, c1, c3, w in that order: spill slots follow
 *     pseudo order, now win 40, rows 36, ret 32, width 28, src 24, c1 20, c3 16, w 12 as native: 192 / 389
 *   next declared in the pass loop, after w: slot 8 as native: 180 / 387
 *   nibble as if (col & 1) b >>= 4; else b = *src++; interblock scheduling hoists the shift
 *     above the test, giving native's tst, lsr, ldreq order: 178 / 387
 *   glyph and wide pointers, then rows and width through them: 177 / 379
 *   wide glyph indexed by idx (one pool word for UiText_Glyphs, as native): 164 / 373
 *   row declared before pass: 164 / 371
 *   rejected: literal colour constants instead of c1, c3 221 (at 214); width read before rows
 *     217 (at 214) and 164 (at 164); width read before idx no change; UiSprite_CursorFrameTiles
 *     [0x800 + idx] 190; byte offset (ch - 32) * 32 for the wide glyph 169, though it gives
 *     native's sub r3, r1, #32 shape; byte offset idx * 26 for the narrow glyph too 169;
 *     a second row local in the pass loop 164 / 369 (no agbcc_arm change); code = ch - 222 once
 *     at the top, also in the font test, 174 / 361
 * Remaining: win is spilled and reloaded for its width (native reads r0 before the spill, idx
 * takes r0 here and ip in native); the narrow glyph's width load is scheduled before rows, so
 * the two arms do not share their last ldrh as native's do; native recomputes ch - 32 for the
 * wide glyph; x and the VRAM base swap r9 and sl; pass and row swap r1 and r2; colour constant
 * initialisation order. agscc ARM adds an APCS frame native lacks. */
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
    u16 *rows;
    s32 ret;
    u32 width;
    u32 b;
    u32 gw;
    s32 bits;
    u32 idx;
    u32 row;
    u32 code;
    u32 pass;
    u32 col;
    struct UiTextGlyph *glyph;
    struct UiTextWideGlyph *wide;

    idx = ch - 32;
    width = win->width;
    if (idx > 111)
        return 1;
    if (ch < 256 || (font != 0 && ch - 222 < 2)) {
        if (font != 0) {
            u8 *src = font + ch * 32;
            u32 c1 = 0x1111;
            u32 c3 = 0x3333;
            s32 w = (UiSprite_CursorFrameTiles + 0x800)[idx];
            u32 px;
            u32 sub;
            u32 color;
            u32 ty;

            for (row = 0; row < 8 && y + row < 32; row++) {
                ty = (y + row) >> 3;
                sub = ((y + row) & 7) * 2;
                for (col = 0; col < 8; col++) {
                    if (col & 1)
                        b >>= 4;
                    else
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
        glyph = &UiText_Glyphs[idx];
        rows = glyph->rows;
        gw = glyph->width;
    } else {
        code = ch - 222;
        wide = &((struct UiTextWideGlyph *)UiText_Glyphs)[idx];
        rows = wide->rows;
        gw = wide->width;
    }
    if (code < 2) {
        x -= 3;
        y -= 2;
        ret = 0;
    } else {
        ret = gw;
    }
    for (pass = 0; pass < 4; pass++) {
        u16 *next;
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
