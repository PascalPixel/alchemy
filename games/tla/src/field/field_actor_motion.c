/* @file */
/* Actors' motion each frame: their scripts, steering toward a target, falling,
 */
/* floating and riding a platform, and being sent somewhere. */
/* Part of field.c. */

#include "field/resource_slot.h"

/* Returns 1 when the actor has no movement target: none of its target */
/* coordinates set, or for an actor moved by the ground (any motion flag) */
/* neither its target x nor z. */
s32 fdActorIsTargetUnset(ACTOR *actor) {
	s32 first;
	s32 second;

	/* One compare of three values: all must equal ACTOR_NO_TARGET, or with */
	/* motion flags only x and z. */
	if(actor->motionFlg == 0) {
		second = actor->tgtX;
		if(second != ACTOR_NO_TARGET)
			return 0;
		first = actor->tgtY;
	}
	else {
		first = actor->tgtX;
		second = ACTOR_NO_TARGET;
	}
	if(first != second || actor->tgtZ != first)
		return 0;
	return 1;
}

/* fdActorIsTargetUnset, for the scenes: 1 once the actor has reached its */
/* destination. Airs Rock waits on it while the lift carries the leader up. */
s32 fdActorHasArrived(ACTOR *actor) { return fdActorIsTargetUnset(actor); }

extern const SCRIPT_COMMAND_FN ActorScript_CommandTable[];
extern const s32 ActorScript_CycleTable[];

/* The highest script word read as a command, an index of */
/* ActorScript_CommandTable; a larger one is stepped over. */
#define SCRIPT_COMMAND_MAX 63
/* A floating actor's bob, one cycle of ActorScript_CycleTable over this many */
/* frames; its rise and fall each frame is the table's value times its gravity
 */
/* over FLOAT_DIVISOR, or FLOAT_HIGH_DIVISOR with ACTOR_FLOATS_HIGH. */
#define FLOAT_CYCLE_FRAMES 64
#define FLOAT_DIVISOR	   64
#define FLOAT_HIGH_DIVISOR 16

/* A whole-pixel distance squared. */
static __inline__ s32 fdObjectSquare(s32 value) { return value * value; }

/* Advances every actor by one frame: their scripts and their motion, with */
/* free flight in 3D, following the ground, floating, and solid actors that */
/* refuse to overlap. */
void fdActorUpdateAllOnField(void) {
	ACTOR *object;
	s32 objectsRemaining;
	s32 x;
	FIXED y;
	s32 z;
	FIXED ground;
	s32 arrivedAtTarget;
	FIXED pos[3];
	s32 dx;
	s32 dy;
	s32 dz;
	s32 velocityX;
	s32 velocityY;
	s32 velocityZ;
	s32 distance;
	FIXED speed;
	FIXED reducedSpeed;
	FIXED ratio;
	s32 maximumChange;
	s32 standing;

	object = Ram_HeapSlots->actors;
	for(objectsRemaining = ACTOR_COUNT - 1; objectsRemaining >= 0;
		objectsRemaining--, object++) {
		const s32 *script;

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
		}
		else {
			for(;;) {
				u32 command;

				/* The local steps to the command's word; the next pass takes
				 * the */
				/* script afresh. */
				script += object->scriptCursor;
				command = *script;

				if(command <= SCRIPT_COMMAND_MAX) {
					if(ActorScript_CommandTable[command](object) == 0)
						break;
				}
				else {
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
			if(object->motionFlg == 0) {
				if(object->tgtX != ACTOR_NO_TARGET) {
					dx = (object->tgtX - x) / FIXED_ONE;
					dy = (object->tgtY - y) / FIXED_ONE;
					dz = (object->tgtZ - z) / FIXED_ONE;
					distance =
						sySquart(fdObjectSquare(dx) + fdObjectSquare(dy) +
								 fdObjectSquare(dz));
					if(distance == 0) {
						x = object->tgtX;
						y = object->tgtY;
						z = object->tgtZ;
					}
					else {
						ratio =
							syDivFX(toFIXED(distance), object->acceleration);
						object->velX += dx * ratio;
						object->velY += dy * ratio;
						object->velZ += dz * ratio;
						velocityX = object->velX;
						velocityY = object->velY;
						velocityZ = object->velZ;
						speed = sySquartFX(syMulFX(velocityX, velocityX) +
										   syMulFX(velocityY, velocityY) +
										   syMulFX(velocityZ, velocityZ));
						if(speed > object->speedLimit) {
							ratio = syDivFX(speed, object->speedLimit);
							object->velX = syMulFX(velocityX, ratio);
							object->velY = syMulFX(velocityY, ratio);
							object->velZ = syMulFX(velocityZ, ratio);
						}
					}
				}
				else {
					dx = object->velX;
					dy = object->velY;
					dz = object->velZ;
					speed = sySquartFX(syMulFX(dx, dx) + syMulFX(dy, dy) +
									   syMulFX(dz, dz));
					if(speed != 0) {
						reducedSpeed = speed - object->acceleration;
						if(reducedSpeed < 0)
							reducedSpeed = 0;
						ratio = syDivFX(speed, reducedSpeed);
						object->velX = syMulFX(dx, ratio);
						object->velY = syMulFX(dy, ratio);
						object->velZ = syMulFX(dz, ratio);
					}
					else {
						object->velX = speed;
						object->velY = speed;
						object->velZ = speed;
					}
				}
			}
			else {
				/* On the ground: steer in the XZ plane the same way. */
				if(object->tgtX != ACTOR_NO_TARGET) {
					dx = (object->tgtX - x) / FIXED_ONE;
					dz = (object->tgtZ - z) / FIXED_ONE;
					distance = toFIXED(
						sySquart(fdObjectSquare(dx) + fdObjectSquare(dz)));
					if(distance <= toFIXED(256) - 1) {
						dx = object->tgtX - x;
						dz = object->tgtZ - z;
						distance =
							sySquartFX(syMulFX(dx, dx) + syMulFX(dz, dz));
					}
					if(distance == 0) {
						x = object->tgtX;
						z = object->tgtZ;
					}
					else {
						ratio = syDivFX(distance, object->acceleration);
						object->velX += syMulFX(dx, ratio);
						object->velZ += syMulFX(dz, ratio);
						velocityX = object->velX;
						velocityZ = object->velZ;
						speed = sySquartFX(syMulFX(velocityX, velocityX) +
										   syMulFX(velocityZ, velocityZ));
						if(speed > object->speedLimit) {
							ratio = syDivFX(speed, object->speedLimit);
							object->velX = syMulFX(velocityX, ratio);
							object->velZ = syMulFX(velocityZ, ratio);
						}
					}
				}
				else {
					dx = object->velX;
					dz = object->velZ;
					speed = sySquartFX(syMulFX(dx, dx) + syMulFX(dz, dz));
					if(speed != 0) {
						reducedSpeed = speed - object->acceleration;
						if(reducedSpeed < 0)
							reducedSpeed = 0;
						ratio = syDivFX(speed, reducedSpeed);
						object->velX = syMulFX(dx, ratio);
						object->velZ = syMulFX(dz, ratio);
					}
					else {
						object->velX = speed;
						object->velZ = speed;
					}
				}

				/* Follow the ground, or the top of a platform 16 pixels below.
				 */
				if(object->motionFlg & ACTOR_FOLLOWS_TERRAIN) {
					velocityX = x + object->velX;
					velocityZ = z + object->velZ;
					standing = 0;
					object->carrier = NULL;
					if(object->priFlg & ACTOR_PRIORITY_WALKS_ON_PLATFORMS) {
						ACTOR *other = Ram_HeapSlots->actors;
						s32 i;

						for(i = 0; i < ACTOR_COUNT; i++, other++) {
							s32 dy;

							if(other->script == NULL)
								continue;
							if(!(other->priFlg & ACTOR_PRIORITY_PLATFORM))
								continue;
							if(other == object)
								continue;
							if((FIELD_CELL(velocityX - other->x.fixed +
										   toFIXED(8))) != 0)
								continue;
							if((FIELD_CELL(velocityZ - other->z.fixed +
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
					   !(object->motionFlg & ACTOR_KEEPS_SPEED_ON_SLOPES) &&
					   !standing) {
						dx = object->velX;
						dy = object->velY;
						dz = object->velZ;
						speed =
							sySquartFX(IwramMulFX(dx, dx) + IwramMulFX(dy, dy) +
									   IwramMulFX(dz, dz));
						if(speed != 0) {
							reducedSpeed = speed - velocityY;
							if(reducedSpeed < 0)
								reducedSpeed = 0;
							ratio = syDivFX(speed, reducedSpeed);
							object->velX = IwramMulFX(dx, ratio);
							object->velY = IwramMulFX(dy, ratio);
							object->velZ = IwramMulFX(dz, ratio);
						}
					}
					object->groundHeight = ground;
				}

				/* Gravity, and a bounce that loses energy on landing. */
				if(object->motionFlg & ACTOR_FALLS) {
					ground = object->groundHeight;
					if(y > ground)
						object->velY -= object->gravity;
					if(y + object->velY <= ground) {
						if(object->velY < 0) {
							FIXED rebound =
								-IwramMulFX(object->velY, object->bounce);

							object->velY = rebound;
							y = ground - rebound;
							if((rebound < 0 ? -rebound : rebound) <=
							   object->gravity) {
								y = ground;
								object->velY = 0;
							}
						}
						else {
							y = ground;
						}
					}
				}

				/* Float: bob up and down on a triangle wave. */
				if(object->motionFlg & ACTOR_FLOATS) {
					u32 phase = object->bounce & (FLOAT_CYCLE_FRAMES - 1);

					if(object->motionFlg & ACTOR_FLOATS_HIGH)
						object->velY = ActorScript_CycleTable[phase >> 1] *
									   object->gravity / FLOAT_HIGH_DIVISOR;
					else
						object->velY = ActorScript_CycleTable[phase >> 1] *
									   object->gravity / FLOAT_DIVISOR;
					object->bounce++;
				}
			}
		}

		x += object->velX;
		y += object->velY;
		z += object->velZ;
		if(object->collisionFlg & ACTOR_BLOCKED_BY_SOLID) {
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
			if(x == object->tgtX ||
			   ((object->x.fixed - object->tgtX) ^ (x - object->tgtX)) < 0)
				arrivedAtTarget = 1;
			break;
		case ARRIVAL_AXIS_Y:
			if(y == object->tgtY ||
			   ((object->y.fixed - object->tgtY) ^ (y - object->tgtY)) < 0)
				arrivedAtTarget = 1;
			break;
		case ARRIVAL_AXIS_Z:
			if(z == object->tgtZ ||
			   ((object->z.fixed - object->tgtZ) ^ (z - object->tgtZ)) < 0)
				arrivedAtTarget = 1;
			break;
		}
		if(arrivedAtTarget != 0) {
			if(object->snapToTgt != 0) {
				x = object->tgtX;
				object->velX = 0;
				z = object->tgtZ;
				object->velZ = 0;
				if(object->motionFlg == 0) {
					y = object->tgtY;
					object->velY = 0;
				}
			}
			object->tgtX = ACTOR_NO_TARGET;
			object->tgtY = ACTOR_NO_TARGET;
			object->tgtZ = ACTOR_NO_TARGET;
			object->arrivalAxis = ARRIVAL_AXIS_NONE;
		}

		object->stepX = x - object->x.fixed;
		object->stepY = y - object->y.fixed;
		object->stepZ = z - object->z.fixed;
		SET_ACTOR_POSITION(object, x, y, z);

		/* Turn to face the direction of travel, at most a sixteenth of a turn a
		 */
		/* frame. */
		if(object->steeringFlg & ACTOR_FACES_MOTION) {
			x = object->velX;
			z = object->velZ;
			if(x != 0 || z != 0) {
				s32 facingStep;
				s32 maximumFacingStep;

				facingStep = (s16)(syAtan(z, x) - object->facing);

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

/* Each frame: an actor that walks on platforms */
/* (ACTOR_PRIORITY_WALKS_ON_PLATFORMS) and rides one, its carrier, moves with */
/* the carrier's last step; one standing on it as it rises or sinks is set on */
/* its top, 16 pixels up. */
void fdActorMoveWithCarriers(void) {
	ACTOR *actor = Ram_HeapSlots->actors;
	s32 i;

	for(i = 0; i < ACTOR_COUNT; i++, actor++) {
		if(actor->script != NULL) {
			FIXED x = actor->x.fixed;
			FIXED y = actor->y.fixed;
			FIXED z = actor->z.fixed;

			if(actor->priFlg & ACTOR_PRIORITY_WALKS_ON_PLATFORMS) {
				ACTOR *carrier = (ACTOR *)actor->carrier;

				if(carrier != NULL) {
					if(carrier->stepY != 0) {
						if(y == actor->groundHeight) {
							actor->groundHeight =
								carrier->y.fixed + toFIXED(16);
							y = actor->groundHeight;
						}
						else {
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

/* Places the actor at (x, y, z), in 16.16 pixels, clears its movement target */
/* and stops it. */
void fdActorSetPosAndResetMotion(ACTOR *actor, FIXED x, FIXED y, FIXED z) {
	actor->z.fixed = z;
	actor->tgtX = ACTOR_NO_TARGET;
	actor->tgtY = ACTOR_NO_TARGET;
	actor->tgtZ = ACTOR_NO_TARGET;
	actor->x.fixed = x;
	actor->y.fixed = y;
	SET_VELOCITY(actor, 0, 0, 0);
}

/* Sends an actor towards (x, y, z), in 16.16 pixels. Within a pixel it arrives
 */
/* at once. */
void fdActorSetMoveTarget(ACTOR *actor, FIXED x, FIXED y, FIXED z) {
	s32 dx;
	FIXED distance;
	s32 dz;
	s32 squaredZ;
	s32 dy;
	u8 *arrivalAxis;

	dx = (x - actor->x.fixed) / FIXED_ONE;
	dy = (y - actor->y.fixed) / FIXED_ONE;
	dz = (z - actor->z.fixed) / FIXED_ONE;
	squaredZ = dz * dz;
	distance = toFIXED(sySquart(dx * dx + dy * dy + squaredZ));
	if(distance < toFIXED(16)) {
		dx = x - actor->x.fixed;
		dy = y - actor->y.fixed;
		dz = z - actor->z.fixed;
		distance =
			sySquartFX(syMulFX(dx, dx) + syMulFX(dy, dy) + syMulFX(dz, dz));
	}
	if(distance < FIXED_ONE) {
		SET_ACTOR_POSITION(actor, x, y, z);
		actor->tgtX = ACTOR_NO_TARGET;
		actor->tgtY = ACTOR_NO_TARGET;
		actor->tgtZ = ACTOR_NO_TARGET;
		return;
	}
	if(actor->snapToTgt == 0) {
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
	actor->tgtX = x;
	actor->tgtY = y;
	actor->tgtZ = z;
	dx = x - actor->x.fixed;
	dy = y - actor->y.fixed;
	dz = z - actor->z.fixed;
	arrivalAxis = &actor->arrivalAxis;
	*arrivalAxis = ARRIVAL_AXIS_X;
	if((dx < 0 ? -dx : dx) < (dz < 0 ? -dz : dz)) {
		*arrivalAxis = ARRIVAL_AXIS_Z;
		dx = dz;
	}
	if(actor->motionFlg == 0) {
		if(dx < 0)
			dx = -dx;
		if(dx < (dy < 0 ? -dy : dy))
			*arrivalAxis = ARRIVAL_AXIS_Y;
	}
}

/* Lists those of the first RUNNING_LIST_SLOTS actors whose script is running,
 */
/* in slot order, ended by NULL; out holds one more. */
void fdActorListRunning(ACTOR *actor, ACTOR **out) {
	ACTOR *end = actor + RUNNING_LIST_SLOTS;

	do {
		/* A running script's address is above 0xffff, so its upper half alone
		 */
		/* tells a running actor from a free one. */
		if(((u16 *)&actor->script)[1] != 0)
			*out++ = actor;
		actor++;
	} while(actor != end);
	*out = NULL;
}
#undef SCRIPT_COMMAND_MAX
#undef FLOAT_CYCLE_FRAMES
#undef FLOAT_DIVISOR
#undef FLOAT_HIGH_DIVISOR
