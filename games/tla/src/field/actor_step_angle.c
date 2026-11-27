// An actor script call that turns an actor toward an angle a step at a time.

#include "field/actor.h"

// SC_CALL function that turns an actor toward the angle kept in its timer: the whole way when
// within 1/16 turn, otherwise 1/32 turn.
Sint32 fdActorStepAngle(ACTOR *actor)
{
	Sint32 delta = 0;

	if (actor != NULL)
	{
		Sint32 targetAngle  = (Uint16)actor->timer;
		Sint32 currentAngle = actor->facing;
		delta               = (Sint16)(targetAngle - currentAngle);
		if (delta != 0)
		{
			if (delta > TRIG_TURN / 16)
				delta = TRIG_TURN / 32;
			if (delta < -(TRIG_TURN / 16))
				delta = -(TRIG_TURN / 32);
			actor->facing = currentAngle + delta;
		}
	}
	return delta;
}
