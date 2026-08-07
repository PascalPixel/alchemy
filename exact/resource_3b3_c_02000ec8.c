/*
 * BYTE-EXACT and adopted 2026-08-07, with no compiler flags: three source-level
 * facts were needed.  The size test is unsigned (`bls', not `ble'), so the two
 * outputs are u32.  The two `record' words that travel on the stack are read
 * into their own locals, which makes both loads precede both `str [sp]'s
 * instead of interleaving load/store.  And the +0x23 flag set takes its mask
 * from a local assigned first, which is what puts the mask in r3 and the
 * loaded byte in r2.
 */
#include "types.h"

/*
 * Resource 3b3 unindexed helper at 0x02000ec8 (144 bytes, 14 calls).
 * Converted from a parked scope note and drafted alongside its sibling
 * 0x02000f58, which shares
 * the same out-param call shape against a different helper
 * (`Func_02001cc4` here, `Func_02001d54` there).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,r6,r7,lr}`, 48-byte stack frame at 0x02000ec8, epilogue `add
 * sp,#48 / pop {r5,r6,r7} / pop {r1} / bx r1` at
 * 0x02000f4e-0x02000f54, immediately followed with no gap by the next
 * owner's push {r5,r6,r7,lr} at 0x02000f58, already this overlay's
 * row `0x02000f58` (drafted alongside this one), so the span is
 * exactly 0x02000ec8-0x02000f58, 144 bytes.
 *
 * `Func_02001cc4` is the same shape of opaque out-param helper as
 * 0x02000f58's `Func_02001d54`: fills two scalars (`out20`, `out16`)
 * and a 24-byte record (bytes +8/+16, `record[2]`/`record[4]` here,
 * are the only ones read back), plus two more scalar out-params
 * (unused after the call in this function -- only the pointers
 * matter, the values written there are not read again).
 *
 * On success, tags the object (`+0x23 |= 2`), runs one setup call
 * (`Func_020039c8`), then picks between two nearly-identical finishing
 * calls (`Func_020039d8` with literals 70/40 vs `Func_020039ee` with
 * 68/40) depending on whether `out20 > out16`, and returns 1. On
 * failure from the helper, returns 0 without touching the object.
 *
 * Raw callee naming.
 */







extern u8 * Func_020039ec();
extern s32 Func_02001cc4();
extern void Func_020039c8();
extern void Func_02003970();
extern void Func_020039d8();
extern void Func_020039ee();
s32 Func_02000ec8(u8 *a0)
{
    u8 *obj = Func_020039ec(a0);
    u32 out20, out16;
    s32 out12, out8;
    s32 record[6];
    s32 r2, r4;
    u8 mask;

    if (Func_02001cc4(a0, &out20, &out16, record, &out12, &out8) == 0) {
        return 0;
    }

    r2 = record[2];
    r4 = record[4];
    Func_020039c8(2, 2, out20, out16, r2, r4);

    Func_02003970(obj, 4);
    mask = 2;
    obj[0x23] = obj[0x23] | mask;

    if (out20 > out16) {
        Func_020039d8(70, 40, record[2] + 32, record[4] + 2, out20, out16);
    } else {
        Func_020039ee(68, 40, record[2] + 32, record[4] + 2, out20, out16);
    }

    return 1;
}
