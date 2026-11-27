/* Script commands on actors: placing, moving and animating them. Part of
   field_mode.c. */

#include "system/field_effect.h"

/* Returns an actor, or NULL if it does not exist. */
ACTOR *fdActorGetById(Uint32 actorId)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor == NULL)
		return NULL;
	return actor;
}

/* The script command "set actor speed": its top speed and acceleration, 16.16
   pixels per tick. Writes the actor's speedLimit and acceleration. */
void fdActorSetSpeedParms(Sint32 actorId, FIXED speedLimit, FIXED acceleration)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		actor->acceleration = acceleration;
		actor->speedLimit = speedLimit;
	}
}

/* Gives an actor a movement type or actor script (fdActorSetMovementType) and
   makes it face where it moves (ACTOR_FACES_MOTION). */
void fdActorSetMovementTypeById(Sint32 actorId, const void *script)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		actor->steeringFlg |= ACTOR_FACES_MOTION;
		fdActorSetMovementType(actor, (Sint32)script);
	}
}

/* The script command "stop actor": stops its movement type and puts it back
   under script control, on the idle script (a lone SC_END), and makes it face
   where it moves. */
void fdActorStop(Sint32 actorId)
{
	ACTOR *actor = fdActorLookup(actorId);
	if(actor != NULL) {
		actor->steeringFlg |= ACTOR_FACES_MOTION;
		fdActorStartIdleStub(actor);
	}
}

/* As fdActorStop, and also clears the actor's target and velocity, so it stands
   where it is. */
void fdActorResetTargetsAndVelocity(Uint32 actorId)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		actor->steeringFlg |= ACTOR_FACES_MOTION;
		fdActorStartIdleStub(actor);
		actor->tgtX = ACTOR_NO_TARGET;
		actor->tgtY = ACTOR_NO_TARGET;
		actor->tgtZ = ACTOR_NO_TARGET;
		SET_VELOCITY(actor, 0, 0, 0);
		actor->arrivalAxis = 0;
	}
}

/* Makes an actor follow another (the watched actor script): writes its
   watchedActor. */
void fdActorWatchAndSetScript(Sint32 actorId, Sint32 watchedActorId)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		actor->watchedActor = (FIELD_OBJECT *)fdActorGetById(watchedActorId);
		fdActorSetMovementType(actor, (Sint32)ActorFaceWatchedScript);
	}
}

#include "field/actor_wait_for_script_end_by_id.c"

/* Starts a movement type or supplied actor script, as
   fdActorSetMovementTypeById does, then waits up to 300 ticks for its script
   to end. */
void fdActorSetMovementTypeAndWaitById(Sint32 actorId, const void *script)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		actor->steeringFlg |= ACTOR_FACES_MOTION;
		fdActorSetMovementType(actor, (Sint32)script);
		fdActorWaitForScriptEndStub(actor);
	}
}

#include "field/actor_set_destination.c"

/* The script command "walk actor to": the actor walks toward (x, z), in
   pixels. */
void fdActorWalkToById(Sint32 actorId, Sint32 x, Sint32 z)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		actor->paused = 0;
		fdActorStartIdleStub(actor);
		fdActorSelectAnimationStub(actor, ANIM_WALK);
		fdActorSetMoveTargetStub(actor, toFIXED(x), actor->y.fixed, toFIXED(z));
	}
}

/* The script command "walk actor to and wait": walks the actor to (x, z), in
   pixels, waits for it to arrive and stands it still. */
void fdActorWalkToAndWaitById(Sint32 actorId, Sint32 x, Sint32 z)
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

#include "field/actor_center_on_column.c"

/* The script command "set actor destination offset": the actor heads (xOfs,
   zOfs) pixels from where it stands. */
void fdActorSetDestinationOffsetById(Sint32 actorId, Sint32 xOfs, Sint32 zOfs)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		actor->paused = 0;
		fdActorStartIdleStub(actor);
		fdActorSetMoveTargetStub(actor, actor->x.fixed + toFIXED(xOfs),
			actor->y.fixed, actor->z.fixed + toFIXED(zOfs));
	}
}

/* Walks the actor (xOfs, zOfs) pixels from where it stands, through the
   offset script. */
void fdActorWalkByById(Uint32 actorId, Sint32 xOfs, Sint32 zOfs)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		actor->paused = 0;
		fdActorStartScriptStub(actor, (Uint32)ActorOffsetScript);
		fdActorSelectAnimationStub(actor, ANIM_WALK);
		fdActorSetMoveTargetStub(actor, actor->x.fixed + toFIXED(xOfs),
			actor->y.fixed, actor->z.fixed + toFIXED(zOfs));
	}
}

#include "field/actor_walk_by_and_wait_by_id.c"

/* Places an actor on the leader, a pixel nearer the back, and sends it walking
   (xOfs, zOfs) pixels away at speed 0.8 and acceleration 0.4, through the
   launch script; the actor then takes movement type action. */
void fdActorLaunchFromLeader(
	Sint32 actorId, Sint32 xOfs, Sint32 zOfs, Sint32 action)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		fdActorSetSpeedParms(actorId, toFIXED(4) / 5, toFIXED(2) / 5);
		fdActorPlaceOnActor(actorId, MainRoutineStateWork.leaderActorId);
		fdActorSetPosAndResetMotionStub(
			actor, actor->x.fixed, actor->y.fixed, actor->z.fixed - toFIXED(1));
		actor->paused = 0;
		fdActorSelectAnimationStub(actor, ANIM_WALK);
		fdActorSetDestinationOffsetById(actorId, xOfs, zOfs);
		fdActorStartScriptStub(actor, ActorLaunchScript);
		actor->tmr = action;
	}
}

/* The script command "wait for actor": waits (at most 600 ticks) for the actor
   to arrive and stands it still. */
void fdActorWaitForMoveById(Sint32 actorId)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		fdActorWaitForMoveStub(actor);
		fdActorSelectAnimationStub(actor, ANIM_STAND);
	}
}

/* The script command "set actor position": puts the actor at (x, z), 16.16
   pixels, at rest; an actor that follows the ground keeps its height above
   it. */
void fdActorSetHorizontalPosWithTerrain(Uint32 actorId, FIXED x, FIXED z)
{
	ACTOR *actor = fdActorLookup(actorId);
	Sint32 terrainHeight;

	if(actor != NULL) {
		fdActorStartIdleStub(actor);
		fdActorSetPosAndResetMotionStub(actor, x, actor->y.fixed, z);
		/* Swap the old ground's height for the new one, so the actor's height
		   above the ground is kept. */
		if(actor->motionFlg & ACTOR_FOLLOWS_TERRAIN) {
			terrainHeight = fdMapGridGetTerrainHeightStub(
				actor->layer, actor->x.fixed, actor->z.fixed);
			actor->y.fixed =
				actor->y.fixed - actor->groundHeight + terrainHeight;
			actor->groundHeight = terrainHeight;
		}
	}
}

/* clang-format off */
/* As fdActorSetHorizontalPosWithTerrain, and turns the actor to angle. */
/* Written in the old style: the angle arrives as a word and is taken as a */
/* halfword. */
void fdActorSetHorizontalPosAndAngWithTerrain(actorId, x, z, ang)
  Sint32 actorId;
  Sint32 x;
  Sint32 z;
  Sint16 ang;
{
	/* clang-format on */
	ACTOR *actor = fdActorLookup(actorId);
	Sint32 terrainHeight;

	if(actor != NULL) {
		fdActorStartIdleStub(actor);
		fdActorSetPosAndResetMotionStub(actor, x, actor->y.fixed, z);
		actor->facing = ang;
		if(actor->motionFlg & ACTOR_FOLLOWS_TERRAIN) {
			terrainHeight = fdMapGridGetTerrainHeightStub(
				actor->layer, actor->x.fixed, actor->z.fixed);
			actor->y.fixed =
				actor->y.fixed - actor->groundHeight + terrainHeight;
			actor->groundHeight = terrainHeight;
		}
	}
}

/* Puts the actor at (x, y, z), 16.16 pixels, at rest; an actor that follows the
   ground is raised by the ground height there, as
   fdActorSetHorizontalPosWithTerrain raises it. */
void fdActorSetPosWithTerrain(Uint32 actorId, FIXED x, FIXED y, FIXED z)
{
	ACTOR *actor = fdActorLookup(actorId);
	Sint32 terrainHeight;

	if(actor != NULL) {
		fdActorStartIdleStub(actor);
		fdActorSetPosAndResetMotionStub(actor, x, y, z);
		if(actor->motionFlg & ACTOR_FOLLOWS_TERRAIN) {
			terrainHeight = fdMapGridGetTerrainHeightStub(
				actor->layer, actor->x.fixed, actor->z.fixed);
			actor->y.fixed =
				actor->y.fixed - actor->groundHeight + terrainHeight;
			actor->groundHeight = terrainHeight;
		}
	}
}

/* Puts the first actor where the second stands, at rest, on the same ground
   height. */
void fdActorPlaceOnActor(Uint32 actorId, Uint32 otherId)
{
	ACTOR *actor = fdActorLookup(actorId);
	ACTOR *other = fdActorLookup(otherId);

	if(actor != NULL && other != NULL) {
		fdActorSetPosAndResetMotionStub(
			actor, other->x.fixed, other->y.fixed, other->z.fixed);
		actor->groundHeight = other->groundHeight;
	}
}

#include "field/actor_set_animation_by_id.c"

/* Sets the animation timer steps of each of an actor's sprites. */
void fdActorSetAnimationSpeedById(Sint32 actorId, Sint32 steps)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL)
		fdActorSetAnimationSpeedStub(actor, steps);
}

/* Waits, at most 90 ticks, for an actor's animation to change or for its first
   sprite animation's script to reach its hold command (ANIMCMD_HOLD). Only an
   actor drawn as a sprite (ACTOR_SPRITE_ONE) is waited for. */
void fdActorWaitForAnimationChange(Uint32 actorId)
{
	ACTOR *actor = fdActorLookup(actorId);
	SPR	  *spr;
	/* DEAD: saved lives in a word on the stack, which nothing here needs; the
	   ROM keeps its str and the ldr of each compare. */
	volatile Uint32 saved;
	Sint32			elapsedTicks;

	if(actor != NULL && actor->sprStorageKind == ACTOR_SPRITE_ONE) {
		spr = actor->spr;
		saved = spr->lastNo;
		for(elapsedTicks = 0; elapsedTicks < 90; elapsedTicks++) {
			sySynch(1);
			if(saved != spr->lastNo)
				break;
			if(spr->entries[0] != NULL && spr->entries[0]->script != NULL &&
				spr->entries[0]->script[spr->entries[0]->scriptOfs] ==
					ANIMCMD_HOLD)
				break;
		}
	}
}

/* The script command "set actor animation and wait": plays the animation and
   waits for it to end. */
void fdActorSetAnimationAndWait(Sint32 actorId, Sint32 anm)
{
	fdActorSetAnimationById(actorId, anm);
	fdActorWaitForAnimationChange(actorId);
}

/* The script command "actor jump": throws the actor upward at speed pixels per
   tick with a jump sound (a higher one above 5), then waits ticks ticks. */
void fdActorJump(Sint32 actorId, Sint32 speed, Sint32 ticks)
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

/* The most squash-and-spring motions ActorVariantScripts holds: it plays the
   motion that many times over, REPEATED_MOTION_SCRIPT_BYTES (32 words) each. */
#define REPEATED_MOTION_MAX			 3
#define REPEATED_MOTION_SCRIPT_BYTES (32 * sizeof(Sint32))

/* The script command "start repeated motion": the actor squashes and springs
   back count times (1 to REPEATED_MOTION_MAX), through ActorVariantScripts. */
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
			(const Sint32 *)(ActorVariantScripts +
				(REPEATED_MOTION_MAX - cnt) * REPEATED_MOTION_SCRIPT_BYTES));
	}
}

#include "field/actor_run_repeated_motion.c"

/* Moves a dust puff of fdEffectSpawnDustPuff: it drifts with its x and z
   velocity (kept in speed and acceleration), which drag slows each tick, and
   rises 1/64 pixel a tick. */
void fdEffectUpdateDustPuff(ACTOR *particle)
{
	FIXED velX = particle->speedLimit;
	FIXED velZ = particle->acceleration;

	particle->x.fixed += velX;
	particle->z.fixed += velZ;
	particle->y.fixed += toFIXED(1) / 64;
	particle->speedLimit = velX - velX / FIELD_EFFECT_DRAG_X;
	particle->acceleration = velZ - velZ / FIELD_EFFECT_DRAG_Z;
}

/* Kicks up a dust puff (SPRITE_DUST_CLOUD) where source stands: it flies off
   toward the back at 0.5 to 1.4 pixels a tick and sideways at up to 1.4, with
   one of two animations, drawn at source's priority, in colMode unless 0. */
void fdEffectSpawnDustPuff(ACTOR *src, Sint32 colMode)
{
	ACTOR *actor;
	SPR	  *child;
	Sint32 speed;
	FIXED  backStep = -(toFIXED(1) / 10); /* 0.1 pixel a tick toward the back */

	actor = fdActorCreateStub(
		SPRITE_DUST_CLOUD, src->x.fixed, src->y.fixed, src->z.fixed);
	if(actor != NULL) {
		child = actor->spr;
		switch(syRandom() & 1) {
		case 1:
			fdActorSelectAnimationStub(actor, 2);
			fdActorStartScriptStub(actor, DustPuffLongScript);
			break;
		default:
			fdActorSelectAnimationStub(actor, 1);
			fdActorStartScriptStub(actor, DustPuffShortScript);
			break;
		}

		if(colMode != 0)
			fdActorSetSprCol(actor, colMode);

		/* speed and acceleration hold the puff's x and z velocity, in steps of
		   toFIXED(1) / 10 (0.1 pixel): z is -0.5 to -1.4, x -1.4 to 1.4 in
		   steps of 0.2. */
		actor->motionFlg = 0;
		speed = syRandom() % 10 + 5;
		actor->acceleration = backStep * speed;
		speed = syRandom() % 15 - 7;
		speed <<= 1;
		actor->speedLimit = toFIXED(1) / 10 * speed;
		actor->tmr = 0;
		actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		actor->frozen = 1;
		actor->update = (OBJECT_UPDATE)fdEffectUpdateDustPuff;
		child->flg = 0;
		child->image.pri = ((SPR *)src->spr)->image.pri;
	}
}

/* Makes an actor jump down off a ledge in front of it: it shows pose for 10
   ticks, hops 12 pixels south from the middle of its 16-pixel column, turns
   toward FACING_SOUTH_SOUTHWEST while sinking 28 pixels with a dust puff every
   other tick in the colour mode dust (none when dust is -1), then jumps on to
   16 pixels south of where it started and lands, waiting at most 180 ticks for
   it unless dust has LEDGE_JUMP_NO_LANDING_WAIT. */
void fdActorJumpDownLedge(Sint32 actorId, Sint32 pose, Sint32 dust)
{
	ACTOR *actor = fdActorGetById(actorId);
	FIXED  startZ = actor->z.fixed;
	FIXED  x = (actor->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
	Sint32 spriteFlags;
	Uint32 tick;

	if(actor == NULL)
		return;
	spriteFlags = ((SPR *)actor->spr)->flg;
	fdActorSelectAnimationStub(actor, pose);
	sySynch(10);

	/* Hop up and forward 12 pixels. */
	fdActorSelectAnimationStub(actor, ANIM_STAND);
	fdActorSetSprFlagsStub(actor, 0);
	actor->motionFlg |= ACTOR_FALLS;
	actor->velY = toFIXED(4);
	fdActorSetMoveTargetStub(actor, x, actor->y.fixed, startZ + toFIXED(12));
	sySynch(6);

	/* Turn and sink 2 pixels a tick for 14 ticks, raising dust, in free
	   flight: no motion flags, so it neither falls nor follows the ground. */
	sySndPlayStub(SE_FLOOR_SWITCH);
	fdActorFaceDirectionById(actorId, FACING_SOUTH_SOUTHWEST, 0);
	actor->motionFlg = 0;
	for(tick = 0; tick < 14; tick++) {
		actor->y.fixed -= toFIXED(2);
		actor->tgtY = actor->y.fixed;
		sySynch(1);
		if(dust != -1 && (tick & 1))
			fdEffectSpawnDustPuff(actor, dust);
	}

	/* Jump 16 pixels forward, following the ground and falling again. */
	actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	actor->velY = toFIXED(3);
	fdActorSetMoveTargetStub(actor, x, actor->y.fixed, startZ + toFIXED(16));
	fdActorWaitForMoveStub(actor);
	if(dust & LEDGE_JUMP_NO_LANDING_WAIT) {
		sySynch(4);
		return;
	}
	tick = 0;
	if(actor->y.fixed > actor->groundHeight) {
		do {
			sySynch(1);
			if(++tick >= 180)
				break;
		} while(actor->y.fixed > actor->groundHeight);
	}
	fdActorSetSprFlagsStub(actor, spriteFlags);
	sySynch(2);
	fdScriptNoOperation();
}

#include "field/actor_face_each_other.c"

/* The script command "release actor": releases the actor and empties its slot
   in the field state's actor table. */
void fdActorReleaseById(Sint32 actorId)
{
	void		*released = fdActorLookup(actorId);
	FIELD_STATE *state = RamHeapSlots->fieldState;

	if(released != NULL) {
		fdActorReleaseStub(released);
		state->actors[actorId] = NULL;
	}
}

/* The script command "set actor sprite colour": gives every sprite animation of
   the actor the colour mode (fdActorSetSprCol); with SPRITE_COLOR_BLINK the
   actor blinks instead (fdActorUpdateBlink). */
void fdActorConfigureSprCol(Sint32 actorId, Sint32 colMode)
{
	ACTOR *actor;

	actor = fdActorLookup(actorId);
	if(actor != NULL) {
		if(colMode & SPRITE_COLOR_BLINK) {
			actor->update = fdActorUpdateBlink;
			return;
		}
		actor->update = NULL;
		fdActorSetSprCol(actor, colMode);
	}
}

/* Per-tick update function of a blinking actor: sets its sprite animations'
   colour mode from ActorBlinkSpriteColors, each entry lasting two ticks. */
void fdActorUpdateBlink(ACTOR *actor)
{
	SPR			  *spr;
	SPR_ANIMATION *anm;
	Sint32		   colMode;
	Sint32		   i;

	if((actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE) {
		spr = actor->spr;
		colMode = ActorBlinkSpriteColors[(TickCount >> 1) & 3];
		for(i = 0; i < spr->cnt; i++) {
			anm = spr->entries[i];
			if(anm != 0 && anm->script != 0) {
				anm->colMode = colMode;
			}
		}
		spr->dirty = 1;
	}
}

/* Sets the colour mode (SPR_COL_MODE) of every running sprite animation of an
   actor's sprite. Scripts set SPRITE_COLOR_NORMAL or SPRITE_COLOR_HIDDEN, and
   alternate the two to make an actor flash. */
void fdActorSetSprCol(ACTOR *actor, Sint32 colMode)
{
	SPR			  *spr;
	SPR_ANIMATION *anm;
	Sint32		   i;

	if((actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE) {
		spr = actor->spr;
		for(i = 0; i < spr->cnt; i++) {
			anm = spr->entries[i];
			if(anm != 0 && anm->script != 0) {
				anm->colMode = colMode;
			}
		}
		spr->dirty = 1;
	}
}

/* fdActorWatchAndStartScript's targetId: the actor to watch, and a flag to keep
   the watcher's own motion. */
typedef enum ACTOR_WATCH_FLAG {
	ACTOR_WATCH_KEEP_MOTION = 0x10000
} ACTOR_WATCH_FLAG;

#define ACTOR_WATCH_ACTOR_MASK 0xff /* the actor's bits */
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
		follower->watchedActor = (FIELD_OBJECT *)tgt;
		if(!(tgtId & ACTOR_WATCH_KEEP_MOTION)) {
			follower->tmr = FOLLOW_DISTANCE;
			follower->acceleration = tgt->acceleration * 2;
			follower->speedLimit = tgt->speedLimit;
			follower->collisionFlg = 0;
		}
		fdActorStartScriptStub(follower, script);
	}
}
