typedef signed short s16;
typedef signed int s32;
typedef unsigned short u16;
typedef unsigned char u8;

/*
 * resource_3c4 owner at 0x020002a8, 164 bytes: code 0x020002a8-0x0200033f and
 * the three pool words 0x0200b1f0, 0xffff0000 and 0x0200b230 at
 * 0x02000340-0x0200034b.
 *
 * Collision/step check for one actor.  It builds a probe position one step
 * ahead of the actor along its current heading, asks the overlay's slot lookup
 * whether anything occupies that cell, and if the occupant is not one of six
 * exempt kinds it clears the actor's motion state.  It then repeats the probe
 * against a second test (import Func_080091d8) and clears the same state again
 * when that test reports a positive result.
 *
 * SHARED ROUTINE.  Halfword-identical to resource_39f's 0x020002a8 over all 82
 * halfwords except one `bl` displacement (a different veneer-table offset) and
 * two data pool words.  `tools/overlay_call_targets.ts` reports the same
 * resolved profile for both: 0x0200006c and veneer -> Func_080091d8.
 * resource_3c4's 0x0200006c is byte-identical to resource_39f's and has
 * byte-exact C in assets/code/resource_3c4_c_0200006c.c, whose "return the
 * matching slot or 0" contract is exactly how the result is used here.
 *
 * Link base 0x02008000.  Both tables were read out of this overlay's image and
 * are word-for-word the same as resource_39f's:
 *  - Data_0200b1f0 (file offset 0x31f0) is the sixteen-entry packed direction
 *    table: X step in the high halfword, Z step in the low halfword, each
 *    +/-16 whole units, promoted to 16.16 by masking with 0xffff0000 and by
 *    shifting left 16 rather than by any multiply.
 *  - Data_0200b230 (file offset 0x3230) is the six-entry exemption list:
 *    0xcf, 0xcd, 0xe4, 0xe5, 0x12a, 0x129.
 *
 * The epilogue is `pop {r5, r6, r7} / pop {r1} / bx r1`, so r0 is the result;
 * it is zeroed on every path, so the owner always returns 0.
 */

extern s32 Data_0200b1f0[];   /* 16 packed direction words */
extern s32 Data_0200b230[];   /* six exempt occupant ids */

/* The overlay's own slot lookup; byte-exact C exists for it.  It returns the
 * matching slot record or 0.  Old-style: this site also sets r1, which that
 * source does not read. */
s32 *Func_0200006c();

/* Import.  Returns a signed result that is tested with `ble`. */
s32 Func_080091d8();

s32 Func_020002a8(u8 *actor)
{
    s32 probe[3];
    s32 step;
    s32 *occupant;

    /* heading is the top nibble of the halfword at +6. */
    step = Data_0200b1f0[*(u16 *)(actor + 6) >> 12];

    probe[0] = *(s32 *)(actor + 8) + (step & (s32)0xffff0000);
    probe[1] = *(s32 *)(actor + 12);
    probe[2] = *(s32 *)(actor + 16) + (step << 16);

    occupant = Func_0200006c(probe, actor);
    if (occupant != 0) {
        s16 kind = **(s16 **)((u8 *)occupant[20] + 40);
        s32 index;

        /* Six entries are scanned, `cmp r1, #5 / bls`.  A match leaves the
         * actor's state untouched and falls straight to the return. */
        for (index = 0; index <= 5; index++) {
            if (kind == Data_0200b230[index]) return 0;
        }

        *(s32 *)(actor + 36) = 0;
        *(s32 *)(actor + 44) = 0;
        *(s32 *)(actor + 56) = (s32)0x80000000;
        *(s32 *)(actor + 64) = (s32)0x80000000;
    }

    /* The probe is rebuilt from the same table entry rather than reused; the
     * direction word is reloaded through the same scaled index in r7. */
    step = Data_0200b1f0[*(u16 *)(actor + 6) >> 12];
    probe[0] = *(s32 *)(actor + 8) + (step & (s32)0xffff0000);
    probe[1] = *(s32 *)(actor + 12);
    probe[2] = *(s32 *)(actor + 16) + (step << 16);

    if (Func_080091d8(actor, probe) > 0) {
        *(s32 *)(actor + 36) = 0;
        *(s32 *)(actor + 44) = 0;
        *(s32 *)(actor + 56) = (s32)0x80000000;
        *(s32 *)(actor + 64) = (s32)0x80000000;
    }

    return 0;
}
