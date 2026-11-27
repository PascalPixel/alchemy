/* A field effect's motion each tick, drawn at the leader's priority. */

#include "system/field_effect.h"
#include "field/script.h"
#include "field/sprite.h"

/* fdEffectAdvanceMotion, then gives the effect's sprite the leader's sprite
   priority, so the effect is drawn at the leader's depth. */
void fdEffectAdvanceMotionAtLeaderPriority(FIELD_OBJECT *object)
{
	FIELD_EFFECT	   *effect = &object->effect;
	Sint32				velocityX = effect->velocity.x;
	Sint32				velocityZ;
	MAIN_ROUTINE_STATE *party;
	FIELD_SPR		   *sprite;
	ACTOR			   *leader;
	PLACED_SPR		   *to;
	PLACED_SPR		   *from;

	effect->x += velocityX;
	effect->y += effect->velocity.y;
	velocityZ = effect->velocity.z;
	effect->z += velocityZ;
	effect->velocity.x = velocityX - velocityX / FIELD_EFFECT_DRAG_X;
	effect->velocity.z = velocityZ - velocityZ / FIELD_EFFECT_DRAG_Z;
	effect->scaleX += effect->scaleRateX;
	sprite = effect->sprite;
	effect->scaleY += effect->scaleRateY;
	sprite->rotation += effect->spin;

	party = &MainRoutineStateWork;

	leader = fdActorGetById(party->leaderActorId);
	to = (PLACED_SPR *)effect->sprite;
	from = (PLACED_SPR *)leader->sprite;
	to->priority = from->priority;
}
