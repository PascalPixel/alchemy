typedef signed int s32;

/*
 * Resource 3c8 at 0x02003050: run one service call over slot indices 15..18.
 *
 * The owner is complete: `push {r5, lr}` at 0x02003050 and the matching
 * `pop {r5} ; pop {r0} ; bx r0` at 0x02003060..0x02003064.  No live frame or
 * register state escapes the row.
 *
 * `pop {r0} ; bx r0` means r0 holds the popped return address, so this owner
 * returns nothing.
 *
 * The loop counter is incremented before the call and tested afterwards with
 * an unsigned `bls #18`, so the call runs for 15, 16, 17 and 18.  Only r0 is
 * set at the call site, so a single argument is asserted.
 *
 * Import naming: `Func_02007f06` is the address this call site computes.  All
 * overlay branch targets in this package are recorded as-encoded identities;
 * see the note in resource_3c8_c_020002f0.c.
 */

/* Old-style declaration: the import's real interface is not known here. */
void Func_02007f06();

void Func_02003050(void)
{
    s32 slot;

    for (slot = 15; slot <= 18; slot++) {
        Func_02007f06(slot);
    }
}
