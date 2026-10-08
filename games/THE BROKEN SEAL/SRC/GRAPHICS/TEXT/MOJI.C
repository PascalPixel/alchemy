#include "TYPES.H"

struct UiTextWindow {
    u8 unk0[10];
    u16 width;
    u8 unkC[10];
    u16 flags;
};

extern const u8 UiText_GlyphWidths[];
extern u8 UiText_Glyphs[];
#if defined(TBS_EDITION_JA)
extern u8 UiText_KanjiGlyphs[];
#endif

/* FAKEMATCH: px & 7 through an asm: stock charges its anti dependence on the mla, which reads r3, the multiply's 16 cycles, tying it with the VRAM add; an asm's links cost 1, so the add and its orr follow the mask shift as native (8 to 0; European 8 to 0; Japanese 8 to 0) */
#define UiText_Dot(px) ({ u32 dot_; asm("and %0, %1, #7" : "=r"(dot_) : "r"(px)); dot_; })

#if defined(TBS_EDITION_JA)
/* FAKEMATCH: s copied to d through an empty asm, for the Japanese loads and stores the bytes put between a compare and its branch; measured where used */
#define UiText_Keep(d, s) ({ asm("" : "=r"(d) : "0"(s)); d; })
#endif

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
#if defined(TBS_EDITION_JA)
    /* FAKEMATCH: an unused 64-byte local: native's frame is 100 bytes, 36 of spill slots and 64 above them (6 to 0) */
    u8 buf[64];
    /* FAKEMATCH: x in r8, where native keeps it: GCSE's register for the font loop's copy of x (below) then prefers r8 through x = xt, so the copies it inserts emit nothing; with x a pseudo, x and y swap r8 and r7 (42 to 0) */
    register u32 x asm("r8");
    u32 xt;
    u32 y;
    u16 *rows;
    s32 ret;
    u32 width;
    s32 bits;
    u32 b;
    u32 row;
    u32 pass;
    u32 col;
    /* FAKEMATCH: code in ip: GCSE's copy of ch - 222 from the head then shares it and the wide arm's copy vanishes, and font takes lr (11 to 0) */
    register u32 code asm("ip");
    /* FAKEMATCH: ofs in r3, as English's (44 to 0) */
    register u32 ofs asm("r3");
    u32 gw;

    /* FAKEMATCH: x's copy through an asm, as English's (6 to 0) */
    asm("mov %0, %1" : "=r"(x) : "r"(x0));
    /* FAKEMATCH: y's copy through an asm, as English's (4 to 0) */
    asm("mov %0, %1" : "=r"(y) : "r"(y0));
    width = win->width;
    /* FAKEMATCH: the width store sits between the cmp and the branch, as English's through b (6 to 0) */
    asm("" : "=r"(font) : "0"(font) : "memory");
    if (ch < 256 || (font != 0 && ch - 222 < 2)) {
        if (font != 0) {
            /* 1bpp, 10 bytes a glyph, low bit leftmost; 222 and 223 draw the glyphs of 254 and 255 two dots left and one up, 2 wide */
            u8 *next;
            u32 color;
            s32 w;
            u32 px;
            u8 *f;
            u32 c;
            /* FAKEMATCH: the font base in lr: f's copy into lr gives it native's register; left in f, f takes r0 and font r2 (33 to 0) */
            register u8 *base asm("lr");

            color = 0x1111;
            w = 8;
            x++;
            /* FAKEMATCH: font copied to f in this block: sched1 moves a single-set copy to its block's end, past the compare, and sched2 keeps it in the branch's slot by its later LUID, so the colour store follows the compare on its hazard as the bytes show (cmp r3, #1; str r2, [sp, #16]; bhi); stock charges a store's link to the branch 2 cycles, so without it the compare takes the slot (2 to 0) */
            UiText_Keep(f, font);
            /* FAKEMATCH: ch - 222 through an asm: compiled, its anti dependence on the w store (r3) costs the store's 2 cycles and gives the compare priority 2, above the colour store's 1; an asm's links cost 1 (2 to 0) */
            asm("sub %0, %1, #222" : "=r"(c) : "r"(ch));
            if (c < 2) {
                ch += 32;
                x -= 2;
                y--;
                w = 2;
            }
            ch -= 32;
            base = f;
            ofs = ch * 10;
            next = base + ofs;
            row = 0;
            b = *next;
            next++;
            /* FAKEMATCH: x copied through xt before the loop, at its head and at its step, on one line so that GCSE sees one expression: PRE inserts the copy after the entry and latch compares, where it keeps the branch's slot, and the byte loads follow the compares as the bytes show (cmp r7, #31; ldrb ip, [lr, r3]; bhi and cmp r3, #31; ldrb ip, [r9], #1; bls); stock charges a load's link to the branch 2 cycles, so without it the compares take the slot (4 to 0) */
            for (UiText_Keep(xt, x); y + row < 32; UiText_Keep(xt, x)) { x = UiText_Keep(xt, x);
                for (col = 0; b != 0; col++, b >>= 1) {
                    if (b & 1) {
                        px = x + col;
                        /* FAKEMATCH: px & 7 through UiText_Dot's asm, measured there */
                        UiText_PlotPixel(width, px, y + row, color);
                    }
                }
                if (++row >= 10)
                    break;
                b = *next++;
            }
            return w;
        }
        code = ch - 222;
        ofs = (ch - 32) * 26;
        rows = (u16 *)(UiText_Glyphs + ofs);
        rows++;
        /* FAKEMATCH: the rows store comes before the glyph width load, as the European branch's (2 to 0) */
        asm volatile("" : : : "memory");
        gw = *(u16 *)(UiText_Glyphs + ofs);
    } else {
        /* 24 bytes a glyph, 10 dots wide */
        code = ch - 222;
        rows = (u16 *)UiText_KanjiGlyphs + (ch - 256) * 12;
        gw = 10;
    }
    if (code < 2) {
        x -= 3;
        y -= 2;
        ret = 0;
        /* FAKEMATCH: the ret store sits next to the branch, as English's (2 to 0) */
        asm volatile("");
    } else {
        ret = gw;
    }
#else
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
            /* FAKEMATCH: the next pointer's reload store sits next to the loop branch: this asm's memory clobber takes the store's dependence at cost 1, where stock charges the branch 2 (2 to 0; European 2 to 0; Japanese 2 to 0) */
            asm("" : "=r"(bits) : "0"(bits) : "memory");
        }
    }
    return ret;
}
