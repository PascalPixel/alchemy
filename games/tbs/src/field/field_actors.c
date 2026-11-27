/* Actors: creating and releasing them, dissolving sprites, starting scripts.
   Part of field.c. */

#include "field_internal.h"

/* A dissolve clears one pixel of each 8-bit tile a step, in
   SpriteDissolve_PixelOrder's order, so a tile is gone after this many. */
#define DISSOLVE_TILE_PIXELS  64
/* The step the first tile starts to dissolve at; each tile after it starts a
   step sooner. */
#define DISSOLVE_FIRST_STEP	  64
/* The steps a dissolve takes, four a frame. */
#define DISSOLVE_STEPS		  (DISSOLVE_FIRST_STEP + DISSOLVE_TILE_PIXELS)
/* How much further along the pixel order each tile reads than the tile before,
   besides the step it is ahead. */
#define DISSOLVE_ORDER_STRIDE 16
/* Keeps a pixel pair's odd pixel, its high byte, clearing the even one. */
#define PIXEL_PAIR_ODD_MASK	  0xff00

/* One step of dissolving a sprite pixel by pixel. */
void fdSprDissolveStep(Uint8 *rawSprite, Uint32 step)
{
	Uint16		*tile;
	Sint32		 tileCount;
	Uint32		 i;
	const Uint8 *order;

	SPR *sprite = (SPR *)rawSprite;

	tile = (Uint16 *)(OBJ_VRAM0 + VramCacheEntries[sprite->slot].ofs);
	tileCount = (sprite->width * sprite->height) / DISSOLVE_TILE_PIXELS;
	i = 0;

	if(i < tileCount) {
		order = SpriteDissolve_PixelOrder;
		{
			Uint32 highMask = PIXEL_PAIR_ODD_MASK;
			Uint32 indexMask = DISSOLVE_TILE_PIXELS - 1;

			for(; i < tileCount; i++, tile += TILE_SIZE_8BPP / 2, step++) {
				if((step - DISSOLVE_FIRST_STEP) <= DISSOLVE_TILE_PIXELS - 1) {
					Uint8	pixel;
					Uint16 *pixelPair;
					Uint32	isOdd;

					/* Each tile reads the order DISSOLVE_ORDER_STRIDE entries
					   and a step on from the tile before, so neighbouring tiles
					   lose different pixels at the same step. */
					pixel =
						order[(step + i * DISSOLVE_ORDER_STRIDE) & indexMask];
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

/* Dissolves a sprite pixel by pixel, four steps a frame, over 32 frames. */
void fdSprDissolveRun(Uint8 *sprite)
{
	Uint32 step;

	step = 0;
	/* Finish all four dissolve steps before waiting for the next video
	   frame. */
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
void fdSprDissolveRunMany(Uint8 **sprites, Sint32 count)
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

/* Writes the actor's screen position, in pixels, to screenPosition[0..1] and
   returns 0 when it is on or near the screen; otherwise writes 0, 0 and returns
   -1. */
Sint32 fdMapGridGetScreenRelativePos(ACTOR *actor, Sint32 *screenPosition)
{
	MAP_GRID_SCROLL_STATE *grid = MapGridState[0];
	Sint32				   viewX;
	Sint32				   viewZ;
	FIXED				   x;
	FIXED				   y;

	viewX = syMapGridScrollStateView(grid)[0] & FIELD_PIXEL_MASK;
	viewZ = syMapGridScrollStateView(grid)[1] & FIELD_PIXEL_MASK;
	x = actor->x.fixed - viewX;
	y = actor->z.fixed - viewZ;
	/* Across, from 32 pixels left of the screen to 32 past its 240; down, below
	   the view's top and above 224. */
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

/* The colour an outlined sprite's edge is drawn in (fdSprComposePicture,
   SPRITE_FLAG_OUTLINED): the sprite palette's white, colour 15, which
   SPRITE_COLOR_WHITE paints with too. */
#define OUTLINE_EDGE_COLOR 15

/* Sets up the actor system: allocates and zeroes the 64 actors and registers
   the per-frame updates. */
void fdActorSystemInit(Sint32 controlMode)
{
	ACTOR_SYSTEM_STATE *system;
	ACTOR			   *pool;
	volatile Uint32		fill;

	system = syHeapSlotAllocEWRAM(HEAP_SLOT_ACTOR_SYSTEM, sizeof(*system));
	pool = syHeapSlotAllocEWRAM(HEAP_SLOT_ACTORS, ACTOR_COUNT * sizeof(*pool));
	fdActorSystemConfigure(controlMode);
	DMA_FILL32_CELL(fill, 0, pool, ACTOR_COUNT * sizeof(*pool));
	DMA_FILL32_CELL(fill, 0, system, sizeof(*system));
	/* The event runner runs higher priorities first: the actors move before the
	   camera follows them. */
	if(controlMode == ACTOR_SYSTEM_BATTLE)
		sySetEvent(fdActorUpdateAllMotion, EVENT_PASS_FRAME + 10);
	else
		sySetEvent(fdActorUpdateAllOnField, EVENT_PASS_FRAME + 10);
	if(controlMode >= ACTOR_SYSTEM_WORLD_MAP &&
		controlMode <= ACTOR_SYSTEM_BATTLE) {
		sySetEvent(fdActorSystemProjectSprs, EVENT_PASS_FRAME);
	} else {
		sySetEvent(fdActorSystemPlaceSprs, EVENT_PASS_FRAME);
		Camera_UnreadWordB = 0;
		Camera_UnreadWordA = 0;
	}
	system->edge = OUTLINE_EDGE_COLOR;
	system->fill = 0;
}

/* Unused: two empty functions. */
UNUSED void fdActorSystemReservedNoOperationA(void)
{
}

UNUSED void fdActorSystemReservedNoOperationB(void)
{
}

/* Returns the first of the 64 actors not in use (with no script), or 0. */
void *fdActorFindFree(void)
{
	ACTOR *actor = ActorsWork;
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
		DMA_FILL32(0, actor, sizeof(ACTOR));
	}
}

/* Creates an actor at (x, y, z), in 16.16 pixels, or returns 0 when every slot
   is in use. encodedSprite / 4096 is its ACTOR_CREATE and the low 12 bits its
   sprite. */
ACTOR *fdActorCreate(Sint32 encodedSprite, FIXED x, FIXED y, FIXED z)
{
	ACTOR *actor;
	SPR	  *sprite;
	Sint32 spriteStorageKind;
	void **spriteSlots;

	/* DEAD: a first search whose result is dropped; the ROM keeps its call. */
	fdActorFindFree();
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
					syResourceGetMetadataRecordStub(encodedSprite)->footprint >>
					1;
			} else {
				actor->sprStorageKind = ACTOR_SPRITE_NONE;
			}
			break;
		case ACTOR_CREATE_GROUP:
			/* Take a zeroed list of four sprites, and fill it with id and id +
			   1. */
			spriteSlots = (void **)ActorSystemStateWork->sprGroups +
				ActorSystemStateWork->cnt++;
			actor->sprStorageKind = spriteStorageKind;
			actor->spr = (SPR *)spriteSlots;
			DMA_FILL32(0, spriteSlots, 4 * sizeof(void *));
			sprite = fdSprCreate(encodedSprite);
			if(sprite != NULL) {
				actor->radius =
					syResourceGetMetadataRecordStub(encodedSprite)->footprint >>
					1;
				*spriteSlots++ = sprite;
			}
			sprite = fdSprCreate(encodedSprite + 1);
			if(sprite != NULL)
				*spriteSlots = sprite;
			break;
		}
	}
	if(actor != NULL) {
		fdActorSetPosAndResetMotion(actor, x, y, z);
		/* Up to 2 pixels a frame, gaining 1 a frame; unit scale; following the
		   ground and falling; turning to face its motion; keeping a quarter of
		   its speed when it bounces; facing down the screen (FACING_SOUTH, the
		   heading WorldMap_KeyHeadings gives Down). */
		actor->script = (Sint32 *)Actor_DefaultScript;
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
		actor->sceneWord = 0;
		actor->facing = FACING_SOUTH;
		/* Its home position, in pixels. */
		actor->homeX = x / toFIXED(1);
		actor->homeZ = z / toFIXED(1);
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
		actor->repeatCnt = 0;
		actor->conditionResult = 0;
	}
}

/* Defines name(actor, value), which calls apply(sprite, value) on an actor's
   one sprite, or on each sprite of its group. */
#define ACTOR_DEFINE_APPLY(name, actor_type, apply) \
	void name(actor_type rawActor, Sint32 value) \
	{ \
		ACTOR *actor; \
		void **sprites; \
		Sint32 part; \
		void  *sprite; \
\
		actor = rawActor; \
		if(actor != 0) { \
			switch(actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) { \
			case ACTOR_SPRITE_ONE: \
				apply(actor->spr, value); \
				break; \
			case ACTOR_SPRITE_GROUP: \
				sprites = (void **)actor->spr; \
				for(part = 0; part < 4; part++) { \
					sprite = *sprites++; \
					if(sprite != 0) \
						apply(sprite, value); \
				} \
				break; \
			} \
		} \
	}

/* Switches all of an actor's sprites to animation value. */
ACTOR_DEFINE_APPLY(fdActorSelectAnimation, void *, fdSprStartAnimation)

/* Sets the animation speed (timer step) of all of an actor's sprites. */
ACTOR_DEFINE_APPLY(fdActorSetAnimationSpeed, ACTOR *, fdSprSetAnimationSpeed)

/* Both of the above: switches all of an actor's sprites to animation and sets
   their timer step to step. */
void fdActorSetAnimationAndSpeed(ACTOR *actor, Sint32 animation, Sint32 step)
{
	void **sprites;
	void  *sprite;
	Sint32 part;

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
void fdActorSetSprRotation(ACTOR *actor, Uint32 rotation)
{
	if(actor != 0 &&
		(actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE)
		actor->spr->rotation = rotation;
}

/* Gives a one-sprite actor's first sprite animation the animation animationId
   and restarts its animations. A negative animationId does nothing. */
void fdActorSetFirstSprAnimation(ACTOR *actor, Sint32 animationId)
{
	SPR *sprite;

	if(actor != NULL &&
		(actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE) {
		sprite = actor->spr;
		if(animationId >= 0) {
			sprite->entries[0]->anmId = animationId;
			fdSprResetAnimations(sprite);
		}
	}
}

/* Adds a layer playing animationId to a one-sprite actor's sprite. A negative
   animationId does nothing. */
void fdActorRegisterSprMetadata(ACTOR *actor, Sint32 animationId)
{
	if(actor != 0 &&
		(actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE) {
		SPR *sprite = actor->spr;
		if(animationId >= 0)
			fdSprAddAnimation(sprite, animationId);
	}
}

/* Starts an actor on the script that approaches its watched actor
   (SC_APPROACH_WATCHED), watching watchedActorHandle. */
void fdActorStartApproachWatched(ACTOR *actor, Sint32 watchedActorHandle)
{
	fdActorStartScript(actor, Actor_ApproachWatchedScript);
	actor->watchedActor = (union ACTOR_OR_EFFECT *)watchedActorHandle;
}

/* Starts an actor on the script that runs SC_KEY_COUNT every frame. */
void fdActorStartKeyCheck(ACTOR *actor)
{
	fdActorStartScript(actor, Actor_KeyCheckScript);
}

/* Starts an actor on the script that moves it by the keys (SC_KEYS). The field
   mode gives it to the leader while he climbs (LEADER_MOVEMENT_CLIMB). */
void fdActorStartMoveByKeys(ACTOR *actor)
{
	fdActorStartScript(actor, Actor_KeysScript);
}

/* Starts an actor on the player control script (SC_PLAYER). The field mode's
   usual choice for the leader on a layered map. */
void fdActorStartPlayerCtrl(ACTOR *actor)
{
	fdActorStartScript(actor, Actor_PlayerScript);
}

/* Starts an actor on the World Map control script (SC_WORLD). The field mode
   gives it to the leader on the World Map. */
void fdActorStartWorldMapCtrl(ACTOR *actor)
{
	fdActorStartScript(actor, Actor_WorldMapScript);
}

/* Starts an actor on the idle script (a lone SC_END). */
void fdActorStartIdle(ACTOR *actor)
{
	fdActorStartScript(actor, Actor_IdleScript);
}

/* Starts an actor on the camera script (SC_CAMERA). With a leader to follow, it
   also gets a speed limit of 4 pixels a frame and an acceleration of 0.5. */
void fdActorStartCamera(ACTOR *actor, Sint32 watchedActorHandle)
{
	fdActorStartScript(actor, Actor_CameraScript);
	if(watchedActorHandle != 0) {
		actor->acceleration = toFIXED(1) / 2;
		actor->speedLimit = toFIXED(4);
		actor->watchedActor = (union ACTOR_OR_EFFECT *)watchedActorHandle;
		actor->tmr = 0;
	}
}

/* Waits until the actor's script reaches SC_END, at most 300 frames (five
   seconds); it returns as soon as the script idles, and after the limit whether
   or not it has. */
void fdActorWaitForScriptEnd(void *rawActor)
{
	ACTOR *interpreter = rawActor;
	Sint32 waitedFrames;

	waitedFrames = 0;
	if(interpreter->script[interpreter->scriptCursor] != SC_END) {
		do {
			sySynch(1);
			waitedFrames++;
			if(waitedFrames > 299)
				break;
		} while(interpreter->script[interpreter->scriptCursor] != SC_END);
	}
}

/* Sets the flags of a one-sprite actor's sprite (bit 0: draw its shadow). */
void fdActorSetSprFlags(ACTOR *actor, Uint32 flags)
{
	if(actor != NULL) {
		if((actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE)
			actor->spr->flg = flags;
	}
}

/* Sets the OAM blend mode of a one-sprite actor's sprite. */
void fdActorSetBlendMode(ACTOR *actor, Uint32 mode)
{
	if(actor != 0 && actor->sprStorageKind == ACTOR_SPRITE_ONE) {
		SPR *sprite = actor->spr;
		sprite->image.objMode = mode;
	}
}

/* Makes the fixed camera draw a one-sprite actor at its own scale instead of
   scaling it by distance. */
void fdActorSetSprScale(ACTOR *actor, Uint32 fixedScale)
{
	if(actor != 0 && actor->sprStorageKind == ACTOR_SPRITE_ONE) {
		SPR *sprite = actor->spr;
		sprite->useOwnScale = fixedScale;
	}
}

/* Sets the colour mode (SPR_COL_MODE) of a one-sprite actor's sprite
   animations, except those drawn hidden (see fdSprSetAnimationColModes). */
void fdActorSetSprColMode(ACTOR *actor, Uint32 colorMode)
{
	if(actor != NULL && actor->sprStorageKind == ACTOR_SPRITE_ONE)
		fdSprSetAnimationColModes(actor->spr, colorMode);
}

/* Suspends the actor camera updates, starts the palette back to normal and, a
   frame later, shows the sprites with BG1-3 hidden, leaving BG0 unchanged. */
void fdActorSystemSuspendProjectionAndShowSprs(void)
{
	sySuspendEvent(fdActorSystemPlaceSprs);
	sySuspendEvent(fdActorSystemProjectSprs);
	fdPaletteFadeSetTargetStub(PALETTE_TINT_NONE, 1);
	fdPaletteFadeStartInterpolationStub(1);
	sySynch(1);
	REG_DISPCNT =
		(REG_DISPCNT & ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON)) |
		DISPCNT_OBJ_ON;
}

/* Resumes the actor camera updates and hides BG1-3 and the sprites. */
void fdActorSystemResumeProjectionAndHideLayers(void)
{
	syResumeEvent(fdActorSystemPlaceSprs);
	syResumeEvent(fdActorSystemProjectSprs);
	REG_DISPCNT &=
		~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON | DISPCNT_OBJ_ON);
}

/* Unused: returns 1. */
UNUSED Sint32 fdActorReturnTrue(void)
{
	return 1;
}

#undef ACTOR_DEFINE_APPLY
