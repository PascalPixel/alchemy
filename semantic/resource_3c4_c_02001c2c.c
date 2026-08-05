#include "types.h"

/*
 * resource_3c4 owner at 0x02001c2c, 216 bytes: code 0x02001c2c-0x02001cff and
 * the single pool word 0xffb00000 at 0x02001d00-0x02001d03.
 *
 * Lands the subject on top of the highest platform sharing its tile.  It scans
 * slots 10..14 (skipping the subject itself), keeps those whose whole-tile x
 * and z match the subject's, and remembers the greatest `y + 1.0` among them
 * together with that slot's index, which is written into the subject's u16 tag
 * at +100.  It then reorients the subject, moves it to (its own x, the winning
 * height, its own z), and plays the landing beat.
 *
 * 0xffb00000 is the initial "best height", -0x00500000 - five whole units
 * below zero in 12.20 - so any candidate at or above -5.0 wins.  If no slot
 * matches, that value is still what the move below is given and the tag at
 * +100 is left untouched.  That is what the assembly says; nothing here
 * guards the no-match case.
 *
 * Targets resolved with `tools/overlay_call_targets.ts` (overlay `bl` rule
 * `true_target_offset = stored_displacement + 2`): 16 sites, 7 distinct
 * callees.  This owner is a good demonstration of the rule - ten of the
 * sixteen sites are Func_0808a080 and all ten carry bit-identical `bl`
 * halfwords, yet the disassembler prints ten different targets (0x02004d56,
 * 0x02004d5e, 0x02004d6e, ... 0x02004dda).  The real callees are the veneers
 * at 0x0200310c (-> Func_0808a080, x10), 0x02003114 (-> Func_0808a090),
 * 0x02003074 (-> Func_08009150), 0x02003134 (-> Func_0808a0e8), 0x020031ac
 * (-> Func_080f9010) and 0x020030ec (-> Func_0808a010), plus this overlay's
 * prologue at 0x020011e0.
 *
 * Previously skipped because 0x02002ecc looked like an in-image address that
 * is not a function start; it was the decoding artefact for the 0x020011e0
 * call.
 *
 * Each record is re-fetched for every field read rather than held in a
 * register - hence the ten lookups for what is logically four records.  The
 * reconstruction keeps that shape.
 *
 * The epilogue is `pop {r3, r5} / mov r8,r3 / mov sl,r5 / pop {r5, r6, r7} /
 * pop {r0} / bx r0`, so the owner is void.
 */

typedef struct Slot_02001c2c {
    u8 pad0[8];
    s32 x;                 /* +8,  12.20 fixed point */
    s32 y;                 /* +12 */
    s32 z;                 /* +16, 12.20 fixed point */
} Slot_02001c2c;

/* Returns the record for the given slot. */
void Func_020011e0();
void Func_08009150();
void Func_0808a010();
Slot_02001c2c *Func_0808a080();
void Func_0808a090();
void Func_0808a0e8();
void Func_080f9010();


void Func_02001c2c(s32 subject)
{
    s32 best = (s32)0xffb00000;
    u32 index;

    for (index = 0; index <= 4; index++) {
        s32 slot = index + 10;

        if (slot == subject) continue;

        if ((Func_0808a080(slot)->x >> 20) != (Func_0808a080(subject)->x >> 20)) continue;
        if ((Func_0808a080(slot)->z >> 20) != (Func_0808a080(subject)->z >> 20)) continue;

        /* 128 << 13 = 0x00100000, one whole unit up from the candidate's own
         * height.  The test is signed (`cmp` / `bgt`), and a tie updates. */
        if (best > Func_0808a080(slot)->y + 0x100000) continue;

        best = Func_0808a080(slot)->y + 0x100000;
        *(u16 *)((u8 *)Func_0808a080(subject) + 100) = (u16)slot;
    }

    Func_0808a090(subject, 0x40000, 0x20000);   /* 128 << 11, 128 << 10 */

    /* Three separate lookups of the same record, in this order, exactly as the
     * assembly does them; the sequencing is spelled out with locals because C
     * does not fix argument evaluation order. */
    {
        Slot_02001c2c *target = Func_0808a080(subject);
        s32 x = Func_0808a080(subject)->x;
        s32 z = Func_0808a080(subject)->z;

        Func_08009150(target, x, best, z);
    }

    Func_0808a0e8(subject);
    Func_080f9010(188);
    Func_020011e0(subject);
    Func_0808a010(30);
}
