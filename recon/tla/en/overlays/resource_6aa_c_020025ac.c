/* Near miss, differ 2 of 47: Effect_AdvanceMotion (field/common/effect/advance_motion.c) with a
 * constant spin of 0x1000 and a leader lookup first; overlays 6ab, 6ac and 6ad hold copies. The
 * native loads the sprite after it stores the Y scale; this draft loads it before. A barrier or a
 * one-pass block there moves the building of 0x1000 after it instead (4, 8); with spin in a local
 * set before the scale step and an asm input barrier the tail matches, but the leader offset's
 * reload then takes r2 for r1 (6). */
#include "types.h"
#include "party_state.h"
#include "field_effect.h"

void *Object_GetById(s32 id);

void Func_020025ac(union FieldObject *object)
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
    effect->scale_y += effect->scale_rate_y;
    effect->sprite->rotation += 0x1000;
}
