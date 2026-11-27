/* Actors: a sprite dissolving pixel by pixel, the actor system, creating and
   releasing actors, their animations and scripts, and placing their sprites
   each tick under the scrolling or the fixed camera. Part of field.c. */

#include "field/actor_script.h"
#include "field/map_grid.h"
#include "field/script_operands.h"
#include "graphics/glyph.h"

/* The ticks fdSpriteWaitForTrackEnd waits at most. */
#define TRACK_END_WAIT_TICKS 90
/* The longest an actor's script or move is waited for: 10 seconds. */
#define ACTOR_WAIT_TICKS_MAX 600

/* Waits, at most TRACK_END_WAIT_TICKS, for a sprite's animation to change or
   for its first layer's script to reach ANIMCMD_HOLD. */
void fdSpriteWaitForTrackEnd(SPR *sprite)
{
	Uint8		   animation = sprite->lastNo;
	SPR_ANIMATION *entry = sprite->entries[0];
	Sint32		   i;

	if(sprite != NULL) {
		i = 0;
		if(entry->script[entry->scriptOffset] != ANIMCMD_HOLD) {
			do {
				sySynch(1);
				i++;
				if(i > TRACK_END_WAIT_TICKS - 1)
					break;
				if(entry->script[entry->scriptOffset] == ANIMCMD_HOLD)
					break;
			} while(sprite->lastNo == animation);
		}
	}
}

/* How much further along the pixel order each tile reads than the tile before,
   besides the step it is ahead. */
#define DISSOLVE_ORDER_STRIDE 16
/* Keeps a pixel pair's odd pixel, its high byte, clearing the even one. */
#define PIXEL_PAIR_ODD_MASK	  0xff00

/* One step of dissolving a sprite pixel by pixel. */
void fdSpriteDissolveStep(Uint8 *object, Uint32 pos)
{
	Uint16		*tile;
	Sint32		 tileCount;
	Uint32		 i;
	const Uint8 *order;

	SPR *sprite = (SPR *)object;

	tile = (Uint16 *)(OBJ_VRAM0 + VramCacheEntries[sprite->slot].offset);
	tileCount = (sprite->width * sprite->height) / DISSOLVE_TILE_PIXELS;
	i = 0;

	if(i < tileCount) {
		order = SpriteDissolvePixelOrder;
		{
			Uint32 highMask = PIXEL_PAIR_ODD_MASK;
			Uint32 indexMask = DISSOLVE_TILE_PIXELS - 1;

			for(; i < tileCount; i++, tile += TILE_SIZE_8BPP / 2, pos++) {
				if((pos - DISSOLVE_FIRST_STEP) <= DISSOLVE_TILE_PIXELS - 1) {
					Uint8	pixel;
					Uint16 *pixelPair;
					Uint32	isOdd;

					/* Each tile reads the order 17 entries on from the tile
					   before, so neighbouring tiles lose different pixels at
					   the same step. */
					pixel =
						order[(pos + i * DISSOLVE_ORDER_STRIDE) & indexMask];
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

/* Dissolves a sprite pixel by pixel, four steps a tick, over 32 ticks. */
void fdSpriteDissolveRun(Uint8 *sprite)
{
	Uint32 step;

	step = 0;
	/* Finish all four dissolve steps before waiting for the next video
	   frame. */
	do {
		fdSpriteDissolveStep(sprite, step);
		fdSpriteDissolveStep(sprite, step + 1);
		fdSpriteDissolveStep(sprite, step + 2);
		fdSpriteDissolveStep(sprite, step + 3);
		step += 4;
		sySynch(1);
	} while(step <= DISSOLVE_STEPS - 1);
}

/* Dissolves count sprites together, pixel by pixel, over 32 ticks. */
void fdSpriteDissolveRunMany(Uint8 **sprites, Sint32 count)
{
	Uint8 **firstSprite;
	Uint8 **spriteCursor;
	Sint32	spritesRemaining;
	Uint32	step;
	Uint32	secondDissolveStep;
	Uint32	thirdDissolveStep;
	Uint32	fourthDissolveStep;

	firstSprite = sprites;
	for(step = 0; step <= DISSOLVE_STEPS - 1U; step += 4) {
		if(count > 0) {
			secondDissolveStep = step + 1;
			thirdDissolveStep = step + 2;
			spriteCursor = firstSprite;
			fourthDissolveStep = step + 3;
			spritesRemaining = count;
			do {
				fdSpriteDissolveStep(*spriteCursor, step);
				fdSpriteDissolveStep(*spriteCursor, secondDissolveStep);
				fdSpriteDissolveStep(*spriteCursor, thirdDissolveStep);
				spritesRemaining -= 1;
				fdSpriteDissolveStep(*spriteCursor++, fourthDissolveStep);
			} while(spritesRemaining != 0);
		}
		sySynch(1U);
	}
}

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

	system = syHeapSlotAllocateEWRAM(HEAP_SLOT_ACTOR_SYSTEM, sizeof(*system));
	pool =
		syHeapSlotAllocateEWRAM(HEAP_SLOT_ACTORS, ACTOR_COUNT * sizeof(*pool));
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
		sySetEvent(fdActorSystemProjectSprites, EVENT_PASS_TICK);
	else
		sySetEvent(fdActorSystemPlaceSprites, EVENT_PASS_TICK);
	system->edge = OUTLINE_EDGE_COLOR;
	system->fill = 0;
}

/* Unused: an empty function beside the actor system's, in both games. */
UNUSED void fdActorSystemReservedNoOperationA(void)
{
}

/* Unused: a second empty function. */
UNUSED void fdActorSystemReservedNoOperationB(void)
{
}

/* Creating and releasing actors, and what they show. */

/* Returns an unused actor (one without a script) from the 64 slots, or 0 if
   none is free. */
void *fdActorFindFree(void)
{
	ACTOR *actor = ((ACTOR *)RamHeapSlots->actors);
	void  *freeActor = 0;
	Sint32 actorIndex = 0;

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
   script lets fdActorFindFree hand the actor out again. */
void fdActorRelease(ACTOR *actor)
{
	Sint32 spritesRemaining;
	void **sprites;

	if(actor) {
		switch(actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) {
		case ACTOR_SPRITE_ONE:
			fdSpriteDestroy(actor->sprite);
			break;
		case ACTOR_SPRITE_GROUP:
			sprites = (void **)actor->sprite;
			for(spritesRemaining = 0; spritesRemaining < 4;
				spritesRemaining++) {
				SPR *sprite = *sprites++;
				if(sprite)
					fdSpriteDestroy(sprite);
			}
			break;
		}
		DMA_FILL32(0, actor, sizeof(ACTOR));
	}
}

/* Creates an actor at (x, y, z), in 16.16 pixels, or returns 0 when every slot
   is in use. encodedSprite / 4096 is the kind of actor (enum ACTOR_CREATE) and
   its low 12 bits the sprite's animation id. */
ACTOR *fdActorCreate(Sint32 encodedSprite, FIXED x, FIXED y, FIXED z)
{
	ACTOR		   *actor;
	void		   *sprite;
	Sint32			spriteStorageKind;
	void		  **spriteSlots;
	volatile Uint32 zero;

	spriteStorageKind = encodedSprite / (ACTOR_CREATE_SPRITE_MASK + 1);
	encodedSprite &= ACTOR_CREATE_SPRITE_MASK;
	actor = fdActorFindFree();
	if(actor != NULL) {
		actor->radius = 16;
		switch(spriteStorageKind) {
		case ACTOR_CREATE_ONE_SPRITE:
			sprite = fdSpriteCreate(encodedSprite);
			if(sprite != NULL) {
				actor->spriteStorageKind = ACTOR_SPRITE_ONE;
				actor->sprite = sprite;
				actor->radius =
					fdResourceGetMetadataRecordStub(encodedSprite)->footprint >>
					1;
			} else {
				actor->spriteStorageKind = ACTOR_SPRITE_NONE;
			}
			break;
		case ACTOR_CREATE_GROUP:
			/* Take a zeroed list of four sprites, and fill it with id and id +
			   1. */
			spriteSlots = (void **)((Uint32 *)RamHeapSlots->actorSystem +
				RamHeapSlots->actorSystem->count++ + 2);
			actor->spriteStorageKind = spriteStorageKind;
			actor->sprite = spriteSlots;
			zero = 0;
			syDMAXCopy(&zero, spriteSlots, DMA_FILL_WORDS(4), REG_DMA3);
			sprite = fdSpriteCreate(encodedSprite);
			if(sprite != NULL) {
				actor->radius =
					fdResourceGetMetadataRecordStub(encodedSprite)->footprint >>
					1;
				*spriteSlots = sprite;
				spriteSlots++;
			}
			sprite = fdSpriteCreate(encodedSprite + 1);
			if(sprite != NULL)
				*spriteSlots = sprite;
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
		actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		actor->gravity = toFIXED(1);
		actor->bounce = toFIXED(1) / 4;
		actor->collisionFlags = 0;
		actor->steeringFlags = ACTOR_FACES_MOTION;
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

/* Gives an actor a script and starts it from the top. Sets the script and
   clears the cursor, the pause, the loop count and the last flag test. */
void fdActorStartScript(ACTOR *actor, const Sint32 *script)
{
	if(actor != 0) {
		actor->scriptCursor = 0;
		actor->script = script;
		actor->paused = 0;
		actor->repeatCount = 0;
		actor->conditionResult = 0;
	}
}

/* Starts animation argument on an actor's sprite, or on each of the four
   sprites of a group (fdSpriteStartAnimation). */
void fdActorSelectAnimation(ACTOR *actor, Sint32 argument)
{
	SPR	 **items;
	SPR	  *item;
	Sint32 count;

	if(actor != 0) {
		switch(actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) {
		case ACTOR_SPRITE_ONE:
			fdSpriteStartAnimation(((SPR *)actor->sprite), argument);
			break;
		case ACTOR_SPRITE_GROUP:
			items = ((SPR **)actor->sprite);
			for(count = 0; count < 4; count++) {
				item = *items++;
				if(item != 0)
					fdSpriteStartAnimation(item, argument);
			}
			break;
		}
	}
}

/* Sets the timer step, the animation speed, of an actor's sprite, or of each
   sprite of a group (ANIM_STEP_NORMAL is normal speed). */
void fdActorSetAnimationSpeed(ACTOR *actor, Sint32 value)
{
	Sint32 count;
	SPR	  *child;
	SPR	 **children;

	if(actor != 0) {
		switch(actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) {
		case ACTOR_SPRITE_ONE:
			fdSpriteSetAnimationSpeed(((SPR *)actor->sprite), value);
			return;
		case ACTOR_SPRITE_GROUP:
			children = ((SPR **)actor->sprite);
			for(count = 0; count < 4; count++) {
				child = *children++;
				if(child != 0)
					fdSpriteSetAnimationSpeed(child, value);
			}
			break;
		}
	}
}

/* Both of the above: switches all of an actor's sprites to animation and sets
   their timer step to step. */
void fdActorSetAnimationAndSpeed(ACTOR *actor, Sint32 animation, Sint32 step)
{
	void **sprites;
	void  *sprite;
	Sint32 part;

	if(actor != 0) {
		switch(actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) {
		case ACTOR_SPRITE_ONE:
			fdSpriteStartAnimation(actor->sprite, animation);
			fdSpriteSetAnimationSpeed(actor->sprite, step);
			break;
		case ACTOR_SPRITE_GROUP:
			sprites = (void **)actor->sprite;
			for(part = 0; part < 4; part++) {
				sprite = *sprites++;
				if(sprite != 0) {
					fdSpriteStartAnimation(sprite, animation);
					fdSpriteSetAnimationSpeed(sprite, step);
				}
			}
			break;
		}
	}
}

/* Sets the rotation of an actor's sprite, when it has just one. */
void fdActorSetSpriteRotation(ACTOR *actor, Uint32 value)
{
	if(actor != 0 &&
		(actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE)
		((SPR *)actor->sprite)->rotation = value;
}

/* Gives a one-sprite actor's first sprite animation animation id animationId
   and restarts its animations. A negative animationId does nothing. */
void fdActorSetFirstSpriteAnimation(ACTOR *actor, Sint32 animationId)
{
	SPR *child;

	if(actor != NULL &&
		(actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) ==
			ACTOR_SPRITE_ONE) {
		child = ((SPR *)actor->sprite);
		if(animationId >= 0) {
			child->entries[0]->animationId = animationId;
			fdSpriteResetAnimations(child);
		}
	}
}

/* Adds a sprite animation of animation id animationId to a one-sprite actor's
   sprite. A negative animationId does nothing. */
void fdActorRegisterSpriteMetadata(ACTOR *actor, Sint32 animationId)
{
	if(actor != 0 &&
		(actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) ==
			ACTOR_SPRITE_ONE) {
		SPR *sprite = actor->sprite;
		if(animationId >= 0)
			fdSpriteAddAnimation(sprite, animationId);
	}
}

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
		actor->timer = 0;
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
void fdActorSetSpriteFlags(ACTOR *actor, Uint32 value)
{
	if(actor != NULL &&
		(actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE)
		((SPR *)actor->sprite)->flags = value;
}

/* Sets the OAM blend mode of a one-sprite actor's sprite. */
void fdActorSetBlendMode(ACTOR *actor, Uint32 mode)
{
	if(actor != 0 && actor->spriteStorageKind == ACTOR_SPRITE_ONE) {
		SPR *sprite = actor->sprite;
		sprite->image.objectMode = mode;
	}
}

/* Sets a one-sprite actor's sprite's own-scale bit: drawn at its own scale, not
   scaled by distance. */
void fdActorSetOwnScale(ACTOR *actor, Uint32 value)
{
	if(actor != NULL && actor->spriteStorageKind == ACTOR_SPRITE_ONE)
		((SPR *)actor->sprite)->useOwnScale = value;
}

/* Sets the colour mode (SPR_COL_MODE) of a one-sprite actor's sprite
   animations, except those drawn hidden. */
void fdActorSetSpriteColorMode(ACTOR *actor, Uint32 value)
{
	if(actor != NULL && actor->spriteStorageKind == ACTOR_SPRITE_ONE)
		fdSpriteSetAnimationColorModes(((SPR *)actor->sprite), value);
}

/* Fixes the priority of a one-sprite actor's sprite and its shadow, which then
   no longer follow the metatiles under it (ACTOR_PRIORITY_AUTOMATIC). */
void fdActorSetSpritePriorityDirect(ACTOR *actor, Sint32 priority)
{
	SPR *sprite;

	if(actor == NULL || actor->spriteStorageKind != ACTOR_SPRITE_ONE) {
		return;
	}
	sprite = actor->sprite;
	sprite->image.priority = priority;
	sprite->shadow.priority = priority;
	actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
}

/* Shows item id's icon on its owner's sprite, 16 pixels square at scale 1.0 in
   16-colour palette 0. */
void fdActorSetItemIcon(ACTOR *owner, Sint32 id)
{
	SPR			  *sprite;
	ICON_TRANSFER *transfer;
	Sint32		   tile;

	if(owner == 0)
		return;
	owner->retainSpriteCache = ACTOR_KEEP_SPRITE_VRAM;
	owner->iconItem = id;
	sprite = owner->sprite;
	if(sprite == 0)
		return;
	transfer =
		syHeapSlotAllocate(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	mnIconDrawItemStub(id);
	tile = syVRAMCacheLoad(sprite->slot, ICON_TILE_BYTES, transfer->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	sprite->count = 0;
	sprite->dirty = 0;
	sprite->width = 16;
	sprite->height = 16;
	sprite->offsetX = 0;
	sprite->offsetY = 4;
	sprite->scale = toFIXED(1);
	sprite->image.tile = tile;
	sprite->image.fullColor = 0;
	sprite->image.palette = 0;
}

/* The sprite id an actor is drawn with (the first halfword of its sprite's
   definition), or 0 for none. */
Sint32 fdActorGetSpriteId(ACTOR *actor)
{
	SPR *sprite = actor->sprite;

	if(actor != NULL && sprite != NULL && sprite->entries != NULL)
		return sprite->entries[0]->animationId;
	return 0;
}

/* fdSpriteWaitForTrackEnd on an actor's sprite. */
void fdActorWaitForTrackEnd(ACTOR *actor)
{
	if(actor != NULL)
		fdSpriteWaitForTrackEnd(actor->sprite);
}

/* Suspends the actor camera updates, starts the palette back to normal and, a
   tick later, shows the sprites with BG1-3 hidden, leaving BG0 unchanged. */
void fdActorSystemSuspendProjectionAndShowSprites(void)
{
	Uint16 dispcnt;

	sySuspendEvent(fdActorSystemPlaceSprites);
	sySuspendEvent(fdActorSystemProjectSprites);
	fdPaletteFadeSetTargetStub(toFIXED(1), 1);
	fdPaletteFadeStartInterpolationStub(1);
	sySynch(1);
	dispcnt = REG_DISPCNT;
	REG_DISPCNT =
		(dispcnt &
			(Uint16)~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON)) |
		DISPCNT_OBJ_ON;
}

/* Resumes the actor camera updates and hides BG1-3 and the sprites. */
void fdActorSystemResumeProjectionAndHideLayers(void)
{
	syResumeEvent(fdActorSystemPlaceSprites);
	syResumeEvent(fdActorSystemProjectSprites);
	REG_DISPCNT &=
		~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON | DISPCNT_OBJ_ON);
}

/* Unused: returns 1. */
UNUSED Sint32 fdActorReturnTrue(void)
{
	return 1;
}

/* The sprite priority an actor takes on the metatile under a point. */
#define METATILE_PRIORITY(metatiles, x, z) \
	((metatiles)[(FIELD_METATILE(z) << 7) + FIELD_METATILE(x)].actorPriority)
/* The map layer an actor takes on the metatile under a point, plus one. */
#define METATILE_LAYER(metatiles, x, z) \
	((metatiles)[(FIELD_METATILE(z) << 7) + FIELD_METATILE(x)].actorLayer)

/* Each tick, with the scrolling camera: places every actor's sprite on the
   screen. */
void fdActorSystemPlaceSprites(void)
{
	/* Unused: gives the stack frame its 36 bytes, as The Broken Seal's. */
	ACTOR_CAMERA_POINT	unusedPoints[3];
	SPR_PLACEMENT		point;
	SPR_SCALE			scale;
	Sint32				objectIndex;
	MAP_STATE		   *map;
	Sint32				viewX;
	Sint32				viewZ;
	ACTOR_SYSTEM_STATE *system;
	ACTOR			   *actor;
	Uint32				kind;
	FIXED				dx;
	FIXED				dz;
	Sint32				originY;
	SPR				   *sprite;
	FIXED				screenTop;
	Uint8			   *priorityFlags;
	Sint32				layer;
	Uint8			   *terrainId;
	MAP_METATILE	   *metatiles;
	FIXED				feetZ;
	Sint32				objectHeight;
	FIXED				probeX;
	Sint32				priority;
	Sint32				leftPriority;
	Sint32				rightPriority;
	Sint32				middlePriority;
	Sint32				height;
	Sint32				rise;
	Uint32				layerPlusOne;
	Sint8			   *basePriority;

	map = RamHeapSlots->mapGrid;
	viewX = syMapStateView(map)[0] & FIELD_PIXEL_MASK;
	viewZ = syMapStateView(map)[1] & FIELD_PIXEL_MASK;
	system = RamHeapSlots->actorSystem;
	originY = map->origin[1];
	btRenderLoadDecoder();
	system->used = 0;
	actor = RamHeapSlots->actors;
	for(objectIndex = 0; objectIndex < ACTOR_COUNT; objectIndex++, actor++) {
		if(actor->script == NULL)
			continue;
		if(actor->x.fixed != 0 || actor->z.fixed != 0) {
			kind = actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK;
			if(kind == ACTOR_SPRITE_NONE)
				continue;
			if(kind != ACTOR_SPRITE_ONE)
				continue;
			if(system->suspended != 0 &&
				!(actor->retainSpriteCache & ACTOR_KEEP_SPRITE_VRAM)) {
				SPR *frozen = actor->sprite;

				syVRAMCacheReleaseVRAM(frozen->slot);
				frozen->dirty = kind;
				continue;
			}
			sprite = actor->sprite;
			dx = actor->x.fixed - viewX;
			dz = actor->z.fixed - viewZ;
			screenTop = dz - actor->y.fixed;
			if(dx > -toFIXED(32) && dx < toFIXED(272) &&
				screenTop > -toFIXED(32) && screenTop < toFIXED(224) &&
				/* The ROM keeps &actor->priorityFlags in a stack slot of its
				   own, ordered among the declared locals' slots and above the
				   slot the loop body's first statements create. Reload orders
				   slots by pseudo number, so the source declared a local
				   holding this address; its type, name and spelling here are
				   not known. */
				(priorityFlags = &actor->priorityFlags,
					!(*priorityFlags & ACTOR_PRIORITY_NOT_DRAWN))) {
				/* &actor->layer in a declared local, by the same stack-slot
				   evidence as priorityFlags above; its type and name are not
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
				if(*priorityFlags & ACTOR_PRIORITY_AUTOMATIC) {
					probeX = actor->x.fixed - toFIXED(7);
					feetZ = actor->z.fixed + toFIXED(2);
					objectHeight = FIXEDtoINT(actor->groundHeight);
					priority = 0;
					leftPriority = 0;
					rightPriority = 0;
					if(!(*priorityFlags & ACTOR_PRIORITY_IGNORES_RISES)) {
						/* &map->basePriority in a declared local, by the same
						   evidence:
						   set inside this arm, yet numbered before the loop
						   body's first statements, so declared at function
						   level or at the loop body's head. Its type and name
						   are not known. */
						basePriority = &map->basePriority;
						priority = *basePriority;
						if(fdMapGridGetTerrainHeight(layer, probeX, feetZ) ==
							actor->groundHeight) {
							leftPriority =
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
								priority = map->basePriority + 1;
						}
						probeX += toFIXED(13);
						if(fdMapGridGetTerrainHeight(layer, probeX, feetZ) ==
							actor->groundHeight) {
							rightPriority =
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
								priority = map->basePriority + 1;
						}
						probeX -= toFIXED(6);
						middlePriority =
							METATILE_PRIORITY(metatiles, probeX, feetZ);
						height = FIXEDtoINT(fdMapGridGetTerrainHeight(layer,
							probeX, feetZ + toFIXED(MAPGRID_METATILE_PIXELS)));
						rise =
							FIXEDtoINT(fdMapGridGetTerrainHeight(layer, probeX,
								feetZ + toFIXED(2 * MAPGRID_METATILE_PIXELS))) -
							MAPGRID_METATILE_PIXELS;
						if(rise > height)
							height = rise;
						if(height > objectHeight)
							priority = map->basePriority + 1;
					} else {
						if(actor->groundHeight ==
							fdMapGridGetTerrainHeight(layer, probeX, feetZ))
							leftPriority =
								METATILE_PRIORITY(metatiles, probeX, feetZ);
						probeX += toFIXED(13);
						if(actor->groundHeight ==
							fdMapGridGetTerrainHeight(layer, probeX, feetZ))
							rightPriority =
								METATILE_PRIORITY(metatiles, probeX, feetZ);
						probeX -= toFIXED(6);
						middlePriority =
							METATILE_PRIORITY(metatiles, probeX, feetZ);
					}
					/* An odd priority is preferred: the left's, the right's,
					   the middle's. */
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
						actor->spriteStorageKind == ACTOR_SPRITE_ONE) {
						SPR *placed = actor->sprite;

						if((Uint32)priority > 3)
							priority = 3;
						placed->image.priority = priority;
						placed->shadow.priority = priority;
					}
				}
				layerPlusOne =
					METATILE_LAYER(metatiles, actor->x.fixed, actor->z.fixed);
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
					fdRenderApplyProjectedPlacementAtDepth(sprite, &point,
						scale.values, actor->facing, actor->sceneByte);
				else
					fdRenderApplyProjectedPlacement(sprite,
						sySpritePlacementValues(&point), scale.values,
						actor->facing);
				continue;
			}
			if(actor->retainSpriteCache & ACTOR_COMPOSE_OFF_SCREEN) {
				fdSpriteComposePicture(sprite, actor->facing);
			} else if(!(actor->retainSpriteCache & ACTOR_KEEP_SPRITE_VRAM) &&
				!sprite->retainCacheEntry) {
				syVRAMCacheReleaseVRAM(sprite->slot);
				sprite->dirty = 1;
			}
		} else {
			/* At (0, 0): not placed; only the sprite's VRAM is given up. */
			kind = actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK;
			if(kind == ACTOR_SPRITE_ONE) {
				SPR *frozen = actor->sprite;

				if(actor->retainSpriteCache == 0 && !frozen->retainCacheEntry) {
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

/* Unused: returns 1. */
UNUSED Sint32 fdActorCameraReturnTrue(void)
{
	return 1;
}

/* Places an actor's sprite (unless ACTOR_PRIORITY_NOT_DRAWN hides it) or each
   of its group's four sprites, turned by angle. While battle commands are
   chosen (FLAG_CHOOSING_COMMANDS) the sprites are drawn at 1.0 scale. */
void fdActorPlaceSprites(ACTOR *actor, Sint32 angle)
{
	const Sint32 *scale = &actor->scaleX;
	Sint32		 *point;
	void		 *sprite;
	void		**sprites;
	Sint32		  remaining;

	if(syGameFlagGet(FLAG_CHOOSING_COMMANDS))
		scale = ActorSpriteFullScale;
	switch(actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) {
	case ACTOR_SPRITE_ONE:
		point = &actor->x.fixed;
		sprite = actor->sprite;
		if(!(actor->priorityFlags & ACTOR_PRIORITY_NOT_DRAWN))
			fdRenderPlaceProjectedSprite(
				sprite, point, scale, actor->facing + angle, actor->layer);
		break;
	case ACTOR_SPRITE_GROUP:
		sprites = actor->sprite;
		for(remaining = 0; remaining < 4; remaining++) {
			sprite = *sprites++;
			if(sprite != NULL)
				fdRenderPlaceProjectedSprite(sprite, &actor->x.fixed, scale,
					actor->facing + angle, actor->layer);
		}
		break;
	case ACTOR_SPRITE_NONE:
		break;
	}
}

/* Each tick, with the fixed camera: places every actor in use, from the 64th
   down. While battle commands are chosen (FLAG_CHOOSING_COMMANDS) it uses the
   fixed view matrix and turns the objects by an eighth of a turn less. */
void fdActorSystemProjectSprites(void)
{
	ACTOR_CAMERA_FIXED *camera = RamHeapSlots->cameraState;
	ACTOR_SYSTEM_STATE *system = RamHeapSlots->actorSystem;
	ACTOR_CAMERA_POINT *eye;
	ACTOR_CAMERA_POINT *target;
	ACTOR			   *actor;
	Sint32				angle;
	Sint32				i;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	ACTOR_CAMERA_POINT unused;

	btRenderLoadDecoder();
	eye = &camera->eye;
	target = &camera->target;
	if(camera->followEye != NULL)
		eye = camera->followEye;
	if(camera->followPos != NULL)
		target = camera->followPos;
	angle = (Sint16)syAtan(
		FIXEDtoINT(eye->x - target->x), FIXEDtoINT(eye->z - target->z));
	system->used = 0;
	if(syGameFlagGet(FLAG_CHOOSING_COMMANDS)) {
		syInitMatrix();
		angle -= DEGtoANG(45);
		syMultiMatrixEntry(CameraFixedViewMatrix);
		syLookAt((Sint32 *)eye, (Sint32 *)target);
	} else {
		syMatrixRewindStack();
		syLookAtLoad((Sint32 *)eye, (Sint32 *)target);
	}
	actor = RamHeapSlots->actors;
	actor += ACTOR_COUNT - 1;
	for(i = ACTOR_COUNT - 1; i >= 0; i--, actor--) {
		/* The upper half of a script's address in ROM is never zero. */
		if(((Uint16 *)&actor->script)[1] != 0)
			fdActorPlaceSprites(actor, angle);
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

/* SC_HIDE_SPRITES: clears spriteStorageKind, hiding the sprites, and moves on
   to the next command. */
Sint32 fdActorScriptHideSprites(ACTOR *interpreter)
{
	interpreter->spriteStorageKind = ACTOR_SPRITE_NONE;
	interpreter->scriptCursor++;
	return 1;
}

/* SC_SHOW_SPRITE: shows the actor's one sprite again (ACTOR_SPRITE_ONE) and
   moves on to the next command. */
Sint32 fdActorScriptShowSprite(ACTOR *interpreter)
{
	interpreter->spriteStorageKind = ACTOR_SPRITE_ONE;
	interpreter->scriptCursor++;
	return 1;
}

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
