/* A field effect's motion each tick, drawn at the leader's priority. */

#include "system/field_effect.h"
#include "field/script.h"
#include "field/sprite.h"

/* fdEffectAdvanceMotion, then gives the effect's sprite the leader's sprite
   priority, so the effect is drawn at the leader's depth. */
void fdEffectAdvanceMotionAtLeaderPriority(FIELD_OBJECT *object)
{
	FIELD_EFFECT	   *effect = &object->eff;
	Sint32				velX = effect->vel.x;
	Sint32				velZ;
	MAIN_ROUTINE_STATE *party;
	FIELD_SPR		   *spr;
	ACTOR			   *leader;
	PLACED_SPR		   *to;
	PLACED_SPR		   *from;

	effect->x += velX;
	effect->y += effect->vel.y;
	velZ = effect->vel.z;
	effect->z += velZ;
	effect->vel.x = velX - velX / FIELD_EFFECT_DRAG_X;
	effect->vel.z = velZ - velZ / FIELD_EFFECT_DRAG_Z;
	effect->scaleX += effect->scaleRateX;
	spr = effect->spr;
	effect->scaleY += effect->scaleRateY;
	spr->rotation += effect->spin;

	party = &MainRoutineStateWork;

	leader = fdActorGetById(party->leaderActorId);
	to = (PLACED_SPR *)effect->spr;
	from = (PLACED_SPR *)leader->spr;
	to->pri = from->pri;
}
