#include "types.h"

/*
 * Resource 3bf camera/entry gate at 0x02001528.
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
 * are set to 0x02f00000 minus the object's +8 word and 0x01f00000 minus its
 * +16 word.  Both constants are built with `movs`/`lsls` (188 << 18 and
 * 248 << 17), so they are plain 16.16 limits.
 *
 * The tail is a four-way decision over the state of query 18 and the signed
 * halfword Data_02000240[294] (byte offset 588), and it ends in one of two
 * halfword stores through the shared `strh` at 0x0200162e: 0x2092 into
 * workspace+382, or 93 into workspace+386.  Note that Func_02006c30(532) is
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
 * Uncertainty: the object returned by Func_02006b82 is only touched at +8, +16
 * and +91, so it is modelled as an opaque byte block rather than given a
 * speculative layout.
 */
u8 *Func_02006b82();
s32 Func_02006b74();
s32 Func_02006ba6();
s32 Func_02006bb8();
s32 Func_02002a96();
s32 Func_02002eec();
s32 Func_02002eea();
void Func_02006c28();
void Func_02006c30();
s32 Func_02006c30_b();

                    

                      /* result is used and cast; a void declaration hard-errors */
s32 Func_02006b82(s32 kind);
s32 Func_02006b74(s32 state);
s32 Func_02006ba6(s32 state);
s32 Func_02006bb8(s32 state);
s32 Func_02002a96(void);
s32 Func_02002eec(s32 kind);
s32 Func_02002eea(s32 kind);
void Func_02006c28(s32 id);
s32 Func_02006c30(s32 state);

void Func_02001528(void)
{
    u8 *object = (u8 *)Func_02006b82(18);
    u8 *base = *(u8 **)0x03001e70;
    s32 *record = (s32 *)(base + 356);
    u8 *workspace = *(u8 **)(base + 76);
    u32 flags = *(u32 *)0x03001e40;
    const s16 *table = (const s16 *)0x02000240;

    if ((flags & 1u) != 0u) {
        record[6] = 1;   /* +24 */
        record[7] = 1;   /* +28 */
    } else {
        record[6] = -1;
        record[7] = -1;
    }

    if (Func_02006b74(262) != 0
        || *(s16 *)(workspace + 382) != 0
        || *(s16 *)(workspace + 384) == 0) {
        object[91] = 1;
        return;
    }

    if (Func_02006ba6(532) != 0) {
        return;
    }
    object[91] = 0;

    if (Func_02006bb8(532) == 0 && object[91] == 0) {
        record[8] = 0x02f00000 - *(s32 *)(object + 8);    /* +32 */
        record[9] = 0x01f00000 - *(s32 *)(object + 16);   /* +36 */
    }

    if (Func_02002a96() != 0) {
        return;
    }

    if (Func_02002eec(18) != 0) {
        if (table[294] != 0) {
            *(s16 *)(workspace + 382) = 0x2092;
            return;
        }
    } else {
        if (table[294] != 0) {
            goto lastGate;
        }
    }

    if (Func_02002eea(18) != 0) {
        Func_02006c28(0x215);
        Func_02006c30(532);
    }

lastGate:
    if (Func_02006c30_b(532) != 0) {
        *(s16 *)(workspace + 386) = 93;
    }
}
