typedef signed int s32;

/*
 * resource_39f owner at 0x02000ea8, 56 bytes (0x02000ea8-0x02000edf).
 *
 * See resource_39f_c_02000030.c for the link base and the `bl` encoding rule.
 * Five calls resolve to import veneers and one is the intra-overlay call to
 * Func_02000e18, whose prologue is at that address.
 *
 * The epilogue is `pop {r5, r6} / pop {r0} / bx r0`, so the owner is void.
 */

void Func_0808a208();
void Func_0808a220();
void Func_0808a218();
void Func_0808a010();
void Func_02000e18();
void Func_0808a158();

void Func_02000ea8(s32 subject, s32 detail)
{
    /* 0x80 << 10 and 0x80 << 7: both are built with movs/lsls rather than
     * pooled, so they are values, not addresses. */
    Func_0808a208(0x20000, 0x4000);

    Func_0808a220(subject, 1);

    /* r0 is not reloaded between these two branches, so this import sees the
     * value Func_0808a220 left in r0.  Whether it reads it is unverified, so
     * no argument is asserted. */
    Func_0808a218();

    Func_0808a010(30);
    Func_02000e18(subject);
    Func_0808a158(subject, detail);
}
