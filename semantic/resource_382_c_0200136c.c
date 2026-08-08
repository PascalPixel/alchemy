#include "types.h"

/*
 * resource_382 owner at 0x0200136c, 716 bytes: THE OVERLAY'S ENTRY
 * DRIVER -- the target of the header veneer at image offset 4
 * (bun tools-rs/overlay-driver resource_382), so the loader enters
 * here and no bl inside the image reaches it, which is why it sat in
 * no structural inventory row.
 *
 * Unlike the drivers that branch on the scene id at Data_02000240[224],
 * this one never reads it.  It is steered by the SUB-SELECTOR at
 * Data_02000240[225] together with a bank of story flags -- the same
 * warning resource_387 gave: the driver anatomy is a habit, not a rule.
 *
 * Head, unconditional:
 * - flag 0x87a: Func_0808a248(14);
 * - flag 0x200: draw panel (55, 26, 4, 2, 23, 26);
 * - Func_020000a0(0x800000, 0, 0x1a40000, 0xdf), an in-image owner;
 * - records 14 and 15 both take callback 0x02008315 at +108, with
 *   +100 set to 1 and 0 respectively;
 * - flag 0x858: place record 19 at (0xd80000, 0x1880000);
 * - flag 0x853 is read and kept; if flag 0x855 is CLEAR and bit 0 of
 *   the 0x853 result is set, record 21 takes callback 0x020083ad.
 *
 * Then two guarded cutscene runs, which are the same seventeen-call
 * sequence with different coordinates:
 * - sub-selector <= 2, flag 0x109 clear: set flag 0x867, and if flag
 *   0x855 is clear and flag 0x856 is set, run it.  Actor 2 is first
 *   placed at record 0's (+8, +16) if that record exists, then moved
 *   to (0x1900000, 0x1c00000) when the sub-selector is exactly 1 and
 *   to (0xe00000, 0xa20000) otherwise.
 * - sub-selector == 11, flags 0x855 and 2 clear and flag 0x856 set:
 *   run it with actor 2 placed at (0x280000, 0x1360000).  Flag 0x12f
 *   is then raised for the whole sub-selector-11 case, taken run or
 *   not.
 * - sub-selector == 13: flag 0x855 places record 20 at the origin.
 * Between the two, flag 0x867 places record 23 at (0x1980000,
 * 0x780000).
 * Returns 0.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x0200136c with
 * `sub sp, #8` at 0x02001370, through the epilogue at
 * 0x020015f4-0x020015fc, one alignment halfword, then the
 * fourteen-word literal pool at 0x02001600-0x02001637; the next
 * function begins at 0x02001638 (`push {r5, lr}`).
 *
 * Cross-checked per the 2026-08-01 ruling, hand-drafted from the bytes
 * first: tools/m2c_guard.ts clears this row of the call-through-
 * register hazard and finds no jump table, and reports full call
 * coverage against this draft.  Every bl was resolved as an ABSOLUTE
 * image offset (stored displacement + 2); the plain objdump listing's
 * arrows are pc-relative and wrong -- the call at 0x020013ac reads as
 * 0x0200144e there and is in fact 0x020000a0.
 *
 * Uncertainty: flag ids are literal, and the 0x853 test is written as
 * `& 1` because that is the compiled test, not because the value is
 * known to be a bitfield; record fields +100 and +108 are recorded by
 * offset with roles open, as are Func_0808a0b8's two s16 reads at +10
 * and +18.  0x02008315 and 0x020083ad are in-image code pointers
 * (image offsets 0x314 | 1 and 0x3ac | 1) installed as callbacks and
 * never called here -- data-installed pointers of the kind neither a
 * BL sweep nor the unindexed check can see.
 */

extern s16 Data_02000240[];

extern s32 Func_080770c0(s32 flagId);
extern void Func_080770d0(s32 flagId);
extern void Func_080091c0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a0f0(s32 id, s32 x, s32 z);
extern void Func_0808a248(s32 arg0);
extern void Func_0808a010(s32 arg0);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a048(s32 id, s32 arg1);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0e8(s32 id);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a110(s32 id, s32 arg1);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a150(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a170(s32 arg0);
extern void Func_0808a188(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a360(void);
extern void Func_0808a370(void);

extern void Func_020000a0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

s32 Func_0200136c(void)
{
    u8 *record;
    s32 held;

    if (Func_080770c0(0x87a) != 0) {
        Func_0808a248(14);
    }
    if (Func_080770c0(0x200) != 0) {
        Func_080091c0(55, 26, 4, 2, 23, 26);
    }
    Func_020000a0(0x800000, 0, 0x1a40000, 0xdf);

    record = Func_0808a080(14);
    *(u16 *)(record + 100) = 1;
    *(u16 *)(record + 100) = 0;
    *(s32 *)(record + 108) = 0x02008315;
    *(s32 *)(record + 108) = 0x02008315;
    record = Func_0808a080(15);

    if (Func_080770c0(0x858) != 0) {
        Func_0808a0f0(19, 0xd80000, 0x1880000);
    }
    held = Func_080770c0(0x853);
    if (Func_080770c0(0x855) == 0 && (held & 1) != 0) {
        *(s32 *)(Func_0808a080(21) + 108) = 0x020083ad;
    }

    if (Data_02000240[225] <= 2 && Func_080770c0(0x109) == 0) {
        Func_080770d0(0x867);
        if (Func_080770c0(0x855) == 0 && Func_080770c0(0x856) != 0) {
            Func_0808a018();
            record = Func_0808a080(0);
            if (record != 0) {
                Func_0808a0f0(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
            }
            if (Data_02000240[225] == 1) {
                Func_0808a0f0(2, 0x1900000, 0x1c00000);
            } else {
                Func_0808a0f0(2, 0xe00000, 0xa20000);
            }
            Func_0808a150(2, 0, 0);
            Func_0808a360();
            Func_0808a370();
            Func_0808a010(30);
            Func_0808a138(2, 2);
            Func_0808a170(0x1328);
            Func_0808a188(2, 0, 20);
            Func_0808a110(0, 3);
            Func_0808a090(2, 0xcccc, 0x6666);
            Func_0808a100(2, 2);
            record = Func_0808a080(0);
            if (record != 0) {
                Func_0808a0b8(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Func_0808a0e8(2);
            Func_0808a0f0(2, 0, 0);
            Func_0808a048(2, 0);
            Func_0808a020();
        }
    }

    if (Func_080770c0(0x867) != 0) {
        Func_0808a0f0(23, 0x1980000, 0x780000);
    }

    if (Data_02000240[225] == 11) {
        if (Func_080770c0(0x855) == 0 && Func_080770c0(0x856) != 0 &&
            Func_080770c0(2) == 0) {
            Func_0808a018();
            Func_0808a0f0(2, 0x280000, 0x1360000);
            Func_0808a150(2, 0, 0);
            Func_0808a360();
            Func_0808a370();
            Func_0808a010(30);
            Func_0808a138(2, 2);
            Func_0808a170(0x1328);
            Func_0808a188(2, 0, 20);
            Func_0808a110(0, 3);
            Func_0808a090(2, 0xcccc, 0x6666);
            Func_0808a100(2, 2);
            record = Func_0808a080(0);
            if (record != 0) {
                Func_0808a0b8(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Func_0808a0e8(2);
            Func_0808a0f0(2, 0, 0);
            Func_0808a048(2, 0);
            Func_0808a020();
        }
        Func_080770d0(0x12f);
    } else if (Data_02000240[225] == 13) {
        if (Func_080770c0(0x855) != 0) {
            Func_0808a0f0(20, 0, 0);
        }
    }
    return 0;
}
