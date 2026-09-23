/* Draft, not exact: 8 differing halfwords, 164-byte candidate for the
   164-byte owner (2026-09-23). Residual: the reload of effect->object is
   scheduled after the three 0x10000 stores instead of before them, which
   swaps r2 and r3 for that constant; store order and chained assignment did
   not move it. */

#include "DMA.H"
#include "EFFECT_0809B11C.H"

void *Func_08009030(s32 kind);
u32 Random16(void);

void EffectSlot_Initialize(struct EffectSlot *effect, s32 kind, s32 x, s32 z)
{
    volatile u32 zero;
    s8 *object;

    zero = 0;
    Dma_Set((const void *)&zero, effect, 0x85000012, (volatile u32 *)0x040000d4);
    object = Func_08009030(kind);
    effect->object = object;
    if (object != NULL)
        object[9] &= ~0x0c;
    EffectSlot_SetPosition(effect, x, z);
    effect->max_speed = 0x20000;
    effect->acceleration = effect->scale_y = effect->scale_x = 0x10000;
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
