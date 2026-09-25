/* Effect slot: clear the slot, attach a new object of `kind`, place it at
   x, z with unit scale and acceleration, and start it active in mode 1. */

#include "DMA.H"
#include "EFFECT_0809B11C.H"

void *Func_08009030(s32 kind);
u32 Random16(void);

void EffectSlot_Initialize(struct EffectSlot *effect, s32 kind, s32 x, s32 z)
{
    s32 unit;
    s8 *object;
    volatile u32 zero;

    zero = 0;
    Dma_Set((const void *)&zero, effect, 0x85000012, (volatile u32 *)0x040000d4);
    object = Func_08009030(kind);
    effect->object = object;
    if (object != NULL)
        object[9] &= ~0x0c;
    EffectSlot_SetPosition(effect, x, z);
    /* FAKEMATCH: 1.0 is held in a local, set before max_speed, so the object
       reload is scheduled ahead of the three unit stores */
    unit = 0x10000;
    effect->max_speed = 0x20000;
    effect->acceleration = effect->scale_y = effect->scale_x = unit;
    effect->origin_x = x;
    effect->origin_z = z;
    ((s8 *)effect->object)[38] = 0;
    effect->flag41 = 1;
    effect->flag42 = 1;
    effect->update_motion = 1;
    effect->render = 1;
    effect->active = 1;
    effect->random_value = Random16();
    effect->flags = 4;
    EffectSlot_SetObjectMode(effect, 1);
}
