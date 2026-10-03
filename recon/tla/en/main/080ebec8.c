/* 2026-10-03 effect-slot suffix probe.
 * The existing draft could not compile: TYPES, EffectSlot and DMA owners
 * were not included, and its flag41 name no longer belongs to EffectSlot.
 * Current TBS EFFECT43 supplies the operation and statement order.
 * TLA creates a resource through ResourceObject_CreateFar and clears byte
 * 0x1a, already owned by FieldSprite, rather than TBS AnimationObject.flags.
 * EN native extent is 160 bytes, including its trailing alignment.
 * T0: unit before max_speed, 160-byte extent, score 360: four reordered
 * instructions, pending mode-call name, and standalone alignment.
 * T1: max_speed before unit, retained 160-byte extent, score 325:
 * five register choices, three reordered instructions, pending mode-call
 * name and standalone alignment. The natural group supplies alignment
 * and all six call relocations resolve to their native target owners.
 * STOP: the complete group retains 21 differing non-call bytes here;
 * object reload, origin stores and unit allocation still differ.
 * D0: the T0 used-object local plus an empty read/write-register and
 * memory constraint retained the 160-byte extent and all 75 instructions,
 * with identical mnemonic, store and branch counts, but 29 non-call
 * bytes still differed. The device did not match and is removed.
 * Axis stopped; two ordinary forms and one device tried. No adoption.
 */
#include "TYPES.H"
#include "EFFECT_SLOT.H"
#include "FIELD_SPRITE.H"
#include "RESOURCE.H"
#include "DMA.H"
#include "FIXED_MATH.H"

void EffectSlot_SetObjectMode(struct EffectSlot *effect, s32 mode);

/* Clear the slot, attach a resource object, place it at x, z with unit
 * scale and acceleration, and start it active in mode 1. */
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
    effect->max_speed = 0x20000;
    unit = 0x10000;
    effect->acceleration = effect->scale_y = effect->scale_x = unit;
    effect->origin_x = x;
    effect->origin_z = z;
    ((struct FieldSprite *)effect->object)->unknown_1a = 0;
    effect->stop_at_target = 1;
    effect->flag42 = 1;
    effect->update_motion = 1;
    effect->render = 1;
    effect->active = 1;
    effect->random_value = Random16();
    effect->flags = 4;
    EffectSlot_SetObjectMode(effect, 1);
}
