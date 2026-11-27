/* Actors: creating and releasing them, dissolving sprites, starting scripts.
   Part of field.c. */

#include "field_internal.h"

/* A dissolve clears one pixel of each 8-bit tile a step, in
   SpriteDissolvePixelOrder's order, so a tile is gone after this many. */
#define DISSOLVE_TILE_PIXELS  64
/* The step the first tile starts to dissolve at; each tile after it starts a
   step sooner. */
#define DISSOLVE_FIRST_STEP	  64
/* The steps a dissolve takes, four a tick. */
#define DISSOLVE_STEPS		  (DISSOLVE_FIRST_STEP + DISSOLVE_TILE_PIXELS)
/* How much further along the pixel order each tile reads than the tile before,
   besides the step it is ahead. */
#define DISSOLVE_ORDER_STRIDE 16
/* Keeps a pixel pair's odd pixel, its high byte, clearing the even one. */
#define PIXEL_PAIR_ODD_MASK	  0xff00

/* One step of dissolving a sprite pixel by pixel. */
void fdSprDissolveStep(Uint8 *rawSpr, Uint32 step)
{
	Uint16		*tile;
	Sint32		 tileCnt;
	Uint32		 i;
	const Uint8 *order;

	SPR *spr = (SPR *)rawSpr;

	tile = (Uint16 *)(OBJ_VRAM0 + VramCacheEntries[spr->slot].ofs);
	tileCnt = (spr->width * spr->height) / DISSOLVE_TILE_PIXELS;
	i = 0;

	if(i < tileCnt) {
		order = SpriteDissolvePixelOrder;
		{
			Uint32 highMask = PIXEL_PAIR_ODD_MASK;
			Uint32 idxMask = DISSOLVE_TILE_PIXELS - 1;

			for(; i < tileCnt; i++, tile += TILE_SIZE_8BPP / 2, step++) {
				if((step - DISSOLVE_FIRST_STEP) <= DISSOLVE_TILE_PIXELS - 1) {
					Uint8	pixel;
					Uint16 *pixelPair;
					Uint32	isOdd;

					/* Each tile reads the order DISSOLVE_ORDER_STRIDE entries
					   and a step on from the tile before, so neighbouring tiles
					   lose different pixels at the same step. */
					pixel = order[(step + i * DISSOLVE_ORDER_STRIDE) & idxMask];
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
		*screenPos++ = FIXEDtoINT(x);
		*screenPos = FIXEDtoINT(y);
		return 0;
	}
	*screenPos++ = 0;
	*screenPos = 0;
	return -1;
}

/* The colour an outlined sprite's edge is drawn in (fdSprComposePicture,
   SPRITE_FLAG_OUTLINED): the sprite palette's white, colour 15, which
   SPRITE_COLOR_WHITE paints with too. */
#define OUTLINE_EDGE_COLOR 15

/* Sets up the actor system: allocates and zeroes the 64 actors and registers
   the per-tick updates. */
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
		sySetEvent(fdActorUpdateAllMotion, EVENT_PASS_TICK + 10);
	else
		sySetEvent(fdActorUpdateAllOnField, EVENT_PASS_TICK + 10);
	if(controlMode >= ACTOR_SYSTEM_WORLD_MAP &&
		controlMode <= ACTOR_SYSTEM_BATTLE) {
		sySetEvent(fdActorSystemProjectSprs, EVENT_PASS_TICK);
	} else {
		sySetEvent(fdActorSystemPlaceSprs, EVENT_PASS_TICK);
		CameraUnreadWordB = 0;
		CameraUnreadWordA = 0;
	}
	system->edge = OUTLINE_EDGE_COLOR;
	system->fill = 0;
}

#include "field/actor_system_reserved.c"

/* Returns the first of the 64 actors not in use (with no script), or 0. */
void *fdActorFindFree(void)
{
	ACTOR *actor = ActorsWork;
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
   is in use. encodedSpr / 4096 is its ACTOR_CREATE and the low 12 bits its
   sprite. */
ACTOR *fdActorCreate(Sint32 encodedSpr, FIXED x, FIXED y, FIXED z)
{
	ACTOR *actor;
	SPR	  *spr;
	Sint32 sprStorageKind;
	void **sprSlots;

	/* DEAD: a first search whose result is dropped; the ROM keeps its call. */
	fdActorFindFree();
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
					syResourceGetMetadataRecordStub(encodedSpr)->footprint >> 1;
			} else {
				actor->sprStorageKind = ACTOR_SPRITE_NONE;
			}
			break;
		case ACTOR_CREATE_GROUP:
			/* Take a zeroed list of four sprites, and fill it with id and id +
			   1. */
			sprSlots = (void **)ActorSystemStateWork->sprGroups +
				ActorSystemStateWork->cnt++;
			actor->sprStorageKind = sprStorageKind;
			actor->spr = (SPR *)sprSlots;
			DMA_FILL32(0, sprSlots, 4 * sizeof(void *));
			spr = fdSprCreate(encodedSpr);
			if(spr != NULL) {
				actor->radius =
					syResourceGetMetadataRecordStub(encodedSpr)->footprint >> 1;
				*sprSlots++ = spr;
			}
			spr = fdSprCreate(encodedSpr + 1);
			if(spr != NULL)
				*sprSlots = spr;
			break;
		}
	}
	if(actor != NULL) {
		fdActorSetPosAndResetMotion(actor, x, y, z);
		/* Up to 2 pixels a tick, gaining 1 a tick; unit scale; following the
		   ground and falling; turning to face its motion; keeping a quarter of
		   its speed when it bounces; facing down the screen (FACING_SOUTH, the
		   heading WorldMapKeyHeadings gives Down). */
		actor->script = (Sint32 *)ActorDefaultScript;
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

#include "field/actor_start_script.c"

/* Defines name(actor, value), which calls apply(sprite, value) on an actor's
   one sprite, or on each sprite of its group. */
#define ACTOR_DEFINE_APPLY(name, actor_type, apply) \
	void name(actor_type rawActor, Sint32 value) \
	{ \
		ACTOR *actor; \
		void **sprs; \
		Sint32 part; \
		void  *spr; \
\
		actor = rawActor; \
		if(actor != 0) { \
			switch(actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) { \
			case ACTOR_SPRITE_ONE: \
				apply(actor->spr, value); \
				break; \
			case ACTOR_SPRITE_GROUP: \
				sprs = (void **)actor->spr; \
				for(part = 0; part < 4; part++) { \
					spr = *sprs++; \
					if(spr != 0) \
						apply(spr, value); \
				} \
				break; \
			} \
		} \
	}

/* Switches all of an actor's sprites to animation value. */
ACTOR_DEFINE_APPLY(fdActorSelectAnimation, void *, fdSprStartAnimation)

/* Sets the animation speed (timer step) of all of an actor's sprites. */
ACTOR_DEFINE_APPLY(fdActorSetAnimationSpeed, ACTOR *, fdSprSetAnimationSpeed)

#include "field/actor_set_animation_and_speed.c"

/* Sets the rotation of an actor's sprite, when it has just one. */
void fdActorSetSprRotation(ACTOR *actor, Uint32 rotation)
{
	if(actor != 0 &&
		(actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE)
		actor->spr->rotation = rotation;
}

/* Gives a one-sprite actor's first sprite animation the animation anmId
   and restarts its animations. A negative anmId does nothing. */
void fdActorSetFirstSprAnimation(ACTOR *actor, Sint32 anmId)
{
	SPR *spr;

	if(actor != NULL &&
		(actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE) {
		spr = actor->spr;
		if(anmId >= 0) {
			spr->entries[0]->anmId = anmId;
			fdSprResetAnimations(spr);
		}
	}
}

#include "field/actor_register_spr_metadata.c"

/* Starts an actor on the script that approaches its watched actor
   (SC_APPROACH_WATCHED), watching watchedActorHandle. */
void fdActorStartApproachWatched(ACTOR *actor, Sint32 watchedActorHandle)
{
	fdActorStartScript(actor, ActorApproachWatchedScript);
	actor->watchedActor = (union ACTOR_OR_EFFECT *)watchedActorHandle;
}

/* Starts an actor on the script that runs SC_KEY_COUNT every tick. */
void fdActorStartKeyCheck(ACTOR *actor)
{
	fdActorStartScript(actor, ActorKeyCheckScript);
}

/* Starts an actor on the script that moves it by the keys (SC_KEYS). The field
   mode gives it to the leader while he climbs (LEADER_MOVEMENT_CLIMB). */
void fdActorStartMoveByKeys(ACTOR *actor)
{
	fdActorStartScript(actor, ActorKeysScript);
}

/* Starts an actor on the player control script (SC_PLAYER). The field mode's
   usual choice for the leader on a layered map. */
void fdActorStartPlayerCtrl(ACTOR *actor)
{
	fdActorStartScript(actor, ActorPlayerScript);
}

/* Starts an actor on the World Map control script (SC_WORLD). The field mode
   gives it to the leader on the World Map. */
void fdActorStartWorldMapCtrl(ACTOR *actor)
{
	fdActorStartScript(actor, ActorWorldMapScript);
}

/* Starts an actor on the idle script (a lone SC_END). */
void fdActorStartIdle(ACTOR *actor)
{
	fdActorStartScript(actor, ActorIdleScript);
}

/* Starts an actor on the camera script (SC_CAMERA). With a leader to follow, it
   also gets a speed limit of 4 pixels a tick and an acceleration of 0.5. */
void fdActorStartCamera(ACTOR *actor, Sint32 watchedActorHandle)
{
	fdActorStartScript(actor, ActorCameraScript);
	if(watchedActorHandle != 0) {
		actor->acceleration = toFIXED(1) / 2;
		actor->speedLimit = toFIXED(4);
		actor->watchedActor = (union ACTOR_OR_EFFECT *)watchedActorHandle;
		actor->tmr = 0;
	}
}

/* Waits until the actor's script reaches SC_END, at most 300 ticks (five
   seconds); it returns as soon as the script idles, and after the limit whether
   or not it has. */
void fdActorWaitForScriptEnd(void *rawActor)
{
	ACTOR *interpreter = rawActor;
	Sint32 waitedTicks;

	waitedTicks = 0;
	if(interpreter->script[interpreter->scriptCursor] != SC_END) {
		do {
			sySynch(1);
			waitedTicks++;
			if(waitedTicks > 299)
				break;
		} while(interpreter->script[interpreter->scriptCursor] != SC_END);
	}
}

/* Sets the flags of a one-sprite actor's sprite (bit 0: draw its shadow). */
void fdActorSetSprFlags(ACTOR *actor, Uint32 flg)
{
	if(actor != NULL) {
		if((actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE)
			actor->spr->flg = flg;
	}
}

#include "field/actor_set_blend_mode.c"

/* Makes the fixed camera draw a one-sprite actor at its own scale instead of
   scaling it by distance. */
void fdActorSetSprScale(ACTOR *actor, Uint32 fixedScale)
{
	if(actor != 0 && actor->sprStorageKind == ACTOR_SPRITE_ONE) {
		SPR *spr = actor->spr;
		spr->useOwnScale = fixedScale;
	}
}

/* Sets the colour mode (SPR_COL_MODE) of a one-sprite actor's sprite
   animations, except those drawn hidden (see fdSprSetAnimationColModes). */
void fdActorSetSprColMode(ACTOR *actor, Uint32 colMode)
{
	if(actor != NULL && actor->sprStorageKind == ACTOR_SPRITE_ONE)
		fdSprSetAnimationColModes(actor->spr, colMode);
}

/* Suspends the actor camera updates, starts the palette back to normal and, a
   tick later, shows the sprites with BG1-3 hidden, leaving BG0 unchanged. */
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

#include "field/actor_system_resume_projection.c"

#undef ACTOR_DEFINE_APPLY
