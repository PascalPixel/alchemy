/*
 * VENEER AUDIT NOTE (2026-08-01) -- COMMENT ONLY, NO CODE CHANGE.
 *
 * This file is byte-exact, so nothing below is rewritten.  The note records
 * what the call sites actually are, so the next reader is not misled.
 *
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank: fifteen four-byte
 * `bx rN; nop` entries in register order r0..lr, ending at 0x08007320.  A
 * `bl` into that range is an INDIRECT CALL through the named register, not a
 * call to a function at the branch target.  The `Func_080072f*` prototype
 * this file declares is therefore a phantom, and the register load that
 * precedes each site -- which reads like dead code -- is the callee load.
 *
 * Why the file is still byte-identical while being wrong: a direct call to a
 * declared function at 0x080072f0 emits exactly the same `bl` the real
 * indirect call emits.  Converting to a function-pointer call would require
 * the compiler to choose the same register and therefore the same veneer
 * entry, which is a byte-exact source question and is deliberately NOT attempted
 * here.
 *
 * Sites in this owner, resolved with tools/veneer_resolve.ts:
 *
 *   0x080939ba  __call_via_r3  ->  0x030001d8
 *     a relocated IWRAM routine taking ONE argument and returning one;
 *     every site in the tree feeds it a sum of squares and consumes the
 *     result as a length. Reads as a square root; NOT asserted
 */
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

s32 Func_080072f0(s32, s32, s32, s32);
s32 Func_080022ec(s32, s32);
s32 Func_08009150(s32, s32, s32, s32);
s32 Func_08009080(s32, s32);

s32 Func_0809397c(s32 arg0)
{
    s32 squareRoot = 0x030001d8;
    s32 object;
    void *target;
    s32 deltaX;
    s32 deltaY;
    s32 cellX;
    s32 cellY;
    s32 newX;
    s32 distance;

    object = arg0;
    target = *(void **)(object + 0x68);
    if (target != 0) {
        deltaX = *(s32 *)(target + 8) - *(s32 *)(object + 8);
        if (deltaX < 0)
            deltaX += 0xffff;
        cellX = deltaX >> 16;
        deltaY = *(s32 *)(target + 0x10) - *(s32 *)(object + 0x10);
        if (deltaY < 0)
            deltaY += 0xffff;
        cellY = deltaY >> 16;
        distance = Func_080072f0(cellX * cellX + cellY * cellY,
                                 (s32)target, cellY, squareRoot);
        arg0 = *(s16 *)(object + 0x64);
        if (distance >= arg0) {
            newX = *(s32 *)(object + 8) +
                Func_080022ec(cellX << 20, arg0);
            Func_08009150(object, newX, *(s32 *)(object + 0x0c),
                          *(s32 *)(object + 0x10) +
                              Func_080022ec(cellY << 20, arg0));
            Func_08009080(object, 2);
        } else {
            Func_08009080(object, 1);
        }
    }
    return 1;
}
