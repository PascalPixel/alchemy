/* Seventeen scenes with things the leader pushes (Altmiller Cave, Babi
   Lighthouse, Crossbone Isle and its rooms, Goma Cave's pillar room, Gondowan
   Cave, Kolima Forest (SceneIdKolimaForestEntrance to
   SceneIdKolimaForestCliff), Lunpa's fortress, Mercury Lighthouse and its
   entrance, Mogall Forest, the Tunnel Ruins, Vale and its cave): things pushed
   a metatile at a time, such as stone pillars, and logs and other large things
   that roll until they meet something, redrawn into the map grid where they
   stop.

   A large thing is a log, a pipe or a fallen column, lying west to east or
   north to south: one of six rectangles, by its sprite
   (PushedActorRectangleKinds), with its edges in PushedActorRectangles
   (pushed_actor_tables.c). */

#include "field/map_grid_scroll.h"
#include "scene/actor_step.h"
#include "system.h"
#include "game/system_call.h"

extern PUSHED_ACTOR_RECTANGLE PushedActorRectangles[];

#include "actor_distance.c"

/* Returns the placed actor on the metatile of the 16.16 position and at its
   height (in whole pixels), or 0. origin is not read. */
static ACTOR *scPushedActorFindAtTile(const FIELD_POS *pos, const ACTOR *origin)
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
		if(FIELD_METATILE(pos->x) == FIELD_METATILE(actor->x.fixed) &&
			(pos->y / toFIXED(1)) == (actor->y.fixed / toFIXED(1)) &&
			FIELD_METATILE(pos->z) == FIELD_METATILE(actor->z.fixed)) {
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
	FIELD_POS dest;
	ACTOR	 *lead;
	ACTOR	 *next;
	ACTOR	 *blocker;
	Sint32	  direction;
	FIXED	  rate;

	lead = fdActorGetById(ACTOR_PARTY_LEADER);
	direction = FACING_INDEX(lead->facing);
	fdActorStepFrom(&dest, lead, PushedActorDirectionSteps[direction]);
	next = scPushedActorFindAtTile(&dest, lead);
	if(next == NULL)
		return;

	fdActorStepFrom(&dest, next, PushedActorDirectionSteps[direction]);
	blocker = scPushedActorFindAtTile(&dest, next);
	if(blocker != NULL && (blocker->collisionFlags & ACTOR_SOLID) != 0)
		return;

	dest.x = next->x.fixed;
	dest.y = next->y.fixed + toFIXED(16);
	dest.z = next->z.fixed;
	blocker = scPushedActorFindAtTile(&dest, next);
	if(blocker != NULL && (blocker->collisionFlags & ACTOR_SOLID) != 0)
		return;

	next->layer = MAPGRID_LAYER_OVERHEAD;
	fdActorStepFrom(&dest, next, PushedActorDirectionSteps[direction]);
	if(syFieldActorCheckMovementCollision(next, &dest) > 0)
		return;

	if(next->sceneCount != 0)
		return;

	fdActorSelectAnimation(lead, ANIM_PUSH);
	rate = toFIXED(1) / 5;
	sySynch(15);
	sySoundPlay(SE_PUSH);
	next->speedLimit = rate;
	next->acceleration = rate;
	syFieldActorSetMoveTargetFX(next, dest.x, dest.y, dest.z);
	lead->speedLimit = rate;
	lead->acceleration = rate;
	syFieldActorSetMoveTargetFX(lead, dest.x, dest.y, dest.z);
	fdActorWaitForMove(next);
	fdPlayQueuedMusic();
	next->x.fixed = dest.x;
	next->z.fixed = dest.z;
	next->velocityX = 0;
	next->velocityZ = 0;
	lead->targetX = ACTOR_NO_TARGET;
	lead->targetZ = ACTOR_NO_TARGET;
	lead->velocityX = 0;
	lead->velocityZ = 0;
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
	Uint32	  direction;
	ACTOR	 *target;

	direction = FACING_INDEX(actor->facing);
	fdActorStepFrom(&probe, actor, PushedActorDirectionSteps[direction]);
	target = scPushedActorFindAtTile(&probe, actor);
	if(target != NULL) {
		Uint32	i = 0;
		Sint32	kind = target->sprite->entries[0]->animationId;
		Sint32 *kinds = PushedActorRectangleKinds;

		do {
			if(kind == *kinds++) {
				RETURN_UNREAD(0);
			}
			i++;
		} while(i <= 5);
		actor->velocityX = 0;
		actor->velocityZ = 0;
		actor->targetX = ACTOR_NO_TARGET;
		actor->targetZ = ACTOR_NO_TARGET;
	}
	fdActorStepFrom(&probe, actor, PushedActorDirectionSteps[direction]);
	if(syFieldActorCheckMovementCollision(actor, &probe) > 0) {
		actor->velocityX = 0;
		actor->velocityZ = 0;
		actor->targetX = ACTOR_NO_TARGET;
		actor->targetZ = ACTOR_NO_TARGET;
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
		for(j = 0, kind = actor->sprite->entries[0]->animationId; j < 6; j++) {
			if(kind == PushedActorRectangleKinds[j]) {
				Sint32 x, z, left, top, right, bottom;
				*rectangle = j;
				x = ((FIXEDtoINT(player->x.fixed)) +
						(FIXEDtoINT(PushedActorDirectionSteps[*direction]))) >>
					4;
				z = ((FIXEDtoINT(player->z.fixed)) +
						(Sint16)PushedActorDirectionSteps[*direction]) >>
					4;
				left =
					(actor->x.part[0].integer + PushedActorRectangles[j].x0) >>
					4;
				top =
					(actor->z.part[0].integer + PushedActorRectangles[j].z0) >>
					4;
				right =
					(actor->x.part[0].integer + PushedActorRectangles[j].x1) >>
					4;
				bottom =
					(actor->z.part[0].integer + PushedActorRectangles[j].z1) >>
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
	FIELD_POS pos;
	ACTOR	 *actor;
	Sint32	  steps;
	Sint32	  width, height;
	Uint8	 *layer;
	Sint32	  column, row;
	Sint32	  nearEdge, farEdge;
	Sint32	  index;

	probe->callback = 0;
	actor = scPushedActorFindLargeThingAhead(
		&direction, &probe->actorSlot, &probe->rectangleIndex);
	if(actor == NULL) {
		return 0;
	}
	layer = &actor->layer;
	*layer = MAPGRID_LAYER_OVERHEAD;
	{
		Sint32 rectangle = probe->rectangleIndex;

		steps = 0;
		index = rectangle;
		nearEdge = PushedActorRectangles[index].z0;
		if(nearEdge < 0) {
			nearEdge = -nearEdge;
		}
		farEdge = PushedActorRectangles[index].z1;
		if(farEdge < 0) {
			farEdge = -farEdge;
		}
		height = (nearEdge + farEdge) >> 4;
		nearEdge = PushedActorRectangles[index].x0;
		if(nearEdge < 0) {
			nearEdge = -nearEdge;
		}
		farEdge = PushedActorRectangles[index].x1;
		if(farEdge < 0) {
			farEdge = -farEdge;
		}
		width = (nearEdge + farEdge) >> 4;
	}
	pos.x = actor->x.fixed +
		DIRECTION_STEP_X_FIXED(PushedActorDirectionSteps[direction]);
	{
		FIXED y = actor->y.fixed;

		pos.y = y;
		pos.z = actor->z.fixed +
			DIRECTION_STEP_Z_FIXED(PushedActorDirectionSteps[direction]);
		probe->pos.y = y;
	}
	for(;;) {
		probe->pos.z =
			pos.z + toFIXED(PushedActorRectangles[probe->rectangleIndex].z0);
		for(row = 0; row < height; row++) {
			probe->pos.x = pos.x +
				toFIXED(PushedActorRectangles[probe->rectangleIndex].x0);
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
		pos.x += DIRECTION_STEP_X_FIXED(PushedActorDirectionSteps[direction]);
		pos.z += DIRECTION_STEP_Z_FIXED(PushedActorDirectionSteps[direction]);
	}
found:
	*layer = MAPGRID_LAYER_GROUND;
	if(steps == 0) {
		return 0;
	}
	probe->pos.x = actor->x.fixed +
		DIRECTION_STEP_X_FIXED(PushedActorDirectionSteps[direction]) * steps;
	probe->pos.y = actor->y.fixed;
	probe->pos.z = actor->z.fixed +
		DIRECTION_STEP_Z_FIXED(PushedActorDirectionSteps[direction]) * steps;
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
	FIELD_POS			   originalPos;
	FIELD_POS			   tilePos;
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
		Sint32 extentA = PushedActorRectangles[probe.rectangleIndex].z0;
		Sint32 extentB;

		if(extentA < 0)
			extentA = -extentA;
		extentB = PushedActorRectangles[probe.rectangleIndex].z1;
		if(extentB < 0)
			extentB = -extentB;
		verticalExtent = (extentA + extentB) >> 4;
		extentA = PushedActorRectangles[probe.rectangleIndex].x0;
		if(extentA < 0)
			extentA = -extentA;
		extentB = PushedActorRectangles[probe.rectangleIndex].x1;
		if(extentB < 0)
			extentB = -extentB;
		horizontalExtent = (extentA + extentB) >> 4;
	}

	actor->speedLimit = toFIXED(1) / 2;
	actor->acceleration = toFIXED(1) / 10;
	originalPos.x = actor->x.fixed;
	originalPos.z = actor->z.fixed;
	tilePos.x = actor->x.fixed +
		toFIXED(PushedActorRectangles[probe.rectangleIndex].x0);
	tilePos.z = actor->z.fixed +
		toFIXED(PushedActorRectangles[probe.rectangleIndex].z0);
	tilePos.x = FIELD_METATILE(tilePos.x);
	tilePos.z = FIELD_METATILE(tilePos.z);

	scMapGridFillCollisionRectangle(0, tilePos.x, tilePos.z, horizontalExtent,
		verticalExtent, METATILE_BEHAVIOR_NORMAL);
	fdActorSetSpeedAndAcceleration(ACTOR_PARTY_LEADER, 0.5, 0.1);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
	fdScriptWait(15);

	{
		/* Half the thing's roll, in whole pixels. */
		Sint32 horizontalDelta = (probe.pos.x - originalPos.x) / toFIXED(2);
		{
			Sint32 verticalDelta = (probe.pos.z - originalPos.z) / toFIXED(2);

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

	sySoundPlay(SE_HEAVY_MOVE);
	syFieldActorSetMoveTargetFX(actor, probe.pos.x, probe.pos.y, probe.pos.z);
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
	fdActorSetSpeedAndAcceleration(ACTOR_PARTY_LEADER, 0.3, 0.1);

	{
		Sint32 packedOrientation = PushedActorDirectionSteps[direction];
		fdActorSetDestinationOffsetById(ACTOR_PARTY_LEADER,
			(Sint16)DIRECTION_STEP_X(packedOrientation) / 2,
			DIRECTION_STEP_Z(packedOrientation) / 2);
	}
	if(probe.callback != 0)
		probe.callback();

	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdActorGetById(ACTOR_PARTY_LEADER)->update = NULL;
	fdActorWaitForMove(actor);
	sySoundPlay(SE_STOP_ACTION_SE);
	sySoundPlay(SE_HEAVY_STOP);

	actor->x.fixed = probe.pos.x;
	actor->z.fixed = probe.pos.z;
	actor->velocityX = 0;
	actor->velocityZ = 0;
	fdActorSelectAnimation(actor, 1);

	{
		Sint32 horizontalOrigin;
		Sint32 verticalOrigin;

		probe.pos.x += toFIXED(PushedActorRectangles[probe.rectangleIndex].x0);
		probe.pos.z += toFIXED(PushedActorRectangles[probe.rectangleIndex].z0);
		probe.pos.x = FIELD_METATILE(probe.pos.x);
		probe.pos.z = FIELD_METATILE(probe.pos.z);
		horizontalOrigin = FIELD_METATILE(state->layers[1].offsetX);
		verticalOrigin = FIELD_METATILE(state->layers[1].offsetY);

		fdMapGridCopyMetatileAttributes(probe.pos.x, probe.pos.z,
			horizontalExtent, verticalExtent, horizontalOrigin + probe.pos.x,
			verticalOrigin + probe.pos.z);
		scMapGridFillCollisionRectangle(0, probe.pos.x, probe.pos.z,
			horizontalExtent, verticalExtent, METATILE_BEHAVIOR_WALL);
		scMapGridFillCollisionRectangle(2, probe.pos.x, probe.pos.z,
			horizontalExtent, verticalExtent, METATILE_BEHAVIOR_WALL);

		originalPos.x +=
			toFIXED(PushedActorRectangles[probe.rectangleIndex].x0);
		originalPos.z +=
			toFIXED(PushedActorRectangles[probe.rectangleIndex].z0);
		originalPos.x = FIELD_METATILE(originalPos.x);
		originalPos.z = FIELD_METATILE(originalPos.z);
		horizontalOrigin += originalPos.x;
		verticalOrigin += originalPos.z;

		fdMapGridCopyMetatileAttributeRect(horizontalOrigin, verticalOrigin,
			horizontalExtent, verticalExtent, originalPos.x, originalPos.z);
		scMapGridFillCollisionRectangle(2, originalPos.x, originalPos.z,
			horizontalExtent, verticalExtent, METATILE_BEHAVIOR_NORMAL);
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
	Uint32				   index = 0;
	Sint32				   rectangleIndex;
	Sint32				   width;
	Sint32				   height;
	Sint32				   x0;
	Sint32				   x1;
	Sint32				   z0;
	Sint32				   z1;
	Sint32				   destZ;

	for(index = 0; index <= 5; index++) {
		if(actor->sprite->entries[0]->animationId ==
			PushedActorRectangleKinds[index]) {
			probe.rectangleIndex = index;
			break;
		}
		probe.rectangleIndex = PUSHED_ACTOR_NO_RECTANGLE;
	}

	if(probe.rectangleIndex < 0 || probe.rectangleIndex > 6)
		return 0;
	probe.pos.x = actor->x.fixed;
	probe.pos.y = actor->y.fixed;
	probe.pos.z = actor->z.fixed;
	rectangleIndex = probe.rectangleIndex;
	z0 = PushedActorRectangles[rectangleIndex].z0;
	if(z0 < 0)
		z0 = -z0;
	z1 = PushedActorRectangles[rectangleIndex].z1;
	if(z1 < 0)
		z1 = -z1;
	height = (z0 + z1) >> 4;
	x0 = PushedActorRectangles[rectangleIndex].x0;
	if(x0 < 0)
		x0 = -x0;
	x1 = PushedActorRectangles[rectangleIndex].x1;
	if(x1 < 0)
		x1 = -x1;
	width = (x0 + x1) >> 4;
	probe.pos.x += toFIXED(PushedActorRectangles[rectangleIndex].x0);
	probe.pos.z += toFIXED(PushedActorRectangles[rectangleIndex].z0);
	probe.pos.x = FIELD_METATILE(probe.pos.x);
	probe.pos.z = FIELD_METATILE(probe.pos.z);
	index = FIELD_METATILE(grid->layers[1].offsetX);
	destZ = FIELD_METATILE(grid->layers[1].offsetY);
	fdMapGridCopyMetatileAttributeRect(probe.pos.x, probe.pos.z, width, height,
		index + probe.pos.x, destZ + probe.pos.z);
	scMapGridFillCollisionRectangle(
		0, probe.pos.x, probe.pos.z, width, height, METATILE_BEHAVIOR_WALL);
	scMapGridFillCollisionRectangle(
		2, probe.pos.x, probe.pos.z, width, height, METATILE_BEHAVIOR_WALL);
	return 1;
}
