#include "effect_0809b11c.h"

struct Output_08096048 {
    s32 x;
    s32 y;
    s32 z;
};

struct PositionSource_08096048 {
    u8 padding00[8];
    struct Output_08096048 position;
};

extern s32 Data_02000240[];
extern u32 Data_03001800;

s32 Func_08092054(u32);
u32 Func_08004458(void);
void Func_0800447c(s32, s32, struct Output_08096048 *);
void Func_080974d8(struct Output_08096048 *);
void Func_080f9010(s32);

void Func_08096048(struct EffectSlot *effect)
{
    struct Output_08096048 position;
    struct PositionSource_08096048 *source;
    s32 state;
    u32 random;

    source = (struct PositionSource_08096048 *)
        Func_08092054(Data_02000240[125]);
    state = effect->state;

    if (state == 0) {
        position.x = source->position.x;
        position.y = source->position.y;
        position.z = source->position.z;

        random = Func_08004458() * 10 + 0xa0000;
        Func_0800447c(
            random,
            Func_08004458(),
            &position);
        Func_080974d8(&position);

        effect->origin_x = position.x;
        effect->origin_z = position.z;
        effect->x = position.x;
        effect->z = position.z;
        position.x = effect->x;
        position.z = effect->z;

        Func_0800447c(0x780000, 0xc000, &position);
        effect->target_x = position.x;
        effect->target_z = position.z;
        effect->acceleration = 0x10000;
        effect->max_speed = 0x50000;
        effect->flag42 = state;
        effect->state++;

        if ((Data_03001800 & 1) != 0)
            Func_080f9010(0x90);
    } else if (state == 1) {
        if (Func_0809ba34(effect) == 0)
            effect->state--;
    } else if (state == 2) {
        if (Func_0809ba34(effect) == 0)
            Func_0809bb34(effect);
    }
}
