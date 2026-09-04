#include "types.h"

extern s32 *Func_02000c84(s32);
extern s32 *Func_02000c8c(s32);
extern void Func_02000c7c(s32);
extern void Func_02000c8c_b(s32);
extern void Func_02000c9c(s32);
extern void Func_02000cae(s32);
extern s32 Func_02000ca4(s32);
extern s32 Func_02000cb0(s32);
extern s32 Func_02000cba(s32);
extern void Func_02000ce2(void);
extern void Func_02000ce0(s32);
extern void Func_02000d2e(s32);
extern void Func_02000d1e(s32, s32);
extern void Func_02000cd2(s32, s32, s32, s32, s32, s32);
extern void Func_02000ce4(s32, s32, s32, s32, s32, s32);
extern void Func_02000d28(void);
extern void Func_02000d0e(s32);
extern s32 Func_02000d0e_b(s32);
extern void Func_02000d36(void);
extern void Func_02000d34(s32);
extern void Func_02000d82(s32);
extern void Func_02000d72(s32, s32);
extern void Func_02000d26(s32, s32, s32, s32, s32, s32);
extern void Func_02000d38(s32, s32, s32, s32, s32, s32);
extern void Func_02000d7c(void);
extern void Func_02000d6a(s32);

void Func_02000304(void)
{
    s32 *first = Func_02000c84(11);
    s32 *second = Func_02000c8c(12);

    if ((first[2] >> 20) == 35 && (first[4] >> 20) == 23) {
        Func_02000c7c(0x303);
    } else {
        Func_02000c8c_b(0x303);
    }

    if ((second[2] >> 20) == 35 && (second[4] >> 20) == 23) {
        Func_02000c9c(0x304);
    } else {
        Func_02000cae(0x304);
    }

    if (Func_02000ca4(0x303) != 0 || Func_02000cb0(0x304) != 0) {
        if (Func_02000cba(0x302) == 0) {
            s32 sx = 36;

            Func_02000ce2();
            Func_02000ce0(0x28);
            Func_02000d2e(0xd2);
            Func_02000d1e(17, 6);
            Func_02000cd2(0, 1, 1, 1, sx, 22);
            Func_02000ce4(0, 2, 1, 1, sx, 24);
            Func_02000d28();
        }
        Func_02000d0e(0x302);
    } else {
        if (Func_02000d0e_b(0x302) != 0) {
            s32 sx = 36;

            Func_02000d36();
            Func_02000d34(0x28);
            Func_02000d82(0xdc);
            Func_02000d72(17, 2);
            Func_02000d26(1, 1, 1, 1, sx, 22);
            Func_02000d38(1, 2, 1, 1, sx, 24);
            Func_02000d7c();
        }
        Func_02000d6a(0x302);
    }
}
