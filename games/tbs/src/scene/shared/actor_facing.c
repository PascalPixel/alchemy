#include "field/script_operands.h"

/* Altin Peak, Bilibin, Imil, Kolima, McCoy's palace and garden, Sol Sanctum's
   Elemental Star room, and Vale's Kraden, Babi, home and rooms scenes: people
   who turn to keep someone in view. */

/* An actor script call: an actor with a facing target turns towards it, at most
   a sixteenth of a turn a tick. */
static Sint32 scTurnTowardTarget(ACTOR *actor)
{
	Sint32 delta;
	Uint16 facing;
	Sint32 ang;
	ACTOR *tgt;

	tgt = (ACTOR *)actor->watchedActor;
	if(tgt != NULL) {
		actor->steeringFlg = ((Uint8)~ACTOR_FACES_MOTION & actor->steeringFlg);
		ang = (Uint16)syAtan(
			tgt->z.fixed - actor->z.fixed, tgt->x.fixed - actor->x.fixed);
		facing = actor->facing;
		delta = (Sint16)(ang - facing);
		if(delta != 0) {
			if(delta > FACING_STEP) {
				delta = FACING_STEP;
			}

			if(delta < -FACING_STEP) {
				delta = -FACING_STEP;
			}
			actor->facing = facing + delta;
		}
	}
	return 1;
}
