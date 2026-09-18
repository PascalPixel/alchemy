#include "TYPES.H"
#include "FIELD_EFFECT.H"

/*
 * Shared per-frame effect integrator. Lunpa already compiles this as
 * Effect_Move. Overlay listings keep the same 54 instruction bytes after
 * AlchemyC_020000a0 for:
 *   resource_382, 387, 396, 39b, 39c, 39e, 3a0, 3a5, 3a6, 3b3, 3be, 3c0, 3c9
 * Adopt as one translation unit with instances; 54-byte function then
 * two-byte alignment, same as runpa-mura-effect.
 */

void Effect_Move(union FieldObject *object)
{
    object->effect.x += object->effect.velocity_x;
    object->effect.y += object->effect.velocity_y;
    object->effect.z += object->effect.velocity_z;
    object->effect.scale_x += object->effect.scale_rate_x;
    object->effect.scale_y += object->effect.scale_rate_y;
    object->effect.sprite->rotation += object->effect.spin;
}
