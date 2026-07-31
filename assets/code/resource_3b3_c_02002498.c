typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 3b3 unindexed helper at 0x02002498 (52 bytes, 1 call).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,lr}` + 8-byte stack buffer at 0x02002498, epilogue `add sp,#8 /
 * pop {r5} / pop {r0} / bx r0` at 0x020024c4-0x020024ca, immediately
 * followed with no gap by the next function's push {r5,r6,lr} at
 * 0x020024cc (not part of this overlay's item-28 population, already
 * indexed elsewhere), so the span is exactly 0x02002498-0x020024cc,
 * 52 bytes.
 *
 * Fetches an object, converts its `+8`/`+16` fixed-point fields to
 * grid coordinates (`>> 20`), and calls a placement function with those
 * plus two trailing literal arguments (1, 0) passed on the stack.
 *
 * Raw callee naming.
 */

u8 *Func_02004fbc();
void Func_02004fb2(void);
s32 Func_020029e6();
void Func_02004fd6(void);

void Func_02002498(void)
{
    u8 *obj = Func_02004fbc(10);
    s32 x;
    s32 z;

    Func_02004fb2();

    x = *(s32 *)(obj + 8) >> 20;
    z = *(s32 *)(obj + 16) >> 20;

    Func_020029e6(2, x, z, 1, 1, 0);
    Func_02004fd6();
}
