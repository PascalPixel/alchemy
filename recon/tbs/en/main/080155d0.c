/* 080155d0: ARM, compiled C (push starts at r5: -fcall-used-r4), for the game ARM route (K1:
 * stock agbcc_arm at the library ARM setting, alchemy diff --arm).
 * Stock agbcc_arm (2026-10-07, S13 session 3): listing 196, draft 196, common 189, differ 14.
 *   Reshaping, 117 to 72: the glyph index and the row bit pattern are one variable (bits): set
 *   more than once, its ch - 32 is no longer a birthing insn, so sched1 puts it before win's
 *   parameter copy, it is born while r0 is live and global gives it ip; the width load is the
 *   function's first reload and takes the first spill register, r0, which still holds win
 *   (reload_cse drops the ldr). rows set in two steps (rows itself is the spilled temporary).
 *   The plot macro takes the pixel's y and computes ty and sub itself; loop.c hoists both out of
 *   the column loop behind col = 0 and row + 1 (native's order), which shortens their live
 *   ranges so ty and sub outrank x and take sl and r8, and x gets r9.
 *   Devices, 72 to 14 (each tagged FAKEMATCH where it stands): ofs in r3, col in lr (also
 *   carrying the code test value), the empty asms at the end of the ret = 0 block and of the
 *   row reload, px & 7 through an asm. Most of what remains is one habit: stock arm_adjust_cost
 *   charges an anti dependence its producer's full cost, where the native was scheduled as if
 *   anti and output dependences cost one cycle (the same habit as 0800a0f8 and 0800a494).
 *   Still to do: (1) block 0, native push; mov r7, r3; sub sp (y's copy anti-depends on the push
 *   at 5 like sub sp, and the higher LUID wins the tie) and mov r9, r2 after ldr r3 (it is ready
 *   from T-2 at priority 5, ahead of the cmp); (2) the font path's head: the c1 and c3 constants
 *   interleave and the row = 0 move and w's store sit as native (stock gives c1's chain
 *   priority 2 through its anti dependence on the src store, and row = 0 priority 2 through c3's).
 *   Trials: plot macro with cell, half and shift as statements in native order: the stream
 *   matches but local-alloc then needs ip in the plot (x spilled, 246 to 271); px & 7 through an
 *   asm as a statement before dst: sched1 moves it past the mla and the same ip appears (244);
 *   x and y pinned to r9 and r7 through register locals: 245 (frame 40); code pinned to lr as a
 *   variable of its own: 48 (src inherits the lr preference and pushes col to r4); gw read in
 *   each arm (the native cross-jumps one ldrh into the join): 85-109 on the stock-tuned draft.
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

/* FAKEMATCH: px & 7 through an asm: stock charges its anti dependence on the mla (which reads r3) the multiply's 16 cycles, tying it with the VRAM add; an asm's links cost 1, so the add and its orr follow the mask shift as native (26 to 18) */
#define UiText_Dot(px) ({ u32 dot_; asm("and %0, %1, #7" : "=r"(dot_) : "r"(px)); dot_; })

#define UiText_PlotPixel(width, px, py, color) \
{ \
    u32 ty = (py) >> 3; \
    u32 sub = ((py) & 7) * 2; \
    u8 *vram = (u8 *)0x06002500; \
    u16 *dst = (u16 *)((u32)(vram + ((px >> 3) * (width - 2) + ty) * 32) | ((sub | (UiText_Dot(px) >> 2)) << 1)); \
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
    u32 row;
    u32 pass;
    /* FAKEMATCH: col in lr, where it also carries the glyph code from each arm to the join: GCSE's copy of ch - 222 in block 3 then prefers lr and its copy in the wide arm vanishes, and the narrow arm computes it first (18 to 14) */
    register u32 col asm("lr");
    /* FAKEMATCH: ofs in r3: expand_preferences hands it r0 (w is returned in r0 and bits dies in wp[bits] and in bits * 26), and with ofs in r0 the arms take r3 for their reloads and every round-robin reload register after them shifts (72 to 30) */
    register u32 ofs asm("r3");

    bits = ch - 32;
    width = win->width;
    if ((u32)bits > 111)
        return 1;
    if (ch < 256 || (font != 0 && ch - 222 < 2)) {
        if (font != 0) {
            u8 *src = font + ch * 32;
            u32 c1 = 0x1111;
            u32 c3 = 0x3333;
            u8 *wp = UiSprite_CursorFrameTiles + 0x800;
            s32 w = wp[bits];
            u32 px;
            u32 color;

            for (row = 0; row < 8 && y + row < 32; row++) {
                for (col = 0; col < 8; col++) {
                    if (col & 1)
                        b >>= 4;
                    else
                        b = *src++;
                    if ((b & 13) == 1) {
                        color = (b & 15) == 1 ? c1 : c3;
                        px = x + col;
                        if (px >> 3 < 30)
                            UiText_PlotPixel(width, px, y + row, color);
                    }
                }
            }
            return w;
        }
        col = ch - 222;
        ofs = bits * 26;
        rows = (u16 *)((u8 *)UiText_Glyphs + ofs);
        rows++;
    } else {
        col = ch - 222;
        ofs = (ch - 32) * 32;
        rows = (u16 *)((u8 *)UiText_Glyphs + ofs);
        rows += 4;
    }
    gw = *(u16 *)((u8 *)UiText_Glyphs + ofs);
    if (col < 2) {
        x -= 3;
        y -= 2;
        ret = 0;
        /* FAKEMATCH: a barrier at the end of the block lets the ret store sit next to the branch (stock charges its anti dependence on the jump 2 cycles, so sub r7 took that slot); reading y adds the reference that keeps y ranked above the row's sub, in r7 (30 to 28) */
        asm volatile("" : : "r"(y));
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
            /* FAKEMATCH: the next pointer's reload store sits next to the loop branch: this asm's memory clobber takes the store's dependence at cost 1, where stock charges the branch 2 (28 to 26) */
            asm("" : "=r"(bits) : "0"(bits) : "memory");
        }
    }
    return ret;
}
