#include "types.h"

/*
 * resource_3a4 owner at 0x02003428, 56 bytes: compare record 0's +12
 * field against a fixed threshold and take one of two branches, the
 * taller of which also sets bit 1 of record 11's byte at +35.  A
 * PUBLISHED CALLBACK -- sweep B of cargo run --release --manifest-path tools/overlay-published/Cargo.toml --, never
 * the target of a `bl`.
 *
 * Complete owner: `push {lr}` at 0x02003428 through the interworking
 * return `pop {r0} / bx r0` at 0x0200345c-0x0200345e; the next owner's
 * `push {lr}` begins immediately at 0x02003460 with NO literal pool
 * between them, so 56 is the whole span and measureSpan agrees for once
 * -- worth saying explicitly, because the pool trap is common enough in
 * this overlay that agreement is the surprise rather than the default.
 * No arguments, void.
 *
 * Four calls, all resolved by targetOffset: two to the Func_0808a080
 * record lookup and two to the Func_0808a1e0 veneer, which the listing
 * prints as four distinct addresses.
 *
 * Uncertainty: the threshold is materialised as `movs r2,#192 / lsls
 * r2,#14`, transcribed as the literal 0x00300000 it computes rather
 * than converted to any assumed fixed-point unit; the comparison is
 * signed (`ble`), so it is spelled with a signed field.  Record ids 0,
 * 11 and 12 and the modes 2 and 3 are literal.  Record fields +12 and
 * +35 are recorded by offset with roles open; +35 is read-modify-
 * written as a flags byte.
 */

extern u8 *Func_02006fc6(s32 id);
extern u8 *Func_02006fd6(s32 id);
extern void Func_020070a8(s32 id, s32 arg1);
extern void Func_020070b2(s32 id, s32 arg1);

void Func_02003428(void)
{
    if (*(s32 *)(Func_02006fc6(0) + 12) > 0x00300000) {
        {
            u8 *flag = Func_02006fd6(11) + 35;
            s32 bit = 2;

            bit |= *flag;
            *flag = bit;
        }
        Func_020070a8(12, 3);
    } else {
        Func_020070b2(12, 2);
    }
}
