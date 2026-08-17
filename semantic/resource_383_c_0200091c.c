#include "types.h"

extern void Func_02005664(void);
extern void Func_02004f4e(s32, s32, s32);
extern s32 Func_02005644(s32);
extern void Func_0200574e(s32);
/* Two different imports collapse onto 0x0200575e: the two-argument setter in
 * the first arm and the one-argument message in the second. The `_b` alias is
 * the same address with its own prototype. */
extern void Func_0200575e(s32, s32);
extern void Func_0200575e_b(s32);
extern void Func_0200576e(s32, s32);
extern s32 Func_020056ce(s32, s32);
extern void Func_020056a0(s32);
extern void Func_02005790(s32, s32);
extern void Func_020056ae(s32);
extern void Func_02005776(s32, s32);
extern void Func_020056bc(s32);
extern s32 Func_020056b8(void);
extern void Func_0200576c(s32, s32);
extern void Func_020056d2(s32);
extern void Func_020057b0(s32);
extern void Func_020057c8(s32, s32);
extern void Func_0200586a(s32, s32);
extern void Func_02005722(s32, s32);
extern void Func_020056d8(s32);
extern void Func_02005710(s32);
extern void Func_020057b8(s32, s32);
extern void Func_0200571e(s32);
extern void Func_0200580e(s32, s32);
extern void Func_02005822(s32, s32, s32);
extern void Func_02005746(void);

void Func_0200091c(void)
{
    u8 *state;

    Func_02005664();
    Func_02004f4e(18, 0, 2);

    if (Func_02005644(0x85B) == 0) {
        Func_0200574e(0x137C);
        Func_0200575e(18, 0);
    } else {
        Func_0200575e_b(0x1385);
        Func_0200576e(18, 0);
    }

    if (Func_020056ce(0, 0) == 0) {
        Func_020056a0(20);
        Func_02005790(18, 0);
        Func_020056ae(20);
        Func_02005776(18, 2);
        Func_020056bc(20);

        if (Func_020056b8() == 0) {
            Func_0200576c(18, 4);
            Func_020056d2(20);
            Func_020057b0(0x1384);
            Func_020057c8(18, 0);
        } else {
            Func_0200586a(231, 3);
            Func_02005722(231, 0);
            Func_020056d8(0x85B);
        }
    } else {
        /* 236 << 1 = 472. */
        state = *(u8 **) 0x03001EBC;
        *(u16 *) (state + 472) += 1;
        Func_02005710(20);
        Func_020057b8(18, 3);
        Func_0200571e(20);
        Func_0200580e(18, 0);
    }

    Func_02005822(18, 0x4000, 0);   /* 128 << 7 */
    Func_02005746();
}
