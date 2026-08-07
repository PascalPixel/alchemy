#include "types.h"

/*
 * Resource 3bf camera/entry gate at 0x0200138c.
 *
 * Sibling of 0x02001528: same structure, same IWRAM slots, same field offsets
 * and the same four-way tail; the differences are the object kind (17 rather
 * than 18), an extra call on the object right after it is fetched, an extra
 * unconditional call before the tail decision, the two 16.16 limits
 * (0x03400000 / 0x02400000) and the final stored value (92 rather than 93).
 *
 * Two IWRAM pointer slots are used.  0x03001e70 holds a base whose +356 area
 * (`178 << 1`) is the record written below, and whose word at +76 is the
 * workspace pointer kept in r8 for the whole routine.  That word lives at
 * 0x03001e70 + 76 = 0x03001ebc, i.e. it is the same workspace pointer the
 * sibling routines in this overlay load directly from 0x03001ebc; the halfword
 * offsets used here (382, 384, 386) are in the same field block those siblings
 * touch.  r8 is pushed and popped explicitly by the prologue/epilogue, so its
 * lifetime is the whole body.
 *
 * 0x03001e40 supplies a flag word; only bit 0 is tested, and it selects
 * whether the record's two direction words (+24, +28) get +1 or -1.
 *
 * The gate then runs three guards.  If the state query for 262 reports
 * non-zero, or the halfword at workspace+382 is non-zero, or the halfword at
 * workspace+384 is zero, the routine sets the object's byte at +91 to 1 and
 * stops.  Both halfword reads are `ldrsh`, i.e. SIGNED.
 *
 * Past the gate the byte at +91 is cleared and, when neither query for 532
 * reports anything and the byte is still clear, the record's +32 and +36 words
 * are set to 0x03400000 minus the object's +8 word and 0x02400000 minus its
 * +16 word.  Both constants are built with `movs`/`lsls` (208 << 18 and
 * 144 << 18), so they are plain 16.16 limits.
 *
 * The tail is a four-way decision over the state of query 17 and the signed
 * halfword Data_02000240[294] (byte offset 588), and it ends in one of two
 * halfword stores through the shared `strh` at 0x0200162e: 0x2092 into
 * workspace+382, or 92 into workspace+386.  Note that Func_02006a9e(532) is
 * genuinely called twice on one of the paths; that is in the assembly, not a
 * transcription slip.
 *
 * The callees are other entry points of this same overlay module, named by the
 * tree's offset convention; none of them is reconstructed here.
 *
 * Link-base note.  This overlay's absolute pool constants sit 0x8000 above the
 * offsets the disassembler prints (the dispatcher at 0x02004638 is the
 * clearest witness), so the module is linked at 0x02008000.  Symbol names here
 * keep the tree's printed-offset spelling, matching the byte-exact sources
 * under assets/code.
 *
 * Uncertainty: the object returned by Func_020069e6 is only touched at +8, +16
 * and +91, so it is modelled as an opaque byte block rather than given a
 * speculative layout.
 */
s32 Func_020069e6();
void Func_02006afa();
s32 Func_020069dc();
s32 Func_02006a0e();
s32 Func_02006a20();
s32 Func_02002756();
void Func_02002dbc();
s32 Func_02002d5a();
s32 Func_02002d58();
void Func_02006a96();
void Func_02006a9e();
s32 Func_02006a9e_b();

                    

                     

void Func_0200138c(void)
{
    u8 *base = *(u8 **)0x03001e70;
    u8 *object = Func_020069e6(17);
    s32 *record = (s32 *)(base + 356);
    u8 *workspace = *(u8 **)(base + 76);
    u32 flags;
    const s16 *table = (const s16 *)0x02000240;

    Func_02006afa(object);
    flags = *(u32 *)0x03001e40;

    if ((flags & 1u) != 0u) {
        record[7] = 1;   /* +28 */
    } else {
        record[6] = 1;   /* +24 */
        record[6] = -1;
        record[7] = -1;
    }

    if (Func_020069dc(262) != 0
        || *(s16 *)(workspace + 382) != 0
        || *(s16 *)(workspace + 384) == 0) {
        object[91] = 1;
        return;
    }

    if (Func_02006a0e(532) != 0) {
        return;
    }
    object[91] = 0;

    if (Func_02006a20(532) == 0 && object[91] == 0) {
        record[8] = 0x03400000 - *(s32 *)(object + 8);    /* +32 */
        record[9] = 0x02400000 - *(s32 *)(object + 16);   /* +36 */
    }

    if (Func_02002756() != 0) {
        return;
    }

    Func_02002dbc(17);

    if (Func_02002d5a(17) != 0) {
        if (table[294] != 0) {
            *(s16 *)(workspace + 382) = 0x2092;
            return;
        }
    } else {
        if (table[294] != 0) {
            goto lastGate;
        }
    }

    if (Func_02002d58(17) != 0) {
        Func_02006a96(0x215);
        Func_02006a9e(532);
    }

lastGate:
    if (Func_02006a9e_b(532) != 0) {
        *(s16 *)(workspace + 386) = 92;
    }
}
