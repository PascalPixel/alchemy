#include "types.h"

/*
 * Resource 3b3 unindexed helper at 0x0200101c (88 bytes incl. pool,
 * 4 calls).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,lr}` at 0x0200101c, epilogue `pop {r5} / pop {r0} / bx r0` at
 * 0x02001068-0x0200106c. The one-word pool at 0x02001070 (0x03001e70,
 * the `Data_03001e70` workspace pointer, same table-lookup shape as
 * 0x02000fcc earlier in this overlay) is included per the usual pool
 * rule, immediately followed by the next owner's push
 * {r5,r6,r7,lr} at 0x02001074, already this overlay's row
 * `0x02001074` (parked earlier in the reconstruction), so the span is
 * 0x0200101c-0x02001074, 88 bytes.
 *
 * Same workspace-keyed table lookup as 0x02000fcc
 * (`table_entry + (a1 + (a2 << 7)) * 4`), but instead of a DMA
 * transfer this merges two bitfields into `target[+1]`: bits 4-5 from
 * `a3`'s 32-bit `+0` field (bits 12-13 of it) and bits 6-7 from `a3[1]`
 * (its own top two bits); bits 0-3 of `target[+1]` are preserved. Then
 * copies `a3[2]`/`a3[3]` straight into `target[+2]`/`target[+3]`. A
 * no-op when the workspace is null.
 *
 * Raw callee naming (no calls in this function; the queue's "4 calls"
 * count is the number of external call sites reaching this row).
 */

extern u8 *Data_03001e70;

void Func_0200101c(s32 a0, s32 a1, s32 a2, u8 *a3)
{
    u8 *workspace = Data_03001e70;

    if (workspace != 0) {
        u8 *table_entry = *(u8 **)(workspace + a0 * 48 + 0x130);
        u8 *target = table_entry + (a1 + (a2 << 7)) * 4;
        u32 flags0 = *(u32 *)(a3 + 0);
        u8 packed = target[1];

        packed = (packed & 0xcf) | (u8)(((flags0 << 18) >> 30) << 4);
        packed = (packed & 0x3f) | (u8)((a3[1] >> 6) << 6);
        target[1] = packed;

        target[2] = a3[2];
        target[3] = a3[3];
    }
}
