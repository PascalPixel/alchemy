/* Seventeen scenes with things the leader pushes (Altmiller Cave, Babi
   Lighthouse, Crossbone Isle and its rooms, Goma Cave's pillar room, Gondowan
   Cave, Kolima Forest (SceneId_KolimaForestEntrance to
   SceneId_KolimaForestCliff), Lunpa's fortress, Mercury Lighthouse and its
   entrance, Mogall Forest, the Tunnel Ruins, Vale and its cave): things pushed
   a metatile at a time, such as stone pillars, and logs and other large things
   that roll until they meet something, redrawn into the map grid where they
   stop.

   A large thing is a log, a pipe or a fallen column, lying west to east or
   north to south: one of six rectangles, by its sprite
   (PushedActor_RectangleKinds), with its edges in PushedActor_Rectangles
   (pushed_actor_tables.c). */

#include "field/map_grid_scroll.h"
#include "scene/actor_step.h"
#include "system.h"
#include "game/system_call.h"

extern PUSHED_ACTOR_RECTANGLE PushedActor_Rectangles[];

#include "actor_distance.c"

/* Returns the placed actor on the metatile of the 16.16 position and at its
   height (in whole pixels), or 0. origin is not read. */
static ACTOR *scPushedActorFindAtTile(
	const FIELD_POS *position, const ACTOR *origin)
{
	FIELD_STATE *state = FieldStateWork;
	ACTOR	   **slots = state->actors;
	Uint32		 i;

	for(i = ACTOR_FIRST_PLACED;
		i < sizeof(state->actors) / sizeof(state->actors[0]); i++) {
		ACTOR *actor = slots[i];

		/* UB: fdActorClearAll leaves unfilled slots null. */
#if FIX_NULL_DEREFERENCE
		if(actor == NULL)
			continue;
#endif
		if(FIELD_METATILE(position->x) == FIELD_METATILE(actor->x.fixed) &&
			(position->y / toFIXED(1)) == (actor->y.fixed / toFIXED(1)) &&
			FIELD_METATILE(position->z) == FIELD_METATILE(actor->z.fixed)) {
			return actor;
		}
	}
	return 0;
}

/* The leader pushes what stands in front of them a metatile on, unless
   something solid is beyond it or on top of it, the ground beyond refuses it or
   it is still moving: after a quarter second's push it and the leader slide one
   metatile together, with SE_PUSH. */
UNUSED static void scPushedActorAdvancePair(void)
{
	FIELD_POS dst;
	ACTOR	 *lead;
	ACTOR	 *next;
	ACTOR	 *blocker;
	Sint32	  dir;
	FIXED	  rate;

	lead = fdActorGetById(ACTOR_PARTY_LEADER);
	dir = FACING_INDEX(lead->facing);
	fdActorStepFrom(&dst, lead, PushedActor_DirectionSteps[dir]);
	next = scPushedActorFindAtTile(&dst, lead);
	if(next == NULL)
		return;

	fdActorStepFrom(&dst, next, PushedActor_DirectionSteps[dir]);
	blocker = scPushedActorFindAtTile(&dst, next);
	if(blocker != NULL && (blocker->collisionFlg & ACTOR_SOLID) != 0)
		return;

	dst.x = next->x.fixed;
	dst.y = next->y.fixed + toFIXED(16);
	dst.z = next->z.fixed;
	blocker = scPushedActorFindAtTile(&dst, next);
	if(blocker != NULL && (blocker->collisionFlg & ACTOR_SOLID) != 0)
		return;

	next->layer = MAPGRID_LAYER_OVERHEAD;
	fdActorStepFrom(&dst, next, PushedActor_DirectionSteps[dir]);
	if(syFieldActorCheckMovementCollision(next, &dst) > 0)
		return;

	if(next->sceneCnt != 0)
		return;

	fdActorSelectAnimation(lead, ANIM_PUSH);
	rate = toFIXED(1) / 5;
	sySynch(15);
	sySndPlay(SE_PUSH);
	next->speedLimit = rate;
	next->acceleration = rate;
	syFieldActorSetMoveTargetFX(next, dst.x, dst.y, dst.z);
	lead->speedLimit = rate;
	lead->acceleration = rate;
	syFieldActorSetMoveTargetFX(lead, dst.x, dst.y, dst.z);
	fdActorWaitForMove(next);
	fdPlayQueuedMusic();
	next->x.fixed = dst.x;
	next->z.fixed = dst.z;
	next->velX = 0;
	next->velZ = 0;
	lead->tgtX = ACTOR_NO_TARGET;
	lead->tgtZ = ACTOR_NO_TARGET;
	lead->velX = 0;
	lead->velZ = 0;
	lead->x.fixed = toFIXED(lead->x.part[0].integer);
	lead->z.fixed = toFIXED(lead->z.part[0].integer);
	fdActorSelectAnimation(lead, ANIM_STAND);
}

#include "pushed_actor_fill_rectangle.c"

/* The leader's update function while pushing a large thing: stop the leader
   when another actor that is not a large thing, or a wall, is in the way. */
static UNREAD_RESULT scPushedActorStopBlockedMotion(ACTOR *actor)
{
	FIELD_POS probe;
	Uint32	  dir;
	ACTOR	 *target;

	dir = FACING_INDEX(actor->facing);
	fdActorStepFrom(&probe, actor, PushedActor_DirectionSteps[dir]);
	target = scPushedActorFindAtTile(&probe, actor);
	if(target != NULL) {
		Uint32	i = 0;
		Sint32	kind = target->spr->entries[0]->anmId;
		Sint32 *kinds = PushedActor_RectangleKinds;

		do {
			if(kind == *kinds++) {
				RETURN_UNREAD(0);
			}
			i++;
		} while(i <= 5);
		actor->velX = 0;
		actor->velZ = 0;
		actor->tgtX = ACTOR_NO_TARGET;
		actor->tgtZ = ACTOR_NO_TARGET;
	}
	fdActorStepFrom(&probe, actor, PushedActor_DirectionSteps[dir]);
	if(syFieldActorCheckMovementCollision(actor, &probe) > 0) {
		actor->velX = 0;
		actor->velZ = 0;
		actor->tgtX = ACTOR_NO_TARGET;
		actor->tgtZ = ACTOR_NO_TARGET;
	}
#if !FIX_FUNCTION_SIGNATURES
	return 0;
#endif
}

/* Returns the large thing whose rectangle covers the metatile ahead of the
   leader (the leader not standing in line with its first column or row), with
   the leader's direction, its actor id and its rectangle; or NULL. */
static ACTOR *scPushedActorFindLargeThingAhead(
	Sint32 *direction, Sint32 *slot, Sint32 *rectangle)
{
	FIELD_STATE *state = FieldStateWork;
	ACTOR		*player = fdActorGetById(ACTOR_PARTY_LEADER);
	ACTOR	   **list;
	Uint32		 i, j;

	*direction = FACING_INDEX(player->facing);
	list = state->actors + ACTOR_FIRST_PLACED;
	for(i = ACTOR_FIRST_PLACED;
		i < sizeof(state->actors) / sizeof(state->actors[0]); list++, i++) {
		ACTOR *actor = *list;
		Sint32 kind;
		/* UB: the same empty slots are scanned here. */
#if FIX_NULL_DEREFERENCE
		if(actor == NULL)
			continue;
#endif
		for(j = 0, kind = actor->spr->entries[0]->anmId; j < 6; j++) {
			if(kind == PushedActor_RectangleKinds[j]) {
				Sint32 x, z, left, top, right, bottom;
				*rectangle = j;
				x = ((FIXEDtoINT(player->x.fixed)) +
						(FIXEDtoINT(PushedActor_DirectionSteps[*direction]))) >>
					4;
				z = ((FIXEDtoINT(player->z.fixed)) +
						(Sint16)PushedActor_DirectionSteps[*direction]) >>
					4;
				left =
					(actor->x.part[0].integer + PushedActor_Rectangles[j].x0) >>
					4;
				top =
					(actor->z.part[0].integer + PushedActor_Rectangles[j].z0) >>
					4;
				right =
					(actor->x.part[0].integer + PushedActor_Rectangles[j].x1) >>
					4;
				bottom =
					(actor->z.part[0].integer + PushedActor_Rectangles[j].z1) >>
					4;
				if(left <= x && x < right && top <= z && z < bottom) {
					if(j & 1) {
						if(left != FIELD_METATILE(player->x.fixed)) {
							*slot = i;
							return actor;
						}
					} else if(top != FIELD_METATILE(player->z.fixed)) {
						*slot = i;
						return actor;
					}
				}
			}
		}
	}
	return NULL;
}

/* Finds where the large thing ahead of the leader would roll to: step its
   rectangle on, a metatile at a time the way the leader faces, until a metatile
   of it meets something (a collision result of 2). Fills probe with the thing,
   its rectangle and that position; returns 0 when there is no thing or it
   cannot move. */
UNUSED static Sint32 scPushedActorFindClearPos(PUSHED_ACTOR_PROBE *probe)
{
	Sint32	  direction;
	FIELD_POS position;
	ACTOR	 *actor;
	Sint32	  steps;
	Sint32	  width, height;
	Uint8	 *layer;
	Sint32	  column, row;
	Sint32	  nearEdge, farEdge;
	Sint32	  index;

	probe->func = 0;
	actor = scPushedActorFindLargeThingAhead(
		&direction, &probe->actorSlot, &probe->rectangleIdx);
	if(actor == NULL) {
		return 0;
	}
	layer = &actor->layer;
	*layer = MAPGRID_LAYER_OVERHEAD;
	{
		Sint32 rectangle = probe->rectangleIdx;

		steps = 0;
		index = rectangle;
		nearEdge = PushedActor_Rectangles[index].z0;
		if(nearEdge < 0) {
			nearEdge = -nearEdge;
		}
		farEdge = PushedActor_Rectangles[index].z1;
		if(farEdge < 0) {
			farEdge = -farEdge;
		}
		height = (nearEdge + farEdge) >> 4;
		nearEdge = PushedActor_Rectangles[index].x0;
		if(nearEdge < 0) {
			nearEdge = -nearEdge;
		}
		farEdge = PushedActor_Rectangles[index].x1;
		if(farEdge < 0) {
			farEdge = -farEdge;
		}
		width = (nearEdge + farEdge) >> 4;
	}
	position.x = actor->x.fixed +
		DIRECTION_STEP_X_FIXED(PushedActor_DirectionSteps[direction]);
	{
		FIXED y = actor->y.fixed;

		position.y = y;
		position.z = actor->z.fixed +
			DIRECTION_STEP_Z_FIXED(PushedActor_DirectionSteps[direction]);
		probe->pos.y = y;
	}
	for(;;) {
		probe->pos.z = position.z +
			toFIXED(PushedActor_Rectangles[probe->rectangleIdx].z0);
		for(row = 0; row < height; row++) {
			probe->pos.x = position.x +
				toFIXED(PushedActor_Rectangles[probe->rectangleIdx].x0);
			for(column = 0; column < width; column++) {
				if(syFieldActorCheckMovementCollision(actor, &probe->pos) ==
					2) {
					goto found;
				}
				probe->pos.x += toFIXED(16);
			}
			probe->pos.z += toFIXED(16);
		}
		steps++;
		position.x +=
			DIRECTION_STEP_X_FIXED(PushedActor_DirectionSteps[direction]);
		position.z +=
			DIRECTION_STEP_Z_FIXED(PushedActor_DirectionSteps[direction]);
	}
found:
	*layer = MAPGRID_LAYER_GROUND;
	if(steps == 0) {
		return 0;
	}
	probe->pos.x = actor->x.fixed +
		DIRECTION_STEP_X_FIXED(PushedActor_DirectionSteps[direction]) * steps;
	probe->pos.y = actor->y.fixed;
	probe->pos.z = actor->z.fixed +
		DIRECTION_STEP_Z_FIXED(PushedActor_DirectionSteps[direction]) * steps;
	return 1;
}

/* Rolls the large thing to the position probe found: the leader pushes, the
   thing rolls (its sheet's animation 3 or 2, by the direction) with
   SE_HEAVY_MOVE while the leader follows half a metatile, and it stops with
   SE_STOP_ACTION_SE and SE_HEAVY_STOP. Its metatiles are blocked
   (METATILE_BEHAVIOR_WALL) at its new place, through the map grid's layer 1
   offset, and its old place is given back what was under it. */
UNUSED static void scPushedActorRoll(PUSHED_ACTOR_PROBE probe)
{
	MAP_GRID_SCROLL_STATE *state;
	ACTOR				  *actor;
	FIELD_POS			   originalPosition;
	FIELD_POS			   tilePosition;
	Sint32				   direction;
	Sint32				   horizontalExtent;
	Sint32				   verticalExtent;

	/* UB: the heap slot is read as a word rather than as the pointer it
	   holds. */
#if FIX_TYPE_PUNNING
	state = MapGridState[0];
#else
	state = (void *)((Uint32 *)MapGridState)[0];
#endif
	direction = FACING_INDEX(fdActorGetById(ACTOR_PARTY_LEADER)->facing);
	actor = fdActorGetById(probe.actorSlot);
	{
		Sint32 extentA = PushedActor_Rectangles[probe.rectangleIdx].z0;
		Sint32 extentB;

		if(extentA < 0)
			extentA = -extentA;
		extentB = PushedActor_Rectangles[probe.rectangleIdx].z1;
		if(extentB < 0)
			extentB = -extentB;
		verticalExtent = (extentA + extentB) >> 4;
		extentA = PushedActor_Rectangles[probe.rectangleIdx].x0;
		if(extentA < 0)
			extentA = -extentA;
		extentB = PushedActor_Rectangles[probe.rectangleIdx].x1;
		if(extentB < 0)
			extentB = -extentB;
		horizontalExtent = (extentA + extentB) >> 4;
	}

	actor->speedLimit = toFIXED(1) / 2;
	actor->acceleration = toFIXED(1) / 10;
	originalPosition.x = actor->x.fixed;
	originalPosition.z = actor->z.fixed;
	tilePosition.x =
		actor->x.fixed + toFIXED(PushedActor_Rectangles[probe.rectangleIdx].x0);
	tilePosition.z =
		actor->z.fixed + toFIXED(PushedActor_Rectangles[probe.rectangleIdx].z0);
	tilePosition.x = FIELD_METATILE(tilePosition.x);
	tilePosition.z = FIELD_METATILE(tilePosition.z);

	scMapGridFillCollisionRectangle(0, tilePosition.x, tilePosition.z,
		horizontalExtent, verticalExtent, METATILE_BEHAVIOR_NORMAL);
	fdActorSetSpeedAndAcceleration(ACTOR_PARTY_LEADER, 0.5, 0.1);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
	fdScriptWait(15);

	{
		/* Half the thing's roll, in whole pixels. */
		Sint32 horizontalDelta =
			(probe.pos.x - originalPosition.x) / toFIXED(2);
		{
			Sint32 verticalDelta =
				(probe.pos.z - originalPosition.z) / toFIXED(2);

			fdActorSetDestinationOffsetById(
				ACTOR_PARTY_LEADER, horizontalDelta, verticalDelta);
		}
	}

	fdActorGetById(ACTOR_PARTY_LEADER)->update =
		(void (*)(ACTOR *))scPushedActorStopBlockedMotion;
	fdScriptWait(4);
	if(direction >= FACING_INDEX(FACING_SOUTHWEST) &&
		direction <= FACING_INDEX(FACING_NORTH_NORTHEAST))
		fdActorSelectAnimation(actor, 3);
	else
		fdActorSelectAnimation(actor, 2);

	sySndPlay(SE_HEAVY_MOVE);
	syFieldActorSetMoveTargetFX(actor, probe.pos.x, probe.pos.y, probe.pos.z);
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
	fdActorSetSpeedAndAcceleration(ACTOR_PARTY_LEADER, 0.3, 0.1);

	{
		Sint32 packedOrientation = PushedActor_DirectionSteps[direction];
		fdActorSetDestinationOffsetById(ACTOR_PARTY_LEADER,
			(Sint16)DIRECTION_STEP_X(packedOrientation) / 2,
			DIRECTION_STEP_Z(packedOrientation) / 2);
	}
	if(probe.func != 0)
		probe.func();

	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdActorGetById(ACTOR_PARTY_LEADER)->update = NULL;
	fdActorWaitForMove(actor);
	sySndPlay(SE_STOP_ACTION_SE);
	sySndPlay(SE_HEAVY_STOP);

	actor->x.fixed = probe.pos.x;
	actor->z.fixed = probe.pos.z;
	actor->velX = 0;
	actor->velZ = 0;
	fdActorSelectAnimation(actor, 1);

	{
		Sint32 horizontalOrigin;
		Sint32 verticalOrigin;

		probe.pos.x += toFIXED(PushedActor_Rectangles[probe.rectangleIdx].x0);
		probe.pos.z += toFIXED(PushedActor_Rectangles[probe.rectangleIdx].z0);
		probe.pos.x = FIELD_METATILE(probe.pos.x);
		probe.pos.z = FIELD_METATILE(probe.pos.z);
		horizontalOrigin = FIELD_METATILE(state->layers[1].ofsX);
		verticalOrigin = FIELD_METATILE(state->layers[1].ofsY);

		fdMapGridCopyMetatileAttributes(probe.pos.x, probe.pos.z,
			horizontalExtent, verticalExtent, horizontalOrigin + probe.pos.x,
			verticalOrigin + probe.pos.z);
		scMapGridFillCollisionRectangle(0, probe.pos.x, probe.pos.z,
			horizontalExtent, verticalExtent, METATILE_BEHAVIOR_WALL);
		scMapGridFillCollisionRectangle(2, probe.pos.x, probe.pos.z,
			horizontalExtent, verticalExtent, METATILE_BEHAVIOR_WALL);

		originalPosition.x +=
			toFIXED(PushedActor_Rectangles[probe.rectangleIdx].x0);
		originalPosition.z +=
			toFIXED(PushedActor_Rectangles[probe.rectangleIdx].z0);
		originalPosition.x = FIELD_METATILE(originalPosition.x);
		originalPosition.z = FIELD_METATILE(originalPosition.z);
		horizontalOrigin += originalPosition.x;
		verticalOrigin += originalPosition.z;

		fdMapGridCopyMetatileAttributeRect(horizontalOrigin, verticalOrigin,
			horizontalExtent, verticalExtent, originalPosition.x,
			originalPosition.z);
		scMapGridFillCollisionRectangle(2, originalPosition.x,
			originalPosition.z, horizontalExtent, verticalExtent,
			METATILE_BEHAVIOR_NORMAL);
	}
	fdPlayQueuedMusic();
}

/* Blocks large thing id's metatiles (METATILE_BEHAVIOR_WALL) where it stands,
   through the map grid's layer 1 offset; returns 0 when its sprite is no large
   thing. */
UNUSED static Sint32 scPushedActorBlockFootprint(Sint32 id)
{
	MAP_GRID_SCROLL_STATE *grid = MapGridState[0];
	ACTOR				  *actor = fdActorGetById(id);
	PUSHED_ACTOR_PROBE	   probe;
	Uint32				   idx = 0;
	Sint32				   index;
	Sint32				   width;
	Sint32				   height;
	Sint32				   x0;
	Sint32				   x1;
	Sint32				   z0;
	Sint32				   z1;
	Sint32				   dstZ;

	for(idx = 0; idx <= 5; idx++) {
		if(actor->spr->entries[0]->anmId == PushedActor_RectangleKinds[idx]) {
			probe.rectangleIdx = idx;
			break;
		}
		probe.rectangleIdx = PUSHED_ACTOR_NO_RECTANGLE;
	}

	if(probe.rectangleIdx < 0 || probe.rectangleIdx > 6)
		return 0;
	probe.pos.x = actor->x.fixed;
	probe.pos.y = actor->y.fixed;
	probe.pos.z = actor->z.fixed;
	index = probe.rectangleIdx;
	z0 = PushedActor_Rectangles[index].z0;
	if(z0 < 0)
		z0 = -z0;
	z1 = PushedActor_Rectangles[index].z1;
	if(z1 < 0)
		z1 = -z1;
	height = (z0 + z1) >> 4;
	x0 = PushedActor_Rectangles[index].x0;
	if(x0 < 0)
		x0 = -x0;
	x1 = PushedActor_Rectangles[index].x1;
	if(x1 < 0)
		x1 = -x1;
	width = (x0 + x1) >> 4;
	probe.pos.x += toFIXED(PushedActor_Rectangles[index].x0);
	probe.pos.z += toFIXED(PushedActor_Rectangles[index].z0);
	probe.pos.x = FIELD_METATILE(probe.pos.x);
	probe.pos.z = FIELD_METATILE(probe.pos.z);
	idx = FIELD_METATILE(grid->layers[1].ofsX);
	dstZ = FIELD_METATILE(grid->layers[1].ofsY);
	fdMapGridCopyMetatileAttributeRect(probe.pos.x, probe.pos.z, width, height,
		idx + probe.pos.x, dstZ + probe.pos.z);
	scMapGridFillCollisionRectangle(
		0, probe.pos.x, probe.pos.z, width, height, METATILE_BEHAVIOR_WALL);
	scMapGridFillCollisionRectangle(
		2, probe.pos.x, probe.pos.z, width, height, METATILE_BEHAVIOR_WALL);
	return 1;
}
