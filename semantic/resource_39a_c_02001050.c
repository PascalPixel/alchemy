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
void Func_02001004();
void Func_080091c0();
s32 Func_080770c0();

void Func_02001050(void)
{
    Func_080091c0(8, 42, 15, 5, 8, 29);

    if (Func_080770c0((s32)0x301) != 0) {
        Func_02001004(8, 22, 31);
        Func_080091c0(9, 30, 1, 3, 8, 30);
    } else {
        Func_02001004(8, 8, 31);
        Func_080091c0(9, 30, 1, 3, 22, 30);
    }

    if (Func_080770c0((s32)0x302) != 0) {
        Func_02001004(9, 12, 29);
        Func_080091c0(14, 33, 3, 1, 11, 33);
    } else {
        Func_02001004(9, 12, 33);
        Func_080091c0(14, 29, 3, 1, 11, 29);
    }

    if (Func_080770c0((s32)0x303) != 0) {
        Func_02001004(10, 18, 29);
        Func_080091c0(14, 33, 3, 1, 17, 33);
    } else {
        Func_02001004(10, 18, 33);
        Func_080091c0(14, 29, 3, 1, 17, 29);
    }
}
