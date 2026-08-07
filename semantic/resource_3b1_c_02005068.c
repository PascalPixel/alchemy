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

u8 *Func_02004fa8();
void Func_0808a018(void);
void Func_020048e8();
void Func_020037b4(void);
void Func_0808a100();
void Func_0808a098();
void Func_0808a088();
void Func_020050e4(void);
void Func_0808a020(void);

void Func_02005068(void)
{
    u8 *obj = Func_02004fa8(0, 0);

    Func_0808a018();
    Func_020048e8(24, 1, 0);
    Func_020048e8(25, 2, 0);
    Func_020037b4();
    Func_020048e8(19, obj, 12);
    Func_0808a100(10, 6);
    Func_0808a098(obj, 0x0200e840);
    Func_0808a088(11);
    Func_0808a098(12, 0x0200e840);
    Func_0808a098(0x24, 0x0200e8e4);
    Func_0808a098(0x25, 0x0200e8e4);
    Func_020050e4();
    Func_0808a020();
}
