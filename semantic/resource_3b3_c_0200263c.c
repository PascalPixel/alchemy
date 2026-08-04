#include "types.h"

/*
 * Resource 3b3 unindexed helper at 0x0200263c (88 bytes incl. pool,
 * 3 calls).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,r6,lr}` + 8-byte stack buffer at 0x0200263c, epilogue `add sp,#8
 * / pop {r5,r6} / pop {r0} / bx r0` at 0x02002680-0x02002686. The
 * trailing pool at 0x02002688-0x02002690 (0x000001f5, 0x0200a0b9,
 * 0x0200ad64 -- same three constants as 0x020020f0 earlier in this
 * overlay) is included per the usual pool rule, immediately followed
 * by the next owner's push {lr} at 0x02002694, already this overlay's
 * row `0x02002694 | 4 calls`, so the span is 0x0200263c-0x02002694,
 * 88 bytes.
 *
 * A close sibling of 0x020020f0: guarded by a check at `a0 + 0x1f5`,
 * install a handler pointer (`0x0200a0b9`) into the fetched object's
 * `+0x6c` field, run a setup call with grid-coordinate conversion, and
 * a final call with a second handler address (`0x0200ad64`). No-op if
 * the guard fails.
 *
 * Raw callee naming.
 */

u8 *Func_0808a080();
s32 Func_080770c0();
void Func_08009080();
void Func_080091c0();
void Func_0808a098();

void Func_0200263c(u8 *a0)
{
    u8 *obj = Func_0808a080(a0);

    if (Func_080770c0(a0 + 0x1f5) != 0) {
        Func_08009080(obj, 5);

        *(u32 *)(obj + 0x6c) = 0x0200a0b9;

        Func_080091c0(20, 14, 1, 1, *(s32 *)(obj + 8) >> 20, *(s32 *)(obj + 16) >> 20);
        Func_0808a098(a0, 0x0200ad64);
    }
}
