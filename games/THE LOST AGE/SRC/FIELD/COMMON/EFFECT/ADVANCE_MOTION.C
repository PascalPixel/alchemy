#include "TYPES.H"
#include "FIELD_EFFECT.H"

/*
 * Moves an effect by its velocity, slows it in X by an eighteenth and in Z
 * by a sixteenth each frame, grows it by its scale rates and turns its
 * sprite by its spin, as ☀️'s overlays' Effect_AdvanceMotion does.
 *
 * ⚓️ reads each next velocity, and the sprite, before the store ahead of
 * it, where agscc keeps them after; the barriers hold those loads in place.
 */
void Effect_AdvanceMotion(union FieldObject *object)
{
    struct FieldEffect *effect = &object->effect;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 x;
    s32 y;
    s32 scale_rate_x;
    s32 scale_x;
    struct FieldSprite *sprite;

    velocity_x = effect->velocity_x;
    x = effect->x;
    velocity_y = effect->velocity_y;
    /* FAKEMATCH: keeps the Y velocity load ahead of the X store. */
    asm("");
    effect->x = x + velocity_x;
    y = effect->y;
    /* FAKEMATCH: keeps the Y load ahead of the Z velocity load. */
    asm("");
    velocity_z = effect->velocity_z;
    /* FAKEMATCH: keeps the Z velocity load ahead of the Y store. */
    asm("");
    effect->y = y + velocity_y;
    effect->z += velocity_z;

    effect->velocity_x = velocity_x - velocity_x / 18;
    effect->velocity_z = velocity_z - velocity_z / 16;

    scale_rate_x = effect->scale_rate_x;
    scale_x = effect->scale_x;
    sprite = effect->sprite;
    /* FAKEMATCH: keeps the sprite load ahead of the scale stores. */
    asm("");
    effect->scale_x = scale_x + scale_rate_x;
    effect->scale_y += effect->scale_rate_y;
    sprite->rotation += effect->spin;
}
