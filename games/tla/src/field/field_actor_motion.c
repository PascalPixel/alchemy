/* Actors' motion each tick: their scripts, steering toward a target, falling,
   floating and riding a platform, and being sent somewhere. Part of field.c. */

#include "field/resource_slot.h"

/* Returns 1 when the actor has no movement target: none of its target
   coordinates set, or for an actor moved by the ground (any motion flag)
   neither its target x nor z. */
Bool fdActorIsTargetUnset(ACTOR *actor)
{
	Sint32 first;
	Sint32 second;

	/* One compare of three values: all must equal ACTOR_NO_TARGET, or with
	   motion flags only x and z. */
	if(actor->motionFlags == 0) {
		second = actor->targetX;
		if(second != ACTOR_NO_TARGET)
			return FALSE;
		first = actor->targetY;
	} else {
		first = actor->targetX;
		second = ACTOR_NO_TARGET;
	}
	if(first != second || actor->targetZ != first)
		return FALSE;
	return TRUE;
}

/* fdActorIsTargetUnset, for the scenes: 1 once the actor has reached its
   destination. Airs Rock waits on it while the lift carries the leader up. */
Sint32 fdActorHasArrived(ACTOR *actor)
{
	return fdActorIsTargetUnset(actor);
}

extern const SCRIPT_COMMAND_FN ActorScriptCommandTable[];
extern const Sint32			   ActorScriptCycleTable[];

/* The highest script word read as a command, an index of
   ActorScriptCommandTable; a larger one is stepped over. */
#define SCRIPT_COMMAND_MAX 63
/* A floating actor's bob, one cycle of ActorScriptCycleTable over this many
   ticks; its rise and fall each tick is the table's value times its gravity
   over FLOAT_DIVISOR, or FLOAT_HIGH_DIVISOR with ACTOR_FLOATS_HIGH. */
#define FLOAT_CYCLE_TICKS  64
#define FLOAT_DIVISOR	   64
#define FLOAT_HIGH_DIVISOR 16

/* A whole-pixel distance squared. */
static __inline__ Sint32 fdObjectSquare(Sint32 value)
{
	return value * value;
}

/* Advances every actor by one tick: their scripts and their motion, with free
   flight in 3D, following the ground, floating, and solid actors that refuse to
   overlap. */
void fdActorUpdateAllOnField(void)
{
	ACTOR *object;
	Sint32 objectsRemaining;
	Sint32 x;
	FIXED  y;
	Sint32 z;
	FIXED  ground;
	Sint32 arrivedAtTarget;
	FIXED  pos[3];
	Sint32 dx;
	Sint32 dy;
	Sint32 dz;
	Sint32 velocityX;
	Sint32 velocityY;
	Sint32 velocityZ;
	Sint32 distance;
	FIXED  speed;
	FIXED  reducedSpeed;
	FIXED  ratio;
	Sint32 maximumChange;
	Sint32 standing;

	object = RamHeapSlots->actors;
	for(objectsRemaining = ACTOR_COUNT - 1; objectsRemaining >= 0;
		objectsRemaining--, object++) {
		const Sint32 *script;

		arrivedAtTarget = 0;
		script = object->script;
		if(script == NULL)
			continue;
		if(object->update != NULL) {
			((OBJECT_UPDATE)object->update)((union FIELD_OBJECT *)object);
			script = object->script;
		}
		if(script == NULL)
			continue;
		if(object->paused != 0) {
			object->stepX = arrivedAtTarget;
			object->stepY = arrivedAtTarget;
			object->stepZ = arrivedAtTarget;
			continue;
		}

		if(object->wait != 0) {
			object->wait--;
		} else {
			for(;;) {
				Uint32 command;

				/* The local steps to the command's word; the next pass takes
				   the script afresh. */
				script += object->scriptCursor;
				command = *script;

				if(command <= SCRIPT_COMMAND_MAX) {
					if(ActorScriptCommandTable[command](object) == 0)
						break;
				} else {
					object->scriptCursor++;
				}
				script = object->script;
			}
			if(object->script == NULL)
				continue;
		}

		x = object->x.fixed;
		y = object->y.fixed;
		z = object->z.fixed;
		if(object->frozen == 0) {
			if(object->motionFlags == 0) {
				if(object->targetX != ACTOR_NO_TARGET) {
					dx = (object->targetX - x) / toFIXED(1);
					dy = (object->targetY - y) / toFIXED(1);
					dz = (object->targetZ - z) / toFIXED(1);
					distance = sySquartIWRAM(fdObjectSquare(dx) +
						fdObjectSquare(dy) + fdObjectSquare(dz));
					if(distance == 0) {
						x = object->targetX;
						y = object->targetY;
						z = object->targetZ;
					} else {
						ratio =
							syDivFX(toFIXED(distance), object->acceleration);
						object->velocityX += dx * ratio;
						object->velocityY += dy * ratio;
						object->velocityZ += dz * ratio;
						velocityX = object->velocityX;
						velocityY = object->velocityY;
						velocityZ = object->velocityZ;
						speed = sySquartFX(syMulFX(velocityX, velocityX) +
							syMulFX(velocityY, velocityY) +
							syMulFX(velocityZ, velocityZ));
						if(speed > object->speedLimit) {
							ratio = syDivFX(speed, object->speedLimit);
							object->velocityX = syMulFX(velocityX, ratio);
							object->velocityY = syMulFX(velocityY, ratio);
							object->velocityZ = syMulFX(velocityZ, ratio);
						}
					}
				} else {
					dx = object->velocityX;
					dy = object->velocityY;
					dz = object->velocityZ;
					speed = sySquartFX(
						syMulFX(dx, dx) + syMulFX(dy, dy) + syMulFX(dz, dz));
					if(speed != 0) {
						reducedSpeed = speed - object->acceleration;
						if(reducedSpeed < 0)
							reducedSpeed = 0;
						ratio = syDivFX(speed, reducedSpeed);
						object->velocityX = syMulFX(dx, ratio);
						object->velocityY = syMulFX(dy, ratio);
						object->velocityZ = syMulFX(dz, ratio);
					} else {
						object->velocityX = speed;
						object->velocityY = speed;
						object->velocityZ = speed;
					}
				}
			} else {
				/* On the ground: steer in the XZ plane the same way. */
				if(object->targetX != ACTOR_NO_TARGET) {
					dx = (object->targetX - x) / toFIXED(1);
					dz = (object->targetZ - z) / toFIXED(1);
					distance = toFIXED(
						sySquartIWRAM(fdObjectSquare(dx) + fdObjectSquare(dz)));
					if(distance <= toFIXED(256) - 1) {
						dx = object->targetX - x;
						dz = object->targetZ - z;
						distance =
							sySquartFX(syMulFX(dx, dx) + syMulFX(dz, dz));
					}
					if(distance == 0) {
						x = object->targetX;
						z = object->targetZ;
					} else {
						ratio = syDivFX(distance, object->acceleration);
						object->velocityX += syMulFX(dx, ratio);
						object->velocityZ += syMulFX(dz, ratio);
						velocityX = object->velocityX;
						velocityZ = object->velocityZ;
						speed = sySquartFX(syMulFX(velocityX, velocityX) +
							syMulFX(velocityZ, velocityZ));
						if(speed > object->speedLimit) {
							ratio = syDivFX(speed, object->speedLimit);
							object->velocityX = syMulFX(velocityX, ratio);
							object->velocityZ = syMulFX(velocityZ, ratio);
						}
					}
				} else {
					dx = object->velocityX;
					dz = object->velocityZ;
					speed = sySquartFX(syMulFX(dx, dx) + syMulFX(dz, dz));
					if(speed != 0) {
						reducedSpeed = speed - object->acceleration;
						if(reducedSpeed < 0)
							reducedSpeed = 0;
						ratio = syDivFX(speed, reducedSpeed);
						object->velocityX = syMulFX(dx, ratio);
						object->velocityZ = syMulFX(dz, ratio);
					} else {
						object->velocityX = speed;
						object->velocityZ = speed;
					}
				}

				/* Follow the ground, or the top of a platform 16 pixels
				   below. */
				if(object->motionFlags & ACTOR_FOLLOWS_TERRAIN) {
					velocityX = x + object->velocityX;
					velocityZ = z + object->velocityZ;
					standing = 0;
					object->carrier = NULL;
					if(object->priorityFlags &
						ACTOR_PRIORITY_WALKS_ON_PLATFORMS) {
						ACTOR *other = RamHeapSlots->actors;
						Sint32 i;

						for(i = 0; i < ACTOR_COUNT; i++, other++) {
							Sint32 dy;

							if(other->script == NULL)
								continue;
							if(!(other->priorityFlags &
								   ACTOR_PRIORITY_PLATFORM))
								continue;
							if(other == object)
								continue;
							if((FIELD_METATILE(velocityX - other->x.fixed +
								   toFIXED(8))) != 0)
								continue;
							if((FIELD_METATILE(velocityZ - other->z.fixed +
								   toFIXED(8))) != 0)
								continue;
							dy = y - other->y.fixed;
							if((dy - toFIXED(16) < 0
									   ? -(dy - toFIXED(16))
									   : dy - toFIXED(16)) > toFIXED(8) - 1)
								continue;
							standing = 1;
							ground = other->y.fixed + toFIXED(16);
							object->carrier = (union FIELD_OBJECT *)other;
							break;
						}
					}
					if(!standing)
						ground = fdMapGridGetTerrainHeight(
							object->layer, velocityX, velocityZ);
					velocityY = ground - object->groundHeight;
					if(ground - y > -toFIXED(4))
						y += velocityY;
					if(velocityY < 0)
						velocityY = -velocityY;
					maximumChange = object->acceleration / 2;
					if(velocityY > maximumChange)
						velocityY = maximumChange;
					velocityY *= 3;
					if(velocityY != 0 &&
						!(object->motionFlags & ACTOR_KEEPS_SPEED_ON_SLOPES) &&
						!standing) {
						dx = object->velocityX;
						dy = object->velocityY;
						dz = object->velocityZ;
						speed = sySquartFX(syIWRAMMulFX(dx, dx) +
							syIWRAMMulFX(dy, dy) + syIWRAMMulFX(dz, dz));
						if(speed != 0) {
							reducedSpeed = speed - velocityY;
							if(reducedSpeed < 0)
								reducedSpeed = 0;
							ratio = syDivFX(speed, reducedSpeed);
							object->velocityX = syIWRAMMulFX(dx, ratio);
							object->velocityY = syIWRAMMulFX(dy, ratio);
							object->velocityZ = syIWRAMMulFX(dz, ratio);
						}
					}
					object->groundHeight = ground;
				}

				/* Gravity, and a bounce that loses energy on landing. */
				if(object->motionFlags & ACTOR_FALLS) {
					ground = object->groundHeight;
					if(y > ground)
						object->velocityY -= object->gravity;
					if(y + object->velocityY <= ground) {
						if(object->velocityY < 0) {
							FIXED rebound = -syIWRAMMulFX(
								object->velocityY, object->bounce);

							object->velocityY = rebound;
							y = ground - rebound;
							if((rebound < 0 ? -rebound : rebound) <=
								object->gravity) {
								y = ground;
								object->velocityY = 0;
							}
						} else {
							y = ground;
						}
					}
				}

				/* Float: bob up and down on a triangle wave. */
				if(object->motionFlags & ACTOR_FLOATS) {
					Uint32 phase = object->bounce & (FLOAT_CYCLE_TICKS - 1);

					if(object->motionFlags & ACTOR_FLOATS_HIGH)
						object->velocityY = ActorScriptCycleTable[phase >> 1] *
							object->gravity / FLOAT_HIGH_DIVISOR;
					else
						object->velocityY = ActorScriptCycleTable[phase >> 1] *
							object->gravity / FLOAT_DIVISOR;
					object->bounce++;
				}
			}
		}

		x += object->velocityX;
		y += object->velocityY;
		z += object->velocityZ;
		if(object->collisionFlags & ACTOR_BLOCKED_BY_SOLID) {
			pos[0] = x;
			pos[1] = y;
			pos[2] = z;
			if(fdActorScriptCheckOverlap(object, pos) != 0) {
				object->refused++;
				continue;
			}
			object->refused = 0;
		}

		switch(object->arrivalAxis) {
		case ARRIVAL_AXIS_X:
			if(x == object->targetX ||
				((object->x.fixed - object->targetX) ^ (x - object->targetX)) <
					0)
				arrivedAtTarget = 1;
			break;
		case ARRIVAL_AXIS_Y:
			if(y == object->targetY ||
				((object->y.fixed - object->targetY) ^ (y - object->targetY)) <
					0)
				arrivedAtTarget = 1;
			break;
		case ARRIVAL_AXIS_Z:
			if(z == object->targetZ ||
				((object->z.fixed - object->targetZ) ^ (z - object->targetZ)) <
					0)
				arrivedAtTarget = 1;
			break;
		}
		if(arrivedAtTarget != 0) {
			if(object->snapToTarget != 0) {
				x = object->targetX;
				object->velocityX = 0;
				z = object->targetZ;
				object->velocityZ = 0;
				if(object->motionFlags == 0) {
					y = object->targetY;
					object->velocityY = 0;
				}
			}
			object->targetX = ACTOR_NO_TARGET;
			object->targetY = ACTOR_NO_TARGET;
			object->targetZ = ACTOR_NO_TARGET;
			object->arrivalAxis = ARRIVAL_AXIS_NONE;
		}

		object->stepX = x - object->x.fixed;
		object->stepY = y - object->y.fixed;
		object->stepZ = z - object->z.fixed;
		SET_ACTOR_POSITION(object, x, y, z);

		/* Turn to face the direction of travel, at most a sixteenth of a turn a
		   tick. */
		if(object->steeringFlags & ACTOR_FACES_MOTION) {
			x = object->velocityX;
			z = object->velocityZ;
			if(x != 0 || z != 0) {
				Sint32 facingStep;
				Sint32 maximumFacingStep;

				facingStep = (Sint16)(syAtan(z, x) - object->facing);

				maximumFacingStep = FACING_STEP;
				if(facingStep > maximumFacingStep)
					facingStep = maximumFacingStep;
				maximumFacingStep = -FACING_STEP;
				if(facingStep < maximumFacingStep)
					facingStep = maximumFacingStep;
				object->facing += facingStep;
			}
		}
	}
}

/* Each tick: an actor that walks on platforms
   (ACTOR_PRIORITY_WALKS_ON_PLATFORMS) and rides one, its carrier, moves with
   the carrier's last step; one standing on it as it rises or sinks is set on
   its top, 16 pixels up. */
void fdActorMoveWithCarriers(void)
{
	ACTOR *actor = RamHeapSlots->actors;
	Sint32 i;

	for(i = 0; i < ACTOR_COUNT; i++, actor++) {
		if(actor->script != NULL) {
			FIXED x = actor->x.fixed;
			FIXED y = actor->y.fixed;
			FIXED z = actor->z.fixed;

			if(actor->priorityFlags & ACTOR_PRIORITY_WALKS_ON_PLATFORMS) {
				ACTOR *carrier = (ACTOR *)actor->carrier;

				if(carrier != NULL) {
					if(carrier->stepY != 0) {
						if(y == actor->groundHeight) {
							actor->groundHeight =
								carrier->y.fixed + toFIXED(16);
							y = actor->groundHeight;
						} else {
							actor->groundHeight += carrier->stepY;
							y += carrier->stepY;
						}
					}
					x += carrier->stepX;
					z += carrier->stepZ;
				}
			}
			SET_ACTOR_POSITION(actor, x, y, z);
		}
	}
}

/* Placing an actor, and sending it somewhere. */

/* Places the actor at (x, y, z), in 16.16 pixels, clears its movement target
   and stops it. */
void fdActorSetPosAndResetMotion(ACTOR *actor, FIXED x, FIXED y, FIXED z)
{
	actor->z.fixed = z;
	actor->targetX = ACTOR_NO_TARGET;
	actor->targetY = ACTOR_NO_TARGET;
	actor->targetZ = ACTOR_NO_TARGET;
	actor->x.fixed = x;
	actor->y.fixed = y;
	SET_VELOCITY(actor, 0, 0, 0);
}

/* Sends an actor towards (x, y, z), in 16.16 pixels. Within a pixel it arrives
   at once. */
void fdActorSetMoveTarget(ACTOR *actor, FIXED x, FIXED y, FIXED z)
{
	Sint32 dx;
	FIXED  distance;
	Sint32 dz;
	Sint32 squaredZ;
	Sint32 dy;
	Uint8 *arrivalAxis;

	dx = (x - actor->x.fixed) / toFIXED(1);
	dy = (y - actor->y.fixed) / toFIXED(1);
	dz = (z - actor->z.fixed) / toFIXED(1);
	squaredZ = dz * dz;
	distance = toFIXED(sySquartIWRAM(dx * dx + dy * dy + squaredZ));
	if(distance < toFIXED(16)) {
		dx = x - actor->x.fixed;
		dy = y - actor->y.fixed;
		dz = z - actor->z.fixed;
		distance =
			sySquartFX(syMulFX(dx, dx) + syMulFX(dy, dy) + syMulFX(dz, dz));
	}
	if(distance < toFIXED(1)) {
		SET_ACTOR_POSITION(actor, x, y, z);
		actor->targetX = ACTOR_NO_TARGET;
		actor->targetY = ACTOR_NO_TARGET;
		actor->targetZ = ACTOR_NO_TARGET;
		return;
	}
	if(actor->snapToTarget == 0) {
		FIXED brakingDistance;
		FIXED ratio;

		brakingDistance = syDivFX(
			actor->acceleration, syMulFX(actor->speedLimit, actor->speedLimit));
		if(distance > brakingDistance)
			brakingDistance = distance - brakingDistance / 2;
		else
			brakingDistance = distance / 2;
		ratio = syDivFX(distance, brakingDistance);
		x = actor->x.fixed + syMulFX(x - actor->x.fixed, ratio);
		y = actor->y.fixed + syMulFX(y - actor->y.fixed, ratio);
		z = actor->z.fixed + syMulFX(z - actor->z.fixed, ratio);
	}
	actor->targetX = x;
	actor->targetY = y;
	actor->targetZ = z;
	dx = x - actor->x.fixed;
	dy = y - actor->y.fixed;
	dz = z - actor->z.fixed;
	arrivalAxis = &actor->arrivalAxis;
	*arrivalAxis = ARRIVAL_AXIS_X;
	if((dx < 0 ? -dx : dx) < (dz < 0 ? -dz : dz)) {
		*arrivalAxis = ARRIVAL_AXIS_Z;
		dx = dz;
	}
	if(actor->motionFlags == 0) {
		if(dx < 0)
			dx = -dx;
		if(dx < (dy < 0 ? -dy : dy))
			*arrivalAxis = ARRIVAL_AXIS_Y;
	}
}

/* Lists those of the first RUNNING_LIST_SLOTS actors whose script is running,
   in slot order, ended by NULL; out holds one more. */
void fdActorListRunning(ACTOR *actor, ACTOR **out)
{
	ACTOR *end = actor + RUNNING_LIST_SLOTS;

	do {
		/* A running script's address is above 0xffff, so its upper half alone
		   tells a running actor from a free one. */
		if(((Uint16 *)&actor->script)[1] != 0)
			*out++ = actor;
		actor++;
	} while(actor != end);
	*out = NULL;
}

#undef SCRIPT_COMMAND_MAX
#undef FLOAT_CYCLE_TICKS
#undef FLOAT_DIVISOR
#undef FLOAT_HIGH_DIVISOR
