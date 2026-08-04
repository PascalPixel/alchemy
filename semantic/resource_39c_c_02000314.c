typedef int s32;

/*
 * resource_39c owner at 0x02000314, 60 bytes through its pool: reduce
 * three signed 16.16 coordinate differences to whole units, sum their
 * squares, pass that sum to one relocated IWRAM helper, and return the
 * helper's result unchanged.
 *
 * The executable owner runs from `push {r5, lr}` at 0x02000314 through
 * `pop {r5} / pop {r1} / bx r1` at 0x02000346-0x0200034a.  m2c_guard
 * measures 56 code bytes ending at 0x0200034c.  The sole referenced pool
 * word at 0x0200034c is the even absolute address 0x030001d8; the next
 * owner's prologue is at 0x02000350, making the whole span 60 bytes.
 *
 * The bounded call sweep classifies the only call as call_via, and the
 * pool load immediately before it establishes 0x030001d8 as its target.
 * This draft records only the visible input and output dataflow; the
 * relocated helper's game-level role remains unknown here.
 */

s32 Func_02000314(s32 *a, s32 *b)
{
    s32 dx = (*a++ - *b++) >> 16;
    s32 dy = (*a++ - *b++) >> 16;
    s32 dz = (*a - *b) >> 16;
    s32 xx = dx * dx;

    return ((s32 (*)(s32))0x030001d8)(xx + dy * dy + dz * dz);
}
