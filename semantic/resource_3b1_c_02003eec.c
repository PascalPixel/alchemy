#include "types.h"

/*
 * Resource 3b1 unindexed helper at 0x02003eec (168 bytes incl. pool,
 * 1 call). A close sibling of 0x02003e34 immediately before it in this
 * overlay (same trailing pool constants, mostly-parallel call shape,
 * different callee addresses and a "16" vs "18" leading argument).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {lr}` at 0x02003eec, epilogue `pop {r0} / bx r0` at
 * 0x02003f74-0x02003f76. The trailing pool at 0x02003f78-0x02003f90
 * (0x024a0000, 0x01000001, 0x0000cccc, 0x00006666, 0x00000242,
 * 0x0000022a, 0x00001e3c) ends exactly where the next owner's `push
 * {lr}` begins (0x02003f94, already this overlay's row
 * `0x02003f94 | 1 call`), so the span is 0x02003eec-0x02003f94,
 * 168 bytes.
 *
 * A flat setter sequence, no branches. The third call
 * (`Func_02008f02`) takes no explicit argument setup and so implicitly
 * reuses whatever `Func_0200a442` last returned in r0 — spelled here as
 * an explicit pass-through to keep the call visible.
 *
 * Raw callee naming.
 */

extern u8 Value_00001e3c;

void Func_0808a018(void);
s32 Func_0808a0f0();
void Func_02005004();
void Func_020048ac();
void Func_020048e8();
void Func_0808a090();
void Func_0808a0d0();
void Func_0808a1b8();
void Func_0808a130();
void Func_0808a170();
void Func_0808a188();

void Func_02003eec(void)
{
    s32 t;

    Func_0808a018();
    t = Func_0808a0f0(0, 0, 0);
    Func_02005004(t);
    Func_0808a0f0(18, 0x960000, 0x024a0000);
    Func_020048ac(0x9c0000, -1, 0x2180000, 0x01000001);
    Func_020048e8(8, 0, 0);
    Func_0808a090(18, 0xcccc, 0x6666);
    Func_0808a0d0(18, 0xa8, 0x242);
    Func_0808a0d0(18, 0xa8, 0x22a);
    Func_0808a1b8(18, 0x8000, 20);
    Func_0808a130(18, 2);
    Func_0808a170((s32)&Value_00001e3c);
    Func_0808a188(18, 0, 20);
    Func_020048e8(9, 12, 0);
}
