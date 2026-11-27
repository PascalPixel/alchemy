// A field effect's motion and spin each tick.

#include "system/field_effect.h"
#include "field/script.h"

// fdEffectAdvanceMotion with a fixed spin: moves the effect by its velocity, which loses drag,
// grows it at its rates and turns its sprite a sixteenth of a turn each tick. Nothing calls it.
UNUSED void fdEffectAdvanceMotionAndSpin(FIELD_OBJECT *object)
{
    FIELD_EFFECT *effect = &object->effect;
    Sint32        velocityX;
    Sint32        velocityZ;
    Sint32        scaleRateX;

    // DEAD: the leader looked up and not used; the ROM keeps the call.
    fdActorGetById(MainRoutineStateWork.leaderActorId);
    velocityX                 = effect->velocity.x;
    effect->x                += velocityX;
    effect->y                += effect->velocity.y;
    velocityZ                 = effect->velocity.z;
    effect->z                += velocityZ;

    effect->velocity.x        = velocityX - velocityX / FIELD_EFFECT_DRAG_X;
    effect->velocity.z        = velocityZ - velocityZ / FIELD_EFFECT_DRAG_Z;

    scaleRateX                = effect->scaleRateX;
    effect->scaleX           += scaleRateX;
    effect->scaleY           += effect->scaleRateY;
    effect->sprite->rotation += TRIG_TURN / 16;
}
