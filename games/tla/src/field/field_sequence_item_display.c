/* @file */
/* Showing a found item, and Reveal's sparkles on hidden things. */
/* Part of field_sequence.c. */

#include "graphics/glyph.h"
#include "field/screen_transition.h"
#include "field/ground_mask.h"

/* Unused: only the stub table refers to it. Keeps the party leader off the */
/* treasure spots: a leader within 16 pixels of a treasure spot's actor is */
/* moved 20 pixels from it, in the direction it stood, and loses its move */
/* target. */
void fdTreasureSpotMoveLeaderAway(void) {
	ACTOR *leader;
	ACTOR *other;
	FIXED dx;
	FIXED dz;
	s32 i;

	leader = fdActorLookup(fdPartyGetLeaderActor());
	for(i = 0; i <= SCOOPED_ITEM_ACTOR - FIELD_TREASURE_ACTOR; i++) {
		other = fdActorLookup(i + FIELD_TREASURE_ACTOR);
		dx = leader->x.fixed - other->x.fixed;
		dz = leader->z.fixed - other->z.fixed;
		if(dx > -toFIXED(16) && dx < toFIXED(16))
			if(dz > -toFIXED(16) && dz < toFIXED(16)) {
				leader->x.fixed = other->x.fixed;
				leader->z.fixed = other->z.fixed;
				syVectorAddPolarOffset(toFIXED(20), (u16)syAtan(dz, dx),
									   &leader->x.fixed);
				leader->tgtX = ACTOR_NO_TARGET;
				leader->tgtY = ACTOR_NO_TARGET;
				leader->tgtZ = ACTOR_NO_TARGET;
				break;
			}
	}
}

/* Starts the particle script and its first animation at priority 1, if the */
/* spawn succeeded. Allocation failure leaves the sparkle absent. */
static inline void fdActorInitSparkle(ACTOR *actor) {
	if(actor != 0) {
		fdActorStartScriptStub(actor, (const s32 *)ItemDisplay_SparkleScript);
		fdActorSelectAnimationStub(actor, 0);
		((SPR *)actor->spr)->image.pri = 1;
	}
}

/* Spawns a sparkle within 16 pixels of source, in a random direction: the */
/* sparkling a found item's picture and a Reveal marker share. */
#define SPAWN_SPARKLE_NEAR(source)                                             \
	{                                                                          \
		POLAR_OFFSET_POS position;                                             \
		ACTOR *sparkle;                                                        \
		u32 randomRadius;                                                      \
                                                                               \
		position.vector.x = (source)->x.fixed;                                 \
		position.vector.y = (source)->y.fixed;                                 \
		position.vector.z = (source)->z.fixed;                                 \
		randomRadius = syRandom();                                             \
		syVectorAddPolarOffset(randomRadius << 4, syRandom(),                  \
							   position.coordinates);                          \
		sparkle = fdActorSpawn(SPRITE_SPARKLE, position.vector.x,              \
							   position.vector.y, position.vector.z);          \
		fdActorInitSparkle(sparkle);                                           \
	}

/* Each frame of a found item's picture: one time in ten, a sparkle appears */
/* within 16 pixels of it. */
void fdItemDispEmitSparkles(ACTOR *emitter) {
	/* Once the picture's rise has all but stopped (under 1/256 pixel a frame),
	 */
	/* clearing its motion flags (ACTOR_FALLS and ACTOR_FOLLOWS_TERRAIN) */
	/* ends gravity, so it hangs at the top of its rise. */
	if(emitter->velY > -(toFIXED(1) / 256) && emitter->velY < toFIXED(1) / 256)
		emitter->motionFlg = 0;

	/* Test the chance before drawing the radius and angle from the RNG. */
	if((100 * syRandom() >> 16) > 9)
		return;

	SPAWN_SPARKLE_NEAR(emitter);
}

/* Returns 1 while Reveal is active, else 0. */
s32 fdIsRevealActive(ACTOR *interpreter) {
	FIELD_STATE *state = FieldStateWork;

	if(state == NULL)
		return 0;
	if(state->revealActive != 0)
		return 1;
	else
		return 0;
}

void fdRevealEmitSparkles(ACTOR *source);

/* Shows the picture of item 16 pixels above actor actorId, or for the leader */
/* at the centre of the cell 16 pixels in front of them: it rises a little and
 */
/* hangs there, sparkling. While Reveal is active it first releases Reveal's */
/* sparkles, and then its mark on the picture's cell. */
ACTOR *fdItemDispShowItemInFront(s32 actorId, s32 item) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR *picture = state->actors[actorId];
	ACTOR *actor;
	SPR *sprite;
	FIXED position[3];
	ICON_TRANSFER *transfer;
	s32 tile;
	s32 i;

	if(picture == NULL)
		return NULL;
	position[0] = picture->x.fixed;
	position[1] = picture->y.fixed;
	position[2] = picture->z.fixed;
	if(actorId == fdPartyGetLeaderActor()) {
		syVectorAddPolarOffset(toFIXED(16), picture->facing, position);
		position[0] = (position[0] & FIELD_CELL_MASK) + toFIXED(8);
		position[2] = (position[2] & FIELD_CELL_MASK) + toFIXED(8);
	}
	if(state->revealActive != 0) {
		/* Stop both the marker emitters and the sparkle actors they already
		 * made. */
		actor = Ram_HeapSlots->actors;
		for(i = 0; i < ACTOR_COUNT; i++) {
			if(actor->script != NULL) {
				if(actor->update == fdRevealEmitSparkles)
					fdActorReleaseStub(actor);
				if(actor->script == (s32 *)ItemDisplay_SparkleScript)
					fdActorReleaseStub(actor);
			}
			actor++;
		}
	}
	sySynch(3);
	picture = fdActorCreateStub(SPRITE_UNSEEN_MARKER, position[0],
								position[1] + toFIXED(16), position[2]);
	if(picture == NULL)
		return NULL;
	fdActorStartScriptStub(picture, ItemDisplay_GrowInScript);
	sprite = picture->spr;
	/* No shadow, outline or sprite animations: a 16-colour sprite in OBJ */
	/* palette 0 whose tiles are the item's icon. */
	sprite->flg = 0;
	sprite->cnt = 0;
	sprite->image.fullCol = 0;
	sprite->image.pal = 0;
	sprite->image.pri = 1;
	picture->motionFlg = ACTOR_FALLS;
	picture->groundHeight = position[1];
	picture->velY = toFIXED(2);
	picture->gravity = toFIXED(1) / 4;
	transfer = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	mnIconDrawItemStub(item);
	tile = syVramCacheLoad(sprite->slot, ICON_TILE_BYTES, transfer->tiles);
	sprite->image.tile = tile;
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	picture->update = fdItemDispEmitSparkles;
	if(state->revealActive != 0) {
		ACTOR *marker = Ram_HeapSlots->actors;
		s32 j;

		for(j = 0; j < ACTOR_COUNT; j++, marker++) {
			if(marker->script != NULL &&
			   marker->script == (s32 *)Reveal_MarkerScript &&
			   FIELD_CELL(marker->x.fixed) == FIELD_CELL(picture->x.fixed) &&
			   FIELD_CELL(marker->z.fixed) == FIELD_CELL(picture->z.fixed)) {
				fdActorReleaseStub(marker);
				break;
			}
		}
	}
	return picture;
}

/* Releases actor unless it is NULL. */
void fdActorReleaseIfPresent(ACTOR *actor) {
	if(actor != 0)
		fdActorReleaseStub(actor);
}

/* Unused: an empty function between the item display's own, which nothing */
/* calls; The Broken Seal has none here. */
UNUSED void fdItemDispUnused(void) {}

/* Raises a found item's picture to 36 pixels above the party leader, who holds
 */
/* it up (ANIM_LEADER_ARMS_RAISED) facing the screen. */
void fdItemDispRaiseAboveLeader(ACTOR *actor) {
	ACTOR *leader;

	if(actor == 0)
		return;

	{
		ACTOR *found = fdActorLookup(fdPartyGetLeaderActor());
		/* Up to 2 pixels a frame, gaining 1 a frame, in free flight: no motion
		 */
		/* flags, so it neither falls nor follows the ground. */
		actor->acceleration = toFIXED(1);
		actor->speedLimit = toFIXED(2);
		actor->motionFlg = 0;
		leader = found;
	}
	fdActorSetMoveTargetStub(actor, leader->x.fixed,
							 leader->y.fixed + toFIXED(36), leader->z.fixed);
	fdActorSetSprPriorityDirectStub(actor, 1);
	sySynch(3);
	fdActorSelectAnimationStub(leader, ANIM_LEADER_ARMS_RAISED);
	fdActorStartScriptStub(actor, ItemDisplay_PulseScript);
	/* FACING_SOUTH, a quarter turn, faces the screen. */
	leader->facing = FACING_SOUTH;
}

/* Shows a found item's picture as flags choose (ITEM_DISPLAY_FLAG), with waits
 */
/* of 60 and 80 frames around its raising when both are set. */
void fdItemDispShowPicture(ACTOR *actor, s32 flags) {
	ACTOR *leader;

	if(actor != NULL) {
		leader = fdActorLookup(fdPartyGetLeaderActor());
		if(flags & ITEM_DISPLAY_SPARKLES) {
			fdActorSetSprFlagsStub(actor, 0);
			fdActorStartScriptStub(actor, ItemDisplay_GrowInScript);
			((ACTOR *)actor)->velY = toFIXED(2);
			((ACTOR *)actor)->gravity = toFIXED(1) / 4;
			((ACTOR *)actor)->update = fdItemDispEmitSparkles;
		}
		if(flags == (ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED))
			sySynch(60);
		if(flags & ITEM_DISPLAY_RAISED)
			fdItemDispRaiseAboveLeader(actor);
		if(flags == (ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED))
			sySynch(80);
		fdActorSelectAnimationStub(leader, ANIM_STAND);
	}
}

/* Shows the picture of item 36 pixels above the party leader for 80 frames, */
/* as flags choose (ITEM_DISPLAY_FLAG). */
void fdItemDispShowItem(s32 item, s32 flags) {
	ACTOR *leader = fdActorLookup(fdPartyGetLeaderActor());
	ICON_TRANSFER *transfer =
		syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	ACTOR *actor =
		fdActorCreateStub(SPRITE_UNSEEN_MARKER, leader->x.fixed,
						  leader->y.fixed + toFIXED(36), leader->z.fixed);

	if(actor != 0) {
		SPR *sprite = actor->spr;

		sprite->flg = 0;
		sprite->cnt = 0;
		sprite->image.fullCol = 0;
		sprite->image.pal = 0;
		sprite->image.pri = 1;

		mnIconDrawItemStub(item);
		syVramCacheLoad(sprite->slot, ICON_TILE_BYTES, transfer->tiles);
		syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);

		if(flags & ITEM_DISPLAY_SPARKLES)
			actor->update = fdItemDispEmitSparkles;
		if(flags & ITEM_DISPLAY_RAISED)
			fdItemDispRaiseAboveLeader(actor);

		sySynch(80);
		fdActorSelectAnimationStub(leader, ANIM_STAND);
		fdActorReleaseStub(actor);
	}
}

/* Each frame of a Reveal marker: one time in ten, a sparkle appears within 16
 */
/* pixels of it, as around a found item's picture (SPAWN_SPARKLE_NEAR). */
void fdRevealEmitSparkles(ACTOR *actor) {
	if((100 * syRandom() >> 16) > 9)
		return;

	SPAWN_SPARKLE_NEAR(actor);
}

/* Places Reveal's markers from the scene's item rows (a type, an event id, a */
/* flag and a pad): for each of the treasure spots' rows whose flag is clear */
/* and whose event has not run, a sparkle marker goes at its actor, or at the */
/* map cell of a tile event, when that point is on screen. */
void fdEffectSpawnRevealMarkers(void) {
	s16 *entry = fdSceneItemGetRows();
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	s32 originX = syMapStateView(map)[0] & FIELD_PIXEL_MASK;
	s32 originZ = syMapStateView(map)[1] & FIELD_PIXEL_MASK;
	/* A word of the map header, then the point fdSceneItemFindPos fills for */
	/* each row. */
	struct {
		s32 headerWord;
		s32 pos[3];
	} found;
	ACTOR *actor;
	s16 type;
	s32 id;
	s32 flag;
	s32 i;
	FIXED x;
	s32 y;
	FIXED z;
	s32 dx;
	s32 dz;

	/* DEAD: the followed point's height is never read; the ROM keeps its load
	 */
	/* and store. */
	found.headerWord = map->origin[1];
	if(entry == NULL)
		return;
	for(i = 0; i < TREASURE_SPOT_MAX && (type = *entry++) != 0 &&
			   !(type & SCENE_ITEMS_HEADER);
		i++) {
		id = entry[0];
		flag = entry[1];
		entry += 3;
		if(type != CONTAINER_CHEST && flag != -1 && syGameFlagGet(flag))
			continue;
		if(fdSceneItemFindPos(id, found.pos))
			continue;
		actor = fdTreasureSpotGetActor(i + FIELD_TREASURE_ACTOR);
		if(actor == NULL)
			continue;
		/* A tile event's actor holds its map cell, marked by this value. */
		if(actor->sceneWord == TREASURE_SPOT_ON_CELL) {
			x = toFIXED(actor->tmr);
			z = toFIXED(actor->userValue);
			y = fdMapGridGetTerrainHeightStub(0, x, z);
		}
		else {
			if((u16)(type - CONTAINER_CHEST) <=
			   CONTAINER_MIMIC - CONTAINER_CHEST)
				continue;
			if(type == CONTAINER_PSYNERGY_STONE ||
			   type == CONTAINER_STONE_TABLET || type == CONTAINER_POND ||
			   type == CONTAINER_GROUND)
				continue;
			x = actor->x.fixed;
			y = actor->y.fixed;
			z = actor->z.fixed;
		}
		dx = x - originX;
		dz = z - originZ - y;
		dx >>= 16;
		dz >>= 16;
		/* On the 240 x 160 screen, give or take 15 pixels at the sides, 47
		 * above */
		/* and 31 below. */
		if((u32)(dx + 15) > 270 || (u32)(dz + 47) > 238)
			continue;
		if(flag == -1 || syGameFlagGet(flag))
			continue;
		actor = fdActorCreateStub(SPRITE_SPARKLE, x, y, z);
		if(actor != NULL) {
			actor->motionFlg = 0;
			fdActorStartScriptStub((ACTOR *)actor, Reveal_MarkerScript);
			fdActorSetSprFlagsStub((ACTOR *)actor, 0);
			fdActorSelectAnimationStub(actor, ANIM_STAND);
			((SPR *)actor->spr)->image.pri = 1;
		}
	}
}
