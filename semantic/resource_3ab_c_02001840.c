#include "types.h"

extern u8 *Func_0808a080(s32);
extern void Func_080091e0(u8 *,s32);
extern s32 Func_080770c0(s32);
extern s32 Func_08000140(s32,s32);
extern void Func_08015250(s32);
extern void Func_080001c8(s32,s32,s32);
extern void Func_08000150(s32);

void Func_02001840(s32 actor)
{
    s32 permuted_4;
    u8 *record = Func_0808a080(actor);
    u8 *sprite = *(u8 **)(record + 80);
    s32 trajectory;

    permuted_4 = (u8)(((sprite[9] & 0xf3) | 4) & 15);
    sprite[5] &= (u8)~32;
    sprite[9]  = permuted_4;
    record[85] = 0;
    sprite[39] = 0;
    Func_080091e0(record, 0);
    record[92] = 0;

        *(s32 *)(record + 12) += 0x00200000;
    if (Func_080770c0(0x109) == 0)

    record[35] &= (u8)~1;
    record[97] = 1;
    trajectory = Func_08000140(17, 0x608);
    Func_08015250(181);
    Func_080001c8(sprite[28], 128, trajectory + 0x400);
    Func_08000150(17);

    *(s32 *)(record + 56) = *(s32 *)(record + 8);
    *(s32 *)(record + 60) = *(s32 *)(record + 12);
    record[92] = 1;
    *(s32 *)(record + 108) = 0x020097d9;
    record[86] = 0;
    *(s32 *)(record + 48) = 0;
}
