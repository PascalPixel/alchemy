#include "types.h"

#define FieldScene_RunStepThen10 Func_0200486c
#define FieldScene_CallPairWith10 Func_02004880
void Func_0200ae2a();
void Func_0200ad38();
void Func_0200ae5a();

/*
 * Both callees live inside this overlay and are declared without a prototype,
 * so each call site fixes its own arity.
 */

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

/*
 * Shared helper for resource_3b1.  The owner at 0x0200486c is 18 bytes; the
 * halfword before the next owner at 0x02004880 is alignment, not part of it.
 */
void FieldScene_RunStepThen10(s32 a)
{
    Func_0200ae2a(a, 0);
    Func_0200ad38(10);
}

void FieldScene_CallPairWith10(s32 a, u16 b)
{
    Func_0200ae5a(a, b, 10);
}
