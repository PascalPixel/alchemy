#include "types.h"
extern u8 *Func_0808a080(s32);
extern s32 Func_080770c0(s32);
extern void Func_0808a580(s32, s32, s32);
extern void Func_0808a3c0(s32, s32, s32);
extern void Func_080091c0(s32, s32, s32, s32, s32, s32);
extern void Func_080091b8(s32, s32, s32, s32, s32, s32);
extern void Func_0808a0f0(s32, s32, s32);
extern void Func_080091e0(u8 *, s32);
extern void Func_080000d0(u8 *, s32);
extern void Func_080770d0(s32);

/* Complete 340-byte conditional collision-map and actor-19 setup owner. */
void Func_0200049c(void)
{
    u8 *leader = Func_0808a080(0);
    s32 tileX = *(s32 *)(leader + 8);
    s32 tileZ = *(s32 *)(leader + 16);

    if (tileX < 0)
        tileX += 0xfffff;
    tileX >>= 20;
    if (tileZ < 0)
        tileZ += 0xfffff;
    tileZ >>= 20;

    if (Func_080770c0(0xf27) == 0) {
        if (tileX == 7 && tileZ == 16)
            Func_0808a580(0, 0, 16);
        Func_0808a3c0(102, -1, -1);
        Func_080091c0(28, 31, 1, 1, 7, 16);
    }

    Func_080091b8(47, 4, 1, 1, 46, 4);
    Func_080091c0(34, 37, 3, 3, 13, 3);
    Func_0808a0f0(8, 0x00e80000, 0x00480000);
    *(s32 *)(Func_0808a080(8) + 12) = 0;

    if (Func_080770c0(0x202) != 0) {
        Func_080091b8(41, 49, 3, 4, 1, 14);
        Func_080091b8(44, 49, 3, 4, 33, 14);
        Func_080091b8(47, 49, 3, 4, 1, 46);
    } else {
        u8 *actor;
        Func_0808a0f0(19, 0x00380000, 0x010c0000);
        actor = Func_0808a080(19);
        Func_080091e0(actor, 0);
        actor = Func_0808a080(19);
        if (actor != 0) {
            *(s32 *)(actor + 12) = 0x100000;
            actor[0x55] = 8;
            actor[0x23] = 2;
            *(s32 *)(actor + 28) = 0x18000;
            *(s32 *)(actor + 24) = 0x00013333;
        }
    }

    Func_080000d0((u8 *)0x02008469, 3200);
    Func_080770d0(0x201);
}
