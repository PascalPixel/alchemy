/* A field effect's motion and spin each tick. */

#include "system/field_effect.h"
#include "field/script.h"

/* fdEffectAdvanceMotion with a fixed spin: moves the effect by its velocity,
   which loses drag, grows it at its rates and turns its sprite a sixteenth of a
   turn each tick. Nothing calls it. */
UNUSED void fdEffectAdvanceMotionAndSpin(FIELD_OBJECT *object)
{
	FIELD_EFFECT *effect = &object->eff;
	Sint32		  velX;
	Sint32		  velZ;
	Sint32		  scaleRateX;

	/* DEAD: the leader looked up and not used; the ROM keeps the call. */
	fdActorGetById(MainRoutineStateWork.leaderActorId);
	velX = effect->vel.x;
	effect->x += velX;
	effect->y += effect->vel.y;
	velZ = effect->vel.z;
	effect->z += velZ;

	effect->vel.x = velX - velX / FIELD_EFFECT_DRAG_X;
	effect->vel.z = velZ - velZ / FIELD_EFFECT_DRAG_Z;

	scaleRateX = effect->scaleRateX;
	effect->scaleX += scaleRateX;
	effect->scaleY += effect->scaleRateY;
	effect->spr->rotation += TRIG_TURN / 16;
}
