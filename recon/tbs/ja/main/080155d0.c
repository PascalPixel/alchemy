/* 080155d0 (Japanese): ARM, compiled C, for the game ARM route (K1: stock agbcc_arm at the
 * library ARM setting). Its own body of 0x30c bytes: the Japanese font is 1bpp, ten bytes a
 * glyph, eight dots a row read low bit first; codes 222 and 223 shift the glyph (ch + 32,
 * x - 1, y - 1, width 2, else width 8); wide glyphs come from UiText_KanjiGlyphs, 24 bytes
 * each with no width word (gw 10); the frame is 100 bytes.
 * Stock agbcc_arm (2026-10-08, S13 session 4): differ 8 against the ROM extent. Since the
 *   staging merge, alchemy diff --edition tbs-ja --arm scores the scaffold itself: listing
 *   193, draft 193, common 189, differ 8 (2026-10-08, session 5). Reshaping:
 *   ch -= 32 in place (native's sub r1, r1, #32), next = font + ofs then next++ with the first
 *   row read through it (next takes r9, colour spills), row = 0 after next, code assigned last
 *   in the wide arm, and the plot macro's VRAM base before sub (shared with the other five).
 *   Devices: tagged with the differ each removal measures.
 *   Remaining (8): three blocks where native puts a load or store between a compare and its
 *   branch: the font block (cmp r3, #1; str r2, [sp, #16]; bhi: the colour store), the font
 *   loop's entry (cmp r7, #31; ldrb ip, [lr, r3]; bhi) and its row end (cmp r3, #31;
 *   ldrb ip, [r9], #1; bls). Stock's reverse list scheduler charges a store's or load's link to
 *   the branch its full cost, 2, so the compare is the only insn ready at T-2 and takes the
 *   slot; nothing can outrank it there: every insn in these blocks has priority 1 (the font
 *   block's compare 2, through the code sub's anti dependence on w's store), and an asm cannot
 *   add priority, as links into and out of an asm cost 1. With anti and output dependences at
 *   one cycle (an analysis-only agbcc_arm built in scratch, never used for credit), the store
 *   or load is ready beside the compare and wins on its function-unit hazard: that compiler
 *   takes this draft to differ 0 without its scheduling devices (keeping the two register
 *   pins and the frame local), as it does the European branch with ofs in r3 alone. So the
 *   Japanese edition stays on its scaffold until K3 settles the anti-dependence cost habit.
 *   Session 5 (2026-10-08) re-derived this from agbcc_arm's sched.c: the branch is scheduled
 *   first, the compare is then always ready (cost 1, class 3, the highest LUID left), and
 *   only a strictly higher priority displaces it; a tie goes to the compare, and a store's
 *   or load's hazard only wins a tie from T-3 on. An empty asm consuming the store or load
 *   (to sit at T-2 with it at T-3) carries only its producers' priority, at most the
 *   compare's here; an asm emitting the load itself costs 1 but loses the same tie; and no
 *   asm can sit between a compare and its branch in the RTL. No S2 device reaches these
 *   three spots under stock.
 */
#include "TYPES.H"

struct UiTextWindow {
    u8 unk0[10];
    u16 width;
    u8 unkC[10];
    u16 flags;
};

extern u8 UiText_Glyphs[];
extern u8 UiText_KanjiGlyphs[];

/* FAKEMATCH: px & 7 through an asm, as English's (16 to 8) */
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
    /* FAKEMATCH: an unused 64-byte local: native's frame is 100 bytes, 36 of spill slots and 64 above them (14 to 8) */
    u8 buf[64];
    u32 x;
    u32 y;
    u16 *rows;
    s32 ret;
    u32 width;
    s32 bits;
    u32 b;
    u32 row;
    u32 pass;
    u32 col;
    /* FAKEMATCH: code in ip: GCSE's copy of ch - 222 from the head then shares it and the wide arm's copy vanishes, and font takes lr (57 to 8) */
    register u32 code asm("ip");
    /* FAKEMATCH: ofs in r3, as English's (52 to 8) */
    register u32 ofs asm("r3");
    u32 gw;

    /* FAKEMATCH: x's copy through an asm, as English's (14 to 8) */
    asm("mov %0, %1" : "=r"(x) : "r"(x0));
    /* FAKEMATCH: y's copy through an asm, as English's (12 to 8) */
    asm("mov %0, %1" : "=r"(y) : "r"(y0));
    width = win->width;
    /* FAKEMATCH: the width store sits between the cmp and the branch, as English's through b (14 to 8) */
    asm("" : "=r"(font) : "0"(font) : "memory");
    if (ch < 256 || (font != 0 && ch - 222 < 2)) {
        if (font != 0) {
            u8 *next;
            u32 color;
            s32 w;
            u32 px;

            color = 0x1111;
            w = 8;
            x++;
            if (ch - 222 < 2) {
                ch += 32;
                x -= 2;
                y--;
                w = 2;
            }
            ch -= 32;
            ofs = ch * 10;
            next = font + ofs;
            row = 0;
            b = *next;
            next++;
            while (y + row < 32) {
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
        /* FAKEMATCH: the rows store comes before the glyph width load, as the European branch's (10 to 8) */
        asm volatile("" : : : "memory");
        gw = *(u16 *)(UiText_Glyphs + ofs);
    } else {
        code = ch - 222;
        rows = (u16 *)UiText_KanjiGlyphs + (ch - 256) * 12;
        gw = 10;
    }
    if (code < 2) {
        x -= 3;
        y -= 2;
        ret = 0;
        /* FAKEMATCH: the ret store sits next to the branch, as English's (10 to 8) */
        asm volatile("");
    } else {
        ret = gw;
    }
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
                    UiText_PlotPixel(width, px, y + row, color);
                }
                if ((bits <<= 1) == 0)
                    break;
            }
            if (++row >= 12)
                break;
            bits = *next++;
            bits <<= 16;
            /* FAKEMATCH: the next pointer's reload store sits next to the loop branch, as English's (10 to 8) */
            asm("" : "=r"(bits) : "0"(bits) : "memory");
        }
    }
    return ret;
}
