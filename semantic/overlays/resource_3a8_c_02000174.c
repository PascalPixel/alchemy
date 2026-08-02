typedef signed int s32;

/*
 * Complete four-byte leaf at 0x02000174: `movs r0, #0 / bx lr`.
 * It has no prologue, stack frame, pool, callees, argument reads or side
 * effects, and returns the integer zero.
 */
s32 Func_02000174(void)
{
    return 0;
}
