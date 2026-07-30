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

void Func_02006b76(s32 count);
void Func_02006c38(s32 x, s32 y, s32 z);
void Func_02006c4c(s32 a, s32 b, s32 selector);
void Func_02006c90(void);
void Func_02006ce2(void);
void Func_02006d7e(s32 channel, s32 slot, s32 value);
void Func_02006d82(s32 script);
void Func_02006da4(s32 channel, s32 slot, s32 value);
void Func_02006dae(s32 channel, s32 slot);

void Func_02000cd0(void)
{
    Func_02006c90();

    /*
     * 0xc0 << 10 and 0x80 << 9 — the reference materialises each constant
     * with `movs` plus `lsls`, so they are spelled as the values produced.
     */
    Func_02006c38(0x30000, 0x30000, 0x10000);
    Func_02006b76(10);
    Func_02006c4c(-1, -1, 0x0000e666);
    Func_02006d82(0x00001c9a);

    Func_02006da4(17, 0, 20);
    Func_02006d7e(17, 0, 20);
    Func_02006dae(17, 0);

    Func_02006ce2();
}
