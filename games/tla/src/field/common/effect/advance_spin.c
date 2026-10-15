#include "types.h"
#include "party_state.h"
#include "field_effect.h"

void *Object_GetById(s32 id);

// Effect_AdvanceMotion with a fixed spin: moves the effect by its velocity,
// damps the X and Z velocity, steps the scale and turns the sprite 0x1000
// each frame. The leader's object is looked up and not used. Overlays 6aa,
// 6ab, 6ac and 6ad hold it.
void Effect_AdvanceMotionAndSpin(union FieldObject *object)
{
    struct FieldEffect *effect = &object->effect;
    s32 velocity_x;
    s32 velocity_z;
    s32 scale_rate_x;

    Object_GetById(gPartyState.current_owner);
    velocity_x = effect->velocity_x;
    effect->x += velocity_x;
    effect->y += effect->velocity_y;
    velocity_z = effect->velocity_z;
    effect->z += velocity_z;

    effect->velocity_x = velocity_x - velocity_x / 18;
    effect->velocity_z = velocity_z - velocity_z / 16;
    /* FAKEMATCH: the ARM9 scheduling would load the scale rate before storing the Z velocity, as in Effect_AdvanceMotion. */
    __asm__("");

    scale_rate_x = effect->scale_rate_x;
    effect->scale_x += scale_rate_x;
    /* FAKEMATCH: the volatile Y scale store and sprite load keep the sprite's load after the store, as the native has it (plain: differ 2, the load moves above the store); a barrier there instead moves the building of 0x1000 after it (differ 4). */
    *(volatile s32 *)&effect->scale_y += effect->scale_rate_y;
    (*(struct FieldSprite *volatile *)&effect->sprite)->rotation += 0x1000;
}
