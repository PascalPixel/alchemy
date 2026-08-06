#include "types.h"











/* Complete 340-byte conditional collision-map and actor-19 setup owner. */
extern u8 * Func_02001ea4(s32);
extern s32 Func_02001e82(s32);
extern void Func_02002000(s32, s32, s32);
extern void Func_02001fce(s32, s32, s32);
extern void Func_02001e8a(s32, s32, s32, s32, s32, s32);
extern void Func_02001e96(s32, s32, s32, s32, s32, s32);
extern void Func_02001eb0(s32, s32, s32, s32, s32, s32);
extern void Func_02001f6e(s32, s32, s32);
extern u8 * Func_02001f34(s32);
extern s32 Func_02001efe(s32);
extern void Func_02001ede(s32, s32, s32, s32, s32, s32);
extern void Func_02001ef0(s32, s32, s32, s32, s32, s32);
extern void Func_02001f00(s32, s32, s32, s32, s32, s32);
extern void Func_02001fc8(s32, s32, s32);
extern u8 * Func_02001f8e(s32);
extern void Func_02001f2c(u8 *, s32);
extern u8 * Func_02001f9a(s32);
extern void Func_02001eee(u8 *, s32);
extern void Func_02001f9c(s32);
void Func_0200049c(void)
{
    u8 *leader = Func_02001ea4(0);
    s32 tileX = *(s32 *)(leader + 8);
    s32 tileZ = *(s32 *)(leader + 16);

    if (tileX < 0)
        tileX += 0xfffff;
    tileX >>= 20;
    if (tileZ < 0)
        tileZ += 0xfffff;
    tileZ >>= 20;

    if (Func_02001e82(0xf27) == 0) {
        if (tileX == 7 && tileZ == 16)
            Func_02002000(0, 0, 16);
        Func_02001fce(102, -1, -1);
        Func_02001e8a(28, 31, 1, 1, 7, 16);
    }

    Func_02001e96(47, 4, 1, 1, 46, 4);
    Func_02001eb0(34, 37, 3, 3, 13, 3);
    Func_02001f6e(8, 0x00e80000, 0x00480000);
    *(s32 *)(Func_02001f34(8) + 12) = 0;

    if (Func_02001efe(0x202) != 0) {
        Func_02001ede(41, 49, 3, 4, 1, 14);
        Func_02001ef0(44, 49, 3, 4, 33, 14);
        Func_02001f00(47, 49, 3, 4, 1, 46);
    } else {
        u8 *actor;
        Func_02001fc8(19, 0x00380000, 0x010c0000);
        actor = Func_02001f8e(19);
        Func_02001f2c(actor, 0);
        actor = Func_02001f9a(19);
        if (actor != 0) {
            *(s32 *)(actor + 12) = 0x100000;
            actor[0x55] = 8;
            actor[0x23] = 2;
            *(s32 *)(actor + 28) = 0x18000;
            *(s32 *)(actor + 24) = 0x00013333;
        }
    }

    Func_02001eee((u8 *)0x02008469, 3200);
    Func_02001f9c(0x201);
}
