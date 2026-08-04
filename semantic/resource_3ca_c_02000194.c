#include "types.h"

/*
 * Resource 3ca per-frame scene driver at 0x02000194 (554 bytes,
 * 0x02000194-0x020003bd).
 *
 * Complete owner.  Prologue at 0x02000194 saves r5-r7 and lr, then r9/sl/fp,
 * then r8, and opens a 20-byte frame; the matching unwind at 0x020003ac
 * restores all of them and returns through `pop {r0} / bx r0`.  The popped
 * register is r0, so it holds the return address and the owner is `void`.
 *
 * One literal pool sits inside the span, 0x0200035a-0x020003ab.  It is reached
 * only by falling past the bare `b.n 0x020003ac` at 0x02000358 — a lone
 * forward hop with no conditional structure around it — so the pool map here
 * comes from walking the owner's control flow, not from a referenced-words
 * listing.  Pool contents:
 *
 *   0x0200035c 0x00000000   the zero stored into +85
 *   0x02000360 0x03001e70   IWRAM record-block pointer cell
 *   0x02000364 0x020097e8   } in-image state words, file offsets 0x17e8,
 *   0x02000368 0x020097ec   } 0x17ec, 0x17f0, 0x17f8, 0x17fc, 0x1800,
 *   0x0200036c 0x03000118   the relocated ARM helper called indirectly
 *   0x02000370 0x020097f0
 *   0x02000374 0x04000052   BLDALPHA
 *   0x02000378 0x020097fc
 *   0x0200037c 0x02009800
 *   0x02000380 0x02009804
 *   0x02000384 0x02009808
 *   0x02000388 0x0200980c
 *   0x0200038c 0xffff0000   the "unset" sentinel and the integer-part mask
 *   0x02000390 0x02009810
 *   0x02000394 0x02009814
 *   0x02000398 0x02009818
 *   0x0200039c 0x020097f8
 *   0x020003a0 0x03001e40
 *   0x020003a4 0x000001f7   the spawn id
 *   0x020003a8 0x02008101   Func_02000100 + Thumb bit — the spawned object's
 *                           per-frame callback
 *
 * That last word is a second independent witness for the 0x02008000 link base
 * on this overlay (0x02008101 - 1 - 0x8000 = file offset 0x100, an ordinary
 * `push {r5, r6, lr}` prologue and a strict-queue row), matching the
 * 0x020091c5 witness in 0x020012ac.  Data_0200980c is confirmed independently
 * by the byte-exact sibling assets/code/resource_3ca_c_020003c0.c, which
 * writes it.
 *
 * Calls.  Eleven `bl` sites, resolved with tools/overlay_call_targets.ts
 * (the disassembler's own annotations are wrong for overlays by
 * construction); the row's `calls=8` is the usual undercount.  Placed calls
 * per target:
 *   Func_08000118 x2  (0x020001b8, 0x020001f0)
 *   Func_0808a080 x4  (0x0200022a, 0x0200024c, 0x02000272, 0x02000298)
 *   Func_080000f8 x2  (0x020002e0, 0x020002f6)
 *   Func_080090c8 x1  (0x02000314)
 *   Func_080091e0 x1  (0x0200034c)
 *   Func_08009080 x1  (0x02000354)
 * Plus two indirect calls that are not `bl` at all and so appear in no site
 * list: `mov ip, pc / bx r3` at 0x020001c0 and 0x020001f8, with r3 loaded from
 * the pool as 0x03000118.  That is the documented interworking call into the
 * relocated ARM kernel — `mov ip, pc` sets the return address to the
 * instruction after the `bx`, and the frame stays live across it.  The pool
 * word is the code address itself, so the callee is (Helper)0x03000118 and not
 * a dereference of that cell.
 *
 * UNCERTAINTIES.  (1) The 20-byte frame is used as scratch in two places that
 * are written and never read back: the halfword at sp+18 in the first block is
 * a store/reload pair whose only effect is truncation to 16 bits, and sp+12 in
 * the last block is written and then dead.  Both are spelled for their effect
 * rather than reproduced.  (2) Func_080091e0 is reached with r0 still holding
 * the object Func_080090c8 returned; that is the natural reading but the
 * register is not reloaded, so it is recorded here.
 */

extern s32 Data_020097e8;   /* gate: run the blend-fade block          */
extern s32 Data_020097ec;   /* blend-fade frame counter                */
extern s32 Data_020097f0;   /* blend-fade base value                   */
extern s32 Data_020097f8;   /* gate: run the spawn block               */
extern s32 Data_020097fc;   /* gate: run the camera block              */
extern s32 Data_02009800;   /* camera frame counter                    */
extern s32 Data_02009804;   /* camera base, written to record + 320    */
extern s32 Data_02009808;   /* camera base, written to record + 368    */
extern s32 Data_0200980c;   /* shared 16.16 coordinate, or 0xffff0000  */
extern s32 Data_02009810;   /* per-slot 16.16 coordinate, slot 1       */
extern s32 Data_02009814;   /* per-slot 16.16 coordinate, slot 3       */
extern s32 Data_02009818;   /* per-slot 16.16 coordinate, slot 2       */

/* Relocated ARM-mode helper in IWRAM, reached by `mov ip, pc / bx r3`. */
typedef s32 (*Helper_02000194)(s32, s32);

/* Main-image imports reached through this overlay's veneer table.  Old-style
 * declarations are mandatory: import arity varies between call sites. */
s32 Func_08000118();
u8 *Func_0808a080();    /* scene-record accessor: slot id -> record */
s32 Func_080000f8();    /* ROM dispatch[7] -> random source, no arguments */
u8 *Func_080090c8();    /* spawns an object, or returns 0 */
void Func_080091e0();
void Func_08009080();

/* The callback published into the spawned object's +108 field. */
void Func_02000100();

void Func_02000194(void)
{
    u8 *records;
    u8 *record;
    s32 blend;
    s32 shift;
    s32 x;
    s32 z;

    records = *(u8 **)0x03001E70;

    /* --- blend fade ------------------------------------------------------ */
    if (Data_020097e8 != 0) {
        blend = ((Helper_02000194)0x03000118)(
                    Func_08000118(Data_020097ec << 9), 3);
        /* The result is biased by 8, scaled into the upper blend field and
         * added to the base; the store/reload through sp+18 truncates it to a
         * halfword before it reaches BLDALPHA. */
        *(volatile u16 *)0x04000052 =
            (u16)(Data_020097f0 + ((blend + 8) << 8));
        Data_020097ec += 1;
    }

    /* --- camera sway ----------------------------------------------------- */
    if (Data_020097fc != 0) {
        /*
         * The helper's result is shifted into the integer half of a 16.16
         * value and then added to every coordinate this block touches.
         */
        shift = ((Helper_02000194)0x03000118)(
                    Func_08000118(Data_02009800 << 9), 2) << 16;

        *(s32 *)(records + 320) = Data_02009804 + shift;
        *(s32 *)(records + 368) = Data_02009808 + shift;

        /*
         * 0xffff0000 is the "no coordinate stored" sentinel here, not a mask.
         * Slot 0 takes the shared coordinate for both of its fields; slots 1,
         * 3 and 2 take their own for +12 and the shared one for +20.
         */
        if (Data_0200980c != (s32)0xffff0000) {
            record = Func_0808a080(0);
            *(s32 *)(record + 12) = Data_0200980c + shift;
            *(s32 *)(record + 20) = Data_0200980c + shift;
            record[85] = 0;
        }
        if (Data_02009810 != (s32)0xffff0000) {
            record = Func_0808a080(1);
            *(s32 *)(record + 12) = Data_02009810 + shift;
            *(s32 *)(record + 20) = Data_0200980c + shift;
            record[85] = 0;
        }
        if (Data_02009814 != (s32)0xffff0000) {
            record = Func_0808a080(3);
            *(s32 *)(record + 12) = Data_02009814 + shift;
            *(s32 *)(record + 20) = Data_0200980c + shift;
            record[85] = 0;
        }
        if (Data_02009818 != (s32)0xffff0000) {
            record = Func_0808a080(2);
            *(s32 *)(record + 12) = Data_02009818 + shift;
            *(s32 *)(record + 20) = Data_0200980c + shift;
            record[85] = 0;
        }
        Data_02009800 += 1;
    }

    /* --- spawn ----------------------------------------------------------- */
    if (Data_020097f8 == 0) return;
    if ((*(s32 *)0x03001E40 & 1) == 0) return;

    /*
     * Start from the integer part of the two 16.16 coordinates at +228 and
     * +232 of the record block, scatter them by a random amount, and drop the
     * result 30 tiles down the z axis (0xf0 << 13 = 0x001e0000).
     */
    x = (*(s32 *)(records + 228) & (s32)0xffff0000)
        + Func_080000f8() * 240;
    z = (*(s32 *)(records + 232) & (s32)0xffff0000)
        + Func_080000f8() * 160
        + 0x001e0000;

    /* The three arguments below are staged through the frame first; only the
     * x and the zero are read back, and the z copy at sp+12 is dead. */
    record = Func_080090c8(0x1f7, x, 0, z);
    if (record == 0) return;

    *(void **)(record + 108) = (void *)Func_02000100;
    *(u16 *)(record + 100) = 60;   /* countdown, consumed by Func_02000100 */
    *(u16 *)(record + 102) = 1;    /* pending restart flag                 */
    record[85] = 0;
    record[35] = 2;

    /* Same +9 mode field the 0x02000da4 setup step rewrites; bits 2-3 are
     * cleared and set to 2 here, where that owner sets them to 1. */
    {
        u8 *display = *(u8 **)(record + 80);
        display[9] = (u8)((display[9] & 0xf3) | 8);
    }

    Func_080091e0(record, 0);
    Func_08009080(record, 0);
}
