/* @file */
/* Unused: an ARM copy of the actors' update, made to run from RAM. Nothing */
/* runs it; the field engine moves its actors with fdActorUpdateAllOnField. */

#include "field/actor_update_arm.h"
#include "fixed_math.h"
#include "macros.h"
#include "types.h"
#include "field/actor_arm_view.h"
#include "field/actor_script.h"
#include "system/globals.h"
#include "system/iwram_call.h"
#include "system/mul_fx.h"

#define ACTOR_SLOTS 8 /* script-driven actors updated each frame */
#define TURN_MAX	(TRIG_TURN / 16) /* the most an actor turns in one frame */

/* Calls a resident IWRAM function at its absolute address, which this code */
/* still reaches when it runs from its copy in RAM. */
#define IWRAM_ENTRY(type, offset) ((type)(IwramIrqMain + (offset)))
#define IwramSqrt				  IWRAM_ENTRY(s32 (*)(s32 value), sySquartOffset)
#define IwramDivFX				  IWRAM_ENTRY(s32 (*)(s32 divisor, s32 value), syDivFXOffset)

/* stub copied to RAM together with this code (actor_update_far.s). */
ANGLE fdTrigAtanStubArm(FIXED z, FIXED x);
s32 fdMathSqrtStubArm(s32 value);
s32 fdFixedDivStubArm(s32 divisor, s32 value);

/* Advances the eight script-driven actors by one frame. */
void fdActorUpdateAll(void) {
	ACTOR_ARM_VIEW *actor;
	s32 y;
	s32 arrived;
	ACTOR_ARM_VIEW *nextActor;
	s32 nextSlot;
	s32 slot;
	s32 x;
	s32 z;
	s32 deltaX;
	s32 deltaZ;
	FIXED velocityX;
	FIXED velocityZ;
	s32 distance;
	s32 speedRatio;
	s32 groundY;
	s32 turnDelta;
	u16 travelAngle;

	for(slot = 0, actor = HEAP_SLOT_BLOCK(HEAP_SLOT_ACTORS); slot < ACTOR_SLOTS;
		slot = nextSlot, actor = nextActor) {
		nextSlot = slot + 1;
		nextActor = actor + 1;
		if(actor->script == NULL)
			continue;
		if(actor->update != NULL)
			actor->update((struct ACTOR *)actor);
		if(actor->script == NULL)
			continue;
		if(actor->paused != 0)
			continue;

		/* Count down a wait, or run commands until one waits for this frame. */
		if(actor->wait != 0) {
			actor->wait--;
		}
		else {
			for(;;) {
				s32 cursor = actor->scriptCursor;
#ifdef UBFIX
				u32 command = actor->script[cursor];
#else
				u32 command = actor->script->word[cursor];
#endif

				if(command <= SCRIPT_COMMAND_MAX) {
					if(ActorScript_CommandTable[command](
						   (struct ACTOR *)actor) == 0)
						break;
				}
				else {
					actor->scriptCursor++;
				}
			}
			if(actor->script == NULL)
				continue;
		}

		x = actor->x;
		y = actor->y;
		z = actor->z;
/* UB: frozen actors skip the initialization below, but still test arrival. */
#ifdef UBFIX
		arrived = 0;
#endif
		if(actor->frozen == 0) {
			arrived = 0;
			if(actor->tgtX != ACTOR_NO_TARGET) {
				/* Accelerate towards the target. */
				deltaX = (actor->tgtX - x) / FIXED_ONE;
				deltaZ = (actor->tgtZ - z) / FIXED_ONE;
				distance =
					toFIXED(IwramSqrt(deltaX * deltaX + deltaZ * deltaZ));
				if(distance <= toFIXED(256) - 1) {
					deltaX = actor->tgtX - x;
					deltaZ = actor->tgtZ - z;
					distance =
						IwramSqrt(MulFX(deltaX, deltaX) + MulFX(deltaZ, deltaZ))
						<< 8;
				}
				if(distance == 0) {
					x = actor->tgtX;
					z = actor->tgtZ;
				}
				else {
					/* speedRatio is acceleration / distance in 16.16. */
					speedRatio = IwramDivFX(distance, actor->acceleration);
					velocityX = actor->velX + MulFX(deltaX, speedRatio);
					actor->velX = velocityX;
					/* Limit the combined horizontal velocity after accelerating
					 * both */
					/* axes. */
					velocityZ = actor->velZ + MulFX(deltaZ, speedRatio);
					actor->velZ = velocityZ;
					distance = IwramSqrt(MulFX(velocityX, velocityX) +
										 MulFX(velocityZ, velocityZ))
							   << 8;
					if(distance > actor->speedLimit) {
						/* Too fast: scale the velocity back to the speed limit.
						 */
						speedRatio = IwramDivFX(distance, actor->speedLimit);
						actor->velX = MulFX(velocityX, speedRatio);
						actor->velZ = MulFX(velocityZ, speedRatio);
					}
				}
			}
			else {
				/* No target: brake by the acceleration until the actor stops.
				 */
				deltaX = actor->velX;
				deltaZ = actor->velZ;
				distance = fdMathSqrtStubArm(MulFX(deltaX, deltaX) +
											 MulFX(deltaZ, deltaZ))
						   << 8;
				if(distance != 0) {
					if(distance - actor->acceleration < 0) {
						actor->velX = 0;
						actor->velZ = 0;
					}
					else {
						speedRatio = fdFixedDivStubArm(
							distance, distance - actor->acceleration);
						actor->velX = MulFX(deltaX, speedRatio);
						actor->velZ = MulFX(deltaZ, speedRatio);
					}
				}
				else {
					actor->velX = 0;
					actor->velZ = 0;
				}
			}

			/* Gravity for an actor that falls, and a bounce that loses energy
			 * on */
			/* landing. */
			if(actor->motionFlg & ACTOR_FALLS) {
				groundY = actor->groundHeight;
				if(y > groundY)
					actor->velY -= actor->gravity;
				if(y <= groundY && actor->velY < 0) {
					y = groundY;
					actor->velY = -MulFX(actor->velY, actor->vertical.bounce);
					if((actor->velY < 0 ? -actor->velY : actor->velY) <=
					   actor->gravity)
						actor->velY = 0;
				}
			}
		}

		/* Integrate velocity even when the acceleration and gravity step is
		 * frozen. */
		x += actor->velX;
		y += actor->velY;
		z += actor->velZ;

		/* Arrived: the watched axis reached the target, or crossed it this
		 * frame */
		/* (the sign of the offset changed). */
		switch(actor->arrivalAxis) {
		case ARRIVAL_AXIS_X:
			if(x == actor->tgtX ||
			   ((actor->x - actor->tgtX) ^ (x - actor->tgtX)) < 0)
				arrived = 1;
			break;
		case ARRIVAL_AXIS_Y:
			if(y == actor->tgtY ||
			   ((actor->y - actor->tgtY) ^ (y - actor->tgtY)) < 0)
				arrived = 1;
			break;
		case ARRIVAL_AXIS_Z:
			if(z == actor->tgtZ ||
			   ((actor->z - actor->tgtZ) ^ (z - actor->tgtZ)) < 0)
				arrived = 1;
			break;
		}
		/* Snapping stops the actor on the target; it takes the target's height
		 */
		/* too only when its motion flags are all clear. */
		if(arrived) {
			if(actor->snapToTgt) {
				x = actor->tgtX;
				actor->velX = 0;
				z = actor->tgtZ;
				actor->velZ = 0;
				if(actor->motionFlg == 0) {
					y = actor->tgtY;
					actor->velY = 0;
				}
			}
			actor->tgtX = ACTOR_NO_TARGET;
			actor->tgtY = ACTOR_NO_TARGET;
			actor->tgtZ = ACTOR_NO_TARGET;
			actor->arrivalAxis = ARRIVAL_AXIS_NONE;
		}
		SET_POSITION(actor, x, y, z);

		/* An actor that faces its motion turns toward its direction of travel.
		 */
		if(actor->steeringFlg & ACTOR_FACES_MOTION) {
			x = actor->velX;
			z = actor->velZ;
			if(x != 0 || z != 0) {
				travelAngle = fdTrigAtanStubArm(z, x);
				/* As a signed halfword the difference is the shorter way round.
				 */
				turnDelta = (s16)(travelAngle - actor->facing);
				if(turnDelta > TURN_MAX)
					turnDelta = TURN_MAX;
				if(turnDelta < -TURN_MAX)
					turnDelta = -TURN_MAX;
				actor->facing += turnDelta;
			}
		}
	}
}
