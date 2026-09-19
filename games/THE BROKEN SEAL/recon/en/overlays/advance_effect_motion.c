#include "TYPES.H"
#include "FIELD_EFFECT.H"

/*
 * Ordinary spelling of the five overlay copies of main:0809a65c
 * (ParticleEffect_UpdateMotionAndScale). Those copies still sit in area
 * modules behind a do { } while (0) load-order barrier. Recheck them from
 * this text with __divsi3 bound to each overlay's divide veneer:
 *   resource_3b2:02000da4
 *   resource_3c5:02000cf0
 *   resource_396:0200185c
 *   resource_3c4:02001068
 *   resource_3c8:0200096c
 * Withdraw rather than keep the barrier if this does not match.
 */

void ParticleEffect_UpdateMotionAndScale(union FieldObject *object)
{
    struct FieldEffect *effect = &object->effect;

    effect->x += effect->velocity_x;
    effect->y += effect->velocity_y;
    effect->z += effect->velocity_z;
    effect->velocity_x -= effect->velocity_x / 18;
    effect->velocity_z -= effect->velocity_z / 16;
    effect->scale_x += effect->scale_rate_x;
    effect->scale_y += effect->scale_rate_y;
    effect->sprite->rotation += effect->spin;
}
