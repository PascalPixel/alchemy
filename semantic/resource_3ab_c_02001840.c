#include "types.h"









extern u8 * Func_0200324c(s32);
extern void Func_02003216(u8 *,s32);
extern s32 Func_02003254(s32);
extern s32 Func_02003214(s32,s32);
extern void Func_02003284(s32);
extern void Func_0200323c(s32,s32,s32);
extern void Func_0200323a(s32);
void Func_02001840(s32 actor)
{
    s32 permuted_4;
    u8 *record = Func_0200324c(actor);
    u8 *sprite = *(u8 **)(record + 80);
    s32 trajectory;

    permuted_4 = (u8)(((sprite[9] & 0xf3) | 4) & 15);
    sprite[5] &= (u8)~32;
    sprite[9]  = permuted_4;
    record[85] = 0;
    sprite[39] = 0;
    Func_02003216(record, 0);
    record[92] = 0;

        *(s32 *)(record + 12) += 0x00200000;
    if (Func_02003254(0x109) == 0)

    record[35] &= (u8)~1;
    trajectory = Func_02003214(17, 0x608);
    record[97] = 1;
    Func_02003284(181);
    Func_0200323c(sprite[28], 128, trajectory + 0x400);
    Func_0200323a(17);

    *(s32 *)(record + 60) = *(s32 *)(record + 12);
    *(s32 *)(record + 108) = 0x020097d9;
    *(s32 *)(record + 56) = *(s32 *)(record + 8);
    record[86] = 0;
    record[92] = 1;
    *(s32 *)(record + 48) = 0;
}
