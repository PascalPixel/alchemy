#include "effect_0809b11c.h"
#include "types.h"

struct Vector_0809aa98 {
    s32 x;
    s32 y;
    s32 z;
};

struct SceneState_0809aa98 {
    u8 unknown_00[4];
    s32 x;
    s32 y;
    s32 z;
};

extern struct SceneState_0809aa98 *Data_03001f30;

u32 Func_08004458(void);
void Func_0800447c(
    s32 magnitude,
    s32 angle,
    struct Vector_0809aa98 *vector);
void Func_080974d8(struct Vector_0809aa98 *vector);

void Func_0809aa98(struct EffectSlot *effect)
{
    for (;;) {
        s8 state = effect->state;

        if (state == 0) {
            struct Vector_0809aa98 vector;
            u16 angle;
            s32 magnitude;

            vector.x = effect->origin_x;
            vector.z = effect->origin_z;
            angle = (u16)Func_08004458();
            magnitude = (s32)(Func_08004458() * 30) + 0x280000;
            Func_0800447c(magnitude, angle, &vector);
            effect->target_x = vector.x;
            effect->target_z = vector.z;
            effect->max_speed = 0x10000;
            effect->acceleration = 0x10000;
            effect->random_value = 0;
            effect->state = (u8)state + 1;
            return;
        }

        if (state == 1) {
            if (Func_0809ba34(effect) != 0)
                return;

            effect->state = (u8)state + 1;
            continue;
        }

        if (state == 2) {
            struct Vector_0809aa98 vector;

            vector.x = Data_03001f30->x;
            vector.y = Data_03001f30->y + 0x100000;
            vector.z = Data_03001f30->z;
            Func_080974d8(&vector);
            Func_0800447c(0x10000, Func_08004458(), &vector);
            effect->target_x = vector.x;
            effect->target_z = vector.z;
            effect->max_turn_step = 0x4000;
            effect->random_value = 1;
            effect->state = (u8)state + 1;
            return;
        }

        if (state == 3 && Func_0809ba34(effect) == 0)
            Func_0809bb34(effect);
        return;
    }
}
