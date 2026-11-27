/* @file */
/* A field effect's motion and spin each frame. */

#include "system/field_effect.h"
#include "field/event.h"

/* fdEffectAdvanceMotion with a fixed spin: moves the effect by its */
/* velocity, which loses drag, grows it at its rates and turns its sprite a */
/* sixteenth of a turn each frame. Nothing calls it. */
UNUSED void fdEffectAdvanceMotionAndSpin(FIELD_OBJECT *object) {
	FIELD_EFFECT *effect = &object->eff;
	s32 velocityX;
	s32 velocityZ;
	s32 scaleRateX;

	/* DEAD: the leader looked up and not used; the ROM keeps the call. */
	fdActorGetById(MainRoutineStateWork.leaderActorId);
	velocityX = effect->vel.x;
	effect->x += velocityX;
	effect->y += effect->vel.y;
	velocityZ = effect->vel.z;
	effect->z += velocityZ;

	effect->vel.x = velocityX - velocityX / FIELD_EFFECT_DRAG_X;
	effect->vel.z = velocityZ - velocityZ / FIELD_EFFECT_DRAG_Z;

	scaleRateX = effect->scaleRateX;
	effect->scaleX += scaleRateX;
	effect->scaleY += effect->scaleRateY;
	effect->spr->rotation += TRIG_TURN / 16;
}
