/* Near miss: differ 4 of 75 (two reordered rows). The native stores max_speed (0x20000 in r3) before it starts the unit 0x10000 in r2, with the object reload in r3 between the unit two halves. A one-pass block on the speed store orders the pair but gives the unit r3 (differ 12); reading the object first swaps r2 and r3 and disturbs the zero kept in r8 (differ 30). */
#include "types.h"
#include "effect_slot.h"
#include "field_sprite.h"
#include "resource.h"
#include "dma.h"
#include "fixed_math.h"

void EffectSlot_SetObjectMode(struct EffectSlot *effect, s32 mode);

void EffectSlot_Initialize(struct EffectSlot *effect, s32 kind, s32 x, s32 z)
{
    s32 unit;
    struct FieldSprite *object;
    volatile u32 zero;

    zero = 0;
    Dma_Set((const void *)&zero, effect, 0x85000000 | (sizeof *effect / 4),
        (volatile u32 *)0x040000d4);
    object = (struct FieldSprite *)ResourceObject_CreateFar(kind);
    effect->object = object;
    if (object != NULL)
        object->priority = 0;
    EffectSlot_SetPosition(effect, x, z);
    unit = 0x10000;
    effect->max_speed = 0x20000;
    effect->acceleration = effect->scale_y = effect->scale_x = unit;
    effect->origin_x = x;
    effect->origin_z = z;
    ((s8 *)effect->object)[26] = 0;
    effect->stop_at_target = 1;
    effect->flag42 = 1;
    effect->update_motion = 1;
    effect->render = 1;
    effect->active = 1;
    effect->random_value = Random16();
    effect->flags = 4;
    EffectSlot_SetObjectMode(effect, 1);
}
