typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 3b3 unindexed helper at 0x0200209c (28 bytes incl. pool,
 * 1 call).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {lr}` at 0x0200209c, epilogue `pop {r0} / bx r0` at
 * 0x020020b0-0x020020b2. The one-word literal pool at 0x020020b4
 * (0x03001ee0, the `Data_03001ee0` pointer already declared in
 * assets/code/resource_3a8_c_02001930.c and
 * semantic/overlays/resource_39a_c_020017e4.c) is included per the
 * usual pool rule, immediately followed by the next function's push
 * {r5,lr} at 0x020020b8 (not part of this overlay's item-28 population,
 * already indexed elsewhere), so the span is exactly
 * 0x0200209c-0x020020b8, 28 bytes.
 *
 * Clears `Data_03001ee0[+24]` and one flag byte on the object returned
 * by `Func_02004bbe`.
 *
 * Raw callee naming.
 */

extern u8 *Data_03001ee0;

u8 *Func_02004bbe();

void Func_0200209c(void)
{
    u8 *obj = Func_02004bbe(0);

    *(s32 *)(Data_03001ee0 + 24) = 0;
    obj[0x62] = 0;
}
