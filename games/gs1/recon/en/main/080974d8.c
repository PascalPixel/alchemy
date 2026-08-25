#include "types.h"

struct EffectVector {
    s32 x;
    s32 y;
    s32 z;
};

extern u8 *Data_03001ebc;

void Func_08005268(struct EffectVector *position, struct EffectVector *result);

void Func_080974d8(struct EffectVector *position)
{
    u8 **data = &Data_03001ebc;

    if (*(s16 *)(*data + 0x19e) == 3) {
        struct EffectVector result;

        Func_08005268(position, &result);
        position->x = result.x << 16;
        position->z = result.y << 16;
    } else {
        u8 *state = *(u8 **)((u8 *)data - 76);
        s32 x = *(s32 *)(state + 228) & 0xffff0000;
        s32 z = *(s32 *)(state + 232) & 0xffff0000;

        position->x -= x;
        position->z -= position->y;
        position->z -= z;
    }
    position->y = 0;
}
