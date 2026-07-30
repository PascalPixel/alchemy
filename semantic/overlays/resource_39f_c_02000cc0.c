/*
 * resource_39f owner at 0x02000cc0, 14 bytes (0x02000cc0-0x02000ccd).
 *
 * A single two-argument import call.  See resource_39f_c_02000030.c for this
 * overlay's link base (0x02008000) and for the `bl` encoding rule that resolves
 * the callee: the stored displacement plus 2 is 0x02002e84, veneer slot 52,
 * whose word is 0x0808a259 - main-image Func_0808a258 with the Thumb bit.
 *
 * The epilogue is `pop {r0} / bx r0`: r0 holds the popped return address, so
 * nothing is returned and the owner is void.
 */

/* Imported through the overlay veneer at 0x02002e84.  Old-style: this overlay
 * reaches several imports with differing argument counts. */
void Func_0808a258();

void Func_02000cc0(void)
{
    /* r0 = 18, r1 = 2; r2 and r3 are left holding the caller's values and are
     * not asserted as arguments. */
    Func_0808a258(18, 2);
}
