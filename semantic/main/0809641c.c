#include "effect_0809b11c.h"
#include "types.h"

struct Vector_0809641c {
    s32 x;
    s32 y;
    s32 z;
};

struct PositionSource_0809641c {
    u8 padding00[8];
    struct Vector_0809641c position;
};

extern s32 Data_02000240[];

struct PositionSource_0809641c *Func_08092054(s32);
u32 Func_08004458(void);
void Func_0800447c(s32, s32, struct Vector_0809641c *);
void Func_080974d8(struct Vector_0809641c *);
void Func_080f9010(s32);

/*
 * Advance a rising projectile through launch, two alternating travel passes,
 * and teardown.  The first completed pass returns to state zero; the second
 * advances to cleanup after assigning its next randomized arc.
 */
void Func_0809641c(struct EffectSlot *effect)
{
    struct PositionSource_0809641c *source =
        Func_08092054(Data_02000240[125]);
    struct Vector_0809641c position;
    s32 state = effect->state;

    if (state == 0) {
        s32 magnitude;

        position.x = source->position.x;
        position.y = source->position.y + Func_08004458() * 5 + 0x0f0000;
        position.z = source->position.z;
        Func_080974d8(&position);

        magnitude = Func_08004458() * 6 + 0x020000;
        Func_0800447c(magnitude, Func_08004458(), &position);

        effect->target_x = position.x;
        effect->target_z = position.z;
        effect->x = position.x;
        effect->z = position.z - 0x640000;
        effect->acceleration = 0x030000;
        effect->max_speed = Func_08004458() * 3 + 0x030000;
        effect->scale_x = 0x010000;
        effect->scale_y = 0x010000;
        effect->flag42 = 0;
        effect->flag41 = 1;
        effect->state++;
        return;
    }

    if (state == 1 || state == 2) {
        if (Func_0809ba34(effect) == 0) {
            position.x = effect->x;
            position.z = effect->z;
            Func_0800447c(0x0c0000, Func_08004458(), &position);

            effect->target_x = position.x;
            effect->target_z = position.z;
            effect->flag41 = 0;
            effect->speed = 0x010000;
            effect->acceleration = 0;
            effect->max_speed = Func_08004458() + 0x023333;
            effect->scale_x = 0x008000;
            effect->scale_y = 0x008000;
            Func_080f9010(0x8f);

            if (state == 1)
                effect->state--;
            else
                effect->state++;
            effect->callback_delay = 6;
        }
        return;
    }

    if (state == 3)
        Func_0809bb34(effect);
}
