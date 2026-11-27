/* Actors: a sprite dissolving pixel by pixel, the actor system, creating and
   releasing actors, their animations and scripts, and placing their sprites
   each tick under the scrolling or the fixed camera. Part of field.c. */

#include "field/actor_script.h"
#include "field/map_grid.h"
#include "field/script_operands.h"
#include "graphics/glyph.h"

/* The ticks fdSprWaitForTrackEnd waits at most. */
#define TRACK_END_WAIT_TICKS 90
/* The longest an actor's script or move is waited for: 10 seconds. */
#define ACTOR_WAIT_TICKS_MAX 600

/* Waits, at most TRACK_END_WAIT_TICKS, for a sprite's animation to change or
   for its first layer's script to reach ANIMCMD_HOLD. */
void fdSprWaitForTrackEnd(SPR *spr)
{
	Uint8		   anm = spr->lastNo;
	SPR_ANIMATION *entry = spr->entries[0];
	Sint32		   i;

	if(spr != NULL) {
		i = 0;
		if(entry->script[entry->scriptOfs] != ANIMCMD_HOLD) {
			do {
				sySynch(1);
				i++;
				if(i > TRACK_END_WAIT_TICKS - 1)
					break;
				if(entry->script[entry->scriptOfs] == ANIMCMD_HOLD)
					break;
			} while(spr->lastNo == anm);
		}
	}
}

/* How much further along the pixel order each tile reads than the tile before,
   besides the step it is ahead. */
#define DISSOLVE_ORDER_STRIDE 16
/* Keeps a pixel pair's odd pixel, its high byte, clearing the even one. */
#define PIXEL_PAIR_ODD_MASK	  0xff00

/* One step of dissolving a sprite pixel by pixel. */
void fdSprDissolveStep(Uint8 *obj, Uint32 pos)
{
	Uint16		*tile;
	Sint32		 tileCnt;
	Uint32		 i;
	const Uint8 *order;

	SPR *spr = (SPR *)obj;

	tile = (Uint16 *)(OBJ_VRAM0 + VramCacheEntries[spr->slot].ofs);
	tileCnt = (spr->width * spr->height) / DISSOLVE_TILE_PIXELS;
	i = 0;

	if(i < tileCnt) {
		order = SpriteDissolvePixelOrder;
		{
			Uint32 highMask = PIXEL_PAIR_ODD_MASK;
			Uint32 idxMask = DISSOLVE_TILE_PIXELS - 1;

			for(; i < tileCnt; i++, tile += TILE_SIZE_8BPP / 2, pos++) {
				if((pos - DISSOLVE_FIRST_STEP) <= DISSOLVE_TILE_PIXELS - 1) {
					Uint8	pixel;
					Uint16 *pixelPair;
					Uint32	isOdd;

					/* Each tile reads the order 17 entries on from the tile
					   before, so neighbouring tiles lose different pixels at
					   the same step. */
					pixel = order[(pos + i * DISSOLVE_ORDER_STRIDE) & idxMask];
					/* The halfword holding the pixel: its offset rounded down
					   to even. */
					pixelPair = (Uint16 *)((Uint8 *)tile +
						(pixel & (DISSOLVE_TILE_PIXELS - 2)));
					isOdd = 1;
					isOdd &= pixel;
					if(isOdd != 0)
						*pixelPair = *(Uint8 *)pixelPair;
					else
						*pixelPair &= highMask;
				}
			}
		}
	}
}

#include "field/spr_dissolve.c"

/* Writes the actor's screen position, in pixels, to screenPos[0..1] and
   returns 0 when it is on or near the screen; otherwise writes 0, 0 and returns
   -1. */
Sint32 fdMapGridGetScreenRelativePos(ACTOR *actor, Sint32 *screenPos)
{
	MAP_STATE *map = RamHeapSlots->mapGrid;
	Sint32	   viewX;
	Sint32	   viewZ;
	FIXED	   x;
	FIXED	   y;

	viewX = syMapStateView(map)[0] & FIELD_PIXEL_MASK;
	viewZ = syMapStateView(map)[1] & FIELD_PIXEL_MASK;
	x = actor->x.fixed - viewX;
	y = actor->z.fixed - viewZ;
	/* Across, from 32 pixels left of the screen to 32 past its 240; down, below
	   the view's top and above 224. */
	if((x >= -(toFIXED(32) - 1) && x <= toFIXED(272) - 1) && y > 0 &&
		y < toFIXED(224)) {
		*screenPos++ = FIXEDtoINT(x);
		*screenPos = FIXEDtoINT(y);
		return 0;
	}
	*screenPos++ = 0;
	*screenPos = 0;
	return -1;
}

/* The colour fdGridDrawOutlineArm draws an outlined sprite's edge in: the basic
   sprite palette's white, which SPRITE_COLOR_WHITE paints with too. */
#define OUTLINE_EDGE_COLOR 15

/* Sets up the actor system: allocates and zeroes the 64 actors and the system's
   state, sets the outline's colours and registers the per-tick updates (mode
   is an ACTOR_SYSTEM_MODE). A battle moves its actors with
   fdActorUpdateAllMotion; the World Map and a battle place their sprites with
   the fixed camera. */
void fdActorSystemInit(Sint32 mode)
{
	ACTOR_SYSTEM_STATE *system;
	ACTOR			   *pool;
	/* The fill word DMA_FILL32_CELL reads its zero from, one for both fills. */
	volatile Uint32 zero;

	system = syHeapSlotAllocEWRAM(HEAP_SLOT_ACTOR_SYSTEM, sizeof(*system));
	pool = syHeapSlotAllocEWRAM(HEAP_SLOT_ACTORS, ACTOR_COUNT * sizeof(*pool));
	fdActorSystemConfigure(mode);
	DMA_FILL32_CELL(zero, 0, pool, ACTOR_COUNT * sizeof(*pool));
	DMA_FILL32_CELL(zero, 0, system, sizeof(*system));
	if(mode == ACTOR_SYSTEM_BATTLE) {
		sySetEvent(fdActorUpdateAllMotion, EVENT_PASS_TICK + 10);
	} else {
		sySetEvent(fdActorUpdateAllOnField, EVENT_PASS_TICK + 11);
		sySetEvent(fdActorMoveWithCarriers, EVENT_PASS_TICK + 10);
	}
	if(mode == ACTOR_SYSTEM_WORLD_MAP || mode == ACTOR_SYSTEM_BATTLE)
		sySetEvent(fdActorSystemProjectSprs, EVENT_PASS_TICK);
	else
		sySetEvent(fdActorSystemPlaceSprs, EVENT_PASS_TICK);
	system->edge = OUTLINE_EDGE_COLOR;
	system->fill = 0;
}

#include "field/actor_system_reserved.c"

/* Creating and releasing actors, and what they show. */

/* Returns an unused actor (one without a script) from the 64 slots, or 0 if
   none is free. */
void *fdActorFindFree(void)
{
	ACTOR *actor = ((ACTOR *)RamHeapSlots->actors);
	void  *freeActor = 0;
	Sint32 actorIdx = 0;

	while(actorIdx <= ACTOR_COUNT - 1) {
		if(actor->script == 0) {
			freeActor = actor;
			break;
		}
		actorIdx++;
		actor++;
	}
	return freeActor;
}

#include "field/actor_release.c"

/* Creates an actor at (x, y, z), in 16.16 pixels, or returns 0 when every slot
   is in use. encodedSpr / 4096 is the kind of actor (enum ACTOR_CREATE) and
   its low 12 bits the sprite's animation id. */
ACTOR *fdActorCreate(Sint32 encodedSpr, FIXED x, FIXED y, FIXED z)
{
	ACTOR		   *actor;
	void		   *spr;
	Sint32			sprStorageKind;
	void		  **sprSlots;
	volatile Uint32 zero;

	sprStorageKind = encodedSpr / (ACTOR_CREATE_SPRITE_MASK + 1);
	encodedSpr &= ACTOR_CREATE_SPRITE_MASK;
	actor = fdActorFindFree();
	if(actor != NULL) {
		actor->radius = 16;
		switch(sprStorageKind) {
		case ACTOR_CREATE_ONE_SPRITE:
			spr = fdSprCreate(encodedSpr);
			if(spr != NULL) {
				actor->sprStorageKind = ACTOR_SPRITE_ONE;
				actor->spr = spr;
				actor->radius =
					fdResourceGetMetadataRecordStub(encodedSpr)->footprint >> 1;
			} else {
				actor->sprStorageKind = ACTOR_SPRITE_NONE;
			}
			break;
		case ACTOR_CREATE_GROUP:
			/* Take a zeroed list of four sprites, and fill it with id and id +
			   1. */
			sprSlots = (void **)((Uint32 *)RamHeapSlots->actorSystem +
				RamHeapSlots->actorSystem->cnt++ + 2);
			actor->sprStorageKind = sprStorageKind;
			actor->spr = sprSlots;
			zero = 0;
			syDMAXCopy(&zero, sprSlots, DMA_FILL_WORDS(4), REG_DMA3);
			spr = fdSprCreate(encodedSpr);
			if(spr != NULL) {
				actor->radius =
					fdResourceGetMetadataRecordStub(encodedSpr)->footprint >> 1;
				*sprSlots = spr;
				sprSlots++;
			}
			spr = fdSprCreate(encodedSpr + 1);
			if(spr != NULL)
				*sprSlots = spr;
			break;
		}
	}
	if(actor != NULL) {
		fdActorSetPosAndResetMotion(actor, x, y, z);
		actor->script = (Sint32 *)ActorScriptEndScript;
		actor->speedLimit = toFIXED(2);
		actor->scriptCursor = 0;
		actor->scaleX = toFIXED(1);
		actor->scaleY = toFIXED(1);
		actor->acceleration = toFIXED(1);
		actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		actor->gravity = toFIXED(1);
		actor->bounce = toFIXED(1) / 4;
		actor->collisionFlg = 0;
		actor->steeringFlg = ACTOR_FACES_MOTION;
		actor->facing = DEGtoANG(90);
		actor->sceneWord = 0;
		actor->homeX = x / toFIXED(1);
		actor->homeZ = z / toFIXED(1);
		actor->carrier = NULL;
		actor->stepX = 0;
		actor->stepY = 0;
		actor->stepZ = 0;
	}
	return actor;
}

#include "field/actor_start_script.c"

/* Starts animation argument on an actor's sprite, or on each of the four
   sprites of a group (fdSprStartAnimation). */
void fdActorSelectAnimation(ACTOR *actor, Sint32 argument)
{
	SPR	 **items;
	SPR	  *item;
	Sint32 cnt;

	if(actor != 0) {
		switch(actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) {
		case ACTOR_SPRITE_ONE:
			fdSprStartAnimation(((SPR *)actor->spr), argument);
			break;
		case ACTOR_SPRITE_GROUP:
			items = ((SPR **)actor->spr);
			for(cnt = 0; cnt < 4; cnt++) {
				item = *items++;
				if(item != 0)
					fdSprStartAnimation(item, argument);
			}
			break;
		}
	}
}

/* Sets the timer step, the animation speed, of an actor's sprite, or of each
   sprite of a group (ANIM_STEP_NORMAL is normal speed). */
void fdActorSetAnimationSpeed(ACTOR *actor, Sint32 value)
{
	Sint32 cnt;
	SPR	  *child;
	SPR	 **children;

	if(actor != 0) {
		switch(actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) {
		case ACTOR_SPRITE_ONE:
			fdSprSetAnimationSpeed(((SPR *)actor->spr), value);
			return;
		case ACTOR_SPRITE_GROUP:
			children = ((SPR **)actor->spr);
			for(cnt = 0; cnt < 4; cnt++) {
				child = *children++;
				if(child != 0)
					fdSprSetAnimationSpeed(child, value);
			}
			break;
		}
	}
}

#include "field/actor_set_animation_and_speed.c"

/* Sets the rotation of an actor's sprite, when it has just one. */
void fdActorSetSprRotation(ACTOR *actor, Uint32 value)
{
	if(actor != 0 &&
		(actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE)
		((SPR *)actor->spr)->rotation = value;
}

/* Gives a one-sprite actor's first sprite animation animation id anmId
   and restarts its animations. A negative anmId does nothing. */
void fdActorSetFirstSprAnimation(ACTOR *actor, Sint32 anmId)
{
	SPR *child;

	if(actor != NULL &&
		(actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE) {
		child = ((SPR *)actor->spr);
		if(anmId >= 0) {
			child->entries[0]->anmId = anmId;
			fdSprResetAnimations(child);
		}
	}
}

#include "field/actor_register_spr_metadata.c"

/* Starts an actor on the script that approaches its watched actor
   (SC_APPROACH_WATCHED), watching watchedActorHandle. */
void fdActorStartApproachWatched(ACTOR *actor, Sint32 watchedActorHandle)
{
	fdActorStartScript(actor, ActorApproachWatchedScript);
	actor->watchedActor = (union FIELD_OBJECT *)watchedActorHandle;
}

/* Starts the leader's control script for the way the party moves now
   (MainRoutineStateWork.movementMode), waiting a tick first for Sand on the
   World Map. */
void fdActorStartMovementModeScript(ACTOR *actor)
{
	const Sint32	   *script;
	MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;

	switch(party->movementMode) {
	case LEADER_MOVEMENT_WALK:
		script = ActorPlayerScript;
		break;
	case LEADER_MOVEMENT_WORLD_MAP:
		script = ActorWorldMapScript;
		break;
	case LEADER_MOVEMENT_KEYS:
		script = ActorKeysScript;
		break;
	case LEADER_MOVEMENT_ROPE:
		script = ActorRopeScript;
		break;
	case LEADER_MOVEMENT_TIGHTROPE:
		script = ActorTightropeScript;
		break;
	case LEADER_MOVEMENT_SAND:
		script = ActorSandScript;
		break;
	case LEADER_MOVEMENT_SHIP_ON_MAP:
		script = ActorSailOnMapScript;
		break;
	case LEADER_MOVEMENT_SHIP:
		script = ActorSailScript;
		break;
	case LEADER_MOVEMENT_FLYING_SHIP:
		script = ActorFlyScript;
		break;
	case LEADER_MOVEMENT_WORLD_SAND:
		sySynch(1);
		script = ActorSandOnWorldMapScript;
		break;
	case LEADER_MOVEMENT_HOVER:
		script = ActorHoverScript;
		break;
	case LEADER_MOVEMENT_WALKING:
		script = ActorWalkingScript;
		break;
	case LEADER_MOVEMENT_NONE:
		script = ActorScriptEndScript;
		break;
	default:
		script = ActorKeyCheckScript;
		break;
	}
	fdActorStartScript(actor, script);
}

/* Starts an actor on the idle script (a lone SC_END). */
void fdActorStartIdle(ACTOR *actor)
{
	fdActorStartScript(actor, ActorScriptEndScript);
}

/* Starts an actor on the camera script (SC_CAMERA). With a leader to follow, it
   also gets a speed limit of 4 pixels a tick and an acceleration of 0.5. */
void fdActorStartCamera(ACTOR *actor, Sint32 watchedActorHandle)
{
	fdActorStartScript(actor, ActorCameraScript);
	if(watchedActorHandle != 0) {
		actor->acceleration = toFIXED(1) / 2;
		actor->speedLimit = toFIXED(4);
		actor->watchedActor = (union FIELD_OBJECT *)watchedActorHandle;
		actor->tmr = 0;
	}
}

/* Waits until the actor's script reaches SC_END, at most ACTOR_WAIT_TICKS_MAX
   ticks; it returns as soon as the script idles, and after the limit whether
   or not it has. */
void fdActorWaitForScriptEnd(ACTOR *actor)
{
	Sint32 waitedTicks = 0;

	if(actor->script != 0 &&
		((const Sint32 *)actor->script)[actor->scriptCursor] != SC_END) {
		do {
			sySynch(1);
			waitedTicks++;
			if(waitedTicks >= ACTOR_WAIT_TICKS_MAX)
				break;
		} while(actor->script != 0 &&
			((const Sint32 *)actor->script)[actor->scriptCursor] != SC_END);
	}
}

/* Sets the flags of a one-sprite actor's sprite (SPR_FLAG). */
void fdActorSetSprFlags(ACTOR *actor, Uint32 value)
{
	if(actor != NULL &&
		(actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE)
		((SPR *)actor->spr)->flg = value;
}

#include "field/actor_set_blend_mode.c"

/* Sets a one-sprite actor's sprite's own-scale bit: drawn at its own scale, not
   scaled by distance. */
void fdActorSetOwnScale(ACTOR *actor, Uint32 value)
{
	if(actor != NULL && actor->sprStorageKind == ACTOR_SPRITE_ONE)
		((SPR *)actor->spr)->useOwnScale = value;
}

/* Sets the colour mode (SPR_COL_MODE) of a one-sprite actor's sprite
   animations, except those drawn hidden. */
void fdActorSetSprColMode(ACTOR *actor, Uint32 value)
{
	if(actor != NULL && actor->sprStorageKind == ACTOR_SPRITE_ONE)
		fdSprSetAnimationColModes(((SPR *)actor->spr), value);
}

/* Fixes the priority of a one-sprite actor's sprite and its shadow, which then
   no longer follow the metatiles under it (ACTOR_PRIORITY_AUTOMATIC). */
void fdActorSetSprPriorityDirect(ACTOR *actor, Sint32 pri)
{
	SPR *spr;

	if(actor == NULL || actor->sprStorageKind != ACTOR_SPRITE_ONE) {
		return;
	}
	spr = actor->spr;
	spr->image.pri = pri;
	spr->shadow.pri = pri;
	actor->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
}

/* Shows item id's icon on its owner's sprite, 16 pixels square at scale 1.0 in
   16-colour palette 0. */
void fdActorSetItemIcon(ACTOR *owner, Sint32 id)
{
	SPR			  *spr;
	ICON_TRANSFER *transfer;
	Sint32		   tile;

	if(owner == 0)
		return;
	owner->retainSprCache = ACTOR_KEEP_SPRITE_VRAM;
	owner->iconItem = id;
	spr = owner->spr;
	if(spr == 0)
		return;
	transfer = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	mnIconDrawItemStub(id);
	tile = syVRAMCacheLoad(spr->slot, ICON_TILE_BYTES, transfer->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	spr->cnt = 0;
	spr->dirty = 0;
	spr->width = 16;
	spr->height = 16;
	spr->ofsX = 0;
	spr->ofsY = 4;
	spr->scale = toFIXED(1);
	spr->image.tile = tile;
	spr->image.fullCol = 0;
	spr->image.pal = 0;
}

/* The sprite id an actor is drawn with (the first halfword of its sprite's
   definition), or 0 for none. */
Sint32 fdActorGetSprId(ACTOR *actor)
{
	SPR *spr = actor->spr;

	if(actor != NULL && spr != NULL && spr->entries != NULL)
		return spr->entries[0]->anmId;
	return 0;
}

/* fdSprWaitForTrackEnd on an actor's sprite. */
void fdActorWaitForTrackEnd(ACTOR *actor)
{
	if(actor != NULL)
		fdSprWaitForTrackEnd(actor->spr);
}

/* Suspends the actor camera updates, starts the palette back to normal and, a
   tick later, shows the sprites with BG1-3 hidden, leaving BG0 unchanged. */
void fdActorSystemSuspendProjectionAndShowSprs(void)
{
	Uint16 dispcnt;

	sySuspendEvent(fdActorSystemPlaceSprs);
	sySuspendEvent(fdActorSystemProjectSprs);
	fdPaletteFadeSetTargetStub(toFIXED(1), 1);
	fdPaletteFadeStartInterpolationStub(1);
	sySynch(1);
	dispcnt = REG_DISPCNT;
	REG_DISPCNT =
		(dispcnt &
			(Uint16)~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON)) |
		DISPCNT_OBJ_ON;
}

#include "field/actor_system_resume_projection.c"

/* The sprite priority an actor takes on the metatile under a point. */
#define METATILE_PRIORITY(metatiles, x, z) \
	((metatiles)[(FIELD_METATILE(z) << 7) + FIELD_METATILE(x)].actorPri)
/* The map layer an actor takes on the metatile under a point, plus one. */
#define METATILE_LAYER(metatiles, x, z) \
	((metatiles)[(FIELD_METATILE(z) << 7) + FIELD_METATILE(x)].actorLayer)

/* Each tick, with the scrolling camera: places every actor's sprite on the
   screen. */
void fdActorSystemPlaceSprs(void)
{
	/* Unused: gives the stack frame its 36 bytes, as The Broken Seal's. */
	ACTOR_CAMERA_POINT	unusedPoints[3];
	SPR_PLACEMENT		point;
	SPR_SCALE			scale;
	Sint32				objectIdx;
	MAP_STATE		   *map;
	Sint32				viewX;
	Sint32				viewZ;
	ACTOR_SYSTEM_STATE *system;
	ACTOR			   *actor;
	Uint32				kind;
	FIXED				dx;
	FIXED				dz;
	Sint32				originY;
	SPR				   *spr;
	FIXED				screenTop;
	Uint8			   *priFlg;
	Sint32				layer;
	Uint8			   *terrainId;
	MAP_METATILE	   *metatiles;
	FIXED				feetZ;
	Sint32				objectHeight;
	FIXED				probeX;
	Sint32				pri;
	Sint32				leftPri;
	Sint32				rightPri;
	Sint32				middlePri;
	Sint32				height;
	Sint32				rise;
	Uint32				layerPlusOne;
	Sint8			   *basePri;

	map = RamHeapSlots->mapGrid;
	viewX = syMapStateView(map)[0] & FIELD_PIXEL_MASK;
	viewZ = syMapStateView(map)[1] & FIELD_PIXEL_MASK;
	system = RamHeapSlots->actorSystem;
	originY = map->origin[1];
	btRenderLoadDecoder();
	system->used = 0;
	actor = RamHeapSlots->actors;
	for(objectIdx = 0; objectIdx < ACTOR_COUNT; objectIdx++, actor++) {
		if(actor->script == NULL)
			continue;
		if(actor->x.fixed != 0 || actor->z.fixed != 0) {
			kind = actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK;
			if(kind == ACTOR_SPRITE_NONE)
				continue;
			if(kind != ACTOR_SPRITE_ONE)
				continue;
			if(system->suspended != 0 &&
				!(actor->retainSprCache & ACTOR_KEEP_SPRITE_VRAM)) {
				SPR *frozen = actor->spr;

				syVRAMCacheReleaseVRAM(frozen->slot);
				frozen->dirty = kind;
				continue;
			}
			spr = actor->spr;
			dx = actor->x.fixed - viewX;
			dz = actor->z.fixed - viewZ;
			screenTop = dz - actor->y.fixed;
			if(dx > -toFIXED(32) && dx < toFIXED(272) &&
				screenTop > -toFIXED(32) && screenTop < toFIXED(224) &&
				/* The ROM keeps &actor->priFlg in a stack slot of its own,
				   ordered among the declared locals' slots and above the slot
				   the loop body's first statements create. Reload orders slots
				   by pseudo number, so the source declared a local holding this
				   address; its type, name and spelling here are not known. */
				(priFlg = &actor->priFlg,
					!(*priFlg & ACTOR_PRIORITY_NOT_DRAWN))) {
				/* &actor->layer in a declared local, by the same stack-slot
				   evidence as priFlg above; its type and name are not
				   known. */
				terrainId = &actor->layer;
				layer = *terrainId & 3;
				metatiles = map->layers[layer].metatiles;
				/* The sprite takes the priority of the metatiles under its
				   feet: 7 pixels left of its middle and 6 right where the
				   ground is level with its own, and its middle. With none, it
				   takes the map's base priority, or one further back where the
				   ground a metatile in front, or two metatiles in front less a
				   metatile's height, rises above its own; with
				   ACTOR_PRIORITY_IGNORES_RISES, it keeps its own. */
				if(*priFlg & ACTOR_PRIORITY_AUTOMATIC) {
					probeX = actor->x.fixed - toFIXED(7);
					feetZ = actor->z.fixed + toFIXED(2);
					objectHeight = FIXEDtoINT(actor->groundHeight);
					pri = 0;
					leftPri = 0;
					rightPri = 0;
					if(!(*priFlg & ACTOR_PRIORITY_IGNORES_RISES)) {
						/* &map->basePri in a declared local, by the same
						   evidence:
						   set inside this arm, yet numbered before the loop
						   body's first statements, so declared at function
						   level or at the loop body's head. Its type and name
						   are not known. */
						basePri = &map->basePri;
						pri = *basePri;
						if(fdMapGridGetTerrainHeight(layer, probeX, feetZ) ==
							actor->groundHeight) {
							leftPri =
								METATILE_PRIORITY(metatiles, probeX, feetZ);
							height = FIXEDtoINT(
								fdMapGridGetTerrainHeight(layer, probeX,
									feetZ + toFIXED(MAPGRID_METATILE_PIXELS)));
							rise =
								FIXEDtoINT(fdMapGridGetTerrainHeight(layer,
									probeX,
									feetZ +
										toFIXED(2 * MAPGRID_METATILE_PIXELS))) -
								MAPGRID_METATILE_PIXELS;
							if(rise > height)
								height = rise;
							if(height > objectHeight)
								pri = map->basePri + 1;
						}
						probeX += toFIXED(13);
						if(fdMapGridGetTerrainHeight(layer, probeX, feetZ) ==
							actor->groundHeight) {
							rightPri =
								METATILE_PRIORITY(metatiles, probeX, feetZ);
							height = FIXEDtoINT(
								fdMapGridGetTerrainHeight(layer, probeX,
									feetZ + toFIXED(MAPGRID_METATILE_PIXELS)));
							rise =
								FIXEDtoINT(fdMapGridGetTerrainHeight(layer,
									probeX,
									feetZ +
										toFIXED(2 * MAPGRID_METATILE_PIXELS))) -
								MAPGRID_METATILE_PIXELS;
							if(rise > height)
								height = rise;
							if(height > objectHeight)
								pri = map->basePri + 1;
						}
						probeX -= toFIXED(6);
						middlePri = METATILE_PRIORITY(metatiles, probeX, feetZ);
						height = FIXEDtoINT(fdMapGridGetTerrainHeight(layer,
							probeX, feetZ + toFIXED(MAPGRID_METATILE_PIXELS)));
						rise =
							FIXEDtoINT(fdMapGridGetTerrainHeight(layer, probeX,
								feetZ + toFIXED(2 * MAPGRID_METATILE_PIXELS))) -
							MAPGRID_METATILE_PIXELS;
						if(rise > height)
							height = rise;
						if(height > objectHeight)
							pri = map->basePri + 1;
					} else {
						if(actor->groundHeight ==
							fdMapGridGetTerrainHeight(layer, probeX, feetZ))
							leftPri =
								METATILE_PRIORITY(metatiles, probeX, feetZ);
						probeX += toFIXED(13);
						if(actor->groundHeight ==
							fdMapGridGetTerrainHeight(layer, probeX, feetZ))
							rightPri =
								METATILE_PRIORITY(metatiles, probeX, feetZ);
						probeX -= toFIXED(6);
						middlePri = METATILE_PRIORITY(metatiles, probeX, feetZ);
					}
					/* An odd priority is preferred: the left's, the right's,
					   the middle's. */
					if(leftPri != 0 || rightPri != 0 || middlePri != 0) {
						pri = leftPri;
						if(!(pri & 1)) {
							if(rightPri != 0)
								pri = rightPri;
							if(!(pri & 1) && middlePri != 0)
								pri = middlePri;
						}
					}
					if(pri != 0 && actor->sprStorageKind == ACTOR_SPRITE_ONE) {
						SPR *placed = actor->spr;

						if((Uint32)pri > 3)
							pri = 3;
						placed->image.pri = pri;
						placed->shadow.pri = pri;
					}
				}
				layerPlusOne =
					METATILE_LAYER(metatiles, actor->x.fixed, actor->z.fixed);
				if(layerPlusOne != 0)
					*terrainId = layerPlusOne - 1;
				scale.x = syMulFX(actor->scaleX, spr->scale);
				scale.y = syMulFX(actor->scaleY, spr->scale);
				point.x = dx;
				point.height = actor->y.fixed - originY;
				point.y = dz - originY;
				point.groundHeight = actor->groundHeight - originY;
				if(*priFlg & ACTOR_PRIORITY_UNDERFOOT) {
					point.height -= toFIXED(320);
					point.y -= toFIXED(320);
					point.groundHeight -= toFIXED(320);
				}
				if(*priFlg & ACTOR_PRIORITY_OVERHEAD) {
					point.height += toFIXED(320);
					point.y += toFIXED(320);
					point.groundHeight += toFIXED(320);
				}
				if(*priFlg & ACTOR_PRIORITY_OWN_DEPTH)
					fdRenderApplyProjectedPlacementAtDepth(spr, &point,
						scale.values, actor->facing, actor->sceneByte);
				else
					fdRenderApplyProjectedPlacement(spr,
						sySprPlacementValues(&point), scale.values,
						actor->facing);
				continue;
			}
			if(actor->retainSprCache & ACTOR_COMPOSE_OFF_SCREEN) {
				fdSprComposePicture(spr, actor->facing);
			} else if(!(actor->retainSprCache & ACTOR_KEEP_SPRITE_VRAM) &&
				!spr->retainCacheEntry) {
				syVRAMCacheReleaseVRAM(spr->slot);
				spr->dirty = 1;
			}
		} else {
			/* At (0, 0): not placed; only the sprite's VRAM is given up. */
			kind = actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK;
			if(kind == ACTOR_SPRITE_ONE) {
				SPR *frozen = actor->spr;

				if(actor->retainSprCache == 0 && !frozen->retainCacheEntry) {
					syVRAMCacheReleaseVRAM(frozen->slot);
					frozen->dirty = kind;
				}
			}
		}
	}
	syHeapSlotRelease(HEAP_SLOT_TICK_SCRATCH);
}

/* The fixed camera, on the World Map and in battle: each tick it places every
   actor's sprites, facing them by the angle from the eye to the target. */

#include "field/actor_camera_return_true.c"

/* Places an actor's sprite (unless ACTOR_PRIORITY_NOT_DRAWN hides it) or each
   of its group's four sprites, turned by angle. While battle commands are
   chosen (FLAG_CHOOSING_COMMANDS) the sprites are drawn at 1.0 scale. */
void fdActorPlaceSprs(ACTOR *actor, Sint32 ang)
{
	const Sint32 *scale = &actor->scaleX;
	Sint32		 *point;
	void		 *spr;
	void		**sprs;
	Sint32		  remaining;

	if(syGameFlagGet(FLAG_CHOOSING_COMMANDS))
		scale = ActorSpriteFullScale;
	switch(actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) {
	case ACTOR_SPRITE_ONE:
		point = &actor->x.fixed;
		spr = actor->spr;
		if(!(actor->priFlg & ACTOR_PRIORITY_NOT_DRAWN))
			fdRenderPlaceProjectedSpr(
				spr, point, scale, actor->facing + ang, actor->layer);
		break;
	case ACTOR_SPRITE_GROUP:
		sprs = actor->spr;
		for(remaining = 0; remaining < 4; remaining++) {
			spr = *sprs++;
			if(spr != NULL)
				fdRenderPlaceProjectedSpr(spr, &actor->x.fixed, scale,
					actor->facing + ang, actor->layer);
		}
		break;
	case ACTOR_SPRITE_NONE:
		break;
	}
}

/* Each tick, with the fixed camera: places every actor in use, from the 64th
   down. While battle commands are chosen (FLAG_CHOOSING_COMMANDS) it uses the
   fixed view matrix and turns the objects by an eighth of a turn less. */
void fdActorSystemProjectSprs(void)
{
	ACTOR_CAMERA_FIXED *camera = RamHeapSlots->cameraState;
	ACTOR_SYSTEM_STATE *system = RamHeapSlots->actorSystem;
	ACTOR_CAMERA_POINT *eye;
	ACTOR_CAMERA_POINT *tgt;
	ACTOR			   *actor;
	Sint32				ang;
	Sint32				i;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	ACTOR_CAMERA_POINT unused;

	btRenderLoadDecoder();
	eye = &camera->eye;
	tgt = &camera->tgt;
	if(camera->followEye != NULL)
		eye = camera->followEye;
	if(camera->followPos != NULL)
		tgt = camera->followPos;
	ang = (Sint16)syAtan(
		FIXEDtoINT(eye->x - tgt->x), FIXEDtoINT(eye->z - tgt->z));
	system->used = 0;
	if(syGameFlagGet(FLAG_CHOOSING_COMMANDS)) {
		syInitMatrix();
		ang -= DEGtoANG(45);
		syMultiMatrixEntry(CameraFixedViewMatrix);
		syLookAt((Sint32 *)eye, (Sint32 *)tgt);
	} else {
		syMatrixRewindStack();
		syLookAtLoad((Sint32 *)eye, (Sint32 *)tgt);
	}
	actor = RamHeapSlots->actors;
	actor += ACTOR_COUNT - 1;
	for(i = ACTOR_COUNT - 1; i >= 0; i--, actor--) {
		/* The upper half of a script's address in ROM is never zero. */
		if(((Uint16 *)&actor->script)[1] != 0)
			fdActorPlaceSprs(actor, ang);
	}
	syHeapSlotRelease(HEAP_SLOT_TICK_SCRATCH);
}

/* SC_GOTO(script): goes on in script, from its first command, this tick. */
Sint32 fdActorScriptGoto(ACTOR *interpreter)
{
	interpreter->script =
		(const Sint32 *)interpreter->script[interpreter->scriptCursor + 1];
	interpreter->scriptCursor = 0;
	return 1;
}

#include "field/actor_script_spr_visibility.c"

/* Waits, at most ACTOR_WAIT_TICKS_MAX ticks, for an actor to reach where it
   is going. */
void fdActorWaitForMove(ACTOR *actor)
{
	Sint32 waitedTicks;

	for(waitedTicks = 0;
		waitedTicks < ACTOR_WAIT_TICKS_MAX && fdActorIsTargetUnset(actor) == 0;
		waitedTicks++) {
		sySynch(1);
	}
}
