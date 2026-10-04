#include "TYPES.H"
#include "FIELD_EFFECT.H"

/* Moves an effect by its velocity, grows it by its scale rates and turns its
   sprite by its spin, as ☀️'s FIELD/COMMON/EFFECT/MOVE.C does. */
void BattleFx_UpdateObjectMotionScaleAndLinkedAngle(union FieldObject *object)
{
    s32 x;
    s32 velocity_x;
    struct FieldSprite *sprite;

    x = object->effect.x;
    velocity_x = object->effect.velocity_x;
    sprite = object->effect.sprite;
    /* ⚓️ loads the sprite third, before the first store; ☀️'s statement
       order leaves it after the scale stores. */
    /* FAKEMATCH: the barrier keeps the three loads ahead of the stores. */
    asm("");
    object->effect.x = x + velocity_x;
    object->effect.y += object->effect.velocity_y;
    object->effect.z += object->effect.velocity_z;
    object->effect.scale_x += object->effect.scale_rate_x;
    object->effect.scale_y += object->effect.scale_rate_y;
    sprite->rotation += object->effect.spin;
}
