#include "types.h"

extern u8 *Func_0808a080(s32 actorId);
extern void Func_08009150(u8 *actor, s32 x, s32 y, s32 z);
extern void Func_080091c0(s32 arg0, s32 arg1, s32 arg2, s32 arg3,
                         s32 arg4, s32 arg5);
extern void Func_0808a0e8(s32 actorId);

/* Complete 132-byte actor-13/14 staging owner and two-word pool. */
void Func_020021ac(void)
{
    u8 *actor14 = Func_0808a080(14);
    u8 *actor13 = Func_0808a080(13);

    *(s32 *)(actor13 + 24) = 0x10000;
    actor13 = Func_0808a080(13);
    *(s32 *)(actor13 + 28) = 0x10000;
    actor13 = Func_0808a080(13);
    (*(u8 **)(actor13 + 80))[9] =
        (u8)(((*(u8 **)(actor13 + 80))[9] & (u8)~12) | 8);
    (*(u8 **)(actor14 + 80))[9] =
        (u8)(((*(u8 **)(actor14 + 80))[9] & (u8)~12) | 8);

    Func_08009150(actor14, *(s32 *)(actor14 + 8), 0x200000,
                  *(s32 *)(actor14 + 16));
    *(s32 *)(actor14 + 48) = 0xcccc;
    Func_0808a0e8(14);
    *(s32 *)(actor14 + 52) = 0x6666;
    Func_080091c0(20, 14, 1, 1, 22, 16);
}
