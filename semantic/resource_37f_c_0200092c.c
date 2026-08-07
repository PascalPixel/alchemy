#include "types.h"

/*
 * Resource 37f, owner at 0x0200092c (222 bytes of code, prologue
 * `push {r5, lr} / sub sp, #8` at 0x0200092c through the interworking return
 * `add sp, #8 / pop {r5} / pop {r0} / bx r0` at 0x02000a02, plus a six-word
 * literal pool at 0x02000a0c-0x02000a23).  r0 is the popped return address,
 * so the owner is `void`.
 *
 * All 13 call sites were resolved with `tools/lib/overlay_call_targets.ts`; all 13
 * are import veneers.  The inventory's `calls=12` is the documented undercount
 * (that field predates the corrected `bl` decoding), and the tool's own
 * `sites=` is the figure used here.
 *
 * Link-base witness, and the cleanest one in this overlay: the pool word
 * 0x02009ac9 is ODD, so under the 0x02008000 link base it is the Thumb entry
 * of the in-image function at file offset 0x1ac8 — i.e. Func_02001ac8, which
 * this file installs through Func_080000d0 and which is the owner that counts
 * Data_0200a69c down.  This file clears that same counter immediately before
 * installing it, which closes the loop.
 *
 * `Data_03001ebc` is the overlay workspace pointer (proven by the byte-exact
 * `assets/code/resource_37f_c_0200079c.c`).  The store is `*(s32 *)(ws + 448)
 * = 32`: r2 is first the displacement 448 and then, after `subs r2, #192`, the
 * stored value 32.  That register-reuse is the documented arithmetic trap, so
 * it is spelled explicitly here.
 *
 * Func_080091c0 is the six-argument renderer-family call; its 5th and 6th
 * arguments are the two stack words at [sp, #0] and [sp, #4].
 */

extern u8 *Data_03001ebc;
extern s32 Data_0200a69c;

void Func_02001ac8(void);

void Func_080770c8();
s32 Func_080770c0();
void Func_080000d0();
void Func_080091c0();
void Func_0808a0f0();

void Func_0200092c(void)
{
    u8 *workspace;

    Func_080770c8(324);

    *(s32 *)(workspace + 448) = 32;
    workspace = Data_03001ebc;

    if (Func_080770c0(0x814) != 0) {
        Data_0200a69c = 0;
        /* The pool word is the callback's code address plus the Thumb bit,
         * not a pointer cell. */
        Func_080000d0((void *)((s32)Func_02001ac8 | 1), 3200);
    }

    if (Func_080770c0(0x879) != 0) {
        Func_080091c0(5, 6, 1, 1, 6, 6);
        Func_080091c0(5, 6, 1, 1, 7, 6);
        Func_080091c0(5, 6, 1, 1, 8, 6);
        Func_080091c0(0, 1, 3, 1, 6, 5);
    }

    if (Func_080770c0(0x815) != 0) {
        Func_0808a0f0(8, 0x780000, 0xe80000);
        Func_080091c0(2, 10, 1, 1, 6, 14);
        Func_080091c0(2, 10, 1, 1, 7, 14);
        Func_080091c0(2, 10, 1, 1, 8, 14);
    }
}
