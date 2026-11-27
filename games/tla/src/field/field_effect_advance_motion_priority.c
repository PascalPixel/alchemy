/* A field effect's motion each frame, drawn at the leader's priority. */

#include "system/field_effect.h"
#include "field/script.h"
#include "field/sprite.h"

/* fdEffectAdvanceMotion, then gives the effect's sprite the leader's sprite
   priority, so the effect is drawn at the leader's depth. */
void fdEffectAdvanceMotionAtLeaderPriority(FIELD_OBJECT *object)
{
	FIELD_EFFECT	   *effect = &object->eff;
	Sint32				velocityX = effect->vel.x;
	Sint32				velocityZ;
	MAIN_ROUTINE_STATE *party;
	FIELD_SPR		   *sprite;
	ACTOR			   *leader;
	PLACED_SPR		   *to;
	PLACED_SPR		   *from;

	effect->x += velocityX;
	effect->y += effect->vel.y;
	velocityZ = effect->vel.z;
	effect->z += velocityZ;
	effect->vel.x = velocityX - velocityX / FIELD_EFFECT_DRAG_X;
	effect->vel.z = velocityZ - velocityZ / FIELD_EFFECT_DRAG_Z;
	effect->scaleX += effect->scaleRateX;
	sprite = effect->spr;
	effect->scaleY += effect->scaleRateY;
	sprite->rotation += effect->spin;

	party = &MainRoutineStateWork;

	leader = fdActorGetById(party->leaderActorId);
	to = (PLACED_SPR *)effect->spr;
	from = (PLACED_SPR *)leader->spr;
	to->pri = from->pri;
}
