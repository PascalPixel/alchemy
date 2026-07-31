typedef signed int s32;

/*
 * Resource 373: short scripted scene beat.
 *
 * Complete owner: `push {lr}` at 0x02000cd0 through `pop {r0} ; bx r0` at
 * 0x02000d20..0x02000d22, followed by its two-word literal pool at
 * 0x02000d24.  Nothing is left live across the return; the routine takes no
 * arguments and returns none.
 *
 * All nine calls are accounted for below and none of them is an interworking
 * `call_via rN` site: r3 is never loaded with a helper address here.
 *
 * UNCERTAINTY: the called service addresses are the ones encoded in the
 * overlay image.  Several of them fall numerically inside this overlay's own
 * span, which is the same situation recorded for resource 394 — overlay-local
 * code and resident services share the 0x02000000 namespace and the module's
 * `bl` displacements are fixed up on load.  They are kept as calls; asserting
 * local owners for them would invent hidden-context ABIs.
 */

void Func_080000c0();
void Func_080091f0();
void Func_0808a018();
void Func_0808a020();
void Func_0808a148();
void Func_0808a170();
void Func_0808a180();
void Func_0808a188();

void Func_02000cd0(void)
{
    Func_0808a018();

    /*
     * 0xc0 << 10 and 0x80 << 9 — the reference materialises each constant
     * with `movs` plus `lsls`, so they are spelled as the values produced.
     */
    Func_080091f0(0x30000, 0x30000, 0x10000);
    Func_080000c0(10);
    Func_080091f0(-1, -1, 0x0000e666);
    Func_0808a170(0x00001c9a);

    Func_0808a188(17, 0, 20);
    Func_0808a148(17, 0, 20);
    Func_0808a180(17, 0);

    Func_0808a020();
}
