typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

/*
 * Resource 3b3 unindexed helper at 0x02000fcc (80 bytes incl. pool,
 * 2 calls).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,lr}` at 0x02000fcc, epilogue `pop {r5} / pop {r0} / bx r0` at
 * 0x02001008-0x0200100c. The trailing pool at 0x02001010-0x02001018
 * (0x03001e70 [the `Data_03001e70` workspace pointer], 0x040000d4
 * [DMA3 source register], 0x84000001 [DMA3 control: enable | 32-bit |
 * 1 word, same encoding documented in
 * semantic/overlays/resource_36f_c_020001c0.c]) is included per the
 * usual pool rule, immediately followed by the next owner's push
 * {r5,lr} at 0x0200101c, already this overlay's row
 * `0x0200101c | 4 calls`, so the span is 0x02000fcc-0x0200101c,
 * 80 bytes.
 *
 * If the workspace is non-null, look up a table entry keyed by `a0`
 * (stride 48 bytes, base offset 0x130) and DMA3 one word from
 * `table_entry + (a1 + (a2 << 7)) * 4` into `*dest`, then spin-wait for
 * the transfer to finish. A no-op when the workspace is null.
 *
 * Raw callee naming (no calls in this function; the queue's "2 calls"
 * count is the number of external call sites reaching this row).
 */

extern u8 *Data_03001e70;

void Func_02000fcc(s32 a0, s32 a1, s32 a2, void *dest)
{
    u8 *workspace = Data_03001e70;

    if (workspace != 0) {
        u8 *table_entry = *(u8 **)(workspace + a0 * 48 + 0x130);
        void *source = table_entry + (a1 + (a2 << 7)) * 4;

        *(volatile u32 *)0x040000d4 = (u32)source;
        *(volatile u32 *)0x040000d8 = (u32)dest;
        *(volatile u32 *)0x040000dc = 0x84000001;

        while ((*(volatile u32 *)0x040000dc & 0x80000000) != 0) {
        }
    }
}
