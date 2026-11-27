/* The motion of a thing a vent throws out, which Aqua Rock's inside and Taopo
   Swamp each keep a copy of. scene/scene.h, which the including scene reads
   first, gives it VENT_WORK, VENT_STATE, DIFF and the two functions' names,
   VENT_TOUCHING and VENT_BOUNCE. */

/* Whether actor a rests on actor b: within 12 pixels of it in x and 8 in z, and
   above it by less than 16 pixels. */
Bool VENT_TOUCHING(ACTOR *a, ACTOR *b)
{
	if(DIFF(a->x.fixed, b->x.fixed) < toFIXED(12)) {
		if((Uint32)(a->y.fixed - b->y.fixed - 1) <= toFIXED(16) - 2) {
			if(DIFF(a->z.fixed, b->z.fixed) < toFIXED(8))
				return TRUE;
		}
	}
	return FALSE;
}

/* Moves the thing by its velocities, slows it in x and z, grows and spins its
   sprite, and stops its drift in z when it meets the leader or one of the
   actors the vent state lists. scVentPuff keeps the puff's motion in fields it
   borrows for the purpose: bounce is its x velocity, gravity its y velocity and
   sceneWord its z velocity, all 16.16 pixels a frame; speedLimit and
   acceleration are what its x and y scale grow by each frame, and timer its
   spin. */
void VENT_BOUNCE(ACTOR *actor)
{
	VENT_WORK *state = VENT_STATE;
	ACTOR	  *other = fdActorGetById(MainRoutineStateWork.leaderActorId);
	FIXED	   velocityX = actor->bounce;
	FIXED	   velocityZ;
	SPR		  *sprite;
	Sint32	   i;

	actor->x.fixed += velocityX;
	actor->y.fixed += actor->gravity;
	velocityZ = actor->sceneWord;
	actor->z.fixed += velocityZ;
	actor->bounce = velocityX - velocityX / 18;
	actor->sceneWord = velocityZ - velocityZ / 16;
	actor->scaleX += actor->speedLimit;
	sprite = actor->spr;
	{
		Sint32 scaleStepY = actor->acceleration;

		actor->scaleY += scaleStepY;
	}
	sprite->rotation += actor->tmr;
	/* Meeting an actor turns a quarter of the z velocity into y velocity. */
	if(other->y.fixed == other->groundHeight && VENT_TOUCHING(actor, other)) {
		actor->z.fixed -= actor->sceneWord;
		actor->gravity += actor->sceneWord >> 2;
		actor->sceneWord = 0;
	}
	for(i = 0; i < 4; i++) {
		Sint32 id = state->ids[i];

		if(id == 0)
			break;
		other = fdActorGetById(id);
		if(other->y.fixed == other->groundHeight &&
			VENT_TOUCHING(actor, other)) {
			actor->z.fixed -= actor->sceneWord;
			actor->gravity += actor->sceneWord >> 2;
			actor->sceneWord = 0;
		}
	}
}
