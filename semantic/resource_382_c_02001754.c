#include "types.h"
extern s32 Func_03000380(s32,s32);

void Func_02001754(u8 *object)
{
    s32 velocity;

    s32 permuted_7;
    s32 permuted_10;
    *(s32 *)(object + 8) += *(s32 *)(object + 48);
    *(s32 *)(object + 56) = *(s32 *)(object + 8);
    if (*(s16 *)(object + 100) != 0) {
        *(s32 *)(object + 12) += *(s32 *)(object + 52);
    } else {
        *(s32 *)(object + 16) += *(s32 *)(object + 52);
        *(s32 *)(object + 64) = *(s32 *)(object + 16);
        *(s32 *)(object + 12) += 0x400;
    }
    *(s32 *)(object + 60) = *(s32 *)(object + 12);

    permuted_7 = *(s32 *)(object + 48);
    *(s32 *)(object + 48) = velocity - Func_03000380(velocity, 28);
    permuted_10 = *(s32 *)(object + 52);
    velocity  = permuted_7;
    velocity  = permuted_10;
    *(s32 *)(object + 52) = velocity - Func_03000380(velocity, 28);
}
