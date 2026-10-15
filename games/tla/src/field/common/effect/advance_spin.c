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
    s32                 velocityX;
    s32                 velocityZ;
    s32                 scaleRateX;

    Object_GetById(gPartyState.currentOwner);
    velocityX = effect->velocityX;
    effect->x += velocityX;
    effect->y += effect->velocityY;
    velocityZ = effect->velocityZ;
    effect->z += velocityZ;

    effect->velocityX = velocityX - velocityX / 18;
    effect->velocityZ = velocityZ - velocityZ / 16;
    /* FAKEMATCH: the ARM9 scheduling would load the scale rate before storing the Z velocity, as in Effect_AdvanceMotion. */
    __asm__("");

    scaleRateX = effect->scaleRateX;
    effect->scaleX += scaleRateX;
    /* FAKEMATCH: the volatile Y scale store and sprite load keep the sprite's load after the store, as the native has it (plain: differ 2, the load moves above the store); a barrier there instead moves the building of 0x1000 after it (differ 4). */
    *(volatile s32 *)&effect->scaleY += effect->scaleRateY;
    (*(struct FieldSprite *volatile *)&effect->sprite)->rotation += 0x1000;
}
