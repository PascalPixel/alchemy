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
    s32 velocityX;
    s32 velocityZ;
    s32 scaleRateX;
    struct FieldSprite *sprite;
    struct FieldActor *leader;

    velocityX = effect->velocityX;
    effect->x += velocityX;
    effect->y += effect->velocityY;
    velocityZ = effect->velocityZ;
    effect->z += velocityZ;

    effect->velocityX = velocityX - velocityX / 18;
    effect->velocityZ = velocityZ - velocityZ / 16;
    __asm__("");

    scaleRateX = effect->scaleRateX;
    effect->scaleX += scaleRateX;
    sprite = effect->sprite;
    effect->scaleY += effect->scaleRateY;
    __asm__("");
    sprite->rotation += effect->spin;
    __asm__("");

    leader = Object_GetById(gPartyState.currentOwner);
    effect->sprite->priority = ((struct FieldSprite *)leader->sprite)->priority;
}
