/*
 * resource_39f owner at 0x02001420, 52 bytes: code 0x02001420-0x0200144d, an
 * alignment halfword at 0x0200144e and one pool word at 0x02001450
 * (0x00000fd4).
 *
 * See resource_39f_c_02000030.c for the link base and the `bl` encoding rule.
 * All six calls resolve to import veneers.
 *
 * Func_0808a018 is reached twice with different argument shapes: once with the
 * caller's incoming r0 untouched and once with the pooled constant 0xfd4.
 * That is why the imports are declared old-style.
 *
 * The epilogue is `pop {r0} / bx r0`, so the owner is void.
 */

void Func_0808a018();
void Func_0808a0f0();
void Func_080770c8();
void Func_0808a398();
void Func_0808a060();
void Func_0808a020();

void Func_02001420(void)
{
    /* No argument register is written before this branch: the caller's r0-r3
     * are forwarded as-is. */
    Func_0808a018();

    Func_0808a0f0(16, 0, 0);
    Func_080770c8(0xfd4);
    Func_0808a398(0xb5, 3);
    Func_0808a060(0xb5, 0);

    /* No argument registers are set; r0-r3 hold call-clobbered values. */
    Func_0808a020();
}
