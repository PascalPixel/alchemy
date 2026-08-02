/*
 * Resource 3b1 unindexed helper at 0x02005ca4 (108 bytes, 1 call).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {lr}` at 0x02005ca4, epilogue `pop {r0} / bx r0` at
 * 0x02005d0c-0x02005d0e; no pool, immediately followed by the next
 * owner's push {r5,r6,r7,lr} at 0x02005d10, already this overlay's row
 * `0x02005d10 | 1 call`, so the span is 0x02005ca4-0x02005d10,
 * 108 bytes.
 *
 * A flat setter sequence, no branches.
 *
 * Raw callee naming.
 */

void Func_0808a018(void);
void Func_020048e8();
void Func_02004890();
void Func_0808a138();
void Func_0808a010();
void Func_0808a1b8();
void Func_0808a110();

void Func_02005ca4(void)
{
    Func_0808a018();
    Func_020048e8(15, 1, 0);
    Func_02004890(9, 0x1d4, 0x268, 0x8000);
    Func_020048e8(8, 1, 20);
    Func_0808a138(9, 2);
    Func_0808a010(20);
    Func_0808a1b8(8, 0xd000, 0x50);
    Func_0808a1b8(8, 0, 20);
    Func_0808a110(8, 3);
    Func_0808a010(20);
    Func_020048e8(9, 21, 0);
}
