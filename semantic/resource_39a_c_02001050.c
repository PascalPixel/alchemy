#include "types.h"

/*
 * Resource 39a overlay scene step at 0x02001050.
 *
 * Complete owner: `push {r5, lr}` and `sub sp, #8` at 0x02001050, and the
 * matching `add sp, #8 / pop {r5} / pop {r0} / bx r0` at 0x0200113e, so
 * nothing is returned.  Three pool words follow the return and are data.
 *
 * The eight bytes of frame are the outgoing stack arguments of the four
 * six-argument import calls; r5 holds the constant 8 across the first pair.
 * All sixteen branches of the row are accounted for.
 *
 * Call convention used throughout this overlay: every `bl` computes an
 * address in the band above the last code row.  The reconstruction's code ends
 * at file offset 0x2258 and the whole image is 0x3328 bytes, yet this overlay's
 * branch targets run from 0x2260 up to 0x5124 - far past the image - so an
 * encoded `bl` address is an import identity, not a place to disassemble.
 * That is the convention the byte-exact sources in this overlay already use
 * (`assets/code/resource_39a_c_02000030.c` declares `Func_02002442`), so
 * imports are named by the address their call site computes and their
 * interfaces are left open.  Declarations are old-style because one name is
 * reached with different argument counts.
 */

/* Imports; the three queried ones are typed for their return value. */




extern void Func_02003356();
extern s32 Func_02003384();
extern void Func_0200207e();
extern void Func_0200337c();
extern void Func_0200209c();
extern void Func_0200339c();
extern s32 Func_020033ca();
extern void Func_020020c4();
extern void Func_020033c4();
extern void Func_020020e4();
extern void Func_020033e4();
extern s32 Func_02003412();
extern void Func_0200210c();
extern void Func_0200340c();
extern void Func_0200212c();
extern void Func_0200342c();
void Func_02001050(void)
{
    { s32 f1 = 8; s32 g1 = 29; Func_02003356(8, 42, 15, 5,  f1, g1); }

    if (Func_02003384((s32)0x301) != 0) {
        Func_0200207e(8, 22, 31);
        { s32 f2 = 8; s32 g2 = 30; Func_0200337c(9, 30, 1, 3,  f2, g2); }
    } else {
        Func_0200209c(8, 8, 31);
        { s32 f3 = 22; s32 g3 = 30; Func_0200339c(9, 30, 1, 3,  f3, g3); }
    }

    if (Func_020033ca((s32)0x302) != 0) {
        Func_020020c4(9, 12, 29);
        { s32 f4 = 11; s32 g4 = 33; Func_020033c4(14, 33, 3, 1,  f4, g4); }
    } else {
        Func_020020e4(9, 12, 33);
        { s32 f5 = 11; s32 g5 = 29; Func_020033e4(14, 29, 3, 1,  f5, g5); }
    }

    if (Func_02003412((s32)0x303) != 0) {
        Func_0200210c(10, 18, 29);
        { s32 f6 = 17; s32 g6 = 33; Func_0200340c(14, 33, 3, 1,  f6, g6); }
    } else {
        Func_0200212c(10, 18, 33);
        { s32 f7 = 17; s32 g7 = 29; Func_0200342c(14, 29, 3, 1,  f7, g7); }
    }
}
