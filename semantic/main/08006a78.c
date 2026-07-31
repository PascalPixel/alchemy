#include "types.h"

/*
 * Retire the two-halfword record the cursor at 0x02004c28 points at and mask
 * the record's interrupt off again.
 *
 * The cursor is written three times: the record's own halfword is cleared,
 * the cursor is advanced one halfword so the following halfword can be
 * cleared through the same pointer, and the cursor is then put back where it
 * started.  Only the final store survives, so the visible effect is that both
 * halfwords are zeroed and the cursor is unchanged.
 *
 * The interrupt edit is the usual GBA critical section: IME is dropped to 0,
 * the IE bit selected by the channel index at 0x02004c20 is cleared, and IME
 * is restored from the saved copy at 0x02004c2c rather than from a constant.
 *
 * Leaf: no frame, no call, and the plain `bx lr` epilogue publishes no
 * result, so the owner is void.  One direct caller, 0x08006b70.
 */

void Func_08006a78(void)
{
    u16 **cursor = (u16 **)0x02004c28;
    volatile u16 *interrupt_master_enable = (volatile u16 *)0x04000208;
    volatile u16 *interrupt_enable = (volatile u16 *)0x04000200;
    const u8 *channel = (const u8 *)0x02004c20;
    const u16 *saved_master_enable = (const u16 *)0x02004c2c;
    u16 *record;

    record = *cursor;
    record[0] = 0;
    *cursor = record + 1;
    record[1] = 0;
    *cursor = record;

    *interrupt_master_enable = 0;
    *interrupt_enable &= ~(u16)(8 << *channel);
    *interrupt_master_enable = *saved_master_enable;
}
