#include "types.h"

extern void Func_02000844(void);
/* 0x0200089e serves two imports: the three-argument setter at +0x06 and the
 * two-argument reader at +0x36. `_b` is the same address with its own
 * prototype. */
extern void Func_0200089e(s32, s32, s32);
extern s32 Func_0200089e_b(s32, s32);
extern s32 Func_0200082c(s32);
extern void Func_020008b6(s32);
/* 0x020008c6 likewise, and in sibling arms: the two-argument setter in the
 * first and the one-argument message in the second. */
extern void Func_020008c6(s32, s32);
extern void Func_020008c6_b(s32);
extern void Func_020008d6(s32, s32);
extern void Func_02000880(s32);
extern void Func_020008f8(s32, s32);
extern void Func_0200088e(s32);
extern void Func_020008e6(s32, s32);
extern void Func_0200089c(s32);
extern s32 Func_02000898(void);
extern void Func_020008f4(s32, s32);
extern void Func_020008b2(s32);
extern void Func_02000918(s32);
extern void Func_02000930(s32, s32);
extern void Func_0200095a(s32, s32);
extern void Func_020008f2(s32, s32);
extern void Func_020008c0(s32);
extern void Func_020008f0(s32);
extern void Func_02000940(s32, s32);
extern void Func_020008fe(s32);
extern void Func_02000976(s32, s32);
extern void Func_02000992(s32, s32, s32);
extern void Func_02000926(void);

#define WORKSPACE (*(u8 **) 0x03001EBC)

void Func_02000204(void)
{
    Func_02000844();
    Func_0200089e(18, 0, 0);

    if (Func_0200082c(0x85B) == 0) {
        Func_020008b6(0x137C);
        Func_020008c6(18, 0);
    } else {
        Func_020008c6_b(0x1385);
        Func_020008d6(18, 0);
    }

    if (Func_0200089e_b(0, 0) == 0) {
        Func_02000880(20);
        Func_020008f8(18, 0);
        Func_0200088e(20);
        Func_020008e6(18, 2);
        Func_0200089c(20);

        if (Func_02000898() == 0) {
            Func_020008f4(18, 4);
            Func_020008b2(20);
            Func_02000918(0x1384);
            Func_02000930(18, 0);
        } else {
            Func_0200095a(0xE7, 3);
            Func_020008f2(0xE7, 0);
            Func_020008c0(0x85B);
        }
    } else {
        /* Skip-beat counter at workspace + 472, built as 236 << 1. */
        ++*(u16 *) (WORKSPACE + 472);
        Func_020008f0(20);
        Func_02000940(18, 3);
        Func_020008fe(20);
        Func_02000976(18, 0);
    }

    /* r1 is built as 128 << 7. */
    Func_02000992(18, 0x4000, 0);
    Func_02000926();
}
