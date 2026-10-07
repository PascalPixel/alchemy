/* 080155d0: ARM, compiled C (push starts at r5: -fcall-used-r4), for the game ARM route (K1:
 * stock agbcc_arm at the library ARM setting, alchemy diff --arm).
 * Japanese branch (2026-10-07): listing193, draft193, common183, differ20.
 *   Reconstructed from its own scaffold: supplied font has ten one-bit rows,
 *   dakuten adjusts its index/position/advance; wide kanji have twelve rows
 *   and fixed advance10. Current linker places UiText_KanjiGlyphs at the
 *   native pool target. Natural branch90; explicit narrow-byte offset61;
 *   the measured unused real-type local restores frame36 to100 (55).
 *   Narrow-row/r2 boundary42; in-place font index subtraction38; initial
 *   next-row/r0 boundary34; nextrow/lr plus column/r5 boundaries32; zero/r3
 *   boundary restores coordinate order and flag reload (24); pass-reset/r1
 *   boundary22; font/lr input boundary with deferred coordinates20.
 *   Remaining: parameter-copy/store order, several font setup/store orders,
 *   the VRAM-base setup, narrow-row store/load, final row shift/store.
 *   Trials: coordinate input boundary from34 unchanged; adding memory/cc48;
 *   nextrow/lr without col/r5 gives60. Explicit flags/r2 from24 gives34 by
 *   changing font colour allocation. Fixed x/r8,y/r7 alone unchanged22;
 *   combined coordinate/ch memory boundary36; adding font/lr22; deferring
 *   coordinates until after font20, then ordinary x/y keeps20. Removed
 *   rejected devices. International branches remain separate and unchanged.
 * Stock agbcc_arm (2026-10-07, completion session, latest): English listing
 *   196, draft196, common189, differ14. A short incoming-x/r2 boundary
 *   retains x until font is loaded, restoring their native order (16 to14).
 *   Remaining: y copy versus frame allocation, width store versus bounds
 *   compare, first colour's register, font-row reset, final shift/store.
 *   Trials from16: delayed width store18; explicit bounds local21;
 *   ordinary width29; read-only width/cc35; incoming-window/r0 constraint18;
 *   volatile width boundary16; guarded do/while172. Ordinary y with
 *   constrained ty/r8, sub/r8 and line/r3 gives41. From14: u16 colours37,
 *   u16 width131; next=p+1 then bits=*p29; split postincrement unchanged;
 *   pointer CSE dead boundary73; multiply instead of shift unchanged;
 *   pointer read/write after shift49. Row-reset read/write32; read-only16;
 *   row/table memory boundary14 but moves the width spill before cmp.
 *   All 24 setup declaration orders leave weighted390/differ14. Colour/r1
 *   read-only with r2/r3 clobbers74; read/write26, regardless of later
 *   assignment placement; paired colour/r1,r2 outputs52; source memory
 *   operand77. Plain register keyword unchanged. Rejected trials removed.
 * Stock agbcc_arm (2026-10-07, completion session, previous): English listing
 *   196, draft196, common188, differ16. From34: scoped initial next pointer
 *   in r2, with next declared before head, gives30. Read-only r1 constraints
 *   before/after the final postincrement preserve its spill and give26;
 *   joint parameter/index boundary24. Native zero/r1 plus flags reload/r0
 *   gives20; row-pointer load/pass reset/add boundaries18. Final row
 *   coordinate in r3 after its pointer reload gives16. A separate y boundary
 *   and deferred x preserve16 while putting y before x.
 *   Remaining: prologue ordering, width store versus bounds compare, first
 *   colour register and initialization, font-row reset, final shift/store.
 *   DE/ES/FR/IT: listing187, draft186, common123, differ127 in all four.
 *   Trials from34: const colours91, volatile src62; initial pointer p+1/head
 *   before next38. Final pointer +r boundary59; readonly pointer with original
 *   next++41; readonly p++ then next=p39; readonly after p++ gives26.
 *   Tail shift moved before next=p, with/without bits constraints or a fixed
 *   ip bits value, holds next in sl and displaces ty (53 or61); direct shift
 *   asm after the copy keeps the spill but moves cmp after the shift/store.
 *   Coordinate boundary with no fixed r3 gives68; fixed r3 gives16.
 *   Ordinary y50, readonly r7 alias80; fixed first ty/r8 and second sub/r8
 *   give34 but use r8 as scratch; first ty alone46. Zero/r1 readonly24,
 *   read/write26; constraining the window reload restores20. Shared src then
 *   colour through one short r1 value gives28: c3 and the width table hoist
 *   ahead of c1. All rejected forms removed; no real asm instructions remain.
 * Stock agbcc_arm (2026-10-07, completion session, previous): English listing
 *   196, draft 196, common 179, differ 34. From76, tagged ofs/r3 gives72;
 *   code/lr and col/lr give75 but remove the extra code copy. Separate
 *   constrained narrow/r1 and wide/r0 rows restore the two stores and shared
 *   width load; y/r7 plus a value boundary restore the loop allocation (46).
 *   Tagged nextrow/r4 and column-reset boundaries restore row setup (42).
 *   Pixel cell/r0, part/r3 and shift/r1 operands retain the native shift
 *   before the VRAM-base add (34). Each is allocation or scheduling only.
 *   Remaining: prologue scheduling, colour initialization, zero return,
 *   first next-row pointer, and final next-row load/test scheduling.
 *   DE/ES/FR/IT source branch: no English bounds/dakuten tests; any non-null
 *   font selects the 8-pixel path, and both glyph paths read their own width.
 *   Initially187 listing/186 draft/114 common/differ145 in all four; current
 *   German comparison129. Japanese still needs its distinct source branch.
 *   Additional trials from76: rows from glyph-end no change; volatile rows
 *   152; isolated pixel-shift local274; narrow rows memory barrier104.
 *   From46: ordinary nextrow local105; unbound row+1 asm operand110;
 *   fixed nextrow used as the increment54; keep ordinary increment46,
 *   then constrain column reset first42. Ordinary y68; r8 clobber60.
 *   From34: scoped or statement-expression first-colour/r1 constraint46;
 *   not retained. The original colour declarations remain.
 * Stock agbcc_arm (2026-10-07, completion session, earlier): listing 196, draft 196,
 *   common 158, differ 76. x in native r9 gives 117 -> 97; width loaded in
 *   incoming r0 through a tagged local and empty constraint removes the
 *   window reload and puts idx in native ip (97 -> 82). Starting each pixel
 *   column before row calculations, as a while loop, gives 78; a cc clobber
 *   keeps the comparison after the width load and gives 76.
 *   Remaining: initial scheduling, code's r2/lr join, glyph address/width
 *   allocation and shared rows store, colour initialization, next-row
 *   scheduling, pixel address scheduling, and the last row-pointer reload.
 *   Trials from 117: u16 gw 129; signed idx 121; fixed idx ip 170;
 *   idx barrier 144, also fixing ip 160; explicit wide-path gotos 148.
 *   From x=r9 (97): fixed code lr 122; also col lr 96; also idx/bits ip 104;
 *   ch CSE barrier 96; offset barriers 85; fixed ofs r3 92 with or without
 *   offset barriers, then fixed idx/bits ip 100. Not retained.
 *   Three-minute source search, seed11/jobs2: weighted2540 ->2290, differ152,
 *   15 rewrites; not retained (extra temporary and guarded constant loop).
 *   From width-r0 (82): fixed code/col lr 85 (195 instructions); signed gw
 *   unchanged; direct ch-222 at join 87. From column-first loops (78):
 *   sub before ty 134; idx read/write in width barrier 124; cc clobber 76.
 *   From76: separate pos/part/shift locals in pixel macro 248; mask before
 *   destination declaration 282. Original pixel expression retained.
 *   Three-minute prefix search, seed23/jobs2 with the width asm ignored:
 *   only four distinct candidates compiled; no improvement over76.
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
#include "EDITION.H"

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
extern const u8 UiText_KanjiGlyphs[];

#define UiText_PlotPixel(width, px, ty, sub, color) \
{ \
    u8 *vram = (u8 *)0x06002500; \
    /* FAKEMATCH: native computes the shift before adding the VRAM base; separate ordinary locals give 274 differences. */ \
    register u32 cell asm("r0") = (px >> 3) * (width - 2) + ty; \
    /* FAKEMATCH: native reuses r3 for the within-tile halfword. */ \
    register u32 part asm("r3") = sub | ((px & 7) >> 2); \
    /* FAKEMATCH: native replaces px with its nibble shift in r1. */ \
    register u32 shift asm("r1") = (px & 3) * 4; \
    u16 *dst; \
    u32 mask; \
    /* FAKEMATCH: keep the measured pixel shift before the VRAM-base add. */ \
    asm("" : "+r"(cell) : "r"(part), "r"(shift)); \
    dst = (u16 *)((u32)(vram + cell * 32) | (part << 1)); \
    mask = 15 << shift; \
 \
    *dst = (*dst & ~mask) | (mask & color); \
}

s32 UiText_DrawGlyphArm(struct UiTextWindow *win, u32 ch, u32 xpos, u32 ypos, u8 *font)
{
#if defined(TBS_EDITION_JA)
    /* FAKEMATCH: native reserves 100 bytes, the live locals use 36; two real wide-glyph records reserve the remaining 64 bytes (alchemy sizes). */
    struct UiTextWideGlyph unused[2];
    u16 *rows;
    s32 ret;
    u32 width;
    u32 gw;
    s32 bits;
    u32 row;
    u32 pass;
    /* FAKEMATCH: native column uses r5; the next-row boundary otherwise swaps column and colour between r4/r5 (60 differences). */
    register u32 col asm("r5");
    u32 x;
    u32 y;
    /* FAKEMATCH: native font uses lr; the input-copy boundary otherwise swaps font and dakuten between ip/lr (36 differences). */
    register u8 *f asm("lr") = font;

    /* FAKEMATCH: native loads font before copying coordinates; the combined boundary alone puts the copies first (22 to20). */
    asm("" : "+r"(f));
    x = xpos;
    y = ypos;
    /* FAKEMATCH: native copies coordinates before the width load and character test; the ordinary setup delays both copies. */
    asm("" : "+r"(x), "+r"(y), "+r"(ch) : "r"(f) : "memory");
    width = win->width;
    if (ch < 256 || (f != 0 && ch - 222 < 2)) {
        if (f != 0) {
            u32 color = 0x1111;
            s32 w = 8;
            u8 *src;
            u32 b;
            u32 px;
            u32 sub;
            u32 ty;

            x++;
            if (ch - 222 < 2) {
                ch += 32;
                x -= 2;
                y--;
                w = 2;
            }
            ch -= 32;
            src = f + ch * 10;
            row = 0;
            b = *src++;
            while (y + row < 32) {
                col = 0;
                if (b != 0) {
                    ty = (y + row) >> 3;
                    sub = ((y + row) & 7) * 2;
                    do {
                        if (b & 1) {
                            px = x + col;
                            /* FAKEMATCH: UiText_PlotPixel retains native's shift before the VRAM-base add. */
                            UiText_PlotPixel(width, px, ty, sub, color);
                        }
                        col++;
                    } while ((b >>= 1) != 0);
                }
                if (++row >= 10)
                    break;
                b = *src++;
            }
            return w;
        }
        {
            u32 ofs = (ch - 32) * 26;
            struct UiTextGlyph *glyph = (struct UiTextGlyph *)((u8 *)UiText_Glyphs + ofs);
            /* FAKEMATCH: native narrow rows use r2 and are stored before the width load; ordinary C merges the pointer stores. */
            register u16 *p asm("r2") = glyph->rows;
            /* FAKEMATCH: retain the native narrow-row pointer register. */
            asm("" : "+r"(p));
            rows = p;
            gw = *(u16 *)((u8 *)UiText_Glyphs + ofs);
        }
    } else {
        gw = 10;
        rows = (u16 *)(UiText_KanjiGlyphs + (ch - 256) * 24);
    }
    if (ch - 222 < 2) {
        /* FAKEMATCH: native stores zero width through r3 after the coordinate adjustment; ordinary C uses r2 before adjusting y. */
        register s32 zero asm("r3");
        x -= 3;
        y -= 2;
        zero = 0;
        /* FAKEMATCH: retain the native zero-store register. */
        asm("" : : "r"(zero));
        ret = zero;
    } else {
        ret = gw;
    }
    {
        u16 *next;
        /* FAKEMATCH: native creates the next-row head in r0; the ordinary loop-hoisted expression uses r3. */
        register u16 *p asm("r0") = rows;
        /* FAKEMATCH: native resets pass in r1 between loading rows and adding one row. */
        register u32 i asm("r1");
        u16 *head;
        /* FAKEMATCH: retain the native head-pointer reload register. */
        asm("" : "+r"(p) : : "r1");
        i = 0;
        /* FAKEMATCH: native resets pass before adding one row. */
        asm("" : "+r"(p) : "r"(i));
        head = p + 1;
    for (pass = i; pass < 4; pass++) {
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
        row = 0;
        next = head;
        bits = rows[0];
        bits <<= 16;
        while (y + row < 32) {
            /* FAKEMATCH: native keeps the next shadow row in lr before computing tile coordinates; ordinary C delays its add. */
            register u32 nextrow asm("lr");
            col = 0;
            /* FAKEMATCH: native resets the column before the next-row add. */
            asm("" : : "r"(col));
            nextrow = row + 1;
            /* FAKEMATCH: retain the native next-row scheduling boundary. */
            asm("" : : "r"(nextrow));
            ty = (y + row) >> 3;
            sub = ((y + row) & 7) * 2;
            while (col < 14) {
                if (bits < 0) {
                    px = x + col;
                    /* FAKEMATCH: UiText_PlotPixel retains native's shift before the VRAM-base add. */
                    UiText_PlotPixel(width, px, ty, sub, color);
                }
                if ((bits <<= 1) == 0)
                    break;
                col++;
            }
            if (++row >= 12)
                break;
            bits = *next++;
            bits <<= 16;
        }
    }
    }
    return ret;
#else
    u16 *rows;
    s32 ret;
    u32 width;
    u32 b;
    u32 gw;
    s32 bits;
    u32 idx;
    u32 row;
    /* FAKEMATCH: native keeps the dakuten code in lr across the glyph join, avoiding the ordinary r2-to-lr copy. */
    register u32 code asm("lr");
    u32 pass;
    /* FAKEMATCH: native reuses lr for the column after the dakuten test. */
    register u32 col asm("lr");
    struct UiTextGlyph *glyph;
    struct UiTextWideGlyph *wide;
    /* FAKEMATCH: native retains the glyph offset in r3; ordinary allocation ties it to the return width in r0. */
    register u32 ofs asm("r3");
    /* FAKEMATCH: native x uses r9; ordinary allocation swaps r9/sl in both paths (117 differences). */
    register u32 x asm("r9");
    /* FAKEMATCH: native y uses r7; constrained row pointers otherwise swap r7/r8 through both loops. */
    register u32 y asm("r7") = ypos;
#if defined(TBS_EDITION_EN)
    /* FAKEMATCH: native retains incoming x in r2 until after loading font from the stack. */
    register u32 xi asm("r2") = xpos;

    /* FAKEMATCH: native moves y out of the incoming font register before preparing x. */
    asm("" : "+r"(y), "+r"(xi) : "r"(font));
    x = xi;
#else
    /* FAKEMATCH: native moves y out of the incoming font register before preparing x. */
    asm("" : "+r"(y));
    x = xpos;
#endif
    /* FAKEMATCH: native prepares font, x and y before computing the glyph index; the ordinary setup is reordered (26 differences). */
    asm("" : "+r"(y), "+r"(ch) : "r"(font), "r"(x));
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
    width = win->width;
    if (ch < 256 || font != 0) {
#else
    idx = ch - 32;
    {
        /* FAKEMATCH: native width uses incoming r0; ordinary allocation reloads win and takes r2 (97 differences). */
        register u32 w asm("r0") = win->width;
        /* FAKEMATCH: retain r0's width load (97 to 82); cc keeps the comparison after it (78 to 76). */
        asm("" : "+r"(w) : : "cc");
        width = w;
    }
    if (idx > 111)
        return 1;
    if (ch < 256 || (font != 0 && ch - 222 < 2)) {
#endif
        if (font != 0) {
            u8 *src = font + ch * 32;
            u32 c1 = 0x1111;
            u32 c3 = 0x3333;
            u8 *wp = UiSprite_CursorFrameTiles + 0x800;
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
            s32 w = wp[ch - 32];
#else
            s32 w = wp[idx];
#endif
            u32 px;
            u32 sub;
            u32 color;
            u32 ty;

            for (row = 0; row < 8 && y + row < 32; row++) {
                /* FAKEMATCH: native keeps the next row in r4 across the pixel loop; ordinary nextrow changes allocation (105). */
                register u32 nextrow asm("r4");
                col = 0;
                /* FAKEMATCH: the native column reset precedes the next-row add. */
                asm("" : : "r"(col));
                nextrow = row + 1;
                /* FAKEMATCH: native schedules next row before the tile coordinates; a source nextrow local gives 105 differences. */
                asm("" : : "r"(nextrow));
                ty = (y + row) >> 3;
                sub = ((y + row) & 7) * 2;
                while (col < 8) {
                    if (col & 1)
                        b >>= 4;
                    else
                        b = *src++;
                    if ((b & 13) == 1) {
                        color = (b & 15) == 1 ? c1 : c3;
                        px = x + col;
                        /* FAKEMATCH: UiText_PlotPixel constrains the native shift-before-address order (42 to34). */
                        if (px >> 3 < 30)
                            UiText_PlotPixel(width, px, ty, sub, color);
                    }
                    col++;
                }
            }
            return w;
        }
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
        ofs = (ch - 32) * 26;
#else
        code = ch - 222;
        ofs = idx * 26;
#endif
        glyph = (struct UiTextGlyph *)((u8 *)UiText_Glyphs + ofs);
        {
            /* FAKEMATCH: native narrow rows use r1 and are stored before the shared width load. */
            register u16 *p asm("r1") = glyph->rows;
            /* FAKEMATCH: retain the narrow rows register before the join. */
            asm("" : "+r"(p));
            rows = p;
        }
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
        gw = *(u16 *)((u8 *)UiText_Glyphs + ofs);
#endif
    } else {
#if !defined(TBS_EDITION_DE) && !defined(TBS_EDITION_ES) && !defined(TBS_EDITION_FR) && !defined(TBS_EDITION_IT)
        code = ch - 222;
#endif
        ofs = (ch - 32) * 32;
        wide = (struct UiTextWideGlyph *)((u8 *)UiText_Glyphs + ofs);
        {
            /* FAKEMATCH: native wide rows use r0 and are stored before the shared width load. */
            register u16 *p asm("r0") = wide->rows;
            /* FAKEMATCH: retain the wide rows register before the join. */
            asm("" : "+r"(p));
            rows = p;
        }
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
        gw = *(u16 *)((u8 *)UiText_Glyphs + ofs);
#endif
    }
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
    ret = gw;
#else
    gw = *(u16 *)((u8 *)UiText_Glyphs + ofs);
    if (code < 2) {
        /* FAKEMATCH: native stores the zero width through r1; ordinary allocation uses r2 (24 differences). */
        register s32 zero asm("r1");
        x -= 3;
        y -= 2;
        zero = 0;
        /* FAKEMATCH: retain the native zero-store register. */
        asm("" : : "r"(zero));
        ret = zero;
    } else {
        ret = gw;
    }
#endif
    {
        u16 *next;
        /* FAKEMATCH: native initializes the next-row pointer in r2; the loop-hoisted ordinary expression uses r3 (34 differences). */
        register u16 *p asm("r2") = rows;
        /* FAKEMATCH: native resets pass in r1 between the row-pointer load and add (20 differences). */
        register u32 i asm("r1");
        u16 *head;
        /* FAKEMATCH: native loads rows before resetting pass; reserve its reset register until after this load. */
        asm("" : "+r"(p) : : "r1");
        i = 0;
        /* FAKEMATCH: retain the native pointer register before its spill. */
        asm("" : "+r"(p) : "r"(i));
        head = p + 1;
    for (pass = i; pass < 4; pass++) {
        u32 ty;
        u32 color;
        u32 px;
        u32 sub;

        color = pass == 3 ? 0x1111 : 0x5555;
        {
        /* FAKEMATCH: native reloads win into r0 for flags; the zero-width constraint otherwise chooses r2. */
        register struct UiTextWindow *wp asm("r0") = win;
        /* FAKEMATCH: retain the native window reload register. */
        asm("" : : "r"(wp));
        if (wp->flags & 64) {
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
        }
        row = 0;
        next = head;
        bits = rows[0];
        bits <<= 16;
        while (y + row < 32) {
            /* FAKEMATCH: native keeps the next row in r4 across the pixel loop; ordinary nextrow changes allocation (105). */
            register u32 nextrow asm("r4");
            col = 0;
            /* FAKEMATCH: the native column reset precedes the next-row add. */
            asm("" : : "r"(col));
            nextrow = row + 1;
            /* FAKEMATCH: native schedules next row before the tile coordinates; a source nextrow local gives 105 differences. */
            asm("" : : "r"(nextrow));
            ty = (y + row) >> 3;
            sub = ((y + row) & 7) * 2;
            while (col < 14) {
                if (bits < 0) {
                    px = x + col;
                    /* FAKEMATCH: UiText_PlotPixel constrains the native shift-before-address order (42 to34). */
                    UiText_PlotPixel(width, px, ty, sub, color);
                }
                if ((bits <<= 1) == 0)
                    break;
                col++;
            }
            if (++row >= 12)
                break;
            {
                /* FAKEMATCH: native reloads the row pointer into r1, interleaving the y test; ordinary reload uses r3 (30 differences). */
                register u16 *p asm("r1") = next;
                /* FAKEMATCH: native holds the next-row y coordinate in r3. */
                register u32 line asm("r3");
                /* FAKEMATCH: retain the measured reload register before postincrement. */
                asm("" : : "r"(p) : "r3");
                line = y + row;
                /* FAKEMATCH: native computes the row coordinate after loading next and before loading its bits. */
                asm("" : : "r"(line) : "memory");
                bits = *p++;
                /* FAKEMATCH: native retains r1 through the postincrement and stack write. */
                asm("" : : "r"(p));
                next = p;
            }
            bits <<= 16;
        }
    }
    }
    return ret;
#endif
}
