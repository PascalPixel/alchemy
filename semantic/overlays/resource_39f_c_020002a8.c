typedef signed short s16;
typedef signed int s32;
typedef unsigned short u16;
typedef unsigned char u8;

/*
 * resource_39f owner at 0x020002a8, 164 bytes: code 0x020002a8-0x0200033f and
 * the three pool words 0x0200aef8, 0xffff0000 and 0x0200af38 at
 * 0x02000340-0x0200034b.
 *
 * Collision/step check for one actor.  It builds a probe position one step
 * ahead of the actor along its current heading, asks the overlay's slot lookup
 * whether anything occupies that cell, and if the occupant is not one of six
 * exempt kinds it clears the actor's motion state.  It then repeats the probe
 * against a second test (import Func_080091d8) and clears the same state again
 * when that test reports a positive result.
 *
 * See resource_39f_c_02000030.c for the link base and the `bl` encoding rule.
 * Both calls here resolve cleanly: 0x0200006c is the overlay's own slot lookup,
 * for which assets/code/resource_39f_c_0200006c.c is already byte-exact, and
 * its use as `p = lookup(probe); if (p) { ... p[80] ... }` matches that source's
 * "return the matching slot or 0" contract exactly.  That agreement is one of
 * the independent checks on the encoding rule.
 *
 * Data_0200aef8 is a packed direction word table - the family HANDOVER records
 * for resource_373 and resource_3bd, here on a third overlay.  Sixteen entries
 * indexed by `heading >> 12`, X step in the high halfword and Z step in the
 * low halfword, each +/-16 whole units, promoted to 16.16 by masking with
 * 0xffff0000 and by shifting left 16 rather than by any multiply.  Under the
 * 0x02008000 link base it is the in-image table at file offset 0x2ef8, and its
 * contents confirm the reading: entries 0-2 and 15 are (+16, 0), 3-6 are
 * (0, +16), 7-10 are (-16, 0) and 11-14 are (0, -16).
 *
 * Data_0200af38 (file offset 0x2f38) is the six-entry exemption list:
 * 0xcf, 0xcd, 0xe4, 0xe5, 0x12a, 0x129.
 *
 * The epilogue is `pop {r5, r6, r7} / pop {r1} / bx r1`, so r0 is the result;
 * it is zeroed on every path, so the owner always returns 0.
 */

extern s32 Data_0200aef8[];   /* 16 packed direction words */
extern s32 Data_0200af38[];   /* six exempt occupant ids */

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
    step = Data_0200aef8[*(u16 *)(actor + 6) >> 12];

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
            if (kind == Data_0200af38[index]) return 0;
        }

        *(s32 *)(actor + 36) = 0;
        *(s32 *)(actor + 44) = 0;
        *(s32 *)(actor + 56) = (s32)0x80000000;
        *(s32 *)(actor + 64) = (s32)0x80000000;
    }

    /* The probe is rebuilt from the same table entry rather than reused; the
     * direction word is reloaded through the same scaled index in r7. */
    step = Data_0200aef8[*(u16 *)(actor + 6) >> 12];
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
