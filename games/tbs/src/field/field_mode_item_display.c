/* Showing a found item, and Reveal's sparkles on hidden things. Part of
   field_mode.c. */

#include "field_mode_internal.h"

/* Keeps the party leader off the treasure spots: a leader within 16 pixels of a
   treasure spot's centre is moved 20 pixels from that centre, in the direction
   it stood, and loses its move target. */
void fdTreasureSpotMoveLeaderAway(void)
{
	ACTOR		  *leader = fdActorLookup(MainRoutineStateWork.leaderActorId);
	TREASURE_SPOT *marker = FieldStateWork->treasureSpots;
	Sint32		   markerIdx = 0;
	FIXED		   x;
	FIXED		   z;
	Sint32		   metatileX;
	Sint32		   metatileZ;
	FIXED		   dx;
	FIXED		   dz;

	/* The slots are filled in order, so the first with id 0 ends the list. */
	if(marker->id != 0) {
		x = leader->x.fixed;
		z = leader->z.fixed;
		for(;;) {
			metatileX = marker->column * toFIXED(MAPGRID_METATILE_PIXELS);
			dx = x - metatileX - toFIXED(8);
			metatileZ = marker->row * toFIXED(MAPGRID_METATILE_PIXELS);
			dz = z - metatileZ - toFIXED(8);
			if((dx > -toFIXED(MAPGRID_METATILE_PIXELS) &&
				   dx < toFIXED(MAPGRID_METATILE_PIXELS)) &&
				(dz > -toFIXED(MAPGRID_METATILE_PIXELS) &&
					dz < toFIXED(MAPGRID_METATILE_PIXELS))) {
				leader->x.fixed = metatileX + toFIXED(8);
				leader->z.fixed = metatileZ + toFIXED(8);
				syVectorAddPolarOffset(toFIXED(20), (Uint16)syAtan(dz, dx),
					(FIXED *)&leader->x.fixed);
				leader->tgtX = ACTOR_NO_TARGET;
				leader->tgtY = ACTOR_NO_TARGET;
				leader->tgtZ = ACTOR_NO_TARGET;
				return;
			}
			markerIdx++;
			marker++;
			if(markerIdx > TREASURE_SPOT_MAX - 1 || marker->id == 0)
				break;
		}
	}
}

/* Starts a sparkle twinkling, drawn at priority 1; one that could not be
   spawned is left out. */
static inline void fdActorInitSparkle(ACTOR *actor)
{
	if(actor != 0) {
		fdActorStartScriptStub(actor, FieldEffectParticleScript);
		fdActorSelectAnimationStub(actor, 0);
		actor->spr->image.pri = 1;
	}
}

/* Each tick of a found item's picture: one time in ten, a sparkle appears
   within 16 pixels of it. */
void fdItemDispEmitSparkles(ACTOR *emitter)
{
	POLAR_OFFSET_POS pos;
	ACTOR			*object;
	Uint32			 randomRadius;

	/* Once the picture's rise has all but stopped, clearing its motion flags
	   (ACTOR_FALLS and ACTOR_FOLLOWS_TERRAIN) ends gravity, so it hangs at the
	   top of its rise. */
	if(emitter->velY > -(toFIXED(1) / 256) && emitter->velY < toFIXED(1) / 256)
		emitter->motionFlg = 0;

	/* Test the chance before drawing the radius and angle from the RNG. */
	if((100 * syRandom() >> 16) > 9)
		return;

	pos.vector.x = emitter->x.fixed;
	pos.vector.y = emitter->y.fixed;
	pos.vector.z = emitter->z.fixed;
	randomRadius = syRandom();
	syVectorAddPolarOffset(randomRadius << 4, syRandom(), pos.coordinates);
	object =
		fdActorSpawn(SPRITE_SPARKLE, pos.vector.x, pos.vector.y, pos.vector.z);
	fdActorInitSparkle(object);
}

/* Shows the picture of item (0 for none) at the centre of the metatile 16
   pixels in front of character actorId, at a height of 16 pixels whatever the
   actor's, with sparkles. */
ACTOR *fdItemDispShowItemInFront(Sint32 actorId, Sint32 item)
{
	ACTOR *picture;

	Sint32 state = (Sint32)FieldStateWork;
	Uint32 actorOffset = (actorId * 4) + offsetof(FIELD_STATE, actors);

	Sint32			 src = (Sint32)*(ACTOR **)(state + actorOffset);
	POLAR_OFFSET_POS picturePos;
	FIXED			 pictureX;

	if(src == 0)
		return 0;

	picturePos.vector.x = ((ACTOR *)src)->x.fixed;
	picturePos.vector.y = ((ACTOR *)src)->y.fixed;
	picturePos.vector.z = ((ACTOR *)src)->z.fixed;
	syVectorAddPolarOffset(
		toFIXED(16), ((ACTOR *)src)->facing, picturePos.coordinates);
	pictureX = (picturePos.vector.x & FIELD_METATILE_MASK) + toFIXED(8);
	/* source now holds the metatile's z, no longer the actor. */
	src = (picturePos.vector.z & FIELD_METATILE_MASK) + toFIXED(8);

	if(((FIELD_STATE *)state)->revealActive != 0) {
		/* Stop both the marker emitters and the sparkle actors they already
		   made. */
		ACTOR *effect = ActorsWork;

		/* state now counts the actors. */
		for(state = 0; state < ACTOR_COUNT; state++) {
			if(effect->script != 0) {
				if(effect->update == fdRevealEmitSparkles) {
					fdActorReleaseStub(effect);
				}
				if(effect->script == FieldEffectParticleScript) {
					fdActorReleaseStub(effect);
				}
			}
			effect++;
		}
	}

	sySynch(3);
	picture = fdActorCreateStub(SPRITE_BLANK, pictureX, toFIXED(16), src);
	if(picture == 0)
		return 0;

	fdActorStartScriptStub(picture, ItemDisplayGrowInScript);
	{
		SPR	 *spr = picture->spr;
		void *iconBuf;

		/* No shadow, outline or sprite animations: a 16-colour sprite in OBJ
		   palette 0 whose tiles are the item's icon. */
		spr->flg = 0;
		spr->cnt = 0;
		spr->image.fullCol = 0;
		spr->image.pal = 0;
		spr->image.pri = 1;

		picture->velY = toFIXED(2);
		picture->gravity = toFIXED(1) / 4;

		/* Draw the item's icon into HEAP_SLOT_ICON_TRANSFER, then send its
		   tiles to the sprite's. */
		iconBuf =
			syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
		mnIconDrawItemStub(item);

		spr->image.tile = syVRAMCacheLoad(spr->slot, ICON_TILE_BYTES,
			(Uint8 *)iconBuf + offsetof(ICON_TRANSFER, tiles));

		syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
		picture->update = fdItemDispEmitSparkles;
	}

	return picture;
}

/* Releases object unless it is NULL. */
void fdActorReleaseIfPresent(void *object)
{
	if(object != 0)
		fdActorReleaseStub(object);
}

/* Raises a found item's picture to 36 pixels above the party leader, who holds
   it up (ANIM_LEADER_ARMS_RAISED) facing the screen. */
void fdItemDispRaiseAboveLeader(ACTOR *actor)
{
	ACTOR *leader;

	if(actor == 0)
		return;

	leader = fdActorLookup(MainRoutineStateWork.leaderActorId);
	/* Up to 2 pixels a tick, gaining 1 a tick, in free flight: no motion
	   flags, so it neither falls nor follows the ground. */
	actor->acceleration = toFIXED(1);
	actor->speedLimit = toFIXED(2);
	actor->motionFlg = 0;
	fdActorSetMoveTargetStub(
		actor, leader->x.fixed, leader->y.fixed + toFIXED(36), leader->z.fixed);
	sySynch(3);
	fdActorSelectAnimationStub(leader, ANIM_LEADER_ARMS_RAISED);
	fdActorStartScriptStub(actor, ItemDisplayPulseScript);
	/* FACING_SOUTH, a quarter turn, faces the screen. */
	leader->facing = FACING_SOUTH;
}

/* Shows a found item's picture, with sparkles (ITEM_DISPLAY_SPARKLES) or held
   up by the leader (ITEM_DISPLAY_RAISED); with both it sparkles for 60 ticks
   before he raises it, and he holds it 80. */
void fdItemDispShowPicture(void *object, Sint32 flg)
{
	void *leader;

	if(object != NULL) {
		leader = fdActorLookup(MainRoutineStateWork.leaderActorId);
		if(flg & ITEM_DISPLAY_SPARKLES) {
			fdActorSetSprFlagsStub(object, 0);
			fdActorStartScriptStub(object, ItemDisplayGrowInScript);
			((ACTOR *)object)->velY = toFIXED(2);
			((ACTOR *)object)->gravity = toFIXED(1) / 4;
			((ACTOR *)object)->update = fdItemDispEmitSparkles;
		}
		if(flg == (ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED)) {
			sySynch(60);
		}
		if(flg & ITEM_DISPLAY_RAISED) {
			fdItemDispRaiseAboveLeader(object);
		}
		if(flg == (ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED)) {
			sySynch(80);
		}
		fdActorSelectAnimationStub(leader, ANIM_STAND);
	}
}

/* Shows the picture of item 36 pixels above the party leader for 80 ticks,
   with sparkles (ITEM_DISPLAY_SPARKLES) or held up as a found item
   (ITEM_DISPLAY_RAISED). */
void fdItemDispShowItem(Sint32 item, Sint32 flg)
{
	ACTOR *leader = fdActorLookup(MainRoutineStateWork.leaderActorId);
	void  *iconBuf =
		syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	ACTOR *actor = fdActorCreateStub(SPRITE_BLANK, leader->x.fixed,
		leader->y.fixed + toFIXED(36), leader->z.fixed);

	if(actor != 0) {
		SPR *spr = actor->spr;

		spr->flg = 0;
		spr->cnt = 0;
		spr->image.fullCol = 0;
		spr->image.pal = 0;
		spr->image.pri = 1;

		mnIconDrawItemStub(item);
		syVRAMCacheLoad(spr->slot, ICON_TILE_BYTES,
			(Uint8 *)iconBuf + offsetof(ICON_TRANSFER, tiles));
		syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);

		if(flg & ITEM_DISPLAY_SPARKLES)
			actor->update = fdItemDispEmitSparkles;
		if(flg & ITEM_DISPLAY_RAISED)
			fdItemDispRaiseAboveLeader(actor);

		sySynch(80);
		fdActorSelectAnimationStub(leader, ANIM_STAND);
		fdActorReleaseStub(actor);
	}
}

/* The German, Spanish, French and Italian editions stop Reveal's sparkles while
   a menu is open. */
#if FIELD_MENU_STOPS_REVEAL_SPARKLES

/* While Reveal is active, releases all its sparkles, leaving its marks. */
void fdEffectReleaseRevealSparkles(void)
{
	if(FieldStateWork->revealActive != 0) {
		ACTOR *effect = ActorsWork;
		Sint32 i;

		for(i = 0; i < ACTOR_COUNT; i++) {
			if(effect->script != 0) {
				if(effect->script == FieldEffectParticleScript)
					fdActorReleaseStub(effect);
			}
			effect++;
		}
	}
}

#endif

/* Each tick of a Reveal marker: one time in ten, a sparkle appears within 16
   pixels of it. */
void fdRevealEmitSparkles(ACTOR *actor)
{
	const ACTOR		*src = (const ACTOR *)actor;
	POLAR_OFFSET_POS pos;
	ACTOR			*object;
	Uint32			 randomRadius;

#if FIELD_MENU_STOPS_REVEAL_SPARKLES
	if(FieldStateWork->menuOpen != 0)
		return;
#endif
	if((100 * syRandom() >> 16) > 9)
		return;

	pos.vector.x = src->x.fixed;
	pos.vector.y = src->y.fixed;
	pos.vector.z = src->z.fixed;
	randomRadius = syRandom();
	syVectorAddPolarOffset(randomRadius << 4, syRandom(), pos.coordinates);
	object =
		fdActorSpawn(SPRITE_SPARKLE, pos.vector.x, pos.vector.y, pos.vector.z);
	fdActorInitSparkle(object);
}

#include "field/is_reveal_active.c"

/* Reveal: marks with sparkles the hidden things within 8 metatiles left or
   right and 5 up or down of the leader: treasure spots (ids 100 to 239) with no
   actor placed, whose examine cue (CUE_EXAMINE) gives an item, coins, a
   Psynergy Stone or a mimic and whose flag is not yet set. */
void fdEffectSpawnRevealMarkers(void)
{
	Uint8	  *markerData;
	ACTOR	  *actor;
	const CUE *entry;
	Sint32	   leaderColumn;
	Sint32	   leaderRow;
	Sint32	   column;
	Sint32	   row;
	Sint32	   id;

	/* The layout's treasure spots: (column, row, id) triples, ended by
	   TREASURE_SPOT_LIST_END twice, as fdPlaceTreasureSpots reads them. */
	markerData = ((MAP_GRID_SCROLL_STATE *)MapGridState[0])->markers;
	actor = fdActorLookup(MainRoutineStateWork.leaderActorId);
	leaderColumn = FIELD_METATILE(actor->x.fixed);
	leaderRow = FIELD_METATILE(actor->z.fixed);
	if(markerData != 0) {
		while(column = *markerData++, row = *markerData++,
			column != TREASURE_SPOT_LIST_END || row != TREASURE_SPOT_LIST_END) {
			id = *markerData++;
			if(fdTreasureSpotGetActor(id) != NULL)
				continue;
			if(id < TREASURE_SPOT_FIRST_ID || id > TREASURE_SPOT_LAST_ID)
				continue;
			if(leaderColumn - column >= 0) {
				if(leaderColumn - column > 8)
					continue;
			} else if(column - leaderColumn > 8)
				continue;
			if(leaderRow - row >= 0) {
				if(leaderRow - row > 5)
					continue;
			} else if(row - leaderRow > 5)
				continue;
			entry = OverlayArea.cues();
			if(entry->kind == -1)
				continue;
			for(; entry->kind != -1; entry++) {
				if(entry->subject == id &&
					(entry->kind & CUE_KIND_MASK) == CUE_EXAMINE) {
					switch(entry->value & FIND_TYPE_MASK) {
					case 0:
					case FIND_ITEM(0):
					case FIND_COINS(0):
					case FIND_PSYNERGY_STONE:
					case FIND_MIMIC:
						if(entry->condition != -1 &&
							gmFlagGetStub(entry->condition) == 0) {
							/* actor now holds the spawned marker. */
							actor = fdActorCreateStub(SPRITE_BLANK,
								column * toFIXED(MAPGRID_METATILE_PIXELS) +
									toFIXED(8),
								0,
								row * toFIXED(MAPGRID_METATILE_PIXELS) +
									toFIXED(8));
							if(actor != 0) {
								fdActorStartScriptStub(
									actor, RevealMarkerScript);
								fdActorSetSprFlagsStub(actor, 0);
								actor->update = fdRevealEmitSparkles;
							}
						}
						break;
					}
				}
			}
		}
	}
}
