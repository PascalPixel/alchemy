#include "types.h"

/*
 * resource_3c4 owner at 0x02000f4c, 156 bytes: code 0x02000f4c-0x02000fdb and
 * the three pool words 0x03001e40, 0x00004ccc and 0x00090001 at
 * 0x02000fdc-0x02000fe7.
 *
 * One per-frame tick of a scattered-dust emitter.  It puts the subject into
 * one of two poses depending on bit 1 of the IWRAM state word at 0x03001e40,
 * and, only while bits 0 and 1 of that word are both clear, spawns one effect
 * at a randomly jittered position through this overlay's eight-argument
 * spawner Func_02000ae8.
 *
 * Targets resolved with `tools/overlay_call_targets.ts` (overlay `bl` rule
 * `true_target_offset = stored_displacement + 2`): 5 sites, 3 distinct
 * callees.  The disassembler's 0x02003fba, 0x02003fc4, 0x02003fc2,
 * 0x02003fd6 and 0x02001ab4 are artefacts; the two Func_08009080 sites and
 * the two Func_080000f8 sites each carry bit-identical `bl` halfwords yet
 * print different targets.  The real callees are the veneers at 0x02003054
 * (-> Func_08009080, twice) and 0x02003034 (-> Func_080000f8, twice) plus
 * this overlay's prologue at 0x02000ae8.
 *
 * Func_080000f8 is entry 7 of the ROM's fixed dispatch stub table at
 * 0x080000c0 (asm/080000c0.s), so it forwards to Func_08004458.  It is called
 * with no argument register set and its result is used as a small random
 * value: `r * 7 >> 16` gives 0..6, and subtracting 3 centres the jitter on
 * [-3, +3] whole units before promotion to 16.16 by `lsls #16`.
 *
 * UNCERTAINTY: naming Func_080000f8 a random source is inference from that
 * use, not from its own body, which is outside this overlay.
 *
 * Previously skipped because 0x02001ab4 looked like an in-image address that
 * is not a function start; it was the decoding artefact.
 *
 * Data_03001e40 is an IWRAM word, not overlay data - 0x03001e40 is outside the
 * 0x0200_8xxx..0200_bxxx in-image band this overlay's 0x02008000 link base
 * defines.
 *
 * flags = 0x00090001 selects exactly the two optional fields the options
 * record initialises: 0x00010000 -> options+4 (5) and 0x00080000 ->
 * options+8/options+12 (both 0x4ccc).  The rest of the 40-byte record is never
 * written and never read.
 *
 * The epilogue is `add sp,#56 / pop {r3} / mov r8,r3 / pop {r5,r6,r7} /
 * pop {r1} / bx r1`.  r1 carries the return address, so r0 survives and is the
 * result - and r0 is 0 on both paths, so the owner always returns 0.
 */

extern s32 Data_03001e40;





/* This overlay's effect spawner: four register arguments plus four stack
 * words - (x, y, z, vx, vy, vz, flags, options). */

extern void Func_02003fba();
extern void Func_02003fc4();
extern unsigned int Func_02003fc2();
extern unsigned int Func_02003fd6();
extern void Func_02001ab4();
s32 Func_02000f4c(u8 *subject)
{
    u8 options[40];        /* sp+16 */
    s32 zero;
    s32 x;
    s32 y;
    s32 z;

    if ((Data_03001e40 & 2) != 0) {
        Func_02003fba(subject, 1);
    } else {
        Func_02003fc4(subject, 2);
    }

    /* Re-read: the state word is loaded a second time through the same pool
     * slot.  `zero` is the surviving register and is reused below as the two
     * zero velocity words. */
    zero = Data_03001e40 & 3;
    if (zero != 0) {
        return 0;
    }

    *(s32 *)&options[8] = 0x4ccc;
    *(s32 *)&options[12] = 0x4ccc;
    *(s32 *)&options[4] = 5;

    /* `lsrs #16` - the reduction is unsigned, so the jitter is 0..6 before the
     * -3 bias regardless of the sign of the helper's result. */
    y = *(s32 *)(subject + 12) + 0x100000;   /* 128 << 13, one unit up */
    x = *(s32 *)(subject + 8)
        + ((s32)(((Func_02003fc2() * 7u) >> 16) - 3u) << 16);
    z = *(s32 *)(subject + 16)
        + ((s32)(((Func_02003fd6() * 7u) >> 16) - 3u) << 16);

    Func_02001ab4(x, y, z, 0, zero, zero, 0x00090001, options);
    return 0;
}
