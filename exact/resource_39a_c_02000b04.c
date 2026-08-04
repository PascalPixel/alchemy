#include "types.h"

/*
 * Resource 39a overlay scene step at 0x02000b04.
 *
 * Complete owner: `push {lr}` at 0x02000b04 and `pop {r0} / bx r0` at
 * 0x02000bae, so nothing is returned.  Five pool words follow the return.
 *
 * 0x02002e62 is reached both as a setter (0x310) and as a query (0x30c),
 * so its result is dropped at the setter site.  The first two arms share the
 * tail call 0x02002e9c(0x312) at 0x02000b70; it is written out in each arm.
 *
 * The final branch computes 0x02001e78, which falls inside the row at
 * 0x02001e08.  Like every other branch in this overlay it is recorded by
 * identity - see the note above - rather than resolved as a location.
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

/* Imports; the queried ones are typed for their return value. */
void Func_02000cac();
void Func_02000cda();
void Func_02000d02();
void Func_02001e78();
s32 Func_02002e2a();
s32 Func_02002e34();
void Func_02002e5a();
void Func_02002e60();
s32 Func_02002e62();
void Func_02002e88();
void Func_02002e96();
void Func_02002e9c();
void Func_02002ea8();
void Func_02002eb8();
void Func_02002ebe();
void Func_02002ec4();
void Func_02002e08();
void Func_02002f1a();
void Func_02002fb2();

void Func_02000b04(void)
{
    Func_02002f1a((s32)0xf1);
    /* movs r0,#0xc2 / lsls r0,#2 builds 0x308. */
    if (Func_02002e2a((s32)0x308) != 0 || Func_02002e34((s32)0x30d) != 0) {
        Func_02000cac(11, 0, -64);
        Func_02002e5a((s32)0x30f);
        Func_02002e62((s32)0x310);
        Func_02002e60((s32)0x311);
        Func_02002e9c((s32)0x312);
        /* movs r0,#0xc3 / lsls r0,#2 builds 0x30c. */
    } else if (Func_02002e62((s32)0x30c) != 0) {
        Func_02000cda(11, 0, -112);
        Func_02002e88((s32)0x30f);
        Func_02002e88((s32)0x310);
        Func_02002e96((s32)0x311);
        Func_02002e9c((s32)0x312);
    } else {
        Func_02000d02(11, 0, -128);
        Func_02002ea8((s32)0x30f);
        Func_02002eb8((s32)0x310);
        Func_02002ebe((s32)0x311);
        Func_02002ec4((s32)0x312);
    }
    Func_02002fb2((s32)0x121);
    Func_02002e08(2);
    Func_02001e78();
}
