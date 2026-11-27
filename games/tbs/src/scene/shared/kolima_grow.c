/* Kolima and Kolima Forest (SceneIdKolima, SceneIdKolimaForestEntrance and
   SceneIdKolimaForestTree): an effect that stretches as it travels. */

#include "field/script_operands.h"

/* An actor script call: widen the actor by 3/25 of its size a tick, and
   release it once it has reached its destination. */
static Sint32 scFieldEffectStretchUntilArrived(ACTOR *streak)
{
	streak->scaleX += toFIXED(3) / 25;
	if(streak->targetX == ACTOR_NO_TARGET &&
		streak->targetY == ACTOR_NO_TARGET &&
		streak->targetZ == ACTOR_NO_TARGET) {
		fdActorRelease(streak);
	}
	return 1;
}
