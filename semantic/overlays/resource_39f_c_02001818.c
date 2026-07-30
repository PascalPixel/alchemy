/*
 * resource_39f owner at 0x02001818, 104 bytes: code 0x02001818-0x02001877 and
 * the two pool words 0x00079999 at 0x02001878 and 0x00000305 at 0x0200187c.
 *
 * A scene-entry sequence: it opens slot 14, places it at (424, 480), tags it,
 * then repeats the same coordinates in 16.16 for slot 17.  The same construct
 * with different constants sits at 0x02002004, and both drive the overlay's own
 * Func_02000ea8 / Func_02000d90 / Func_02000e18.
 *
 * See resource_39f_c_02000030.c for the link base and the `bl` encoding rule.
 *
 * The epilogue is `pop {r0} / bx r0`, so the owner is void.
 */

void Func_0808a018();
void Func_02000ea8();
void Func_02000d90();
void Func_0808a010();
void Func_02000e18();
void Func_0808a158();
/* Used for its return value: r0 is not reloaded between Func_0808a080 and
 * Func_080091e0. */
int Func_0808a080();
void Func_080091e0();
void Func_080770c8();
void Func_0808a0f0();
void Func_0808a020();

void Func_02001818(void)
{
    /* No argument register is written before this branch: the caller's r0-r3
     * reach the import unchanged. */
    Func_0808a018();

    Func_02000ea8(14, 1);

    /* 424 = 212 << 1 and 480 = 240 << 1, both built with movs/lsls.  0x79999
     * comes from the pool word at 0x02001878. */
    Func_02000d90(14, 424, 480, 0x79999);

    Func_0808a010(2);
    Func_02000e18(14);
    Func_0808a158(14, 15);

    /* Only r1 is set for the second branch; r0 still carries the value
     * Func_0808a080 left there.  Whether Func_080091e0 reads it is unverified,
     * but the dataflow is preserved as written. */
    Func_080091e0(Func_0808a080(14), 0);

    Func_0808a010(30);

    /* 0x305 from the pool word at 0x0200187c. */
    Func_080770c8(0x305);

    /* The same 424 and 480 as above, now promoted to 16.16 by shifting the
     * literals 17 instead of 1. */
    Func_0808a0f0(17, 424 << 16, 480 << 16);

    /* No argument registers are set. */
    Func_0808a020();
}
