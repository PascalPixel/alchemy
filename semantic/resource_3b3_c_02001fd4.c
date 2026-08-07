#include "types.h"

/*
 * Resource 3b3 unindexed helper at 0x02001fd4 (96 bytes, 1 call).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,lr}` at 0x02001fd4, epilogue `pop {r5} / pop {r0} / bx r0` at
 * 0x0200202c-0x02002030; the halfword at 0x02002032 is the alignment
 * pad, immediately followed by the next function's push {lr} at
 * 0x02002034 (not part of this overlay's item-28 population, already
 * indexed elsewhere), so the span is exactly 0x02001fd4-0x02002034,
 * 96 bytes.
 *
 * If `Func_02003244` returns non-zero, skip straight to the final
 * cleanup call; otherwise clear bit 0 of two objects' `+0x55`/`+0x23`
 * flag bytes, run two bare calls, then set bit 0 of two more objects'
 * `+0x55`/`+0x23` flag bytes.
 *
 * Raw callee naming.
 */

void Func_0808a018();
int Func_02001268();
u8 *Func_0808a080();
void Func_020003a8();
void Func_02001d84();
void Func_0808a020();

void Func_02001fd4(void)
{
    Func_0808a018();

    if (Func_02001268() == 0) {
        u8 *p1 = Func_0808a080(0);
        u8 *p2;
        u8 *p3;
        u8 *p4;

        p1[0x55] &= 0xfe;

        p2 = Func_0808a080(0);
        p2[0x23] &= 0xfe;

        Func_020003a8();
        Func_02001d84();

        p3 = Func_0808a080(0);
        p3[0x55] |= 1;

        p4 = Func_0808a080(0);
        p4[0x23] |= 1;
    }

    Func_0808a020();
}
