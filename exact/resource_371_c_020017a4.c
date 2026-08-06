#include "types.h"

/*
 * Resource 371 owner at 0x020017a4 (88 bytes, 6 calls).
 *
 * Complete owner: `push {r5, lr}` at 0x020017a4 and the interworking return
 * `pop {r5} ; pop {r0} ; bx r0` at 0x020017f6.  r0 holds the popped return
 * address, so the owner returns nothing.  No literal pool: 0xa0000, 512 and
 * the small identifiers are all built with `movs`/`lsls`.
 *
 * Takes no arguments — r0 is loaded with 15 before the first call.  The two
 * lookups return objects; the field offsets used on them (+0x08 X, +0x10 Z,
 * +0x0c between them, +0x64 the halfword frame counter) are the layout proven
 * by the byte-exact sibling `assets/code/resource_371_c_020004a0.c` and used
 * by the owners at 0x02003fb4/0x02004004.
 *
 * The whole tail is gated twice: once on the object still being below
 * Y = 0xa0000, and once on 0x02005a5e(512) reporting zero.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes; that address is a per-call-site label for a
 * load-time-relocated import, not a place to disassemble and not a global
 * identity (see the note in resource_371_c_0200008c.c).  Old-style
 * declarations, because the interfaces are unknown.
 *
 * STILL-OPEN (8 differing bytes / candidate compiles 4 bytes LARGER than
 * the 88-byte span): the reference builds the final `*counter = 1` store
 * with `adds r2,#100` then `movs r3,#1` (no pool). Every phrasing tried
 * (nested block, direct single-statement `*(u16*)(self+0x64) = 1`)
 * instead compiles to `ldr r3,[pc,#8]` pulling 1 from a spilled literal
 * pool word, growing the function past its registered span. alchemist.ts
 * refused (tier: unaligned only). Not yet understood why gcc routes this
 * particular small immediate through the pool instead of movs.
 */

u8 *Func_02005aa6();
u8 *Func_02005aae();
s32 Func_02005a5e();
void Func_02005c80();
void Func_020059e0();
void Func_02005a80();

void Func_020017a4(void)
{
    u8 *self = Func_02005aa6(15);
    u8 *source = Func_02005aae(14);

    *(s32 *)(self + 0x08) = *(s32 *)(source + 0x08);
    *(s32 *)(self + 0x10) = *(s32 *)(source + 0x10);

    if (*(s32 *)(self + 0x0c) >= 0xa0000) {
        return;
    }
    *(s32 *)(self + 0x0c) = 0xa0000;

    if (Func_02005a5e(512) != 0) {
        return;
    }
    Func_02005c80(145);
    Func_020059e0(self, 3);
    Func_02005a80(512);
    {
        u16 *counter = (u16 *)(self + 0x64);
        u16 value = 1;
        *counter = value;
    }
}
