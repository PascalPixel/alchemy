// Script commands on actors: placing, moving and animating them. Part of field_mode.c.

#include "system/field_effect.h"

// Returns an actor, or NULL if it does not exist.
ACTOR *fdActorGetById(Uint32 actorId)
{
	ACTOR *actor = fdActorLookup(actorId);

	if (actor == NULL)
		return NULL;
	return actor;
}

// The script command "set actor speed": its top speed and acceleration, 16.16 pixels per tick.
// Writes the actor's speedLimit and acceleration.
void fdActorSetSpeedParams(Sint32 actorId, FIXED speedLimit, FIXED acceleration)
{
	ACTOR *actor = fdActorLookup(actorId);

	if (actor != NULL)
	{
		actor->acceleration = acceleration;
		actor->speedLimit   = speedLimit;
	}
}

// Gives an actor a movement type or actor script (fdActorSetMovementType) and makes it face where
// it moves (ACTOR_FACES_MOTION).
void fdActorSetMovementTypeById(Sint32 actorId, const void *script)
{
	ACTOR *actor = fdActorLookup(actorId);

	if (actor != NULL)
	{
		actor->steeringFlags |= ACTOR_FACES_MOTION;
		fdActorSetMovementType(actor, (Sint32)script);
	}
}

// The script command "stop actor": stops its movement type and puts it back under script control,
// on the idle script (a lone SC_END), and makes it face where it moves.
void fdActorStop(Sint32 actorId)
{
	ACTOR *actor = fdActorLookup(actorId);
	if (actor != NULL)
	{
		actor->steeringFlags |= ACTOR_FACES_MOTION;
		fdActorStartIdleStub(actor);
	}
}

// As fdActorStop, and also clears the actor's target and velocity, so it stands where it is.
void fdActorResetTargetsAndVelocity(Uint32 actorId)
{
	ACTOR *actor = fdActorLookup(actorId);

	if (actor != NULL)
	{
		actor->steeringFlags |= ACTOR_FACES_MOTION;
		fdActorStartIdleStub(actor);
		actor->targetX = ACTOR_NO_TARGET;
		actor->targetY = ACTOR_NO_TARGET;
		actor->targetZ = ACTOR_NO_TARGET;
		SET_VELOCITY(actor, 0, 0, 0);
		actor->arrivalAxis = 0;
	}
}

// Makes an actor follow another (the watched actor script): writes its watchedActor.
void fdActorWatchAndSetScript(Sint32 actorId, Sint32 watchedActorId)
{
	ACTOR *actor = fdActorLookup(actorId);

	if (actor != NULL)
	{
		actor->watchedActor = (FIELD_OBJECT *)fdActorGetById(watchedActorId);
		fdActorSetMovementType(actor, (Sint32)ActorFaceWatchedScript);
	}
}

// Waits, at most 300 ticks, for an actor's script to reach its end (SC_END).
void fdActorWaitForScriptEndById(Uint32 actorId)
{
	ACTOR *actor = fdActorLookup(actorId);

	if (actor != NULL)
		fdActorWaitForScriptEndStub(actor);
}

// Starts a movement type or supplied actor script, as fdActorSetMovementTypeById does, then waits
// up to 300 ticks for its script to end.
void fdActorSetMovementTypeAndWaitById(Sint32 actorId, const void *script)
{
	ACTOR *actor = fdActorLookup(actorId);

	if (actor != NULL)
	{
		actor->steeringFlags |= ACTOR_FACES_MOTION;
		fdActorSetMovementType(actor, (Sint32)script);
		fdActorWaitForScriptEndStub(actor);
	}
}

// The script command "set actor destination": the actor heads for (x, z), in pixels, keeping its
// current animation.
void fdActorSetDestinationById(Uint32 actorId, Sint32 x, Sint32 z)
{
	ACTOR *actor = fdActorLookup(actorId);

	if (actor != NULL)
	{
		actor->paused = 0;
		fdActorStartIdleStub(actor);
		fdActorSetMoveTargetStub(actor, toFIXED(x), actor->y.fixed, toFIXED(z));
	}
}

// The script command "move actor and wait": as fdActorSetDestinationById, then waits (at most 600
// ticks) for the actor to arrive.
void fdActorMoveToAndWaitById(Uint32 actorId, Sint32 x, Sint32 z)
{
	ACTOR *actor = fdActorLookup(actorId);

	if (actor != NULL)
	{
		actor->paused = 0;
		fdActorStartIdleStub(actor);
		fdActorSetMoveTargetStub(actor, toFIXED(x), actor->y.fixed, toFIXED(z));
		fdActorWaitForMoveStub(actor);
	}
}

// The script command "walk actor to": the actor walks toward (x, z), in pixels.
void fdActorWalkToById(Sint32 actorId, Sint32 x, Sint32 z)
{
	ACTOR *actor = fdActorLookup(actorId);

	if (actor != NULL)
	{
		actor->paused = 0;
		fdActorStartIdleStub(actor);
		fdActorSelectAnimationStub(actor, ANIM_WALK);
		fdActorSetMoveTargetStub(actor, toFIXED(x), actor->y.fixed, toFIXED(z));
	}
}

// The script command "walk actor to and wait": walks the actor to (x, z), in pixels, waits for it
// to arrive and stands it still.
void fdActorWalkToAndWaitById(Sint32 actorId, Sint32 x, Sint32 z)
{
	ACTOR *actor = fdActorLookup(actorId);

	if (actor != NULL)
	{
		actor->paused = 0;
		fdActorStartIdleStub(actor);
		fdActorSelectAnimationStub(actor, ANIM_WALK);
		fdActorSetMoveTargetStub(actor, toFIXED(x), actor->y.fixed, toFIXED(z));
		fdActorWaitForMoveStub(actor);
		fdActorSelectAnimationStub(actor, ANIM_STAND);
	}
}

// The script command "center actor and walk": walks the actor sideways to the middle of its
// 16-pixel column, sets its sprite priority to priority (fdActorSetSpritePriority), then sends it
// zOffset pixels along z.
void fdActorCenterOnColumn(Uint32 actorId, Sint32 priority, Sint32 zOffset)
{
	ACTOR *actor;
	Sint16 pixelX;
	Sint32 columnX;
	Sint16 offsetInColumn;

	actor = fdActorLookup(actorId);
	if (actor != NULL)
	{
		// The column's left edge, from a division by 16 that rounds toward zero (left of x = 0 it
		// finds the edge to the right); its middle is 8 pixels in.
		pixelX         = actor->x.part[0].integer;
		columnX        = pixelX / 16 * 16;
		offsetInColumn = pixelX - columnX;
		actor->paused  = 0;
		fdActorStartIdleStub(actor);
		fdActorSelectAnimationStub(actor, ANIM_WALK);
		fdActorSetMoveTargetStub(
			actor, actor->x.fixed + toFIXED(8 - offsetInColumn), actor->y.fixed, actor->z.fixed);
		fdActorWaitForMoveStub(actor);
		fdActorSetSpritePriority(actorId, priority);
		fdActorSetMoveTargetStub(
			actor, actor->x.fixed, actor->y.fixed, actor->z.fixed + toFIXED(zOffset));
	}
}

// The script command "set actor destination offset": the actor heads (xOffset, zOffset) pixels from
// where it stands.
void fdActorSetDestinationOffsetById(Sint32 actorId, Sint32 xOffset, Sint32 zOffset)
{
	ACTOR *actor = fdActorLookup(actorId);

	if (actor != NULL)
	{
		actor->paused = 0;
		fdActorStartIdleStub(actor);
		fdActorSetMoveTargetStub(actor,
		                         actor->x.fixed + toFIXED(xOffset),
		                         actor->y.fixed,
		                         actor->z.fixed + toFIXED(zOffset));
	}
}

// Walks the actor (xOffset, zOffset) pixels from where it stands, through the offset script.
void fdActorWalkByById(Uint32 actorId, Sint32 xOffset, Sint32 zOffset)
{
	ACTOR *actor = fdActorLookup(actorId);

	if (actor != NULL)
	{
		actor->paused = 0;
		fdActorStartScriptStub(actor, (Uint32)ActorOffsetScript);
		fdActorSelectAnimationStub(actor, ANIM_WALK);
		fdActorSetMoveTargetStub(actor,
		                         actor->x.fixed + toFIXED(xOffset),
		                         actor->y.fixed,
		                         actor->z.fixed + toFIXED(zOffset));
	}
}

// Walks the actor (xOffset, zOffset) pixels, waits for it to arrive and stands it still.
void fdActorWalkByAndWaitById(Uint32 actorId, Sint32 xOffset, Sint32 zOffset)
{
	ACTOR *actor = fdActorLookup(actorId);

	fdActorWalkByById(actorId, xOffset, zOffset);
	if (actor != NULL)
	{
		fdActorWaitForMoveStub(actor);
		fdActorSelectAnimationStub(actor, ANIM_STAND);
	}
}

// Places an actor on the leader, a pixel nearer the back, and sends it walking (xOffset, zOffset)
// pixels away at speed 0.8 and acceleration 0.4, through the launch script; the actor then takes
// movement type action.
void fdActorLaunchFromLeader(Sint32 actorId, Sint32 xOffset, Sint32 zOffset, Sint32 action)
{
	ACTOR *actor = fdActorLookup(actorId);

	if (actor != NULL)
	{
		fdActorSetSpeedParams(actorId, toFIXED(4) / 5, toFIXED(2) / 5);
		fdActorPlaceOnActor(actorId, MainRoutineStateWork.leaderActorId);
		fdActorSetPosAndResetMotionStub(
			actor, actor->x.fixed, actor->y.fixed, actor->z.fixed - toFIXED(1));
		actor->paused = 0;
		fdActorSelectAnimationStub(actor, ANIM_WALK);
		fdActorSetDestinationOffsetById(actorId, xOffset, zOffset);
		fdActorStartScriptStub(actor, ActorLaunchScript);
		actor->timer = action;
	}
}

// The script command "wait for actor": waits (at most 600 ticks) for the actor to arrive and stands
// it still.
void fdActorWaitForMoveById(Sint32 actorId)
{
	ACTOR *actor = fdActorLookup(actorId);

	if (actor != NULL)
	{
		fdActorWaitForMoveStub(actor);
		fdActorSelectAnimationStub(actor, ANIM_STAND);
	}
}

// The script command "set actor position": puts the actor at (x, z), 16.16 pixels, at rest; an
// actor that follows the ground keeps its height above it.
void fdActorSetHorizontalPosWithTerrain(Uint32 actorId, FIXED x, FIXED z)
{
	ACTOR *actor = fdActorLookup(actorId);
	Sint32 terrainHeight;

	if (actor != NULL)
	{
		fdActorStartIdleStub(actor);
		fdActorSetPosAndResetMotionStub(actor, x, actor->y.fixed, z);
		// Swap the old ground's height for the new one, so the actor's height above the ground is
		// kept.
		if (actor->motionFlags & ACTOR_FOLLOWS_TERRAIN)
		{
			terrainHeight =
				fdMapGridGetTerrainHeightStub(actor->layer, actor->x.fixed, actor->z.fixed);
			actor->y.fixed      = actor->y.fixed - actor->groundHeight + terrainHeight;
			actor->groundHeight = terrainHeight;
		}
	}
}

// clang-format off
// As fdActorSetHorizontalPosWithTerrain, and turns the actor to angle. Written in the old style: the angle arrives as a word and is taken as a halfword.
void fdActorSetHorizontalPosAndAngleWithTerrain(actorId, x, z, angle)
  Sint32 actorId;
  Sint32 x;
  Sint32 z;
  Sint16 angle;
{
	// clang-format on
	ACTOR *actor = fdActorLookup(actorId);
	Sint32 terrainHeight;

	if (actor != NULL)
	{
		fdActorStartIdleStub(actor);
		fdActorSetPosAndResetMotionStub(actor, x, actor->y.fixed, z);
		actor->facing = angle;
		if (actor->motionFlags & ACTOR_FOLLOWS_TERRAIN)
		{
			terrainHeight =
				fdMapGridGetTerrainHeightStub(actor->layer, actor->x.fixed, actor->z.fixed);
			actor->y.fixed      = actor->y.fixed - actor->groundHeight + terrainHeight;
			actor->groundHeight = terrainHeight;
		}
	}
}

// Puts the actor at (x, y, z), 16.16 pixels, at rest; an actor that follows the ground is raised by
// the ground height there, as fdActorSetHorizontalPosWithTerrain raises it.
void fdActorSetPosWithTerrain(Uint32 actorId, FIXED x, FIXED y, FIXED z)
{
	ACTOR *actor = fdActorLookup(actorId);
	Sint32 terrainHeight;

	if (actor != NULL)
	{
		fdActorStartIdleStub(actor);
		fdActorSetPosAndResetMotionStub(actor, x, y, z);
		if (actor->motionFlags & ACTOR_FOLLOWS_TERRAIN)
		{
			terrainHeight =
				fdMapGridGetTerrainHeightStub(actor->layer, actor->x.fixed, actor->z.fixed);
			actor->y.fixed      = actor->y.fixed - actor->groundHeight + terrainHeight;
			actor->groundHeight = terrainHeight;
		}
	}
}

// Puts the first actor where the second stands, at rest, on the same ground height.
void fdActorPlaceOnActor(Uint32 actorId, Uint32 otherId)
{
	ACTOR *actor = fdActorLookup(actorId);
	ACTOR *other = fdActorLookup(otherId);

	if (actor != NULL && other != NULL)
	{
		fdActorSetPosAndResetMotionStub(actor, other->x.fixed, other->y.fixed, other->z.fixed);
		actor->groundHeight = other->groundHeight;
	}
}

// The script command "set actor animation" (an ACTOR_ANIMATION: ANIM_STAND, ANIM_WALK, ANIM_NOD,
// ANIM_SHAKE_HEAD and so on).
void fdActorSetAnimationById(Uint32 actorId, Sint32 animation)
{
	ACTOR *actor = fdActorLookup(actorId);

	if (actor != NULL)
		fdActorSelectAnimationStub(actor, animation);
}

// Sets the animation timer steps of each of an actor's sprites.
void fdActorSetAnimationSpeedById(Sint32 actorId, Sint32 steps)
{
	ACTOR *actor = fdActorLookup(actorId);

	if (actor != NULL)
		fdActorSetAnimationSpeedStub(actor, steps);
}

// Waits, at most 90 ticks, for an actor's animation to change or for its first sprite animation's
// script to reach its hold command (ANIMCMD_HOLD). Only an actor drawn as a sprite
// (ACTOR_SPRITE_ONE) is waited for.
void fdActorWaitForAnimationChange(Uint32 actorId)
{
	ACTOR          *actor = fdActorLookup(actorId);
	SPR            *sprite;
	// DEAD: saved lives in a word on the stack, which nothing here needs; the ROM keeps its str and
	// the ldr of each compare.
	volatile Uint32 saved;
	Sint32          elapsedTicks;

	if (actor != NULL && actor->spriteStorageKind == ACTOR_SPRITE_ONE)
	{
		sprite = actor->sprite;
		saved  = sprite->lastNo;
		for (elapsedTicks = 0; elapsedTicks < 90; elapsedTicks++)
		{
			sySynch(1);
			if (saved != sprite->lastNo)
				break;
			if (sprite->entries[0] != NULL && sprite->entries[0]->script != NULL
			    && sprite->entries[0]->script[sprite->entries[0]->scriptOffset] == ANIMCMD_HOLD)
				break;
		}
	}
}

// The script command "set actor animation and wait": plays the animation and waits for it to end.
void fdActorSetAnimationAndWait(Sint32 actorId, Sint32 animation)
{
	fdActorSetAnimationById(actorId, animation);
	fdActorWaitForAnimationChange(actorId);
}

// The script command "actor jump": throws the actor upward at speed pixels per tick with a jump
// sound (a higher one above 5), then waits ticks ticks.
void fdActorJump(Sint32 actorId, Sint32 speed, Sint32 ticks)
{
	ACTOR *actor;

	actor = fdActorLookup(actorId);
	if (actor != NULL)
	{
		actor->motionFlags |= ACTOR_FALLS;
		actor->velocityY    = toFIXED(speed);
		if (speed > 5)
		{
			sySoundPlayStub(SE_HIGH_JUMP);
		}
		else
		{
			sySoundPlayStub(SE_JUMP);
		}
		fdScriptWait(ticks);
	}
}

// The most squash-and-spring motions ActorVariantScripts holds: it plays the motion that many times
// over, REPEATED_MOTION_SCRIPT_BYTES (32 words) each.
#define REPEATED_MOTION_MAX          3
#define REPEATED_MOTION_SCRIPT_BYTES (32 * sizeof(Sint32))

// The script command "start repeated motion": the actor squashes and springs back count times (1 to
// REPEATED_MOTION_MAX), through ActorVariantScripts.
void fdActorStartRepeatedMotion(Uint32 actorId, Sint32 count)
{
	ACTOR *actor;

	actor = fdActorLookup(actorId);
	if (actor != NULL && count > 0)
	{
		if (count > REPEATED_MOTION_MAX)
		{
			count = REPEATED_MOTION_MAX;
		}
		// Start count motions from the script's end.
		fdActorStartScriptStub(
			actor,
			(const Sint32 *)(ActorVariantScripts
		                     + (REPEATED_MOTION_MAX - count) * REPEATED_MOTION_SCRIPT_BYTES));
	}
}

// The script command "run repeated motion": as fdActorStartRepeatedMotion, then waits for it to
// end.
void fdActorRunRepeatedMotion(Uint32 actorId, Sint32 count)
{
	fdActorStartRepeatedMotion(actorId, count);
	fdActorWaitForScriptEndById(actorId);
}

// Moves a dust puff of fdEffectSpawnDustPuff: it drifts with its x and z velocity (kept in speed
// and acceleration), which drag slows each tick, and rises 1/64 pixel a tick.
void fdEffectUpdateDustPuff(ACTOR *particle)
{
	FIXED velocityX         = particle->speedLimit;
	FIXED velocityZ         = particle->acceleration;

	particle->x.fixed      += velocityX;
	particle->z.fixed      += velocityZ;
	particle->y.fixed      += toFIXED(1) / 64;
	particle->speedLimit    = velocityX - velocityX / FIELD_EFFECT_DRAG_X;
	particle->acceleration  = velocityZ - velocityZ / FIELD_EFFECT_DRAG_Z;
}

// Kicks up a dust puff (SPRITE_DUST_CLOUD) where source stands: it flies off toward the back at 0.5
// to 1.4 pixels a tick and sideways at up to 1.4, with one of two animations, drawn at source's
// priority, in colorMode unless 0.
void fdEffectSpawnDustPuff(ACTOR *src, Sint32 colorMode)
{
	ACTOR *actor;
	SPR   *child;
	Sint32 speed;
	FIXED  backStep = -(toFIXED(1) / 10); // 0.1 pixel a tick toward the back

	actor = fdActorCreateStub(SPRITE_DUST_CLOUD, src->x.fixed, src->y.fixed, src->z.fixed);
	if (actor != NULL)
	{
		child = actor->sprite;
		switch (syRandom() & 1)
		{
			case 1:
				fdActorSelectAnimationStub(actor, 2);
				fdActorStartScriptStub(actor, DustPuffLongScript);
				break;
			default:
				fdActorSelectAnimationStub(actor, 1);
				fdActorStartScriptStub(actor, DustPuffShortScript);
				break;
		}

		if (colorMode != 0)
			fdActorSetSpriteColor(actor, colorMode);

		// speed and acceleration hold the puff's x and z velocity, in steps of toFIXED(1) / 10 (0.1
		// pixel): z is -0.5 to -1.4, x -1.4 to 1.4 in steps of 0.2.
		actor->motionFlags      = 0;
		speed                   = syRandom() % 10 + 5;
		actor->acceleration     = backStep * speed;
		speed                   = syRandom() % 15 - 7;
		speed                 <<= 1;
		actor->speedLimit       = toFIXED(1) / 10 * speed;
		actor->timer            = 0;
		actor->priorityFlags   |= ACTOR_PRIORITY_UNDERFOOT;
		actor->frozen           = 1;
		actor->update           = (OBJECT_UPDATE)fdEffectUpdateDustPuff;
		child->flags            = 0;
		child->image.priority   = ((SPR *)src->sprite)->image.priority;
	}
}

// Makes an actor jump down off a ledge in front of it: it shows pose for 10 ticks, hops 12 pixels
// south from the middle of its 16-pixel column, turns toward FACING_SOUTH_SOUTHWEST while sinking
// 28 pixels with a dust puff every other tick in the colour mode dust (none when dust is -1), then
// jumps on to 16 pixels south of where it started and lands, waiting at most 180 ticks for it
// unless dust has LEDGE_JUMP_NO_LANDING_WAIT.
void fdActorJumpDownLedge(Sint32 actorId, Sint32 pose, Sint32 dust)
{
	ACTOR *actor  = fdActorGetById(actorId);
	FIXED  startZ = actor->z.fixed;
	FIXED  x      = (actor->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
	Sint32 spriteFlags;
	Uint32 tick;

	if (actor == NULL)
		return;
	spriteFlags = ((SPR *)actor->sprite)->flags;
	fdActorSelectAnimationStub(actor, pose);
	sySynch(10);

	// Hop up and forward 12 pixels.
	fdActorSelectAnimationStub(actor, ANIM_STAND);
	fdActorSetSpriteFlagsStub(actor, 0);
	actor->motionFlags |= ACTOR_FALLS;
	actor->velocityY    = toFIXED(4);
	fdActorSetMoveTargetStub(actor, x, actor->y.fixed, startZ + toFIXED(12));
	sySynch(6);

	// Turn and sink 2 pixels a tick for 14 ticks, raising dust, in free flight: no motion flags, so
	// it neither falls nor follows the ground.
	sySoundPlayStub(SE_FLOOR_SWITCH);
	fdActorFaceDirectionById(actorId, FACING_SOUTH_SOUTHWEST, 0);
	actor->motionFlags = 0;
	for (tick = 0; tick < 14; tick++)
	{
		actor->y.fixed -= toFIXED(2);
		actor->targetY  = actor->y.fixed;
		sySynch(1);
		if (dust != -1 && (tick & 1))
			fdEffectSpawnDustPuff(actor, dust);
	}

	// Jump 16 pixels forward, following the ground and falling again.
	actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	actor->velocityY   = toFIXED(3);
	fdActorSetMoveTargetStub(actor, x, actor->y.fixed, startZ + toFIXED(16));
	fdActorWaitForMoveStub(actor);
	if (dust & LEDGE_JUMP_NO_LANDING_WAIT)
	{
		sySynch(4);
		return;
	}
	tick = 0;
	if (actor->y.fixed > actor->groundHeight)
	{
		do
		{
			sySynch(1);
			if (++tick >= 180)
				break;
		} while (actor->y.fixed > actor->groundHeight);
	}
	fdActorSetSpriteFlagsStub(actor, spriteFlags);
	sySynch(2);
	fdScriptNoOperation();
}

// The script command "actor face actor": turns the first actor at once to face the second, then
// waits the given number of ticks.
void fdActorFaceActorById(Sint32 firstId, Sint32 secondId, Sint32 ticks)
{
	ACTOR *first;
	ACTOR *second;

	first  = fdActorLookup(firstId);
	second = fdActorLookup(secondId);
	if (first != 0 && second != 0)
	{
		first->facing = syAtan(second->z.fixed - first->z.fixed, second->x.fixed - first->x.fixed);
		fdScriptWait(ticks);
	}
}

// The script command "actors face each other": turns two actors toward each other, then waits the
// given number of ticks.
void fdActorTurnToFaceEachOther(Sint32 firstId, Sint32 secondId, Sint32 ticks)
{
	void *first  = fdActorLookup(firstId);
	void *second = fdActorLookup(secondId);

	if (first != NULL && second != NULL)
	{
		fdActorTurnPairToFaceEachOther(first, second);
		fdScriptWait(ticks);
	}
}

// Turns two actors to face each other, at most 1/16 turn a tick each, until both do (at most 60
// ticks).
void fdActorTurnPairToFaceEachOther(ACTOR *first, ACTOR *second)
{
	Sint32 firstTargetAngle;
	Sint32 secondTargetAngle;
	Sint32 frame;
	Sint32 actorsTurning;
	Sint32 angleDifference;

	if (first == NULL || second == NULL)
		return;
	firstTargetAngle =
		(Uint16)syAtan(second->z.fixed - first->z.fixed, second->x.fixed - first->x.fixed);
	// The second actor faces the opposite way, half a turn round.
	secondTargetAngle = firstTargetAngle + DEGtoANG(180);
	for (frame = 0; frame < 60; frame++)
	{
		// Each actor that already faces its way takes one off the count; both facing ends the turn
		// early.
		actorsTurning   = 2;
		angleDifference = (Sint16)(firstTargetAngle - first->facing);
		if (angleDifference != 0)
		{
			if (angleDifference > FACING_STEP)
				angleDifference = FACING_STEP;
			if (angleDifference < -FACING_STEP)
				angleDifference = -FACING_STEP;
			first->facing += angleDifference;
		}
		else
		{
			actorsTurning = 1;
		}
		angleDifference = (Sint16)(secondTargetAngle - second->facing);
		if (angleDifference != 0)
		{
			if (angleDifference > FACING_STEP)
				angleDifference = FACING_STEP;
			if (angleDifference < -FACING_STEP)
				angleDifference = -FACING_STEP;
			second->facing += angleDifference;
		}
		else
		{
			actorsTurning--;
		}
		if (actorsTurning == 0)
			break;
		sySynch(1);
	}
}

// The script command "release actor": releases the actor and empties its slot in the field state's
// actor table.
void fdActorReleaseById(Sint32 actorId)
{
	void        *released = fdActorLookup(actorId);
	FIELD_STATE *state    = RamHeapSlots->fieldState;

	if (released != NULL)
	{
		fdActorReleaseStub(released);
		state->actors[actorId] = NULL;
	}
}

// The script command "set actor sprite colour": gives every sprite animation of the actor the
// colour mode (fdActorSetSpriteColor); with SPRITE_COLOR_BLINK the actor blinks instead
// (fdActorUpdateBlink).
void fdActorConfigureSpriteColor(Sint32 actorId, Sint32 colorMode)
{
	ACTOR *actor;

	actor = fdActorLookup(actorId);
	if (actor != NULL)
	{
		if (colorMode & SPRITE_COLOR_BLINK)
		{
			actor->update = fdActorUpdateBlink;
			return;
		}
		actor->update = NULL;
		fdActorSetSpriteColor(actor, colorMode);
	}
}

// Per-tick update function of a blinking actor: sets its sprite animations' colour mode from
// ActorBlinkSpriteColors, each entry lasting two ticks.
void fdActorUpdateBlink(ACTOR *actor)
{
	SPR           *sprite;
	SPR_ANIMATION *animation;
	Sint32         colorMode;
	Sint32         i;

	if ((actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE)
	{
		sprite    = actor->sprite;
		colorMode = ActorBlinkSpriteColors[(TickCount >> 1) & 3];
		for (i = 0; i < sprite->count; i++)
		{
			animation = sprite->entries[i];
			if (animation != 0 && animation->script != 0)
			{
				animation->colorMode = colorMode;
			}
		}
		sprite->dirty = 1;
	}
}

// Sets the colour mode (SPR_COL_MODE) of every running sprite animation of an actor's sprite.
// Scripts set SPRITE_COLOR_NORMAL or SPRITE_COLOR_HIDDEN, and alternate the two to make an actor
// flash.
void fdActorSetSpriteColor(ACTOR *actor, Sint32 colorMode)
{
	SPR           *sprite;
	SPR_ANIMATION *animation;
	Sint32         i;

	if ((actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE)
	{
		sprite = actor->sprite;
		for (i = 0; i < sprite->count; i++)
		{
			animation = sprite->entries[i];
			if (animation != 0 && animation->script != 0)
			{
				animation->colorMode = colorMode;
			}
		}
		sprite->dirty = 1;
	}
}

// fdActorWatchAndStartScript's targetId: the actor to watch, and a flag to keep the watcher's own
// motion.
typedef enum ACTOR_WATCH_FLAG
{
	ACTOR_WATCH_KEEP_MOTION = 0x10000
} ACTOR_WATCH_FLAG;

#define ACTOR_WATCH_ACTOR_MASK 0xff // the actor's bits
// The distance, in pixels, a follower stays behind the actor it watches (fdActorMoveTowardTarget
// reads it from the follower's timer).
#define FOLLOW_DISTANCE        40

// Makes an actor follow the actor in targetId's ACTOR_WATCH_ACTOR_MASK bits and starts script on
// it. Unless targetId has ACTOR_WATCH_KEEP_MOTION it keeps FOLLOW_DISTANCE, takes the target's top
// speed with twice its acceleration, to keep up with it, and no collision flags, so it cannot block
// the target.
void fdActorWatchAndStartScript(Uint32 actorId, Sint32 targetId, const void *script)
{
	ACTOR *follower = fdActorLookup(actorId);
	ACTOR *target   = fdActorLookup(targetId & ACTOR_WATCH_ACTOR_MASK);

	if (follower != NULL && target != NULL)
	{
		follower->watchedActor = (FIELD_OBJECT *)target;
		if (!(targetId & ACTOR_WATCH_KEEP_MOTION))
		{
			follower->timer          = FOLLOW_DISTANCE;
			follower->acceleration   = target->acceleration * 2;
			follower->speedLimit     = target->speedLimit;
			follower->collisionFlags = 0;
		}
		fdActorStartScriptStub(follower, script);
	}
}
