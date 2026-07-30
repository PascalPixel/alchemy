#include "effect_0809b11c.h"

u8 *Func_08009030(s32 kind);
u32 Func_08004458(void);

void Func_0809ba90(
    struct EffectSlot *effect,
    s32 objectKind,
    s32 x,
    s32 z)
{
    u32 zero;
    volatile u32 *dma;
    u32 source;
    u32 destination;
    u32 control;

    zero = 0;
    dma = (volatile u32 *)0x040000d4;
    source = (u32)&zero;
    destination = (u32)effect;
    control = 0x85000012;
    dma[0] = source;
    dma[1] = destination;
    dma[2] = control;

    effect->object = Func_08009030(objectKind);
    {
        u8 *object = effect->object;
        s32 objectFlagsMask = -13;

        if (object != 0) {
            object[9] &= objectFlagsMask;
        }
    }

    Func_0809ba5c(effect, x, z);
    effect->max_speed = 0x10000;
    effect->scale_x = 0x10000;
    effect->scale_y = 0x10000;
    effect->acceleration = 0x10000;
    effect->origin_x = x;
    effect->origin_z = z;
    ((u8 *)effect->object)[38] = 0;

    effect->flag41 = 1;
    effect->flag42 = 1;
    effect->update_motion = 1;
    effect->render = 1;
    effect->active = 1;
    effect->random_value = Func_08004458();
    effect->flags = 4;

    Func_0809ba70(effect, 1);
}
