/* @file */
/* Actors: a sprite dissolving pixel by pixel, the actor system, creating and */
/* releasing actors, their animations and scripts, and placing their sprites */
/* each frame under the scrolling or the fixed camera. */
/* Part of field.c. */

#include "field/actor_script.h"
#include "field/map_grid.h"
#include "field/script_operands.h"
#include "graphics/glyph.h"

/* The frames fdSprWaitForTrackEnd waits at most. */
#define TRACK_END_WAIT_FRAMES 90
/* The longest an actor's script or move is waited for: 10 seconds. */
#define ACTOR_WAIT_FRAMES_MAX 600

/* Waits, at most TRACK_END_WAIT_FRAMES, for a sprite's animation to change or
 */
/* for its first layer's script to reach ANIMCMD_HOLD. */
void fdSprWaitForTrackEnd(SPR *sprite) {
	u8 animation = sprite->lastNo;
	SPR_ANIMATION *entry = sprite->entries[0];
	s32 i;

	if(sprite != NULL) {
		i = 0;
		if(entry->script[entry->scriptOfs] != ANIMCMD_HOLD) {
			do {
				sySynch(1);
				i++;
				if(i > TRACK_END_WAIT_FRAMES - 1)
					break;
				if(entry->script[entry->scriptOfs] == ANIMCMD_HOLD)
					break;
			} while(sprite->lastNo == animation);
		}
	}
}

/* How much further along the pixel order each tile reads than the tile */
/* before, besides the step it is ahead. */
#define DISSOLVE_ORDER_STRIDE 16
/* Keeps a pixel pair's odd pixel, its high byte, clearing the even one. */
#define PIXEL_PAIR_ODD_MASK 0xff00

/* One step of dissolving a sprite pixel by pixel. */
void fdSprDissolveStep(u8 *obj, u32 pos) {
	u16 *tile;
	s32 tileCount;
	u32 i;
	const u8 *order;

	SPR *sprite = (SPR *)obj;

	tile = (u16 *)(OBJ_VRAM0 + VramCacheEntries[sprite->slot].ofs);
	tileCount = (sprite->width * sprite->height) / DISSOLVE_TILE_PIXELS;
	i = 0;

	if(i < tileCount) {
		order = SpriteDissolve_PixelOrder;
		{
			u32 highMask = PIXEL_PAIR_ODD_MASK;
			u32 indexMask = DISSOLVE_TILE_PIXELS - 1;

			for(; i < tileCount; i++, tile += TILE_SIZE_8BPP / 2, pos++) {
				if((pos - DISSOLVE_FIRST_STEP) <= DISSOLVE_TILE_PIXELS - 1) {
					u8 pixel;
					u16 *pixelPair;
					u32 isOdd;

					/* Each tile reads the order 17 entries on from the tile
					 * before, so */
					/* neighbouring tiles lose different pixels at the same
					 * step. */
					pixel =
						order[(pos + i * DISSOLVE_ORDER_STRIDE) & indexMask];
					/* The halfword holding the pixel: its offset rounded down
					 * to even. */
					pixelPair = (u16 *)((u8 *)tile +
										(pixel & (DISSOLVE_TILE_PIXELS - 2)));
					isOdd = 1;
					isOdd &= pixel;
					if(isOdd != 0)
						*pixelPair = *(u8 *)pixelPair;
					else
						*pixelPair &= highMask;
				}
			}
		}
	}
}

/* Dissolves a sprite pixel by pixel, four steps a frame, over 32 frames. */
void fdSprDissolveRun(u8 *sprite) {
	u32 step;

	step = 0;
	/* Finish all four dissolve steps before waiting for the next video frame.
	 */
	do {
		fdSprDissolveStep(sprite, step);
		fdSprDissolveStep(sprite, step + 1);
		fdSprDissolveStep(sprite, step + 2);
		fdSprDissolveStep(sprite, step + 3);
		step += 4;
		sySynch(1);
	} while(step <= DISSOLVE_STEPS - 1);
}

/* Dissolves count sprites together, pixel by pixel, over 32 frames. */
void fdSprDissolveRunMany(u8 **sprites, s32 count) {
	u8 **firstSprite;
	u8 **spriteCursor;
	s32 spritesRemaining;
	u32 step;
	u32 secondDissolveStep;
	u32 thirdDissolveStep;
	u32 fourthDissolveStep;

	firstSprite = sprites;
	for(step = 0; step <= DISSOLVE_STEPS - 1U; step += 4) {
		if(count > 0) {
			secondDissolveStep = step + 1;
			thirdDissolveStep = step + 2;
			spriteCursor = firstSprite;
			fourthDissolveStep = step + 3;
			spritesRemaining = count;
			do {
				fdSprDissolveStep(*spriteCursor, step);
				fdSprDissolveStep(*spriteCursor, secondDissolveStep);
				fdSprDissolveStep(*spriteCursor, thirdDissolveStep);
				spritesRemaining -= 1;
				fdSprDissolveStep(*spriteCursor++, fourthDissolveStep);
			} while(spritesRemaining != 0);
		}
		sySynch(1U);
	}
}

/* Writes the actor's screen position, in pixels, to screenPosition[0..1] and */
/* returns 0 when it is on or near the screen; otherwise writes 0, 0 and */
/* returns -1. */
s32 fdMapGridGetScrRelativePos(ACTOR *actor, s32 *screenPosition) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	s32 viewX;
	s32 viewZ;
	FIXED x;
	FIXED y;

	viewX = syMapStateView(map)[0] & FIELD_PIXEL_MASK;
	viewZ = syMapStateView(map)[1] & FIELD_PIXEL_MASK;
	x = actor->x.fixed - viewX;
	y = actor->z.fixed - viewZ;
	/* Across, from 32 pixels left of the screen to 32 past its 240; down, */
	/* below the view's top and above 224. */
	if((x >= -(toFIXED(32) - 1) && x <= toFIXED(272) - 1) && y > 0 &&
	   y < toFIXED(224)) {
		*screenPosition++ = FIXEDtoINT(x);
		*screenPosition = FIXEDtoINT(y);
		return 0;
	}
	*screenPosition++ = 0;
	*screenPosition = 0;
	return -1;
}

/* The colour fdGridDrawOutlineArm draws an outlined sprite's edge in: the */
/* basic sprite palette's white, which SPRITE_COLOR_WHITE paints with too. */
#define OUTLINE_EDGE_COLOR 15

/* Sets up the actor system: allocates and zeroes the 64 actors and the */
/* system's state, sets the outline's colours and registers the per-frame */
/* updates (mode is an ACTOR_SYSTEM_MODE). A battle moves its actors with */
/* fdActorUpdateAllMotion; the World Map and a battle place their sprites with
 */
/* the fixed camera. */
void fdActorSystemInit(s32 mode) {
	ACTOR_SYSTEM_STATE *system;
	ACTOR *pool;
	/* The fill word btDmaFill32Cell reads its zero from, one for both fills. */
	volatile u32 zero;

	system = syHeapSlotAllocEwram(HEAP_SLOT_ACTOR_SYSTEM, sizeof(*system));
	pool = syHeapSlotAllocEwram(HEAP_SLOT_ACTORS, ACTOR_COUNT * sizeof(*pool));
	fdActorSystemConfigure(mode);
	btDmaFill32Cell(zero, 0, pool, ACTOR_COUNT * sizeof(*pool));
	btDmaFill32Cell(zero, 0, system, sizeof(*system));
	if(mode == ACTOR_SYSTEM_BATTLE) {
		sySetEvent(fdActorUpdateAllMotion, TASK_PASS_FRAME + 10);
	}
	else {
		sySetEvent(fdActorUpdateAllOnField, TASK_PASS_FRAME + 11);
		sySetEvent(fdActorMoveWithCarriers, TASK_PASS_FRAME + 10);
	}
	if(mode == ACTOR_SYSTEM_WORLD_MAP || mode == ACTOR_SYSTEM_BATTLE)
		sySetEvent(fdActorSystemProjectSprs, TASK_PASS_FRAME);
	else
		sySetEvent(fdActorSystemPlaceSprs, TASK_PASS_FRAME);
	system->edge = OUTLINE_EDGE_COLOR;
	system->fill = 0;
}

/* Unused: an empty function beside the actor system's, in both games. */
UNUSED void fdActorSystemReservedNoOpA(void) {}

/* Unused: a second empty function. */
UNUSED void fdActorSystemReservedNoOpB(void) {}

/* Creating and releasing actors, and what they show. */

/* Returns an unused actor (one without a script) from the 64 slots, or 0 */
/* if none is free. */
void *fdActorFindFree(void) {
	ACTOR *actor = ((ACTOR *)Ram_HeapSlots->actors);
	void *freeActor = 0;
	s32 actorIndex = 0;

	while(actorIndex <= ACTOR_COUNT - 1) {
		if(actor->script == 0) {
			freeActor = actor;
			break;
		}
		actorIndex++;
		actor++;
	}
	return freeActor;
}

/* Releases an actor's sprites and zeroes it, releasing its slot. Clearing the
 */
/* script lets fdActorFindFree hand the actor out again. */
void fdActorRelease(ACTOR *actor) {
	s32 spritesRemaining;
	void **sprites;

	if(actor) {
		switch(actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) {
		case ACTOR_SPRITE_ONE:
			fdSprDestroy(actor->spr);
			break;
		case ACTOR_SPRITE_GROUP:
			sprites = (void **)actor->spr;
			for(spritesRemaining = 0; spritesRemaining < 4;
				spritesRemaining++) {
				SPR *sprite = *sprites++;
				if(sprite)
					fdSprDestroy(sprite);
			}
			break;
		}
		btDmaFill32(0, actor, sizeof(ACTOR));
	}
}

/* Creates an actor at (x, y, z), in 16.16 pixels, or returns 0 when every slot
 */
/* is in use. encodedSprite / 4096 is the kind of actor (enum */
/* ACTOR_CREATE) and its low 12 bits the sprite's animation id. */
ACTOR *fdActorCreate(s32 encodedSprite, FIXED x, FIXED y, FIXED z) {
	ACTOR *actor;
	void *sprite;
	s32 spriteStorageKind;
	void **spriteSlots;
	volatile u32 zero;

	spriteStorageKind = encodedSprite / (ACTOR_CREATE_SPRITE_MASK + 1);
	encodedSprite &= ACTOR_CREATE_SPRITE_MASK;
	actor = fdActorFindFree();
	if(actor != NULL) {
		actor->radius = 16;
		switch(spriteStorageKind) {
		case ACTOR_CREATE_ONE_SPRITE:
			sprite = fdSprCreate(encodedSprite);
			if(sprite != NULL) {
				actor->sprStorageKind = ACTOR_SPRITE_ONE;
				actor->spr = sprite;
				actor->radius =
					fdResourceGetMetadataRecordStub(encodedSprite)->footprint >>
					1;
			}
			else {
				actor->sprStorageKind = ACTOR_SPRITE_NONE;
			}
			break;
		case ACTOR_CREATE_GROUP:
			/* Take a zeroed list of four sprites, and fill it with id and id
			 * + 1. */
			spriteSlots = (void **)((u32 *)Ram_HeapSlots->actorSystem +
									Ram_HeapSlots->actorSystem->cnt++ + 2);
			actor->sprStorageKind = spriteStorageKind;
			actor->spr = spriteSlots;
			zero = 0;
			syDMAXCopy(&zero, spriteSlots, DMA_FILL_WORDS(4), REG_DMA3);
			sprite = fdSprCreate(encodedSprite);
			if(sprite != NULL) {
				actor->radius =
					fdResourceGetMetadataRecordStub(encodedSprite)->footprint >>
					1;
				*spriteSlots = sprite;
				spriteSlots++;
			}
			sprite = fdSprCreate(encodedSprite + 1);
			if(sprite != NULL)
				*spriteSlots = sprite;
			break;
		}
	}
	if(actor != NULL) {
		fdActorSetPosAndResetMotion(actor, x, y, z);
		actor->script = (s32 *)ActorScript_EndScript;
		actor->speedLimit = toFIXED(2);
		actor->scriptCursor = 0;
		actor->scaleX = FIXED_ONE;
		actor->scaleY = FIXED_ONE;
		actor->acceleration = FIXED_ONE;
		actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		actor->gravity = FIXED_ONE;
		actor->bounce = FIXED_ONE / 4;
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

/* Gives an actor a script and starts it from the top. Sets the script and */
/* clears the cursor, the pause, the loop count and the last flag test. */
void fdActorStartScript(ACTOR *actor, const s32 *script) {
	if(actor != 0) {
		actor->scriptCursor = 0;
		actor->script = script;
		actor->paused = 0;
		actor->repeatCnt = 0;
		actor->conditionResult = 0;
	}
}

/* Starts animation argument on an actor's sprite, or on each of the four */
/* sprites of a group (fdSprStartAnimation). */
void fdActorSelectAnimation(ACTOR *actor, s32 argument) {
	SPR **items;
	SPR *item;
	s32 count;

	if(actor != 0) {
		switch(actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) {
		case ACTOR_SPRITE_ONE:
			fdSprStartAnimation(((SPR *)actor->spr), argument);
			break;
		case ACTOR_SPRITE_GROUP:
			items = ((SPR **)actor->spr);
			for(count = 0; count < 4; count++) {
				item = *items++;
				if(item != 0)
					fdSprStartAnimation(item, argument);
			}
			break;
		}
	}
}

/* Sets the timer step, the animation speed, of an actor's sprite, or of each */
/* sprite of a group (ANIM_STEP_NORMAL is normal speed). */
void fdActorSetAnimationSpeed(ACTOR *actor, s32 value) {
	s32 count;
	SPR *child;
	SPR **children;

	if(actor != 0) {
		switch(actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) {
		case ACTOR_SPRITE_ONE:
			fdSprSetAnimationSpeed(((SPR *)actor->spr), value);
			return;
		case ACTOR_SPRITE_GROUP:
			children = ((SPR **)actor->spr);
			for(count = 0; count < 4; count++) {
				child = *children++;
				if(child != 0)
					fdSprSetAnimationSpeed(child, value);
			}
			break;
		}
	}
}

/* Both of the above: switches all of an actor's sprites to animation and */
/* sets their timer step to step. */
void fdActorSetAnimationAndSpeed(ACTOR *actor, s32 animation, s32 step) {
	void **sprites;
	void *sprite;
	s32 part;

	if(actor != 0) {
		switch(actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) {
		case ACTOR_SPRITE_ONE:
			fdSprStartAnimation(actor->spr, animation);
			fdSprSetAnimationSpeed(actor->spr, step);
			break;
		case ACTOR_SPRITE_GROUP:
			sprites = (void **)actor->spr;
			for(part = 0; part < 4; part++) {
				sprite = *sprites++;
				if(sprite != 0) {
					fdSprStartAnimation(sprite, animation);
					fdSprSetAnimationSpeed(sprite, step);
				}
			}
			break;
		}
	}
}

/* Sets the rotation of an actor's sprite, when it has just one. */
void fdActorSetSprRotation(ACTOR *actor, u32 value) {
	if(actor != 0 &&
	   (actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE)
		((SPR *)actor->spr)->rotation = value;
}

/* Gives a one-sprite actor's first sprite animation animation id animationId */
/* and restarts its animations. A negative animationId does nothing. */
void fdActorSetFirstSprAnimation(ACTOR *actor, s32 animationId) {
	SPR *child;

	if(actor != NULL &&
	   (actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE) {
		child = ((SPR *)actor->spr);
		if(animationId >= 0) {
			child->entries[0]->anmId = animationId;
			fdSprResetAnimations(child);
		}
	}
}

/* Adds a sprite animation of animation id animationId to a one-sprite actor's
 */
/* sprite. A negative animationId does nothing. */
void fdActorRegisterSprMetadata(ACTOR *actor, s32 animationId) {
	if(actor != 0 &&
	   (actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE) {
		SPR *sprite = actor->spr;
		if(animationId >= 0)
			fdSprAddAnimation(sprite, animationId);
	}
}

/* Starts an actor on the script that approaches its watched actor */
/* (SC_APPROACH_WATCHED), watching watchedActorHandle. */
void fdActorStartApproachWatched(ACTOR *actor, s32 watchedActorHandle) {
	fdActorStartScript(actor, Actor_ApproachWatchedScript);
	actor->watchedActor = (union FIELD_OBJECT *)watchedActorHandle;
}

/* Starts the leader's control script for the way the party moves now */
/* (MainRoutineStateWork.movementMode), waiting a frame first for Sand on the */
/* World Map. */
void fdActorStartMovementModeScript(ACTOR *actor) {
	const s32 *script;
	MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;

	switch(party->movementMode) {
	case LEADER_MOVEMENT_WALK:
		script = Actor_PlayerScript;
		break;
	case LEADER_MOVEMENT_WORLD_MAP:
		script = Actor_WorldMapScript;
		break;
	case LEADER_MOVEMENT_KEYS:
		script = Actor_KeysScript;
		break;
	case LEADER_MOVEMENT_ROPE:
		script = Actor_RopeScript;
		break;
	case LEADER_MOVEMENT_TIGHTROPE:
		script = Actor_TightropeScript;
		break;
	case LEADER_MOVEMENT_SAND:
		script = Actor_SandScript;
		break;
	case LEADER_MOVEMENT_SHIP_ON_MAP:
		script = Actor_SailOnMapScript;
		break;
	case LEADER_MOVEMENT_SHIP:
		script = Actor_SailScript;
		break;
	case LEADER_MOVEMENT_FLYING_SHIP:
		script = Actor_FlyScript;
		break;
	case LEADER_MOVEMENT_WORLD_SAND:
		sySynch(1);
		script = Actor_SandOnWorldMapScript;
		break;
	case LEADER_MOVEMENT_HOVER:
		script = Actor_HoverScript;
		break;
	case LEADER_MOVEMENT_WALKING:
		script = Actor_WalkingScript;
		break;
	case LEADER_MOVEMENT_NONE:
		script = ActorScript_EndScript;
		break;
	default:
		script = Actor_KeyCheckScript;
		break;
	}
	fdActorStartScript(actor, script);
}

/* Starts an actor on the idle script (a lone SC_END). */
void fdActorStartIdle(ACTOR *actor) {
	fdActorStartScript(actor, ActorScript_EndScript);
}

/* Starts an actor on the camera script (SC_CAMERA). With a leader to follow, */
/* it also gets a speed limit of 4 pixels a frame and an acceleration of 0.5. */
void fdActorStartCamera(ACTOR *actor, s32 watchedActorHandle) {
	fdActorStartScript(actor, Actor_CameraScript);
	if(watchedActorHandle != 0) {
		actor->acceleration = toFIXED(1) / 2;
		actor->speedLimit = toFIXED(4);
		actor->watchedActor = (union FIELD_OBJECT *)watchedActorHandle;
		actor->tmr = 0;
	}
}

/* Waits until the actor's script reaches SC_END, at most */
/* ACTOR_WAIT_FRAMES_MAX frames; it */
/* returns as soon as the script idles, and after the limit whether or not it */
/* has. */
void fdActorWaitForScriptEnd(ACTOR *actor) {
	s32 waitedFrames = 0;

	if(actor->script != 0 &&
	   ((const s32 *)actor->script)[actor->scriptCursor] != SC_END) {
		do {
			sySynch(1);
			waitedFrames++;
			if(waitedFrames >= ACTOR_WAIT_FRAMES_MAX)
				break;
		} while(actor->script != 0 &&
				((const s32 *)actor->script)[actor->scriptCursor] != SC_END);
	}
}

/* Sets the flags of a one-sprite actor's sprite (SPR_FLAG). */
void fdActorSetSprFlags(ACTOR *actor, u32 value) {
	if(actor != NULL &&
	   (actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE)
		((SPR *)actor->spr)->flg = value;
}

/* Sets the OAM blend mode of a one-sprite actor's sprite. */
void fdActorSetBlendMode(ACTOR *actor, u32 mode) {
	if(actor != 0 && actor->sprStorageKind == ACTOR_SPRITE_ONE) {
		SPR *sprite = actor->spr;
		sprite->image.objMode = mode;
	}
}

/* Sets a one-sprite actor's sprite's own-scale bit: drawn at its own scale, */
/* not scaled by distance. */
void fdActorSetOwnScale(ACTOR *actor, u32 value) {
	if(actor != NULL && actor->sprStorageKind == ACTOR_SPRITE_ONE)
		((SPR *)actor->spr)->useOwnScale = value;
}

/* Sets the colour mode (SPR_COL_MODE) of a one-sprite actor's sprite */
/* animations, except those drawn hidden. */
void fdActorSetSprColMode(ACTOR *actor, u32 value) {
	if(actor != NULL && actor->sprStorageKind == ACTOR_SPRITE_ONE)
		fdSprSetAnimationColModes(((SPR *)actor->spr), value);
}

/* Fixes the priority of a one-sprite actor's sprite and its shadow, which */
/* then no longer follow the cells under it (ACTOR_PRIORITY_AUTOMATIC). */
void fdActorSetSprPriorityDirect(ACTOR *actor, s32 priority) {
	SPR *sprite;

	if(actor == NULL || actor->sprStorageKind != ACTOR_SPRITE_ONE) {
		return;
	}
	sprite = actor->spr;
	sprite->image.pri = priority;
	sprite->shadow.pri = priority;
	actor->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
}

/* Shows item id's icon on its owner's sprite, 16 pixels square at scale */
/* 1.0 in 16-colour palette 0. */
void fdActorSetItemIcon(ACTOR *owner, s32 id) {
	SPR *sprite;
	ICON_TRANSFER *transfer;
	s32 tile;

	if(owner == 0)
		return;
	owner->retainSprCache = ACTOR_KEEP_SPRITE_VRAM;
	owner->iconItem = id;
	sprite = owner->spr;
	if(sprite == 0)
		return;
	transfer = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	mnIconDrawItemStub(id);
	tile = syVramCacheLoad(sprite->slot, ICON_TILE_BYTES, transfer->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	sprite->cnt = 0;
	sprite->dirty = 0;
	sprite->width = 16;
	sprite->height = 16;
	sprite->ofsX = 0;
	sprite->ofsY = 4;
	sprite->scale = FIXED_ONE;
	sprite->image.tile = tile;
	sprite->image.fullCol = 0;
	sprite->image.pal = 0;
}

/* The sprite id an actor is drawn with (the first halfword of its sprite's */
/* definition), or 0 for none. */
s32 fdActorGetSprId(ACTOR *actor) {
	SPR *sprite = actor->spr;

	if(actor != NULL && sprite != NULL && sprite->entries != NULL)
		return sprite->entries[0]->anmId;
	return 0;
}

/* fdSprWaitForTrackEnd on an actor's sprite. */
void fdActorWaitForTrackEnd(ACTOR *actor) {
	if(actor != NULL)
		fdSprWaitForTrackEnd(actor->spr);
}

/* Suspends the actor camera updates, starts the palette back to normal and, a
 */
/* frame later, shows the sprites with BG1-3 hidden, leaving BG0 unchanged. */
void fdActorSystemSuspendProjectionAndShowSprs(void) {
	u16 dispcnt;

	syTaskSuspend(fdActorSystemPlaceSprs);
	syTaskSuspend(fdActorSystemProjectSprs);
	fdPaletteFadeSetTargetStub(FIXED_ONE, 1);
	fdPaletteFadeStartInterpolationStub(1);
	sySynch(1);
	dispcnt = REG_DISPCNT;
	REG_DISPCNT =
		(dispcnt & (u16)~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON)) |
		DISPCNT_OBJ_ON;
}

/* Resumes the actor camera updates and hides BG1-3 and the sprites. */
void fdActorSystemResumeProjectionAndHideLayers(void) {
	syTaskResume(fdActorSystemPlaceSprs);
	syTaskResume(fdActorSystemProjectSprs);
	REG_DISPCNT &=
		~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON | DISPCNT_OBJ_ON);
}

/* Unused: returns 1. */
UNUSED s32 fdActorReturnTrue(void) { return 1; }

/* The sprite priority an actor takes on the cell under a point. */
#define CELL_PRIORITY(cells, x, z)                                             \
	((cells)[(FIELD_CELL(z) << 7) + FIELD_CELL(x)].actorPri)
/* The map layer an actor takes on the cell under a point, plus one. */
#define CELL_LAYER(cells, x, z)                                                \
	((cells)[(FIELD_CELL(z) << 7) + FIELD_CELL(x)].actorLayer)

/* Each frame, with the scrolling camera: places every actor's sprite on the */
/* screen. */
void fdActorSystemPlaceSprs(void) {
	/* Unused: gives the stack frame its 36 bytes, as The Broken Seal's. */
	ACTOR_CAMERA_POINT unusedPoints[3];
	SPR_PLACEMENT point;
	SPR_SCALE scale;
	s32 objectIndex;
	MAP_STATE *map;
	s32 viewX;
	s32 viewZ;
	ACTOR_SYSTEM_STATE *system;
	ACTOR *actor;
	u32 kind;
	FIXED dx;
	FIXED dz;
	s32 originY;
	SPR *sprite;
	FIXED screenTop;
	u8 *priorityFlags;
	s32 layer;
	u8 *terrainId;
	MAP_CELL *cells;
	FIXED feetZ;
	s32 objectHeight;
	FIXED probeX;
	s32 priority;
	s32 leftPriority;
	s32 rightPriority;
	s32 middlePriority;
	s32 height;
	s32 rise;
	u32 layerPlusOne;
	s8 *basePriority;

	map = Ram_HeapSlots->mapGrid;
	viewX = syMapStateView(map)[0] & FIELD_PIXEL_MASK;
	viewZ = syMapStateView(map)[1] & FIELD_PIXEL_MASK;
	system = Ram_HeapSlots->actorSystem;
	originY = map->origin[1];
	btRenderLoadDecoder();
	system->used = 0;
	actor = Ram_HeapSlots->actors;
	for(objectIndex = 0; objectIndex < ACTOR_COUNT; objectIndex++, actor++) {
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

				syVramCacheReleaseVram(frozen->slot);
				frozen->dirty = kind;
				continue;
			}
			sprite = actor->spr;
			dx = actor->x.fixed - viewX;
			dz = actor->z.fixed - viewZ;
			screenTop = dz - actor->y.fixed;
			if(dx > -toFIXED(32) && dx < toFIXED(272) &&
			   screenTop > -toFIXED(32) && screenTop < toFIXED(224) &&
			   /* The ROM keeps &actor->priFlg in a stack slot of its
				  own, */
			   /* ordered among the declared locals' slots and above the slot
				  the */
			   /* loop body's first statements create. Reload orders slots by */
			   /* pseudo number, so the source declared a local holding this */
			   /* address; its type, name and spelling here are not known. */
			   (priorityFlags = &actor->priFlg,
				!(*priorityFlags & ACTOR_PRIORITY_NOT_DRAWN))) {
				/* &actor->layer in a declared local, by the same stack-slot
				 * evidence */
				/* as priorityFlags above; its type and name are not known. */
				terrainId = &actor->layer;
				layer = *terrainId & 3;
				cells = map->layers[layer].cells;
				/* The sprite takes the priority of the cells under its feet: 7
				 * pixels */
				/* left of its middle and 6 right where the ground is level with
				 * its */
				/* own, and its middle. With none, it takes the map's base
				 * priority, */
				/* or one further back where the ground a cell in front, or two
				 * cells */
				/* in front less a cell's height, rises above its own; with */
				/* ACTOR_PRIORITY_IGNORES_RISES, it keeps its own. */
				if(*priorityFlags & ACTOR_PRIORITY_AUTOMATIC) {
					probeX = actor->x.fixed - toFIXED(7);
					feetZ = actor->z.fixed + toFIXED(2);
					objectHeight = FIXEDtoINT(actor->groundHeight);
					priority = 0;
					leftPriority = 0;
					rightPriority = 0;
					if(!(*priorityFlags & ACTOR_PRIORITY_IGNORES_RISES)) {
						/* &map->basePri in a declared local, by the same
						 * evidence: */
						/* set inside this arm, yet numbered before the loop
						 * body's first */
						/* statements, so declared at function level or at the
						 * loop body's */
						/* head. Its type and name are not known. */
						basePriority = &map->basePri;
						priority = *basePriority;
						if(fdMapGridGetTerrainHeight(layer, probeX, feetZ) ==
						   actor->groundHeight) {
							leftPriority = CELL_PRIORITY(cells, probeX, feetZ);
							height = FIXEDtoINT(fdMapGridGetTerrainHeight(
								layer, probeX,
								feetZ + toFIXED(MAPGRID_CELL_PIXELS)));
							rise =
								FIXEDtoINT(fdMapGridGetTerrainHeight(
									layer, probeX,
									feetZ + toFIXED(2 * MAPGRID_CELL_PIXELS))) -
								MAPGRID_CELL_PIXELS;
							if(rise > height)
								height = rise;
							if(height > objectHeight)
								priority = map->basePri + 1;
						}
						probeX += toFIXED(13);
						if(fdMapGridGetTerrainHeight(layer, probeX, feetZ) ==
						   actor->groundHeight) {
							rightPriority = CELL_PRIORITY(cells, probeX, feetZ);
							height = FIXEDtoINT(fdMapGridGetTerrainHeight(
								layer, probeX,
								feetZ + toFIXED(MAPGRID_CELL_PIXELS)));
							rise =
								FIXEDtoINT(fdMapGridGetTerrainHeight(
									layer, probeX,
									feetZ + toFIXED(2 * MAPGRID_CELL_PIXELS))) -
								MAPGRID_CELL_PIXELS;
							if(rise > height)
								height = rise;
							if(height > objectHeight)
								priority = map->basePri + 1;
						}
						probeX -= toFIXED(6);
						middlePriority = CELL_PRIORITY(cells, probeX, feetZ);
						height = FIXEDtoINT(fdMapGridGetTerrainHeight(
							layer, probeX,
							feetZ + toFIXED(MAPGRID_CELL_PIXELS)));
						rise = FIXEDtoINT(fdMapGridGetTerrainHeight(
								   layer, probeX,
								   feetZ + toFIXED(2 * MAPGRID_CELL_PIXELS))) -
							   MAPGRID_CELL_PIXELS;
						if(rise > height)
							height = rise;
						if(height > objectHeight)
							priority = map->basePri + 1;
					}
					else {
						if(actor->groundHeight ==
						   fdMapGridGetTerrainHeight(layer, probeX, feetZ))
							leftPriority = CELL_PRIORITY(cells, probeX, feetZ);
						probeX += toFIXED(13);
						if(actor->groundHeight ==
						   fdMapGridGetTerrainHeight(layer, probeX, feetZ))
							rightPriority = CELL_PRIORITY(cells, probeX, feetZ);
						probeX -= toFIXED(6);
						middlePriority = CELL_PRIORITY(cells, probeX, feetZ);
					}
					/* An odd priority is preferred: the left's, the right's,
					 * the */
					/* middle's. */
					if(leftPriority != 0 || rightPriority != 0 ||
					   middlePriority != 0) {
						priority = leftPriority;
						if(!(priority & 1)) {
							if(rightPriority != 0)
								priority = rightPriority;
							if(!(priority & 1) && middlePriority != 0)
								priority = middlePriority;
						}
					}
					if(priority != 0 &&
					   actor->sprStorageKind == ACTOR_SPRITE_ONE) {
						SPR *placed = actor->spr;

						if((u32)priority > 3)
							priority = 3;
						placed->image.pri = priority;
						placed->shadow.pri = priority;
					}
				}
				layerPlusOne =
					CELL_LAYER(cells, actor->x.fixed, actor->z.fixed);
				if(layerPlusOne != 0)
					*terrainId = layerPlusOne - 1;
				scale.x = syMulFX(actor->scaleX, sprite->scale);
				scale.y = syMulFX(actor->scaleY, sprite->scale);
				point.x = dx;
				point.height = actor->y.fixed - originY;
				point.y = dz - originY;
				point.groundHeight = actor->groundHeight - originY;
				if(*priorityFlags & ACTOR_PRIORITY_UNDERFOOT) {
					point.height -= toFIXED(320);
					point.y -= toFIXED(320);
					point.groundHeight -= toFIXED(320);
				}
				if(*priorityFlags & ACTOR_PRIORITY_OVERHEAD) {
					point.height += toFIXED(320);
					point.y += toFIXED(320);
					point.groundHeight += toFIXED(320);
				}
				if(*priorityFlags & ACTOR_PRIORITY_OWN_DEPTH)
					fdRenderApplyProjectedPlacementAtDepth(
						sprite, &point, scale.values, actor->facing,
						actor->sceneByte);
				else
					fdRenderApplyProjectedPlacement(
						sprite, sySprPlacementValues(&point), scale.values,
						actor->facing);
				continue;
			}
			if(actor->retainSprCache & ACTOR_COMPOSE_OFF_SCREEN) {
				fdSprComposePicture(sprite, actor->facing);
			}
			else if(!(actor->retainSprCache & ACTOR_KEEP_SPRITE_VRAM) &&
					!sprite->retainCacheEntry) {
				syVramCacheReleaseVram(sprite->slot);
				sprite->dirty = 1;
			}
		}
		else {
			/* At (0, 0): not placed; only the sprite's VRAM is given up. */
			kind = actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK;
			if(kind == ACTOR_SPRITE_ONE) {
				SPR *frozen = actor->spr;

				if(actor->retainSprCache == 0 && !frozen->retainCacheEntry) {
					syVramCacheReleaseVram(frozen->slot);
					frozen->dirty = kind;
				}
			}
		}
	}
	syHeapSlotRelease(HEAP_SLOT_FRAME_SCRATCH);
}

/* The fixed camera, on the World Map and in battle: each frame it places */
/* every actor's sprites, facing them by the angle from the eye to the target.
 */

/* Unused: returns 1. */
UNUSED s32 fdActorCameraReturnTrue(void) { return 1; }

/* Places an actor's sprite (unless ACTOR_PRIORITY_NOT_DRAWN hides it) or each
 */
/* of its group's four sprites, turned by angle. While battle commands are */
/* chosen (FLAG_CHOOSING_COMMANDS) the sprites are drawn at 1.0 scale. */
void fdActorPlaceSprs(ACTOR *actor, s32 angle) {
	const s32 *scale = &actor->scaleX;
	s32 *point;
	void *sprite;
	void **sprites;
	s32 remaining;

	if(syGameFlagGet(FLAG_CHOOSING_COMMANDS))
		scale = ActorSprite_FullScale;
	switch(actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) {
	case ACTOR_SPRITE_ONE:
		point = &actor->x.fixed;
		sprite = actor->spr;
		if(!(actor->priFlg & ACTOR_PRIORITY_NOT_DRAWN))
			fdRenderPlaceProjectedSpr(sprite, point, scale,
									  actor->facing + angle, actor->layer);
		break;
	case ACTOR_SPRITE_GROUP:
		sprites = actor->spr;
		for(remaining = 0; remaining < 4; remaining++) {
			sprite = *sprites++;
			if(sprite != NULL)
				fdRenderPlaceProjectedSpr(sprite, &actor->x.fixed, scale,
										  actor->facing + angle, actor->layer);
		}
		break;
	case ACTOR_SPRITE_NONE:
		break;
	}
}

/* Each frame, with the fixed camera: places every actor in use, from the */
/* 64th down. While battle commands are chosen (FLAG_CHOOSING_COMMANDS) it uses
 */
/* the fixed view matrix and turns the objects by an eighth of a turn less. */
void fdActorSystemProjectSprs(void) {
	ACTOR_CAMERA_FIXED *camera = Ram_HeapSlots->cameraState;
	ACTOR_SYSTEM_STATE *system = Ram_HeapSlots->actorSystem;
	ACTOR_CAMERA_POINT *eye;
	ACTOR_CAMERA_POINT *target;
	ACTOR *actor;
	s32 angle;
	s32 i;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	ACTOR_CAMERA_POINT unused;

	btRenderLoadDecoder();
	eye = &camera->eye;
	target = &camera->tgt;
	if(camera->followEye != NULL)
		eye = camera->followEye;
	if(camera->followPos != NULL)
		target = camera->followPos;
	angle = (s16)syAtan(FIXEDtoINT(eye->x - target->x),
						FIXEDtoINT(eye->z - target->z));
	system->used = 0;
	if(syGameFlagGet(FLAG_CHOOSING_COMMANDS)) {
		syInitMatrix();
		angle -= DEGtoANG(45);
		syMatrixMultiplyEntry(Camera_FixedViewMatrix);
		syLookAt((s32 *)eye, (s32 *)target);
	}
	else {
		syMatrixRewindStack();
		syMatrixSetLookAt((s32 *)eye, (s32 *)target);
	}
	actor = Ram_HeapSlots->actors;
	actor += ACTOR_COUNT - 1;
	for(i = ACTOR_COUNT - 1; i >= 0; i--, actor--) {
		/* The upper half of a script's address in ROM is never zero. */
		if(((u16 *)&actor->script)[1] != 0)
			fdActorPlaceSprs(actor, angle);
	}
	syHeapSlotRelease(HEAP_SLOT_FRAME_SCRATCH);
}

/* SC_GOTO(script): goes on in script, from its first command, this frame. */
s32 fdActorScriptGoto(ACTOR *interpreter) {
	interpreter->script =
		(const s32 *)interpreter->script[interpreter->scriptCursor + 1];
	interpreter->scriptCursor = 0;
	return 1;
}

/* SC_HIDE_SPRITES: clears spriteStorageKind, hiding the sprites, and moves on
 */
/* to the next command. */
s32 fdActorScriptHideSprs(ACTOR *interpreter) {
	interpreter->sprStorageKind = ACTOR_SPRITE_NONE;
	interpreter->scriptCursor++;
	return 1;
}

/* SC_SHOW_SPRITE: shows the actor's one sprite again (ACTOR_SPRITE_ONE) and */
/* moves on to the next command. */
s32 fdActorScriptShowSpr(ACTOR *interpreter) {
	interpreter->sprStorageKind = ACTOR_SPRITE_ONE;
	interpreter->scriptCursor++;
	return 1;
}

/* Waits, at most ACTOR_WAIT_FRAMES_MAX frames, for an actor to reach where it
 */
/* is going. */
void fdActorWaitForMove(ACTOR *actor) {
	s32 waitedFrames;

	for(waitedFrames = 0; waitedFrames < ACTOR_WAIT_FRAMES_MAX &&
						  fdActorIsTargetUnset(actor) == 0;
		waitedFrames++) {
		sySynch(1);
	}
}
