typedef unsigned short u16;
typedef signed int s32;

/*
 * Resource 3b1 unindexed helper at 0x02004880 (16 bytes, 69 calls).
 *
 * Derived span: no inventory row (item 28). `push {lr}` at 0x02004880,
 * epilogue `pop {r0} / bx r0` at 0x0200488c-0x0200488e returns void.
 * Adjoins 0x0200486c below and 0x02004890 above with no pad or pool on
 * either side.
 *
 * The second argument is masked to 16 bits (`lsls`/`lsrs` #16) before the
 * call, so it is genuinely `u16`, not a truncated `s32`.
 */

void Func_0200ae5a();

void Func_02004880(s32 a, u16 b)
{
    Func_0200ae5a(a, b, 10);
}
