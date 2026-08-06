#include "types.h"

/*
 * Resource 3b1 unindexed helper at 0x02005068 (124 bytes incl. pool,
 * 1 call).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,r6,lr}` at 0x02005068, epilogue `pop {r5,r6} / pop {r0} / bx r0`
 * at 0x020050d4-0x020050d8. The two-word pool at 0x020050dc-0x020050e0
 * (0x0200e840, 0x0200e8e4, both handle/pointer values shared with
 * 0x02004198 earlier in this overlay) is included per the usual pool
 * rule, immediately followed by the next owner's push
 * {r5,r6,r7,lr} at 0x020050e4, already this overlay's row
 * `0x020050e4 | 3 calls`, so the span is 0x02005068-0x020050e4,
 * 124 bytes.
 *
 * A flat setter sequence, no branches.
 *
 * Raw callee naming.
 */











extern u8 * Func_0200a018();
extern void Func_0200b53e(void);
extern void Func_02009968();
extern void Func_02009972();
extern void Func_02008842(void);
extern void Func_02009980();
extern void Func_0200b5f0();
extern void Func_0200b5aa();
extern void Func_0200b5a0();
extern void Func_0200b5b8();
extern void Func_0200b5c2();
extern void Func_0200b5ca();
extern void Func_0200a1b2(void);
extern void Func_0200b5a2(void);
void Func_02005068(void)
{
    u8 *obj = Func_0200a018(0, 0);

    Func_0200b53e();
    Func_02009968(24, 1, 0);
    Func_02009972(25, 2, 0);
    Func_02008842();
    Func_02009980(19, obj, 12);
    Func_0200b5f0(10, 6);
    Func_0200b5aa(obj, 0x0200e840);
    Func_0200b5a0(11);
    Func_0200b5b8(12, 0x0200e840);
    Func_0200b5c2(0x24, 0x0200e8e4);
    Func_0200b5ca(0x25, 0x0200e8e4);
    Func_0200a1b2();
    Func_0200b5a2();
}
