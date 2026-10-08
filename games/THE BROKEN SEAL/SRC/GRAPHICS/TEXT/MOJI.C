#include "TYPES.H"

struct UiTextWindow {
    u8 unk0[10];
    u16 width;
    u8 unkC[10];
    u16 flags;
};

extern const u8 UiText_GlyphWidths[];
extern u8 UiText_Glyphs[];

/* FAKEMATCH: px & 7 through an asm: stock charges its anti dependence on the mla, which reads r3, the multiply's 16 cycles, tying it with the VRAM add; an asm's links cost 1, so the add and its orr follow the mask shift as native (8 to 0; European 8 to 0) */
#define UiText_Dot(px) ({ u32 dot_; asm("and %0, %1, #7" : "=r"(dot_) : "r"(px)); dot_; })

#define UiText_PlotPixel(width, px, py, color) \
{ \
    u32 ty = (py) >> 3; \
    u8 *vram = (u8 *)0x06002500; \
    u32 sub = ((py) & 7) * 2; \
    u16 *dst = (u16 *)((u32)(vram + ((px >> 3) * (width - 2) + ty) * 32) | ((sub | (UiText_Dot(px) >> 2)) << 1)); \
    u32 mask = 15 << ((px & 3) * 4); \
 \
    *dst = (*dst & ~mask) | (mask & color); \
}

s32 UiText_DrawGlyphArm(struct UiTextWindow *win, u32 ch, u32 x0, u32 y0, u8 *font)
{
    u32 x;
    u32 y;
    u16 *rows;
    s32 ret;
    u32 width;
    u32 b;
    u32 gw;
    s32 bits;
    u32 row;
    u32 pass;
    /* FAKEMATCH: col in lr, also carrying ch - 222 from each arm to the join: GCSE's copy of ch - 222 in block 3 then prefers lr, the wide arm's copy vanishes and the narrow arm computes it first (15 to 0; as a variable of its own, 15; the European branch has no code and does not change) */
    register u32 col asm("lr");
    /* FAKEMATCH: ofs in r3: expand_preferences hands it r0 (w is returned in r0, and bits dies in wp[bits] and in bits * 26); in r0 the arms take r3 for their reloads and every round-robin reload register after them shifts (58 to 0; European 28 to 0) */
    register u32 ofs asm("r3");

#if defined(TBS_EDITION_EN)
    /* FAKEMATCH: y's copy through an asm: stock charges its anti dependence on the push the push's 5 cycles, as sub sp's true one, and the higher LUID then puts it below sub sp; an asm's link costs 1 (2 to 0) */
    asm("mov %0, %1" : "=r"(y) : "r"(y0));
    /* FAKEMATCH: x's copy through an asm: compiled, it is ready at T-2 with priority 5 from the push and takes the branch's slot (6 to 0) */
    asm("mov %0, %1" : "=r"(x) : "r"(x0));
    bits = ch - 32;
#else
    /* FAKEMATCH: x's copy through an asm, as English's (2 to 0) */
    asm("mov %0, %1" : "=r"(x) : "r"(x0));
    /* FAKEMATCH: y's copy through an asm, as English's (4 to 0) */
    asm("mov %0, %1" : "=r"(y) : "r"(y0));
    bits = ch;
#endif
    width = win->width;
    /* FAKEMATCH: the width store sits between the cmp and the branch: this asm's memory clobber takes the store's dependence at cost 1, where stock charges the branch 2 (6 to 0; European 4 to 0) */
    asm("" : "=r"(b) : "0"(b) : "memory");
#if defined(TBS_EDITION_EN)
    if ((u32)bits > 111)
        return 1;
    if (ch < 256 || (font != 0 && ch - 222 < 2)) {
#else
    if ((u32)bits < 256 || font != 0) {
#endif
        if (font != 0) {
#if defined(TBS_EDITION_EN)
            u8 *src = font + ch * 32;
#else
            u8 *src = font + bits * 32;
#endif
            u32 c1;
            u32 c3;
            /* FAKEMATCH: the width table through a local copy: indexed directly, the y-bound cmp swaps with the w store in en, de, es, fr and it (2 to 0) */
            const u8 *wp;
#if !defined(TBS_EDITION_EN)
            /* FAKEMATCH: the index kept apart from the table base, the uncombined form the bytes show (sub r3, ip, #32; ldrb r2, [r2, r3]); wp[bits - 32] folds the 32 into the load and 52 lines differ in de, es, fr and it */
            u32 idx;
#endif
            s32 w;
            u32 px;
            u32 color;

#if defined(TBS_EDITION_EN)
            /* FAKEMATCH: c1's mov through an asm: stock gives c1's chain priority 2 through its anti dependence on the src store, which puts the c3 constant and the table load before it (8 to 0) */
            asm("mov %0, #4352" : "=r"(c1));
            c1 += 17;
            c3 = 0x3333;
            /* FAKEMATCH: rewrites y and starts row: the cmp of y then follows this asm, and row = 0 no longer anti-depends on the c3 store (cost 2), so row = 0, the w load and the cmp take the native order (30 to 0) */
            asm("" : "=r"(y), "=r"(row) : "0"(y));
            wp = UiText_GlyphWidths;
            w = wp[bits];
            row = 0;
            /* FAKEMATCH: reads row and, through memory, w's store, so the store sits between the cmp and the branch and row = 0 waits for the w load (4 to 0) */
            asm("" : "=r"(b) : "0"(b), "r"(row) : "memory");
#else
            c1 = 0x1111;
            /* FAKEMATCH: c3's mov through an asm: stock charges its anti dependence on the src store (r2) 2, which puts c1's add below the src store and c3's mov below c1's store; an asm's links cost 1 (4 to 0) */
            asm("mov %0, #13056" : "=r"(c3));
            c3 += 51;
            /* FAKEMATCH: a barrier after the colour constants: native's row = 0, table load and index follow every colour store; without it stock moves the y compare up among them and sched1 swaps b and row + 1 between r5 and r4 (38 to 0) */
            asm volatile("");
            row = 0;
            wp = UiText_GlyphWidths;
            idx = bits - 32;
            w = wp[idx];
            /* FAKEMATCH: w's store sits between the cmp and the branch: this asm's memory clobber takes the store's dependence at cost 1, where stock charges the branch 2 (6 to 0) */
            asm("" : "=r"(b) : "0"(b) : "memory");
#endif
            for (; row < 8 && y + row < 32; row++) {
                for (col = 0; col < 8; col++) {
                    if (col & 1)
                        b >>= 4;
                    else
                        b = *src++;
                    if ((b & 13) == 1) {
                        color = (b & 15) == 1 ? c1 : c3;
                        px = x + col;
                        if (px >> 3 < 30)
                            /* FAKEMATCH: px & 7 through UiText_Dot's asm, measured there */
                            UiText_PlotPixel(width, px, y + row, color);
                    }
                }
            }
            return w;
        }
#if defined(TBS_EDITION_EN)
        col = ch - 222;
        ofs = bits * 26;
        rows = (u16 *)(UiText_Glyphs + ofs);
        rows++;
    } else {
        col = ch - 222;
        ofs = (ch - 32) * 32;
        rows = (u16 *)(UiText_Glyphs + ofs);
        rows += 4;
    }
    gw = *(u16 *)(UiText_Glyphs + ofs);
    if (col < 2) {
        x -= 3;
        y -= 2;
        ret = 0;
        /* FAKEMATCH: a barrier at the end of the block lets the ret store sit next to the branch (stock charges its anti dependence on the jump 2 cycles, so sub r7 took that slot) (2 to 0) */
        asm volatile("");
    } else {
        ret = gw;
    }
#else
        ofs = (bits - 32) * 26;
        rows = (u16 *)(UiText_Glyphs + ofs);
        rows++;
        /* FAKEMATCH: the rows store comes before the glyph width load: without this barrier stock hoists the load over the rows add and store, and the arms no longer share their ret store (39 to 0) */
        asm volatile("" : : : "memory");
        ret = *(u16 *)(UiText_Glyphs + ofs);
    } else {
        ofs = (bits - 32) * 32;
        rows = (u16 *)(UiText_Glyphs + ofs);
        rows += 4;
        ret = *(u16 *)(UiText_Glyphs + ofs);
    }
#endif
    for (pass = 0; pass < 4; pass++) {
        u16 *next;
        u32 color;
        u32 px;

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
        row = 0;
        next = rows + 1;
        bits = rows[0];
        bits <<= 16;
        while (y + row < 32) {
            for (col = 0; col < 14; col++) {
                if (bits < 0) {
                    px = x + col;
                    /* FAKEMATCH: px & 7 through UiText_Dot's asm, measured there */
                    UiText_PlotPixel(width, px, y + row, color);
                }
                if ((bits <<= 1) == 0)
                    break;
            }
            if (++row >= 12)
                break;
            bits = *next++;
            bits <<= 16;
            /* FAKEMATCH: the next pointer's reload store sits next to the loop branch: this asm's memory clobber takes the store's dependence at cost 1, where stock charges the branch 2 (2 to 0; European 2 to 0) */
            asm("" : "=r"(bits) : "0"(bits) : "memory");
        }
    }
    return ret;
}
