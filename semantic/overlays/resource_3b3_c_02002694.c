typedef unsigned char u8;
typedef unsigned int u32;

/*
 * Resource 3b3 unindexed helper at 0x02002694 (36 bytes incl. pool,
 * 4 calls).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {lr}` at 0x02002694, epilogue `pop {r0} / bx r0` at
 * 0x020026ae-0x020026b0. The one-word pool at 0x020026b4 (0x02009aa9,
 * a handler address) is included per the usual pool rule (the halfword
 * at 0x020026b2 is the alignment pad), immediately followed by the
 * next function's push {r5,r6,lr} at 0x020026b8 (not part of this
 * overlay's item-28 population, already indexed elsewhere), so the
 * span is exactly 0x02002694-0x020026b8, 36 bytes.
 *
 * Fetches an object, stamps two flag bytes (`+0x22`=2, `+0x55`=0), and
 * installs a handler pointer into its `+0x6c` field.
 *
 * Raw callee naming.
 */

u8 *Func_020051b4(void);

void Func_02002694(void)
{
    u8 *obj = Func_020051b4();

    obj[0x22] = 2;
    obj[0x55] = 0;
    *(u32 *)(obj + 0x6c) = 0x02009aa9;
}
