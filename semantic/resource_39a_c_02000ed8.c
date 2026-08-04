#include "types.h"
#define NULL ((void *)0)

/*
 * Resource 39a overlay object setup at 0x02000ed8.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02000ed8 and `pop {r5, r6} /
 * pop {r1} / bx r1` at 0x02000f2a.  r1 carries the return address, so r0
 * survives and is the result: the object the import produced, or a null
 * pointer when it produced none.
 *
 * Uncertainty: the prologue saves only r5 and r6 but the body also writes r4
 * (0x02000eda) without saving it.  r4 is dead after 0x02000ee4, so the effect
 * is a clobbered callee-saved register in the caller and nothing else; it is
 * reproduced here as a plain local.
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

/* Imports.  0x020031a2 is used for its return value. */
s32 Func_080090c8();
void Func_080091e0();
void Func_0808a160();

u8 *Func_02000ed8(s32 a, s32 b, s32 c, s32 d)
{
    u8 *object;

    /* The four incoming arguments are permuted before the call: r0<-d,
     * r1<-a, r2<-b, r3<-c. */
    object = Func_080090c8(d, a, b, c);
    if (object == NULL) {
        return NULL;
    }

    {
        u8 *record = *(u8 **)(object + 80);

        /* movs r3,#13 / negs r3,r3 gives the mask ~0x0c. */
        record[9] = (u8)((record[9] & ~0x0c) | 0x04);
        object[85] = 0;
    }

    /* r0 still holds the object here: it was never reloaded after the call. */
    Func_080091e0(object, 0);
    Func_0808a160(object, 15);
    object[35] |= 2;
    return object;
}
