#include "types.h"

typedef s32 (*Multiply_020015b4)(s32, s32);











/*
 * resource_399 owner at 0x020015b4, 248 bytes: advance a 64-step angle,
 * update the source object's orbit coordinate through the relocated fixed-
 * point multiply, then conditionally spawn and configure a randomized clone.
 * The `mov ip,pc / bx r3` site is a call to 0x03000118, not a return.
 */
extern s32 Func_02003756();
extern s32 Func_0200376c();
extern u32 Func_020037a0(void);
extern u32 Func_020037a6(void);
extern void Func_020037cc();
extern u8 * Func_020037f8();
extern void Func_02003860();
extern void Func_02003800();
extern void Func_0200396c();
extern void Func_02003834();
void Func_020015b4(u8 *object)
{
    Multiply_020015b4 multiply = (Multiply_020015b4)0x03000118;
    s16 *angle = (s16 *)(object + 100);
    s32 position[3];
    u32 random_a;
    u32 random_b;
    u8 *clone;
    s16 next_angle;

    next_angle = *angle + 1;
        *(s32 *)0x0200b1f0 + multiply(0x00060000,
    *(s32 *)(object + 8) =
                                     Func_02003756((s32)*angle << 10));
    *angle = (s16)((next_angle + 64) % 64);

    if (Func_0200376c(*(s32 *)0x03001e40, 3) != 0)
        return;

    position[0] = *(s32 *)(object + 8);
    position[1] = *(s32 *)(object + 12) + 0x00020000;
    position[2] = *(s32 *)(object + 16);
    random_a = Func_020037a0();
    random_b = Func_020037a6();
    Func_020037cc(random_a * 6, random_b, position);

    clone = Func_020037f8(0x11d, position[0], position[1], position[2]);
    if (clone != 0) {
        u8 *presentation = *(u8 **)(clone + 80);
        presentation[9] &= (u8)-13;
        Func_02003860(clone, 0);
        *(s32 *)(clone + 24) = 0x9999;
        *(s32 *)(clone + 28) = 0x9999;
        Func_02003800(clone, 1);
        clone[0x23] = 2;
        clone[0x55] = 0;
        Func_0200396c(clone, 9);
        Func_02003834(clone, (void *)0x0200a64c);
    }
}
