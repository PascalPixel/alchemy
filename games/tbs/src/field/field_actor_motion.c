/* Actor motion, each frame, and the first actor script commands. Part of
   field.c. */

#include "field_internal.h"

/* SC_BASE: restarts the script at the next word, and returns 1 to run on. */
Sint32 fdActorScriptSetBase(ACTOR *interpreter)
{
	interpreter->script = interpreter->script + interpreter->scriptCursor + 1;
	interpreter->scriptCursor = 0;
	return 1;
}

/* SC_HIDE_SPRITES: hides the actor's sprites and runs on. */
Sint32 fdActorScriptHideSprs(ACTOR *interpreter)
{
	interpreter->sprStorageKind = ACTOR_SPRITE_NONE;
	interpreter->scriptCursor++;
	return 1;
}

/* SC_SHOW_SPRITE: shows the actor's one sprite and runs on. */
Sint32 fdActorScriptShowSpr(ACTOR *interpreter)
{
	interpreter->sprStorageKind = ACTOR_SPRITE_ONE;
	interpreter->scriptCursor++;
	return 1;
}

/* Waits a frame at a time until an actor has arrived at its target, at most 600
   frames (ten seconds), after which it returns with the actor still on its
   way. */
void fdActorWaitForMove(void *actor)
{
	Sint32 waitedFrames;

	for(waitedFrames = 0;
		waitedFrames < 600 && fdActorIsTargetUnset(actor) == 0;
		waitedFrames++) {
		sySynch(1);
	}
}

/* Returns 1 when the actor has no movement target: none of its target
   coordinates set, or for an actor moved by the ground (any motion flag)
   neither its target x nor z. */
Bool fdActorIsTargetUnset(ACTOR *actor)
{
	Sint32 first;
	Sint32 second;

	/* One compare of three values: all must equal ACTOR_NO_TARGET, or with
	   motion flags only x and z. */
	if(actor->motionFlg == 0) {
		second = actor->tgtX;
		if(second != ACTOR_NO_TARGET)
			return FALSE;
		first = actor->tgtY;
	} else {
		first = actor->tgtX;
		second = ACTOR_NO_TARGET;
	}
	if(first != second || actor->tgtZ != first)
		return FALSE;
	return TRUE;
}

/* A floating actor's bob, one cycle of ActorScript_CycleTable over this many
   frames; its rise and fall each frame is the table's value times its gravity
   over FLOAT_DIVISOR, or FLOAT_HIGH_DIVISOR with ACTOR_FLOATS_HIGH. */
#define FLOAT_CYCLE_FRAMES 64
#define FLOAT_DIVISOR	   64
#define FLOAT_HIGH_DIVISOR 16

static __inline__ Sint32 fdActorSquare(Sint32 value)
{
	return value * value;
}

/* Advances all 64 actors by one frame, as fdActorUpdateAll does for the eight
   script-driven actors, with the field's extras: free flight in 3D, following
   the ground, floating, and solid actors that refuse to overlap. */
void fdActorUpdateAllOnField(void)
{
	ACTOR		 *actor;
	const Sint32 *script;
	Sint32		  actorsLeft;
	FIXED		  x;
	FIXED		  y;
	FIXED		  z;
	FIXED		  ground;
	Sint32		  arrivedAtTarget;
	Sint32		  pos[3];
	Sint32		  dx;
	Sint32		  dy;
	Sint32		  dz;
	FIXED		  velocityX;
	Sint32		  velocityY;
	FIXED		  velocityZ;
	Sint32		  distance;
	FIXED		  speed;
	FIXED		  ratio;
	FIXED		  reducedSpeed;
	Sint32		  maxTerrainChange;
	Sint32		  facingStep;
	Uint32		  command;
	Uint32		  phase;

	actor = ActorsWork;
	for(actorsLeft = ACTOR_COUNT - 1; actorsLeft >= 0; actorsLeft--, actor++) {
		arrivedAtTarget = 0;

		script = actor->script;
		if(script == NULL)
			continue;

		/* The update function runs even for a paused actor, and may release
		   it. */
		if(actor->update != NULL) {
			actor->update((ACTOR *)actor);
			script = actor->script;
		}
		if(script == NULL)
			continue;
		if(actor->paused != 0)
			continue;

		/* Count down a wait, or run commands until one waits for this frame. */
		if(actor->wait != 0) {
			actor->wait--;
		} else {
			for(;;) {
				/* The local steps to the command's word; the next pass takes
				   the script afresh. */
				script += actor->scriptCursor;
				command = *script;
				if(command <= SCRIPT_COMMAND_MAX) {
					if(ActorScript_CommandTable[command]((ACTOR *)actor) == 0)
						break;
				} else {
					actor->scriptCursor++;
				}
				script = actor->script;
			}
			if(actor->script == NULL)
				continue;
		}

		x = actor->x.fixed;
		y = actor->y.fixed;
		z = actor->z.fixed;

		if(actor->frozen == 0) {
			/* Free flight (no motion flags): steer in 3D towards the target,
			   capped at the speed limit, or brake when there is none. */
			if(actor->motionFlg == 0) {
				if(actor->tgtX != ACTOR_NO_TARGET) {
					dx = (actor->tgtX - x) / toFIXED(1);
					dy = (actor->tgtY - y) / toFIXED(1);
					dz = (actor->tgtZ - z) / toFIXED(1);
					distance = sySquartIWRAM(fdActorSquare(dx) +
						fdActorSquare(dy) + fdActorSquare(dz));
					if(distance == 0) {
						x = actor->tgtX;
						y = actor->tgtY;
						z = actor->tgtZ;
					} else {
						/* ratio is acceleration / distance in 16.16, and dx, dy
						   and dz are whole pixels, so each axis gains its share
						   of the acceleration toward the target. */
						ratio = syDivFX(toFIXED(distance), actor->acceleration);
						actor->velX += dx * ratio;
						actor->velY += dy * ratio;
						actor->velZ += dz * ratio;
						velocityX = actor->velX;
						velocityY = actor->velY;
						velocityZ = actor->velZ;
						speed = sySquartFX(syMulFX(velocityX, velocityX) +
							syMulFX(velocityY, velocityY) +
							syMulFX(velocityZ, velocityZ));
						/* Too fast: scale the velocity by speedLimit /
						   speed. */
						if(speed > actor->speedLimit) {
							ratio = syDivFX(speed, actor->speedLimit);
							SET_VELOCITY(actor, syMulFX(velocityX, ratio),
								syMulFX(velocityY, ratio),
								syMulFX(velocityZ, ratio));
						}
					}
				} else {
					/* No target: take acceleration off the speed each frame,
					   keeping the direction, until it stops. */
					dx = actor->velX;
					dy = actor->velY;
					dz = actor->velZ;
					speed = sySquartFX(
						syMulFX(dx, dx) + syMulFX(dy, dy) + syMulFX(dz, dz));
					if(speed != 0) {
						reducedSpeed = speed - actor->acceleration;
						if(reducedSpeed < 0)
							reducedSpeed = 0;
						ratio = syDivFX(speed, reducedSpeed);
						SET_VELOCITY(actor, syMulFX(dx, ratio),
							syMulFX(dy, ratio), syMulFX(dz, ratio));
					} else {
						SET_VELOCITY(actor, 0, 0, 0);
					}
				}
			} else {
				/* On the ground: steer in the XZ plane the same way. */
				if(actor->tgtX != ACTOR_NO_TARGET) {
					dx = (actor->tgtX - x) / toFIXED(1);
					dz = (actor->tgtZ - z) / toFIXED(1);
					distance = toFIXED(
						sySquartIWRAM(fdActorSquare(dx) + fdActorSquare(dz)));
					/* Under 256 pixels, measure again in 16.16 for sub-pixel
					   accuracy; further away the squares would overflow. */
					if(distance <= toFIXED(256) - 1) {
						dx = actor->tgtX - x;
						dz = actor->tgtZ - z;
						distance =
							sySquartFX(syMulFX(dx, dx) + syMulFX(dz, dz));
					}
					if(distance == 0) {
						x = actor->tgtX;
						z = actor->tgtZ;
					} else {
						ratio = syDivFX(distance, actor->acceleration);
						actor->velX += syMulFX(dx, ratio);
						actor->velZ += syMulFX(dz, ratio);
						velocityX = actor->velX;
						velocityZ = actor->velZ;
						speed = sySquartFX(syMulFX(velocityX, velocityX) +
							syMulFX(velocityZ, velocityZ));
						if(speed > actor->speedLimit) {
							ratio = syDivFX(speed, actor->speedLimit);
							actor->velX = syMulFX(velocityX, ratio);
							actor->velZ = syMulFX(velocityZ, ratio);
						}
					}
				} else {
					dx = actor->velX;
					dz = actor->velZ;
					speed = sySquartFX(syMulFX(dx, dx) + syMulFX(dz, dz));
					if(speed != 0) {
						reducedSpeed = speed - actor->acceleration;
						if(reducedSpeed < 0)
							reducedSpeed = 0;
						ratio = syDivFX(speed, reducedSpeed);
						actor->velX = syMulFX(dx, ratio);
						actor->velZ = syMulFX(dz, ratio);
					} else {
						actor->velX = 0;
						actor->velZ = 0;
					}
				}

				/* Follow the ground: take the ground height where the actor is
				   going and move with its change, unless the actor is 4 or more
				   pixels above the ground. */
				if((actor->motionFlg & ACTOR_FOLLOWS_TERRAIN) != 0) {
					velocityX = x + actor->velX;
					velocityZ = z + actor->velZ;
					ground = fdMapGridGetTerrainHeight(
						actor->layer, velocityX, velocityZ);
					velocityY = ground - actor->groundHeight;
					if(ground - y > -toFIXED(4))
						y += velocityY;
					if(velocityY < 0)
						velocityY = -velocityY;
					maxTerrainChange = actor->acceleration / 2;
					if(velocityY > maxTerrainChange)
						velocityY = maxTerrainChange;
					velocityY *= 3;
					if(velocityY != 0 &&
						(actor->motionFlg & ACTOR_KEEPS_SPEED_ON_SLOPES) == 0) {
						dx = actor->velX;
						dy = actor->velY;
						dz = actor->velZ;
						speed = sySquartFX(syMulFX(dx, dx) + syMulFX(dy, dy) +
							syMulFX(dz, dz));
						if(speed != 0) {
							reducedSpeed = speed - velocityY;
							if(reducedSpeed < 0)
								reducedSpeed = 0;
							ratio = syDivFX(speed, reducedSpeed);
							SET_VELOCITY(actor, syMulFX(dx, ratio),
								syMulFX(dy, ratio), syMulFX(dz, ratio));
						}
					}
					actor->groundHeight = ground;
				}

				/* Gravity, and a bounce that loses energy on landing. */
				if((actor->motionFlg & ACTOR_FALLS) != 0) {
					ground = actor->groundHeight;
					if(y > ground) {
						actor->velY -= actor->gravity;
					} else if(actor->velY < 0) {
						y = ground;
						actor->velY = -syMulFX(actor->velY, actor->bounce);
						/* A rebound no faster than one frame of gravity ends
						   the bouncing. */
						if((actor->velY < 0 ? -actor->velY : actor->velY) <=
							actor->gravity)
							actor->velY = 0;
					}
				}

				/* Float: bob up and down on a triangle wave (ACTOR_FLOATS_HIGH
				   makes it bigger). */
				if((actor->motionFlg & ACTOR_FLOATS) != 0) {
					phase = actor->bounce & (FLOAT_CYCLE_FRAMES - 1);
					if((actor->motionFlg & ACTOR_FLOATS_HIGH) != 0)
						actor->velY = ActorScript_CycleTable[phase >> 1] *
							actor->gravity / FLOAT_HIGH_DIVISOR;
					else
						actor->velY = ActorScript_CycleTable[phase >> 1] *
							actor->gravity / FLOAT_DIVISOR;
					actor->bounce++;
				}
			}
		}

		x += actor->velX;
		y += actor->velY;
		z += actor->velZ;

		/* An actor with ACTOR_BLOCKED_BY_SOLID does not move into an
		   ACTOR_SOLID one; count the refusals. */
		if((actor->collisionFlg & ACTOR_BLOCKED_BY_SOLID) != 0) {
			SET_VECTOR(pos, x, y, z);
			if(fdActorScriptCheckOverlap(actor, pos) != 0) {
				actor->refused++;
				continue;
			}
			actor->refused = 0;
		}

		/* Arrived: the watched axis reached the target, or crossed it. */
		switch(actor->arrivalAxis) {
		case ARRIVAL_AXIS_X:
			if(x == actor->tgtX ||
				((actor->x.fixed - actor->tgtX) ^ (x - actor->tgtX)) < 0)
				arrivedAtTarget = 1;
			break;
		case ARRIVAL_AXIS_Y:
			if(y == actor->tgtY ||
				((actor->y.fixed - actor->tgtY) ^ (y - actor->tgtY)) < 0)
				arrivedAtTarget = 1;
			break;
		case ARRIVAL_AXIS_Z:
			if(z == actor->tgtZ ||
				((actor->z.fixed - actor->tgtZ) ^ (z - actor->tgtZ)) < 0)
				arrivedAtTarget = 1;
			break;
		default:
			break;
		}

		if(arrivedAtTarget != 0) {
			if(actor->snapToTgt != 0) {
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

		SET_ACTOR_POSITION(actor, x, y, z);

		/* Turn to face the direction of travel, at most FACING_STEP a frame. */
		if((actor->steeringFlg & ACTOR_FACES_MOTION) != 0) {
			x = actor->velX;
			z = actor->velZ;
			if(x != 0 || z != 0) {
				Sint32 maximumFacingStep;

				/* As a signed halfword the difference is the shorter way
				   round. */
				facingStep = (Sint16)(syAtan(z, x) - actor->facing);
				maximumFacingStep = FACING_STEP;
				if(facingStep > maximumFacingStep)
					facingStep = maximumFacingStep;
				maximumFacingStep = -FACING_STEP;
				if(facingStep < maximumFacingStep)
					facingStep = maximumFacingStep;
				actor->facing += facingStep;
			}
		}
	}
}

/* Places the actor at (x, y, z), in 16.16 pixels, clears its movement target
   and stops it. */
void fdActorSetPosAndResetMotion(ACTOR *actor, FIXED x, FIXED y, FIXED z)
{
	actor->z.fixed = z;
	actor->tgtX = ACTOR_NO_TARGET;
	actor->tgtY = ACTOR_NO_TARGET;
	actor->tgtZ = ACTOR_NO_TARGET;
	actor->x.fixed = x;
	actor->y.fixed = y;
	SET_VELOCITY(actor, 0, 0, 0);
}

/* Sends an actor towards (x, y, z), in 16.16 pixels. Within a pixel it arrives
   at once. */
void fdActorSetMoveTarget(ACTOR *actor, FIXED x, FIXED y, FIXED z)
{
	Sint32 dx;
	Sint32 distance;
	Sint32 dz;
	Sint32 squaredZ;

	dx = (x - actor->x.fixed) / toFIXED(1);
	distance = (y - actor->y.fixed) / toFIXED(1);
	dz = (z - actor->z.fixed) / toFIXED(1);
	squaredZ = dz * dz;
	distance = toFIXED(sySquartIWRAM(dx * dx + distance * distance + squaredZ));
	/* Under 16 whole pixels, measure again in 16.16 for sub-pixel accuracy. */
	if(distance < toFIXED(16)) {
		dx = x - actor->x.fixed;
		distance = y - actor->y.fixed;
		dz = z - actor->z.fixed;
		distance = sySquartFX(
			syMulFX(dx, dx) + syMulFX(distance, distance) + syMulFX(dz, dz));
	}
	if(distance < toFIXED(1)) {
		SET_ACTOR_POSITION(actor, x, y, z);
		actor->tgtX = ACTOR_NO_TARGET;
		actor->tgtY = ACTOR_NO_TARGET;
		actor->tgtZ = ACTOR_NO_TARGET;
		return;
	}
	if(actor->snapToTgt == 0) {
		Sint32 brakingDistance;

		/* speedLimit squared over acceleration, in 16.16 pixels: twice the
		   distance an actor at its limit coasts while it brakes to a stop, so
		   the shortened target leaves it room to coast onto the point asked
		   for. */
		brakingDistance = syDivFX(
			actor->acceleration, syMulFX(actor->speedLimit, actor->speedLimit));
		if(distance > brakingDistance)
			brakingDistance = distance - brakingDistance / 2;
		else
			brakingDistance = distance / 2;
		/* Scale the offset to the target by the shortened distance's share of
		   the whole. */
		distance = syDivFX(distance, brakingDistance);
		x = actor->x.fixed + syMulFX(x - actor->x.fixed, distance);
		y = actor->y.fixed + syMulFX(y - actor->y.fixed, distance);
		z = actor->z.fixed + syMulFX(z - actor->z.fixed, distance);
	}
	actor->tgtX = x;
	actor->tgtY = y;
	actor->tgtZ = z;
	/* Watch the axis with the furthest to go: a crossing on an axis with little
	   to go can come long before the target is reached. y only counts in free
	   flight (no motion flags); otherwise the ground sets the height. */
	dx = x - actor->x.fixed;
	distance = y - actor->y.fixed;
	dz = z - actor->z.fixed;
	actor->arrivalAxis = ARRIVAL_AXIS_X;
	if((dx < 0 ? -dx : dx) < (dz < 0 ? -dz : dz)) {
		actor->arrivalAxis = ARRIVAL_AXIS_Z;
		dx = dz;
	}
	if(actor->motionFlg == 0) {
		if(dx < 0)
			dx = -dx;
		if(dx < (distance < 0 ? -distance : distance))
			actor->arrivalAxis = ARRIVAL_AXIS_Y;
	}
}

/* Unused: copies fdActorUpdateAll (ARM code) to the heap, runs it once and
   frees it. */
UNUSED void fdActorRunUpdateAllFromHeap(void)
{
	void (*updateActors)(void);

	RAM_CODE_LOAD(updateActors, fdActorUpdateAll, fdActorUpdateAllCodeSize);
	updateActors();
	syHeapRelease(updateActors);
}

/* Advances the battle's actors by one frame, without running their scripts:
   steering towards a target or braking, gravity and bouncing, arrival, and
   turning to face the direction of travel. */
void fdActorUpdateAllMotion(void)
{
	ACTOR *actor;
	Sint32 actorsLeft;
	FIXED  y;
	Sint32 arrivedAtTarget;
	FIXED  x;
	FIXED  z;
	Sint32 dx;
	Sint32 dz;
	FIXED  velocityX;
	Sint32 distance;
	FIXED  ratio;
	Sint32 facingStep;

	/* The first UNIT_MAX actors, as a battle makes one for each unit
	   (btObjectMake). */
	actor = ActorsWork;
	for(actorsLeft = 0; actorsLeft < UNIT_MAX; actorsLeft++, actor++) {
		if(actor->script == 0)
			continue;
		x = actor->x.fixed;
		y = actor->y.fixed;
		z = actor->z.fixed;
		/* UB: a frozen actor otherwise reads an uninitialized arrival flag, or
		   inherits the preceding actor's flag. */
#if FIX_UNINITIALIZED_READ
		arrivedAtTarget = 0;
#endif
		if(actor->frozen == 0) {
#if !FIX_UNINITIALIZED_READ
			arrivedAtTarget = 0;
#endif
			/* Steer in the XZ plane as fdActorUpdateAllOnField does on the
			   ground, or brake; this loop has no free flight, ground following
			   or floating. */
			if(actor->tgtX != ACTOR_NO_TARGET) {
				dx = (actor->tgtX - x) / toFIXED(1);
				dz = (actor->tgtZ - z) / toFIXED(1);
				distance = toFIXED(sySquartIWRAM(dx * dx + dz * dz));
				/* Under 256 pixels, measure again in 16.16: the root of a 16.16
				   square is 8.8, and << 8 makes it 16.16 again. */
				if(distance <= toFIXED(256) - 1) {
					dx = actor->tgtX - x;
					dz = actor->tgtZ - z;
					distance = sySquartIWRAM(syMulFX(dx, dx) + syMulFX(dz, dz))
						<< 8;
				}
				if(distance == 0) {
					x = actor->tgtX;
					z = actor->tgtZ;
				} else {
					ratio = syDivFX(distance, actor->acceleration);
					velocityX = actor->velX + syMulFX(dx, ratio);
					actor->velX = velocityX;
					dx = actor->velZ + syMulFX(dz, ratio);
					actor->velZ = dx;
					distance = sySquartIWRAM(syMulFX(velocityX, velocityX) +
								   syMulFX(dx, dx))
						<< 8;
					if(distance > actor->speedLimit) {
						ratio = syDivFX(distance, actor->speedLimit);
						actor->velX = syMulFX(velocityX, ratio);
						actor->velZ = syMulFX(dx, ratio);
					}
				}
			} else {
				dx = actor->velX;
				dz = actor->velZ;
				if((dx | dz) != 0) {
					distance = sySquartIWRAM(syMulFX(dx, dx) + syMulFX(dz, dz))
						<< 8;
					if(distance != 0) {
						if(distance - actor->acceleration < 0) {
							actor->velX = 0;
							actor->velZ = 0;
						} else {
							ratio = syDivFX(
								distance, distance - actor->acceleration);
							actor->velX = syMulFX(dx, ratio);
							actor->velZ = syMulFX(dz, ratio);
						}
					} else {
						actor->velX = 0;
						actor->velZ = 0;
					}
				}
			}
			/* Gravity, and a bounce that loses energy on landing. */
			if(actor->motionFlg & ACTOR_FALLS) {
				if(y > actor->groundHeight) {
					actor->velY -= actor->gravity;
				} else if(actor->velY < 0) {
					y = actor->groundHeight;
					actor->velY = -syMulFX(actor->velY, actor->bounce);
					if((actor->velY < 0 ? -actor->velY : actor->velY) <=
						actor->gravity)
						actor->velY = 0;
				}
			}
		}
		x += actor->velX;
		y += actor->velY;
		z += actor->velZ;
		/* Arrived: the watched axis reached the target, or crossed it. */
		if(actor->arrivalAxis != ARRIVAL_AXIS_NONE)
			switch(actor->arrivalAxis) {
			case ARRIVAL_AXIS_X:
				if(x == actor->tgtX ||
					((actor->x.fixed - actor->tgtX) ^ (x - actor->tgtX)) < 0)
					arrivedAtTarget = 1;
				break;
			case ARRIVAL_AXIS_Y:
				if(y == actor->tgtY ||
					((actor->y.fixed - actor->tgtY) ^ (y - actor->tgtY)) < 0)
					arrivedAtTarget = 1;
				break;
			case ARRIVAL_AXIS_Z:
				if(z == actor->tgtZ ||
					((actor->z.fixed - actor->tgtZ) ^ (z - actor->tgtZ)) < 0)
					arrivedAtTarget = 1;
				break;
			}
		if(arrivedAtTarget) {
			if(actor->snapToTgt) {
				actor->velX = 0;
				actor->velZ = 0;
				x = actor->tgtX;
				z = actor->tgtZ;
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
		SET_ACTOR_POSITION(actor, x, y, z);
		/* Turn to face the direction of travel, at most FACING_STEP a frame,
		   the shorter way round. */
		if(actor->steeringFlg & ACTOR_FACES_MOTION) {
			x = actor->velX;
			z = actor->velZ;
			if(x != 0 || z != 0) {
				facingStep = (Sint16)(syAtan(z, x) - actor->facing);
				if(facingStep > FACING_STEP)
					facingStep = FACING_STEP;
				if(facingStep < -FACING_STEP)
					facingStep = -FACING_STEP;
				actor->facing += facingStep;
			}
		}
	}
}

#undef FLOAT_CYCLE_FRAMES
#undef FLOAT_DIVISOR
#undef FLOAT_HIGH_DIVISOR
