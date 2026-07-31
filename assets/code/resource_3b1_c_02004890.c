typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

/*
 * Resource 3b1 unindexed helper at 0x02004890 (28 bytes, 55 calls).
 *
 * Derived span: no inventory row (item 28). `push {r5,r6,lr}` at
 * 0x02004890, epilogue `pop {r5,r6} / pop {r0} / bx r0` at
 * 0x020048a6-0x020048aa returns void. Adjoins 0x02004880 below and
 * 0x020048ac above with no pad or pool.
 *
 * `b` and `c` are shifted left 16 (16.16 fixed point) before the first
 * call but not masked, so they stay full `s32`. `Func_0200ad8a` returns a
 * pointer whose halfword at offset 6 is then set to `d`.
 */

void Func_0200ade4();
void *Func_0200ad8a();

void Func_02004890(void *a, s32 b, s32 c, s32 d)
{
    Func_0200ade4(a, b << 16, c << 16, d);
    *(s16 *) ((u8 *) Func_0200ad8a(a) + 6) = d;
}
