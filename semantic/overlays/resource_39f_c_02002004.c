/*
 * resource_39f owner at 0x02002004, 116 bytes: code 0x02002004-0x02002071, an
 * alignment halfword at 0x02002072 and the pool word 0x0000030a at 0x02002074.
 *
 * The same scene-entry construct as 0x02001818, here for slots 18 and 22: open
 * the slot, place it, tag it, then repeat the placement in 16.16 for the second
 * slot.  744 = 0x2e8 and 504 = 0x1f8 are the whole-unit coordinates; the
 * 0x02e80000 / 0x01f80000 pair is the same numbers shifted 16 further.
 *
 * See resource_39f_c_02000030.c for the link base and the `bl` encoding rule.
 *
 * The epilogue is `pop {r0} / bx r0`, so the owner is void.
 */

void Func_0808a018();
void Func_02000ea8();
void Func_0808a210();
void Func_02000d90();
void Func_02000e18();
void Func_0808a158();
/* Used for its return value: r0 is not reloaded before Func_080091e0. */
int Func_0808a080();
void Func_080091e0();
void Func_0808a010();
void Func_080770c8();
void Func_0808a0f0();
void Func_0808a020();

void Func_02002004(void)
{
    /* No argument register is written before this branch: the caller's r0-r3
     * reach the import unchanged. */
    Func_0808a018();

    Func_02000ea8(18, 1);

    /* r1 is `movs r1,#1 / negs r1,r1`, i.e. -1, not 1. */
    Func_0808a210(744 << 16, -1, 504 << 16, 1);

    Func_02000d90(18, 744, 504, 0x90000);
    Func_02000e18(18);
    Func_0808a158(18, 15);

    /* Only r1 is set for the second branch; r0 still carries what
     * Func_0808a080 returned.  Dataflow preserved as written; whether the
     * import reads it is unverified. */
    Func_080091e0(Func_0808a080(18), 0);

    Func_0808a010(30);

    /* 0x30a from the pool word at 0x02002074. */
    Func_080770c8(0x30a);

    Func_0808a0f0(22, 744 << 16, 504 << 16);

    /* No argument registers are set. */
    Func_0808a020();
}
