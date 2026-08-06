#include "types.h"

/*
 * Resource 3c3, scene bracket at 0x02000334 (142 bytes, 11 call sites).
 *
 * Complete owner: `push {r5, lr} / sub sp, #8` at 0x02000334 and
 * `add sp, #8 / pop {r5} / pop {r0} / bx r0` at 0x020003ba.  The popped branch
 * register IS r0, so it holds the return address and the owner is `void`.
 * The span ends with the two-byte zero alignment word at 0x020003c2; there is
 * NO literal pool — every constant in this body is built with
 * `movs` + `lsls`, which is why the row runs straight into the next owner's
 * prologue at 0x020003c4.
 *
 * Shift-built constants, spelled as their values below: 0x80 << 9 = 0x10000,
 * 0x80 << 8 = 0x8000, 0xc0 << 1 = 384, 0x80 << 7 = 0x4000.
 *
 * The body is the standard begin/…/end bracket: Func_0808a018 opens it,
 * Func_0808a020 closes it.  Between them slots 8 and 9 are scaled to
 * 1.0/0.5 in 16.16 (0x10000 / 0x8000 against 0x10000 as one), placed at
 * columns 136 and 152 of row 384, faced along heading 0x4000, and slot 8 is
 * put in presentation mode 1.  The two Func_080091c0 requests share r5 = 7 as
 * their first stack argument; the second stack argument differs (27 then 26)
 * and is spelled per call.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts`; all 11 sites are
 * import veneers.  Per-target multiset, reproduced exactly by the C below:
 *   Func_080091c0 x2, Func_0808a090 x2, Func_0808a1b8 x2, Func_0808a018 x1,
 *   Func_0808a020 x1, Func_0808a0c8 x1, Func_0808a0d0 x1, Func_0808a100 x1.
 */

/* Old-style declarations: overlay import arities vary per call site. */
   /* six-argument presentation request, last two on the stack */
   /* open a scripted scene */
   /* close a scripted scene */
   /* set a slot's x/y scale in 16.16 */
   /* place an entity */
   /* place an entity */
   /* set a slot's presentation mode */
   /* face the slot along a heading */

extern void Func_02000bd6();
extern void Func_02000c04();
extern void Func_02000c12();
extern void Func_02000c2e();
extern void Func_02000c42();
extern void Func_02000cbe();
extern void Func_02000cca();
extern void Func_02000c82();
extern void Func_02000c0e();
extern void Func_02000c20();
extern void Func_02000c5c();
void Func_02000334(void)
{
    int mode = 7;

    Func_02000bd6();

    Func_02000c04(8, 0x10000, 0x8000);
    Func_02000c12(9, 0x10000, 0x8000);

    Func_02000c2e(8, 136, 384);
    Func_02000c42(9, 152, 384);

    Func_02000cbe(8, 0x4000, 0);
    Func_02000cca(9, 0x4000, 0);

    Func_02000c82(8, 1);

    Func_02000c0e(6, 27, 1, 1, mode, 27);
    Func_02000c20(9, 26, 2, 1, mode, 26);

    Func_02000c5c();
}
