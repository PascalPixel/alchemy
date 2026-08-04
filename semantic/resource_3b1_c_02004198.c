#include "types.h"

/*
 * Resource 3b1 unindexed helper at 0x02004198 (128 bytes incl. pool,
 * 1 call).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,lr}` at 0x02004198, epilogue `pop {r5} / pop {r0} / bx r0` at
 * 0x0200420a-0x0200420e. The two-word pool at 0x02004210-0x02004214
 * (0x0200e840, 0x0200e8e4, both handle/pointer values) is included per
 * the usual pool rule, immediately followed by the next owner's push
 * {lr} at 0x02004218, already this overlay's row `0x02004218 | 1 call`,
 * so the span is 0x02004198-0x02004218, 128 bytes.
 *
 * A flat setter sequence, no branches. The final call
 * (`Func_0200a6d8`, same target as an earlier call in this function)
 * carries no explicit argument setup in the reference, so it is spelled
 * with no arguments here too.
 *
 * Raw callee naming.
 */

void Func_0808a018(void);
void Func_020048e8();
void Func_020037b4(void);
void Func_0808a100();
void Func_0808a098();
void Func_0808a158();
void Func_020050e4(void);
void Func_0808a020(void);

void Func_02004198(void)
{
    Func_0808a018();
    Func_020048e8(24, 1, 0);
    Func_020048e8(25, 1, 0);
    Func_020037b4();
    Func_020048e8(19, 11, 12);
    Func_0808a100(10, 6);
    Func_0808a098(12, 0x0200e840);
    Func_0808a098(0x24, 0x0200e8e4);
    Func_0808a098(0x25, 0x0200e8e4);
    Func_0808a098(0x26, 0x0200e8e4);
    Func_0808a158(0x24, 3);
    Func_0808a158(0x25, 3);
    Func_0808a158(0x26, 3);
    Func_020050e4();
    Func_0808a020();
}
