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

void Func_0200c170(void);
void Func_0200a59a();
void Func_0200a554();
void Func_0200a5b6();
void Func_0200c256();
void Func_0200c19c();
void Func_0200c2b8();
void Func_0200c2c2();
void Func_0200c25a();
void Func_0200c1c0();
void Func_0200a5f2();

void Func_02005ca4(void)
{
    Func_0200c170();
    Func_0200a59a(15, 1, 0);
    Func_0200a554(9, 0x1d4, 0x268, 0x8000);
    Func_0200a5b6(8, 1, 20);
    Func_0200c256(9, 2);
    Func_0200c19c(20);
    Func_0200c2b8(8, 0xd000, 0x50);
    Func_0200c2c2(8, 0, 20);
    Func_0200c25a(8, 3);
    Func_0200c1c0(20);
    Func_0200a5f2(9, 21, 0);
}
