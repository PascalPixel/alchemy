/* Script commands on actors: placing, moving and animating them. Part of
   script_commands.c. */

#include "script_commands_internal.h"

/* Returns an actor, or NULL if it does not exist. */
ACTOR *fdActorGetById(Sint32 actorId)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor == NULL)
		return NULL;
	return actor;
}

/* Sets an actor's top speed and acceleration, in 16.16 pixels per tick. */
void fdActorSetSpeedParms(Uint32 actorId, FIXED speedLimit, FIXED acceleration)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		actor->acceleration = acceleration;
		actor->speedLimit = speedLimit;
	}
}

/* Gives an actor a movement type or actor script (fdActorSetMovementType) and
   makes it face where it moves (ACTOR_FACES_MOTION). */
void fdActorSetMovementTypeById(Uint32 actorId, Sint32 movement)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		actor->steeringFlg |= ACTOR_FACES_MOTION;
		fdActorSetMovementType(actor, movement);
	}
}

/* Stops an actor's movement type and puts it back under script control, on the
   idle script (a lone SC_END), and sets ACTOR_FACES_MOTION. */
void fdActorStop(Uint32 actorId)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		actor->steeringFlg |= ACTOR_FACES_MOTION;
		fdActorStartIdleStub(actor);
	}
}

/* Makes an actor follow another (the watched actor script): writes its
   watchedActor. */
void fdActorWatchAndSetScript(Uint32 actorId, Uint32 watchedActorId)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		actor->watchedActor =
			(union ACTOR_OR_EFFECT *)fdActorGetById(watchedActorId);
		fdActorSetMovementType(actor, (Sint32)ActorFaceWatchedScript);
	}
}

/* Waits, at most 300 ticks, for an actor's script to reach its end
   (SC_END). */
void fdActorWaitForScriptEndById(Uint32 actorId)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL)
		fdActorWaitForScriptEndStub(actor);
}

/* Starts a movement type or supplied actor script, as
   fdActorSetMovementTypeById does, then waits up to 300 ticks for its script
   to end. */
void fdActorSetMovementTypeAndWaitById(Uint32 actorId, Sint32 movement)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		actor->steeringFlg |= ACTOR_FACES_MOTION;
		fdActorSetMovementType(actor, movement);
		fdActorWaitForScriptEndStub(actor);
	}
}

/* Sends an actor toward (x, z), in pixels, keeping its current animation. */
void fdActorSetDestinationById(Uint32 actorId, Sint32 x, Sint32 z)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		actor->paused = 0;
		fdActorStartIdleStub(actor);
		fdActorSetMoveTargetStub(actor, toFIXED(x), actor->y.fixed, toFIXED(z));
	}
}

/* As fdActorSetDestinationById, then waits (at most 600 ticks) for the actor
   to arrive. */
void fdActorMoveToAndWaitById(Uint32 actorId, Sint32 x, Sint32 z)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		actor->paused = 0;
		fdActorStartIdleStub(actor);
		fdActorSetMoveTargetStub(actor, toFIXED(x), actor->y.fixed, toFIXED(z));
		fdActorWaitForMoveStub(actor);
	}
}

/* Walks an actor toward (x, z), in pixels. */
void fdActorWalkToById(Uint32 actorId, Sint32 x, Sint32 z)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		actor->paused = 0;
		fdActorStartIdleStub(actor);
		fdActorSelectAnimationStub(actor, ANIM_WALK);
		fdActorSetMoveTargetStub(actor, toFIXED(x), actor->y.fixed, toFIXED(z));
	}
}

/* Walks the actor to (x, z), in pixels, waits for it to arrive and stands it
   still. */
void fdActorWalkToAndWaitById(Uint32 actorId, Sint32 x, Sint32 z)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		actor->paused = 0;
		fdActorStartIdleStub(actor);
		fdActorSelectAnimationStub(actor, ANIM_WALK);
		fdActorSetMoveTargetStub(actor, toFIXED(x), actor->y.fixed, toFIXED(z));
		fdActorWaitForMoveStub(actor);
		fdActorSelectAnimationStub(actor, ANIM_STAND);
	}
}

/* Walks the actor sideways to the middle of its 16-pixel column, sets its
   sprite priority to priority (fdActorSetSprPriority), then sends it zOfs
   pixels along z. */
void fdActorCenterOnColumn(Uint32 actorId, Sint32 pri, Sint32 zOfs)
{
	ACTOR *actor;
	Sint16 pixelX;
	Sint32 columnX;
	Sint16 ofsInColumn;

	actor = fdActorLookup(actorId);
	if(actor != NULL) {
		/* The column's left edge, from a division by 16 that rounds toward zero
		   (left of x = 0 it finds the edge to the right); its middle is 8
		   pixels in. */
		pixelX = actor->x.part[0].integer;
		columnX = pixelX / 16 * 16;
		ofsInColumn = pixelX - columnX;
		actor->paused = 0;
		fdActorStartIdleStub(actor);
		fdActorSelectAnimationStub(actor, ANIM_WALK);
		fdActorSetMoveTargetStub(actor,
			actor->x.fixed + toFIXED(8 - ofsInColumn), actor->y.fixed,
			actor->z.fixed);
		fdActorWaitForMoveStub(actor);
		fdActorSetSprPriority(actorId, pri);
		fdActorSetMoveTargetStub(actor, actor->x.fixed, actor->y.fixed,
			actor->z.fixed + toFIXED(zOfs));
	}
}

/* Sends an actor (xOfs, zOfs) pixels from where it stands, keeping its
   current animation. */
void fdActorSetDestinationOffsetById(Uint32 actorId, Sint32 xOfs, Sint32 zOfs)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		actor->paused = 0;
		fdActorStartIdleStub(actor);
		fdActorSetMoveTargetStub(actor, actor->x.fixed + toFIXED(xOfs),
			actor->y.fixed, actor->z.fixed + toFIXED(zOfs));
	}
}

/* Walks the actor (xOfs, zOfs) pixels from where it stands. Scripts reach
   it (fdActorWalkBy); fdActorWalkByAndWaitById calls it too. */
void fdActorWalkByById(Uint32 actorId, Sint32 xOfs, Sint32 zOfs)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		actor->paused = 0;
		fdActorStartIdleStub(actor);
		fdActorSelectAnimationStub(actor, ANIM_WALK);
		fdActorSetMoveTargetStub(actor, actor->x.fixed + toFIXED(xOfs),
			actor->y.fixed, actor->z.fixed + toFIXED(zOfs));
	}
}

/* Walks the actor (xOfs, zOfs) pixels, waits for it to arrive and stands
   it still. */
void fdActorWalkByAndWaitById(Uint32 actorId, Sint32 xOfs, Sint32 zOfs)
{
	ACTOR *actor = fdActorLookup(actorId);

	fdActorWalkByById(actorId, xOfs, zOfs);
	if(actor != NULL) {
		fdActorWaitForMoveStub(actor);
		fdActorSelectAnimationStub(actor, ANIM_STAND);
	}
}

/* Places an actor on the leader and sends it walking (xOfs, zOfs) pixels
   away at speed 0.6 and acceleration 0.3; once there, the launch script turns
   it to face facing. */
void fdActorLaunchFromLeader(
	Uint32 actorId, Sint32 xOfs, Sint32 zOfs, Sint32 facing)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		ACTOR *leader;

		fdActorSetSpeedParms(actorId, toFIXED(3) / 5, toFIXED(3) / 10);
		leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
		if(leader != NULL)
			fdActorSetHorizontalPosWithTerrain(
				actorId, leader->x.fixed, leader->z.fixed);
		actor->paused = 0;
		fdActorSelectAnimationStub(actor, ANIM_WALK);
		fdActorSetDestinationOffsetById(actorId, xOfs, zOfs);
		fdActorStartScriptStub(actor, (const Sint32 *)&ActorLaunchScript);
		actor->tmr = facing;
	}
}

/* Waits (at most 600 ticks) for the actor to arrive and stands it still. */
void fdActorWaitForMoveById(Uint32 actorId)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		fdActorWaitForMoveStub(actor);
		fdActorSelectAnimationStub(actor, ANIM_STAND);
	}
}

/* Puts the actor at (x, z), 16.16 pixels, at rest. */
void fdActorSetHorizontalPosWithTerrain(Uint32 actorId, FIXED x, FIXED z)
{
	ACTOR *actor;
	FIXED  groundHeight;
	Sint32 tileX;
	Sint32 tileZ;
	Sint32 terrainId;

	actor = fdActorLookup(actorId);
	if(actor != NULL) {
		fdActorStartIdleStub(actor);
		SET_VELOCITY(actor, 0, 0, 0);
		actor->tgtY = ACTOR_NO_TARGET;
		actor->tgtX = ACTOR_NO_TARGET;
		actor->x.fixed = x;
		actor->z.fixed = z;
		/* Swap the old ground's height for the new one, so the actor's height
		   above the ground is kept. */
		if(actor->motionFlg & ACTOR_FOLLOWS_TERRAIN) {
			terrainId = actor->layer;
			tileX = x / toFIXED(1);
			tileZ = z / toFIXED(1);
			groundHeight =
				toFIXED(fdMapGridGetTerrainHeightStub(terrainId, tileX, tileZ));
			actor->y.fixed =
				(actor->y.fixed - actor->groundHeight) + groundHeight;
			actor->groundHeight = groundHeight;
		}
	}
}

/* Puts the actor at (x, y, z), 16.16 pixels, at rest; an actor that follows the
   ground (ACTOR_FOLLOWS_TERRAIN) is raised by the ground height there, read as
   fdActorSetHorizontalPosWithTerrain reads it. */
void fdActorSetPosWithTerrain(Uint32 actorId, FIXED x, FIXED y, FIXED z)
{
	ACTOR *actor;
	FIXED  groundHeight;
	Sint32 tileX;
	Sint32 tileZ;

	actor = fdActorLookup(actorId);
	if(actor != NULL) {
		fdActorStartIdleStub(actor);
		SET_VELOCITY(actor, 0, 0, 0);
		actor->tgtY = ACTOR_NO_TARGET;
		actor->tgtX = ACTOR_NO_TARGET;
		SET_ACTOR_POSITION(actor, x, y, z);
		if(actor->motionFlg & ACTOR_FOLLOWS_TERRAIN) {
			Sint32 terrainId = actor->layer;
			tileX = x / toFIXED(1);
			tileZ = z / toFIXED(1);
			groundHeight =
				toFIXED(fdMapGridGetTerrainHeightStub(terrainId, tileX, tileZ));
			actor->y.fixed =
				(actor->y.fixed - actor->groundHeight) + groundHeight;
			actor->groundHeight = groundHeight;
		}
	}
}

/* Switches an actor to an animation (an ACTOR_ANIMATION: ANIM_STAND, ANIM_WALK,
   ANIM_NOD, ANIM_SHAKE_HEAD and so on). */
void fdActorSetAnimationById(Uint32 actorId, Sint32 anm)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL)
		fdActorSelectAnimationStub(actor, anm);
}

/* Sets the animation timer steps of each of an actor's sprites
   (fdSprSetAnimationSpeed). */
void fdActorSetAnimationSpeedById(Uint32 actorId, Sint32 steps)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL)
		fdActorSetAnimationSpeedStub(actor, steps);
}

/* Waits, at most 90 ticks, for an actor's animation to change. Only an actor
   drawn as a sprite (ACTOR_SPRITE_ONE) is waited for. */
void fdActorWaitForAnimationChange(Uint32 actorId)
{
	ACTOR *actor;
	SPR	  *spr;
	/* DEAD: saved lives in a word on the stack, which nothing here needs; the
	   ROM keeps its str and the ldr of each compare. */
	volatile Sint32 saved;
	Sint32			elapsedTicks;

	actor = fdActorLookup(actorId);
	if(actor != NULL && actor->sprStorageKind == ACTOR_SPRITE_ONE) {
		spr = actor->spr;
		saved = spr->lastNo;
		for(elapsedTicks = 0; elapsedTicks < 90; elapsedTicks++) {
			sySynch(1);
			if(saved != spr->lastNo) {
				break;
			}
		}
	}
}

/* Plays an animation and waits for it to end. Scripts reach it
   (fdActorSetAnimationAndWait). */
void fdActorSetAnimationAndWait(Uint32 actorId, Sint32 anm)
{
	fdActorSetAnimationById(actorId, anm);
	fdActorWaitForAnimationChange(actorId);
}

/* An empty function, reached only through an unnamed stub in the stub call
   table of this object's module. */
void fdActorNoOperation(void)
{
}

/* Throws the actor upward at speed pixels per tick with a jump sound (a higher
   one above 5), then waits ticks ticks. */
void fdActorJump(Uint32 actorId, Sint32 speed, Sint32 ticks)
{
	ACTOR *actor;

	actor = fdActorLookup(actorId);
	if(actor != NULL) {
		actor->motionFlg |= ACTOR_FALLS;
		actor->velY = toFIXED(speed);
		if(speed > 5) {
			sySndPlayStub(SE_HIGH_JUMP);
		} else {
			sySndPlayStub(SE_JUMP);
		}
		fdScriptWait(ticks);
	}
}

/* The most times a repeated motion repeats: ActorVariantScripts holds the
   motion that many times over, REPEATED_MOTION_SCRIPT_BYTES (32 words) each. */
#define REPEATED_MOTION_MAX			 3
#define REPEATED_MOTION_SCRIPT_BYTES (32 * sizeof(Sint32))

/* Makes an actor squash and spring back count times (1 to REPEATED_MOTION_MAX),
   through ActorVariantScripts. */
void fdActorStartRepeatedMotion(Uint32 actorId, Sint32 cnt)
{
	ACTOR *actor;

	actor = fdActorLookup(actorId);
	if(actor != NULL && cnt > 0) {
		if(cnt > REPEATED_MOTION_MAX) {
			cnt = REPEATED_MOTION_MAX;
		}
		/* Start count motions from the script's end. */
		fdActorStartScriptStub(actor,
			ActorVariantScripts +
				(REPEATED_MOTION_MAX - cnt) *
					(REPEATED_MOTION_SCRIPT_BYTES / 4));
	}
}

/* As fdActorStartRepeatedMotion, then waits for it to end. */
void fdActorRunRepeatedMotion(Uint32 actorId, Sint32 cnt)
{
	fdActorStartRepeatedMotion(actorId, cnt);
	fdActorWaitForScriptEndById(actorId);
}

/* The share of a dust puff's x and z velocity that drag takes each tick, as
   one part in this many. */
#define DUST_PUFF_DRAG_X 18
#define DUST_PUFF_DRAG_Z 16

/* Moves a dust puff of fdEffectSpawnDustPuff: it drifts with its x and z
   velocity (kept in speed and acceleration), which drag slows each tick, and
   rises 1/64 pixel a tick. */
void fdEffectUpdateDustPuff(ACTOR *particle)
{
	Sint32 velX;
	FIXED  x;
	Sint32 velZ;
	FIXED  z;
	FIXED  y;

	velX = particle->speedLimit;
	x = particle->x.fixed + velX;
	particle->x.fixed = x;
	particle->tgtX = x;
	velZ = particle->acceleration;
	z = particle->z.fixed + velZ;
	particle->z.fixed = z;
	particle->tgtZ = z;
	y = particle->y.fixed + toFIXED(1) / 64;
	particle->y.fixed = y;
	particle->tgtY = y;
	particle->speedLimit = velX - velX / DUST_PUFF_DRAG_X;
	particle->acceleration = velZ - velZ / DUST_PUFF_DRAG_Z;
}

/* Kicks up a dust puff (SPRITE_DUST_PUFF) where source stands: it flies off
   toward the back at 0.5 to 1.4 pixels a tick and sideways at up to 1.4, with
   one of two animations, in the colour mode color. */
void fdEffectSpawnDustPuff(ACTOR *src, Sint32 col)
{
	ACTOR *actor;
	SPR	  *spr;
	Sint32 speed;

	actor = fdActorCreateStub(
		SPRITE_DUST_PUFF, src->x.fixed, src->y.fixed, src->z.fixed);
	if(actor != 0) {
		spr = actor->spr;
		switch(syRandom() & 1) {
		case 1:
			fdActorSelectAnimationStub(actor, 2);
			fdActorStartScriptStub(
				actor, (const Sint32 *)FieldEffectBurstParticleScriptA);
			break;
		default:
			fdActorSelectAnimationStub(actor, 1);
			fdActorStartScriptStub(
				actor, (const Sint32 *)FieldEffectBurstParticleScriptB);
			break;
		}

		if(col != SPRITE_COLOR_NORMAL)
			fdActorSetSprCol(actor, col);

		/* speed and acceleration hold the puff's x and z velocity, in steps of
		   toFIXED(1) / 10 (0.1 pixel): z is -0.5 to -1.4, x -1.4 to 1.4 in
		   steps of 0.2. */
		actor->motionFlg = 0;
		speed = syRandom() % 10 + 5;
		actor->acceleration = -(toFIXED(1) / 10) * speed;
		speed = syRandom() % 15 - 7;
		speed <<= 1;
		actor->speedLimit = toFIXED(1) / 10 * speed;
		actor->tmr = 0;
		actor->update = fdEffectUpdateDustPuff;
		spr->flg = 0;
		spr->image.pri = src->spr->image.pri;
	}
}

/* The ticks a ledge jump sinks for, 2 pixels a tick, and the most it waits to
   land. */
#define LEDGE_JUMP_SINK_TICKS	 14
#define LEDGE_JUMP_LANDING_TICKS 180

/* Makes an actor jump down off a ledge in front of it (the field runs it for
   the leader when the player presses down at a ledge): it shows pose for 10
   ticks, hops 12 pixels south, turns toward FACING_SOUTH_SOUTHWEST (1/16 turn
   past FACING_SOUTH) while sinking 28 pixels with a dust puff every other tick
   in the colour mode dustColor (none when it is -1), then jumps on to 16 pixels
   south of where it started and lands. */
void fdActorJumpDownLedge(Sint32 actorId, Uint32 pose, Sint32 dustColor)
{
	FIXED  nextY;
	FIXED  startZ;
	ACTOR *actor;

	actor = fdActorGetById(actorId);
	startZ = actor->z.fixed;
	if(actor != 0) {
		sySndPlayStub(SOUND_FADE_OUT_ACTION_SE);
		fdActorSelectAnimationStub(actor, pose);
		sySynch(10);

		/* Hop up and forward 12 pixels. */
		fdActorSelectAnimationStub(actor, ANIM_STAND);
		actor->motionFlg |= ACTOR_FALLS;
		actor->velY = toFIXED(4);
		fdActorSetMoveTargetStub(
			actor, actor->x.fixed, actor->y.fixed, startZ + toFIXED(12));
		sySynch(6);

		/* Turn and sink, raising dust, in free flight: no motion flags, so it
		   neither falls nor follows the ground. */
		sySndPlayStub(SE_FLOOR_SWITCH);
		/* From here pose counts ticks. */
		fdActorFaceDirectionById(actorId, FACING_SOUTH_SOUTHWEST, 0);
		actor->motionFlg = 0;
		for(pose = 0; pose < LEDGE_JUMP_SINK_TICKS; pose++) {
			nextY = actor->y.fixed - toFIXED(2);
			actor->y.fixed = nextY;
			actor->tgtY = nextY;
			sySynch(1);
			if((dustColor != -1) && (pose & 1)) {
				fdEffectSpawnDustPuff(actor, dustColor);
			}
		}

		/* Jump 16 pixels forward and wait to land, following the ground and
		   falling again. */
		actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		actor->velY = toFIXED(3);
		fdActorSetMoveTargetStub(
			actor, actor->x.fixed, actor->y.fixed, startZ + toFIXED(16));
		fdActorWaitForMoveStub(actor);
		for(pose = 0; pose < LEDGE_JUMP_LANDING_TICKS &&
			actor->y.fixed > actor->groundHeight;
			pose++) {
			sySynch(1);
		}
		sySynch(2);
		fdPlayQueuedMusic();
	}
}

/* Turns the first actor at once to face the second, then waits the given number
   of ticks. */
void fdActorFaceActorById(Sint32 firstId, Sint32 secondId, Sint32 ticks)
{
	ACTOR *first;
	ACTOR *second;

	first = fdActorLookup(firstId);
	second = fdActorLookup(secondId);
	if(first != 0 && second != 0) {
		first->facing = syAtan(
			second->z.fixed - first->z.fixed, second->x.fixed - first->x.fixed);
		fdScriptWait(ticks);
	}
}

/* Turns two actors toward each other, then waits the given number of ticks. */
void fdActorTurnToFaceEachOther(Sint32 firstId, Sint32 secondId, Sint32 ticks)
{
	void *first = fdActorLookup(firstId);
	void *second = fdActorLookup(secondId);

	if(first != NULL && second != NULL) {
		fdActorTurnPairToFaceEachOther(first, second);
		fdScriptWait(ticks);
	}
}

/* Turns two actors to face each other, at most 1/16 turn a tick each, until
   both do (at most 60 ticks). Only fdActorTurnToFaceEachOther calls it. */
void fdActorTurnPairToFaceEachOther(ACTOR *first, ACTOR *second)
{
	Sint32 firstTgtAng;
	Sint32 secondTgtAng;
	Sint32 frm;
	Sint32 actorsTurning;
	Sint32 angDifference;

	if(first == NULL || second == NULL)
		return;
	firstTgtAng = (Uint16)syAtan(
		second->z.fixed - first->z.fixed, second->x.fixed - first->x.fixed);
	/* The second actor faces the opposite way, half a turn round. */
	secondTgtAng = firstTgtAng + DEGtoANG(180);
	for(frm = 0; frm < 60; frm++) {
		/* Each actor that already faces its way takes one off the count; both
		   facing ends the turn early. */
		actorsTurning = 2;
		angDifference = (Sint16)(firstTgtAng - first->facing);
		if(angDifference != 0) {
			if(angDifference > FACING_STEP)
				angDifference = FACING_STEP;
			if(angDifference < -FACING_STEP)
				angDifference = -FACING_STEP;
			first->facing += angDifference;
		} else {
			actorsTurning = 1;
		}
		angDifference = (Sint16)(secondTgtAng - second->facing);
		if(angDifference != 0) {
			if(angDifference > FACING_STEP)
				angDifference = FACING_STEP;
			if(angDifference < -FACING_STEP)
				angDifference = -FACING_STEP;
			second->facing += angDifference;
		} else {
			actorsTurning--;
		}
		if(actorsTurning == 0)
			break;
		sySynch(1);
	}
}

/* Releases the actor and empties its slot, writing NULL to its place in the
   field state's actor table. */
void fdActorReleaseById(Sint32 actorId)
{
	void		*released = fdActorLookup(actorId);
	FIELD_STATE *table = FieldStateWork;
#if FIX_POINTER_OUTSIDE_OBJECT
	ACTOR *empty = NULL;
#endif

	if(released != 0) {
		fdActorReleaseStub(released);
#if FIX_POINTER_OUTSIDE_OBJECT
		/* The getter accepts slots through 191, including non-actor storage.
		   Copy the pointer representation without extending the actor array. */
		memcpy((Uint8 *)table + offsetof(FIELD_STATE, actors) +
				actorId * sizeof(void *),
			&empty, sizeof(empty));
#else
		/* UB: the getter accepts slots through 191, past the 66 actors. */
		table->actors[actorId] = NULL;
#endif
	}
}

UNUSED void fdActorReleaseNoOperation(void)
{
}

/* Gives every sprite part of an actor the colour mode color (fdActorSetSprCol),
   or with SPRITE_COLOR_BLINK makes it blink. */
void fdActorConfigureSprCol(Sint32 actorId, Sint32 col)
{
	Sint32 blink;
	ACTOR *actor;

	actor = fdActorLookup(actorId);
	if(actor != NULL) {
		blink = SPRITE_COLOR_BLINK & col;
		if(blink != 0) {
			actor->update = fdActorUpdateBlink;
			return;
		}
		actor->update = NULL;
		fdActorSetSprCol(actor, col);
	}
}

/* Per-tick update function of a blinking actor: sets its sprite parts' colour
   mode from ActorBlinkSpriteColors (SPRITE_COLOR_WHITE for two ticks out of
   eight, otherwise SPRITE_COLOR_NORMAL). */
void fdActorUpdateBlink(ACTOR *actor)
{
	if((actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE) {
		Uint8 col;
		SPR	 *container;
		Uint8 partCnt;

		/* The table's four entries each last two ticks. */
		col = ActorBlinkSpriteColors[(TickCount >> 1) & 3];
		container = actor->spr;
		partCnt = container->cnt;
		if(partCnt != 0) {
			SPR_ANIMATION **entries = container->entries;
			Sint32			remaining = partCnt;
			do {
				SPR_ANIMATION *entry = *entries++;
				if(entry != 0 && entry->script != 0) {
					entry->colMode = col;
				}
				remaining--;
			} while(remaining != 0);
		}
		container->dirty = 1;
	}
}

/* Sets the colour mode of every animated part of an actor's sprite. Scripts set
   SPRITE_COLOR_NORMAL or SPRITE_COLOR_HIDDEN, and alternate the two to make an
   actor flash. */
void fdActorSetSprCol(ACTOR *actor, Sint32 col)
{
	if((actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE) {
		SPR	 *container = actor->spr;
		Uint8 partCnt = container->cnt;

		if(partCnt != 0) {
			SPR_ANIMATION **entry = container->entries;
			Uint32			cnt = partCnt;
			do {
				SPR_ANIMATION *part = *entry++;
				if(part != NULL && part->script != 0) {
					part->colMode = col;
				}
				cnt--;
			} while(cnt != 0);
		}
		container->dirty = 1;
	}
}

/* The actor in fdActorWatchAndStartScript's targetId, below its enum
   ACTOR_WATCH_FLAG bits. */
#define ACTOR_WATCH_ACTOR_MASK 0xff
/* The distance, in pixels, a follower stays behind the actor it watches
   (fdActorMoveTowardTarget reads it from the follower's timer). */
#define FOLLOW_DISTANCE		   40

/* Makes an actor follow the actor in tgtId's ACTOR_WATCH_ACTOR_MASK bits and
   starts script on it. Unless tgtId has ACTOR_WATCH_KEEP_MOTION it keeps
   FOLLOW_DISTANCE, takes the target's top speed with twice its acceleration, to
   keep up with it, and no collision flags, so it cannot block the target. */
void fdActorWatchAndStartScript(
	Uint32 actorId, Sint32 tgtId, const void *script)
{
	ACTOR *follower = fdActorLookup(actorId);
	ACTOR *tgt = fdActorLookup(tgtId & ACTOR_WATCH_ACTOR_MASK);

	if(follower != NULL && tgt != NULL) {
		follower->watchedActor = (union ACTOR_OR_EFFECT *)tgt;
		if(!(tgtId & ACTOR_WATCH_KEEP_MOTION)) {
			follower->tmr = FOLLOW_DISTANCE;
			follower->acceleration = tgt->acceleration * 2;
			follower->speedLimit = tgt->speedLimit;
			follower->collisionFlg = 0;
		}
		fdActorStartScriptStub(follower, script);
	}
}

/* SC_CALL function that turns an actor toward the angle kept in its timer: the
   whole way when within 1/16 turn, otherwise 1/32 turn. */
Sint32 fdActorStepAng(ACTOR *actor)
{
	Sint32 delta = 0;

	if(actor != NULL) {
		Sint32 targetAngle = (Uint16)actor->tmr;
		Sint32 currentAngle = actor->facing;
		delta = (Sint16)(targetAngle - currentAngle);
		if(delta != 0) {
			if(delta > TRIG_TURN / 16)
				delta = TRIG_TURN / 32;
			if(delta < -(TRIG_TURN / 16))
				delta = -(TRIG_TURN / 32);
			actor->facing = currentAngle + delta;
		}
	}
	return delta;
}

/* Clears an actor's destination, puts it under script control and stands it
   still. */
void fdActorClearDestinationAndStand(Uint32 actorId)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		actor->tgtX = ACTOR_NO_TARGET;
		actor->tgtY = ACTOR_NO_TARGET;
		actor->tgtZ = ACTOR_NO_TARGET;
		fdActorStartIdleStub(actor);
		fdActorSelectAnimationStub(actor, ANIM_STAND);
	}
}

/* Turns an actor toward angle (TRIG_TURN a full turn; fdActorStepAng), then the
   script waits the given number of ticks. */
void fdActorFaceDirectionById(Sint32 actorId, Sint32 ang, Sint32 ticks)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		actor->tmr = ang;
		fdActorStartScriptStub(actor, (const Sint32 *)ActorStepAngleScript);
		fdScriptWait(ticks);
	}
}

/* Fixes the priority of the actor's sprite, which then no longer follows its
   depth: clears ACTOR_PRIORITY_AUTOMATIC. */
void fdActorSetSprPriority(Sint32 actorId, Sint32 priority)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL &&
		(actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE) {
		SPR *sprite = actor->spr;

		sprite->image.pri = priority;
		sprite->shadow.pri = priority;
		actor->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	}
}

/* Makes the target actor's sprite show the source actor's graphics (its VRAM
   block and tile). Neither actor is checked for NULL. */
void fdActorCopyGraphics(Uint32 tgtId, Uint32 srcId)
{
	ACTOR *actor;
	SPR	  *spr;
	Uint8  vramBlock;
	Uint32 tile;

	actor = fdActorGetById(srcId);
	spr = actor->spr;
	vramBlock = spr->slot;
	tile = spr->image.tile;

	actor = fdActorGetById(tgtId);
	spr = actor->spr;
	spr->slot = vramBlock;
	spr->image.tile = tile;
}

UNUSED void fdActorReservedNoOperation(void)
{
}

/* Sets the number of the script's next message. */
void fdScriptSetMessageId(Sint32 msg)
{
	FieldStateWork->msg = msg;
}

/* Returns the sprite (the first animation's id) of the actor in the low 12 bits
   of actorId, or -1 if there is no such animated actor. */
Sint32 fdActorGetSprById(Sint32 actorId)
{
#if FIX_SPEAKER_SPRITE_PAST_TABLE
	FIELD_STATE *state = FieldStateWork;
	Sint32		 spr = -1;
	ACTOR		*entry;

	actorId &= SPEAKER_ACTOR_MASK;
	/* BUG: the -1 sentinel becomes index 4095, beyond the field state. All
	   indices from ACTOR_ID_COUNT to 4095 lie outside the actor table. */
	if(actorId >= ACTOR_ID_COUNT)
		return -1;
	entry = state->actors[actorId];
#else
	FIELD_STATE *state = FieldStateWork;
	Sint32		 spr = -1;
	ACTOR		*entry = state->actors[actorId & SPEAKER_ACTOR_MASK];
#endif

	if(entry != 0 && entry->sprStorageKind == ACTOR_SPRITE_ONE)
		spr = entry->spr->entries[0]->anmId;
	return spr;
}

/* Returns the first of the scene's actors (ACTOR_FIRST_PLACED to ACTOR_LAST_ID)
   showing sprite, or -1. */
Sint32 fdActorFindBySpr(Sint32 spr)
{
	FIELD_STATE *state = FieldStateWork;
	Sint32		 result = -1;
	Sint32		 idx;
	ACTOR		*actor;

	for(idx = ACTOR_FIRST_PLACED; idx <= ACTOR_LAST_ID; idx++) {
		actor = state->actors[idx];
		if(actor != 0 && actor->sprStorageKind == ACTOR_SPRITE_ONE &&
			actor->spr->entries[0]->anmId == spr) {
			result = idx;
			break;
		}
	}
	return result;
}

#undef DUST_PUFF_DRAG_X
#undef DUST_PUFF_DRAG_Z
#undef ACTOR_WATCH_ACTOR_MASK
#undef FOLLOW_DISTANCE
#undef REPEATED_MOTION_MAX
#undef REPEATED_MOTION_SCRIPT_BYTES
#undef LEDGE_JUMP_SINK_TICKS
#undef LEDGE_JUMP_LANDING_TICKS
