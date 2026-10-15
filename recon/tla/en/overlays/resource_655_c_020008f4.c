/* Draft of overlay 655's Func_020008f4, differ 2 of 59: Effect_AdvanceMotion (common/effect/
 * advance_motion.c) with a tail that gives the effect's sprite the leader's priority. The three
 * barriers keep the native's stores ahead of the next loads (the common file has the first two;
 * the third puts the gPartyState load ahead of the 0x214 offset). Left: the native reads the
 * effect's sprite byte (ldrb r1, [r4, #9]) before it builds the 12 mask (movs r2, #12; ands);
 * sched2 ranks the mask first, one higher, because the -13 mask reuses r3 after the and. Tried:
 * the copy through two sprite pointers (6), explicit byte masks in either order (12 to 20), the
 * effect byte read into a local first (20); permute 3 minutes found nothing. */
#include "types.h"
#include "field_effect.h"
#include "field_sprite.h"
#include "party_state.h"

struct FieldActor *Object_GetById(s32 id);

void Func_020008f4(union FieldObject *object)
{
    struct FieldEffect *effect = &object->effect;
    s32 velocity_x;
    s32 velocity_z;
    s32 scale_rate_x;
    struct FieldSprite *sprite;
    struct FieldActor *leader;

    velocity_x = effect->velocity_x;
    effect->x += velocity_x;
    effect->y += effect->velocity_y;
    velocity_z = effect->velocity_z;
    effect->z += velocity_z;

    effect->velocity_x = velocity_x - velocity_x / 18;
    effect->velocity_z = velocity_z - velocity_z / 16;
    __asm__("");

    scale_rate_x = effect->scale_rate_x;
    effect->scale_x += scale_rate_x;
    sprite = effect->sprite;
    effect->scale_y += effect->scale_rate_y;
    __asm__("");
    sprite->rotation += effect->spin;
    __asm__("");

    leader = Object_GetById(gPartyState.current_owner);
    effect->sprite->priority = ((struct FieldSprite *)leader->sprite)->priority;
}
