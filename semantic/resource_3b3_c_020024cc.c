#include "types.h"

/*
 * Resource 3b3 unindexed helper at 0x020024cc (112 bytes incl. pool,
 * 2 calls).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,r6,lr}` + 8-byte stack buffer at 0x020024cc, epilogue `add sp,#8
 * / pop {r5,r6} / pop {r0} / bx r0` at 0x0200252e-0x02002534. The
 * one-word pool at 0x02002538 (0xfffe0000) is included per the usual
 * pool rule, immediately followed by the next function's push
 * {r5,lr} at 0x0200253c (not part of this overlay's item-28
 * population, already indexed elsewhere), so the span is exactly
 * 0x020024cc-0x0200253c, 112 bytes.
 *
 * A close sibling of 0x02002498 (same fetch + grid-coordinate call
 * shape), extended with a conditional block: if the object's x
 * coordinate is exactly 16 and a lookup at a fixed address returns
 * null, install a fallback (a sentinel object pointer, clear an
 * activity byte, and stamp two fields to 0xfffe0000) before the
 * unconditional finishing call.
 *
 * Raw callee naming.
 */

u8 *Func_0808a080();
void Func_0808a018();
void Func_02000528();
s32 Func_080770c0();
void Func_0808a010();
void Func_080f9010();
void Func_080770c8();
void Func_0808a020();

void Func_020024cc(void)
{
    u8 *obj = Func_0808a080(10);
    s32 x;
    s32 z;

    Func_0808a018();

    x = *(s32 *)(obj + 8) >> 20;
    z = *(s32 *)(obj + 16) >> 20;

    Func_02000528(2, x, z, 1, 1, 255);

    x = *(s32 *)(obj + 8) >> 20;
    if (x == 16) {
        s32 fallback = Func_080770c0(0x204);

        if (fallback == 0) {
            Func_0808a010(10);
            Func_080f9010(0x9f);

            obj[0x55] = 0;
            *(u32 *)(obj + 20) = 0xfffe0000;
            *(u32 *)(obj + 12) = 0xfffe0000;

            Func_080770c8(0x204);
        }
    }

    Func_0808a020();
}
