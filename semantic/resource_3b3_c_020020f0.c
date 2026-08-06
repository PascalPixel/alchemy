#include "types.h"

/*
 * Resource 3b3 unindexed helper at 0x020020f0 (84 bytes incl. pool,
 * 2 calls).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,r6,lr}` + 8-byte stack buffer at 0x020020f0, epilogue `add sp,#8
 * / pop {r5,r6} / pop {r0} / bx r0` at 0x02002130-0x02002136. The
 * trailing pool at 0x02002138-0x02002140 (0x0200a0b9, 0x000001f5,
 * 0x0200ad64 -- a handler address, a byte offset, and another address)
 * is included per the usual pool rule, immediately followed by the
 * next function's push {lr} at 0x02002144 (not part of this overlay's
 * item-28 population, already indexed elsewhere), so the span is
 * exactly 0x020020f0-0x02002144, 84 bytes.
 *
 * Fetches an object, installs a handler pointer (`0x0200a0b9`, the
 * "veneer address + 1" idiom for interworking) into its `+0x6c` field,
 * then runs three setup calls: one passing the object's `+8`/`+16`
 * fields converted to grid coordinates (`>> 20`) alongside fixed
 * literals, one at `a0 + 0x1f5`, and one with a second handler address
 * (`0x0200ad64`).
 *
 * Raw callee naming.
 */








extern u8 * Func_02004c14();
extern void Func_02004c0a();
extern void Func_02004bde();
extern void Func_02004c16();
extern void Func_02004c56();
extern void Func_02004c42();
void Func_020020f0(u8 *a0)
{
    u8 *obj = Func_02004c14(a0);

    Func_02004c0a();

    *(u32 *)(obj + 0x6c) = 0x0200a0b9;

    Func_02004bde(20, 14, 1, 1, *(s32 *)(obj + 8) >> 20, *(s32 *)(obj + 16) >> 20);
    Func_02004c16(a0 + 0x1f5);
    Func_02004c56(a0, 0x0200ad64);
    Func_02004c42();
}
