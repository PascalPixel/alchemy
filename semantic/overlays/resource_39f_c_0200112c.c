/*
 * resource_39f owner at 0x0200112c, 36 bytes: code 0x0200112c-0x02001149, an
 * alignment halfword at 0x0200114a and one pool word at 0x0200114c
 * (0x000017e6).
 *
 * See resource_39f_c_02000030.c for the link base and the `bl` encoding rule;
 * all four calls here resolve to import veneers (slots 0x02002da4, 0x02002dfc,
 * 0x02002d7c and 0x02002dac).
 *
 * The epilogue is `pop {r0} / bx r0`, so the owner is void.
 */

void Func_0808a018();
void Func_0808a100();
void Func_08015040();
void Func_0808a020();

void Func_0200112c(void)
{
    /* First branch: no argument register is written between the prologue and
     * the `bl`, so whatever the caller passed in r0-r3 reaches this import
     * unchanged.  Nothing here asserts an arity, hence the bare call. */
    Func_0808a018();

    Func_0808a100(0, 1);

    /* 0x17e6 is materialised from the pool word at 0x0200114c.  It is a plain
     * value in this overlay's numbering, not an address: it lies far below the
     * 0x02008000 link base. */
    Func_08015040(0x17e6, 1);

    /* r0 and r1 still hold the previous import's clobbered values; no
     * arguments are asserted. */
    Func_0808a020();
}
