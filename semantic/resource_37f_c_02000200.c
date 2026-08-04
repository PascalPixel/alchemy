#include "types.h"

/*
 * Resource 37f, owner at 0x02000200 (66 bytes of code + a five-word literal
 * pool at 0x02000244-0x02000257).  `push {lr}` at 0x02000200, interworking
 * return `pop {r0} / bx r0` at 0x0200023e — r0 holds the popped return
 * address, so the owner is `void`.
 *
 * All six call sites were resolved with `tools/overlay_call_targets.ts`; all
 * six are import veneers.  Func_080770c0(flag) is used purely as a predicate
 * (its result is compared against 0), so it is declared returning s32.
 *
 * `Data_03001ebc` is the overlay workspace pointer, the same one the
 * byte-exact `assets/code/resource_37f_c_0200079c.c` loads.  The store is a
 * u16 1 at workspace + 370 (built as 185 << 1).
 *
 * Near-identical sibling of 0x020003bc, which differs only in the flag ids and
 * in having one extra arm.
 */

extern u8 *Data_03001ebc;

void Func_0808a018();
s32 Func_080770c0();
void Func_08015040();
void Func_0808a020();

void Func_02000200(void)
{
    Func_0808a018();

    if (Func_080770c0(0x81a) != 0) {
        Func_08015040(0x1034, 1);
    } else {
        Func_08015040(0x1031, 1);
        if (Func_080770c0(0xf01) != 0) {
            *(u16 *)(Data_03001ebc + 370) = 1;
        }
    }

    Func_0808a020();
}
