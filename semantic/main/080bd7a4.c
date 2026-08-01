/*
 * Correctness fix, veneer audit (mars, 2026-08-01).
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank -- fifteen four-byte
 * `bx rN; nop` entries, r0..lr, ending at 0x08007320.  A `bl` into that range
 * is an indirect call through the named register.
 *
 * The regularity that makes these readable, and it is CHECKED per site rather
 * than assumed: the callee value always appeared in the draft's argument list
 * at exactly the position matching the veneer's register index.  The compiler
 * loaded the callee into rN and the draft read rN as argument N.  So a
 * __call_via_r0 site has ZERO real arguments, r1 has one, r2 has two.  Each
 * site below was confirmed by checking that the register and the argument
 * position agree.
 *
 * `state[0]` is *(u32 *)0x030000c4 -- the callee, reached through
 * __call_via_r0, so the call takes no arguments. What the routine DOES is
 * not established and is not named.
 */
typedef unsigned int u32;

typedef void (*Callee_030000C4)(void);


void Func_080bd7a4(void)
{
    u32 *dma;
    u32 *state;
    u32 source;
    u32 destination;
    u32 control;

    dma = (u32 *)0x040000d4;
    source = 0;
    destination = 0;
    control = 0x84000000;
    dma[0] = source;
    dma[1] = destination;
    dma[2] = control;

    source = 0;
    control = 0x84000000;
    dma[0] = source;
    dma[1] = destination;
    dma[2] = control;

    source = 0;
    control = 0x84000000;
    dma[0] = source;
    dma[1] = destination;
    dma[2] = control;

    state = (u32 *)0x030000c4;
    ((Callee_030000C4)state[0])();
}
