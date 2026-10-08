/* 080155d0: ARM, compiled C (push starts at r5: -fcall-used-r4), for the game ARM route (K1:
 * stock agbcc_arm at the library ARM setting, alchemy diff --arm).
 * Stock agbcc_arm (2026-10-08, S13 session 4): differ 0 in English (alchemy diff --arm) and in
 *   German, Spanish, French and Italian (diff cannot score their ARM scaffolds: each build of
 *   this draft compared instruction by instruction with the ROM extent, 0x2f4 bytes at
 *   080142d0 de, 080145d0 es and fr, 080155d0 it; pool words differ only by relocation).
 *   The European branch: bits holds ch itself (one variable for the code and the row bits, so
 *   ch takes ip as native), the font test is ch < 256 || font != 0 (stock gives native's
 *   subs/movne/movls flag), the width table is indexed through idx = bits - 32 (written
 *   wp[bits - 32] the - 32 folds into the address), each arm subtracts 32 itself and reads the
 *   glyph width (cross jumping then shares the ret store), no bits > 111 return, no code test.
 *   Its devices, measured as differ against the German extent when removed: x's and y's
 *   copies through asms (2, 4), the width store's memory asm (4), c3's mov through an asm (4),
 *   a volatile barrier after the colour constants (38), w's store memory asm (6), a memory
 *   barrier after the narrow arm's rows (39), the pass loop's next store asm (2), px & 7
 *   through UiText_Dot (8), ofs in r3 (28); col in lr changes nothing there (0).
 *   The plot macro now sets the VRAM base before sub (Japanese needs it; no change in the
 *   other five). An analysis-only agbcc_arm whose arm_adjust_cost charges anti and output
 *   dependences 1 (built in scratch, never used for credit) compiles the European branch
 *   without any device but ofs in r3 to differ 0, and the Japanese draft (recon/tbs/ja/main/
 *   080155d0.c) without scheduling devices to differ 0: the devices here stand in for that one
 *   habit.
 *   Japanese: its own body of 0x30c bytes (1bpp 10-byte font rows, a kanji table, frame 100);
 *   see its draft for why stock cannot place its last loads and stores.
 * Stock agbcc_arm (2026-10-07, S13 session 3): listing 196, draft 196, common 196, differ 0
 *   in English. Reshaping (117 to 72): bits is also the glyph index; rows set in two steps;
 *   the plot macro takes the pixel's y. Devices (72 to 0), each tagged with the differ its
 *   removal measures: ofs in r3 and col in lr (also carrying ch - 222 to the join), px & 7
 *   and the copies of y and x and c1's mov through asms, and empty asms that let a reload
 *   store sit next to its branch or reorder a block's head. All but the two register pins
 *   stand in for one habit: the native was scheduled as if anti and output dependences cost
 *   one cycle, where stock arm_adjust_cost charges the producer's full cost.
 *   Editions: German, Spanish, French and Italian share one body of 0x2f4 bytes (relocated
 *   copies; no idx > 111 return, the font test as a flag, the code test gone from the join),
 *   and Japanese is another of 0x30c (frame 100, a different font loop): each needs a branch
 *   of its own.
 * Stock agbcc_arm (2026-10-07, S13 session 2): listing 196, draft 197, common 138, differ 117.
 *   127 to 117: row = 0 first in the pass setup, before next and bits (permute's reorder);
 *   bits = rows[0] before next = rows + 1: no change (117).
 *   Still to do: idx takes r0 (native ip), code r2 then ip (native lr in both), x and the
 *   VRAM base swap r9 and sl. Diagnosis (greg): 43 (idx) inherits preference r0 through
 *   expand_preferences, 43 -> 111 -> 50 (ofs) -> gw; win (32) gets no register in global, so
 *   its reload for the width cannot inherit r0 while idx holds it.
 *   161 to 127: each glyph row loaded into bits and shifted in place, bits = *next++;
 *   bits <<= 16, so the halfword load and the shift share bits' pseudo, as native's
 *   ldrh ip, [r1], #2 ... lsl ip, ip, #16 (a separate load temporary was local-allocated r2
 *   or r3); pass and row now take native's r1 and r2. Trial: code = ch - 222 assigned in the
 *   font test, (code = ch - 222) < 2, and not recomputed in the wide arm: 132 (GCSE no longer
 *   hoists the sub above the beq; native's single lr is GCSE's copy, 175 = code).
 * Stock agbcc_arm (2026-10-07, S13 session 1): listing 196, draft 197, common 116, differ 161
 *   (from 177). 163 to 161: the width table through a local pointer, wp[idx], so the address
 *   is (plus base idx) as native's ldrb r3, [r3, ip] (the symbol form expands to (plus idx base)
 *   and ties idx to w's register). idx still takes r0 (native ip): global's preference chain
 *   idx -> idx * 13 -> ofs -> gw -> the return register; native's ofs is r3; ofs declared
 *   before idx: no change (161); wp declared before src: no change (161); w read before the
 *   colour constants: 169. The other five editions keep their scaffold, which diff --arm
 *   cannot compare (no separate listing object); score them once English matches. 177 to 169: the wide glyph as a byte offset, (u8 *)UiText_Glyphs + (ch - 32) * 32,
 *   so the - 32 stays out of the symbol (native sub r3, r1, #32; lsl r3, r3, #5; one pool word).
 *   169 to 163: ofs set in each arm and the glyph width read once after the join through it,
 *   gw = *(u16 *)((u8 *)UiText_Glyphs + ofs). Read in each arm, the load is anti-dependent on
 *   the narrow arm's jump at its full latency, so sched1 hoisted it above the rows adds; at the
 *   join it is native's shared ldrh r0, [r3, r2], and ch, font and y now take native's r1, r3, r7.
 *   Trials (stock): rows through a cast to the glyph struct, (struct UiTextGlyph *)(... + ofs)->rows,
 *   folds the + 2 into the pool symbol: 179. gw read before rows in each arm: no change (169).
 *   code = ch - 222 once after the join: 170; the test as ch - 222 < 2 after the join: 170;
 *   code assigned inside the font test, (code = ch - 222) < 2: 168 (and code is unset on the
 *   font == 0 path). Native's code is GCSE's: computed before the font test's beq, lr in both.
 * Route decision (2026-10-07), this draft under agscc -marm with the game options:
 *   differ 377; with -mno-apcs-frame 381; with -mno-apcs-frame -ffixed-fp 377.
 * alchemy diff --camelot-arm, upstream's whole arm_adjust_cost (2026-10-06, session 6):
 *   listing 196, draft 198, common 119, differ 156; the same draft under stock agbcc_arm:
 *   draft 199, common 109, differ 177. 165 to 156 (session 6, aimed by --heat):
 *   the wide branch indexes by ch - 32 again rather than idx, as the listing's sub r3, r1, #32
 *   inside that branch. Trial (session 6): code = ch - 222 once, before the test, used there
 *   and by both branches (the listing's cmp lr, #1 after both): 159.
 * Earlier (session 5): listing 196, draft 197, common 114, differ 165, as under the
 *   anti/output half alone (stock agbcc_arm 164).
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
 * alchemy permute --camelot-arm (2026-10-06, 4 minutes, seed 1): candidates at differ 118, 108
 *   and 98 (weighted 1790 from 2550), the best after 14 rewrites: statement reorders, loop
 *   forms, pointer indexing, an assignment moved into a condition and 3 temporaries shared by
 *   two statements each. Not yet taken: the shared temporaries need natural forms first.
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

extern u8 UiSprite_CursorFrameTiles[];
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
            u8 *wp;
#if !defined(TBS_EDITION_EN)
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
            wp = UiSprite_CursorFrameTiles + 0x800;
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
            wp = UiSprite_CursorFrameTiles + 0x800;
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
