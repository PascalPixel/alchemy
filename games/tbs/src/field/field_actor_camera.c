/* The actor cameras, which place every actor's sprite each tick. Part of
   field.c. */

#include "field_internal.h"

/* A map metatile's word, as the camera reads it. */
typedef struct ACTOR_CAMERA_TILE {
	Uint32 block : 12; /* the number of the metatile's tiles */
	Uint32 layer : 2;  /* if not 0, the layer of an actor standing on it, + 1 */
	Uint32 pri : 2;	   /* if not 0, the priority of an actor standing on it */
	/* MAP_GRID_METATILE's metatile behavior and shape; not read here. */
	Uint32 behaviorAndCollision : 16;
} ACTOR_CAMERA_TILE;

/* A point the fixed camera looks from or at. */
typedef struct ACTOR_CAMERA_POINT {
	FIXED x; /* 16.16 fixed-point pixels */
	FIXED y; /* 16.16 fixed-point pixels */
	FIXED z; /* 16.16 fixed-point pixels */
} ACTOR_CAMERA_POINT;

/* The start of the 3D camera's state (heap slot HEAP_SLOT_CAMERA,
   BattleCameraWork, a BATTLE_CAMERA), as the fixed camera reads it: the eye and
   target it looks from and at, or the points that stand in for them. */
typedef struct ACTOR_CAMERA_FIXED {
	ACTOR_CAMERA_POINT	eye;
	ACTOR_CAMERA_POINT	tgt;
	ACTOR_CAMERA_POINT *followEye; /* when set, used as the eye */
	ACTOR_CAMERA_POINT *followPos; /* when set, used as the target */
} ACTOR_CAMERA_FIXED;

/* Each tick, with the scrolling camera: places every actor's sprite on the
   screen. */
void fdActorSystemPlaceSprs(void)
{
	/* LEFTOVER(frame): three unused points the ROM's frame reserves and never
	 * touches. */
	UNUSED ACTOR_CAMERA_POINT unusedPoints[3];
	Sint32					  actorIdx;
	MAP_GRID_SCROLL_STATE	 *grid;
	Sint32					  viewX;
	Sint32					  viewZ;
	ACTOR_SYSTEM_STATE		 *system;
	ACTOR					 *actor;
	SPR						 *spr;
	Uint32					  kind;
	FIXED					  dx;
	FIXED					  dz;
	FIXED					  screenTop;
	ACTOR_CAMERA_TILE		 *tile;
	Uint32					  pri;
	SPR_SCALE				  scale;
	SPR_PLACEMENT			  drawPos;
	Uint32					  layerPlusOne;

	/* MAP_GRID_SCROLL_STATE owns all three layer metatile pointers at their
	   48-byte stride. */
	grid = MapGridState[0];
	viewX = syMapGridScrollStateView(grid)[0] & FIELD_PIXEL_MASK;
	viewZ = syMapGridScrollStateView(grid)[1] & FIELD_PIXEL_MASK;
	system = ActorSystemStateWork;
	/* Copy the picture decoder (fdRenderDecodePicture) into its heap slot. */
	RAM_CODE_LOAD_SLOT(HEAP_SLOT_TICK_SCRATCH, fdRenderDecodePicture,
		RenderDecodePictureCodeSize);
	system->used = 0;
	actor = ActorsWork;
	for(actorIdx = 0; actorIdx < ACTOR_COUNT; actorIdx++, actor++) {
		if(actor->script == 0)
			continue;
		/* An actor at (x, z) = (0, 0) is not placed; only its sprite's VRAM is
		   given up. */
		if(actor->x.fixed != 0 || actor->z.fixed != 0) {
			kind = actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK;
			if(kind == ACTOR_SPRITE_NONE)
				continue;
			/* This camera places single sprites only; groups are left. */
			if(kind != ACTOR_SPRITE_ONE)
				continue;
			/* While a menu has sprite projection suspended, only held actors
			   are drawn. */
			if(system->suspended != 0 && actor->retainSprCache == 0) {
				SPR *frozen = actor->spr;

				syVRAMCacheReleaseVRAM(frozen->slot);
				frozen->dirty = kind;
				continue;
			}
			dx = actor->x.fixed - viewX;
			dz = actor->z.fixed - viewZ;
			screenTop = dz - actor->y.fixed;
			spr = actor->spr;
			/* From 32 pixels left of the screen to 32 past its 240, and from 32
			   pixels above it to 224. */
			if(dx > -toFIXED(32) && dx < toFIXED(272) &&
				screenTop > -toFIXED(32) && screenTop < toFIXED(224)) {
				/* The metatile under the actor: x and z >> 20 are 16-pixel
				   metatiles, and a row holds 128 of them. */
				tile = &((ACTOR_CAMERA_TILE *)grid->layers[actor->layer]
						.metatiles)[(FIELD_METATILE(actor->x.fixed)) +
					FIELD_METATILE(actor->z.fixed) * MAPGRID_SIZE];
				if(actor->priFlg & ACTOR_PRIORITY_AUTOMATIC) {
					pri = tile->pri;
					if(pri != 0) {
						spr->image.pri = pri;
						spr->shadow.pri = pri;
					}
				}
				layerPlusOne = tile->layer;
				if(layerPlusOne != 0)
					actor->layer = layerPlusOne - 1;
				scale.x = syMulFX(actor->scaleX, spr->scale);
				scale.y = syMulFX(actor->scaleY, spr->scale);
				drawPos.x = dx;
				drawPos.height = actor->y.fixed;
				drawPos.y = dz;
				drawPos.groundHeight = actor->groundHeight;
				/* ACTOR_PRIORITY_UNDERFOOT and _OVERHEAD move the point 320
				   pixels up or down the map with its height and ground height,
				   so the sprite and shadow stay where they are on screen; only
				   the draw slot changes, which fdRenderApplyProjectedPlacement
				   takes from the point's y before its height is taken off. */
				if(actor->priFlg & ACTOR_PRIORITY_UNDERFOOT) {
					drawPos.height += -toFIXED(320);
					drawPos.y += -toFIXED(320);
					drawPos.groundHeight += -toFIXED(320);
				}
				if(actor->priFlg & ACTOR_PRIORITY_OVERHEAD) {
					drawPos.height += toFIXED(320);
					drawPos.y += toFIXED(320);
					drawPos.groundHeight += toFIXED(320);
				}
				fdRenderApplyProjectedPlacement(
					spr, &drawPos, &scale, actor->facing);
				continue;
			}
			/* Off screen: the sprite's VRAM goes back to the cache, and dirty
			   makes it redraw when it comes back. */
			if(actor->retainSprCache == 0) {
				if(!spr->retainCacheEntry) {
					syVRAMCacheReleaseVRAM(spr->slot);
					spr->dirty = 1;
				}
			}
		} else {
			kind = actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK;
			if(kind == ACTOR_SPRITE_ONE) {
				spr = actor->spr;
				if(actor->retainSprCache == 0 && !spr->retainCacheEntry) {
					syVRAMCacheReleaseVRAM(spr->slot);
					spr->dirty = kind;
				}
			}
		}
	}
	syHeapSlotRelease(HEAP_SLOT_TICK_SCRATCH);
}

/* Unused: returns 1. */
UNUSED Sint32 fdActorCameraReturnTrue(void)
{
	return 1;
}

/* Each tick, with the fixed camera: places every actor's sprite, from the 64th
   actor down, facing them by the angle from the eye to the target. */
void fdActorSystemProjectSprs(void)
{
	Sint32				i;
	ACTOR_CAMERA_FIXED *camera;
	ACTOR_SYSTEM_STATE *system;
	ACTOR_CAMERA_POINT *eye;
	ACTOR_CAMERA_POINT *tgt;
	ACTOR			   *actor;
	Sint32				ang;
	Sint32				part;
	Sint32				kind;
	Sint32			   *pos;
	void			  **sprs;
	void			   *spr;
	FIXED				groupScale[2];
	FIXED				scale[2];
	/* LEFTOVER(frame): two unused points the ROM's frame reserves and never
	 * touches. */
	UNUSED ACTOR_CAMERA_POINT unusedPoints[2];

	camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
	system = ActorSystemStateWork;
	RAM_CODE_LOAD_SLOT(HEAP_SLOT_TICK_SCRATCH, fdRenderDecodePicture,
		RenderDecodePictureCodeSize);
	eye = &camera->eye;
	tgt = &camera->tgt;
	if(camera->followEye != NULL)
		eye = camera->followEye;
	if(camera->followPos != NULL)
		tgt = camera->followPos;
	/* The camera's heading over the ground, from the eye's offset from the
	   target in whole pixels; each sprite's facing is drawn relative to it, so
	   a sprite keeps its world heading as the camera turns. */
	ang = (Sint16)syAtan(
		FIXEDtoINT(eye->x - tgt->x), FIXEDtoINT(eye->z - tgt->z));
	system->used = 0;
	syInitMatrix();
	/* Under the fixed view matrix every sprite also faces an eighth of a turn
	   further back. */
	if(gmFlagGetStub(FLAG_CHOOSING_COMMANDS)) {
		ang += -(DEGtoANG(45));
		syMultiMatrix(CameraFixedViewMatrix);
		syLookAt((Sint32 *)eye, (Sint32 *)tgt);
	} else {
		syLookAtLoad((Sint32 *)eye, (Sint32 *)tgt);
	}
	/* The actors are placed from the last down. */
	actor = ActorsWork;
	actor += ACTOR_COUNT - 1;
	{
		FIXED fixedScale = toFIXED(1);

		for(i = ACTOR_COUNT - 1; i >= 0; i--, actor--) {
			if(actor->script == 0)
				continue;
			kind = actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK;
			switch(kind) {
			case ACTOR_SPRITE_ONE:
				pos = &actor->x.fixed;
				scale[0] = actor->scaleX;
				scale[1] = actor->scaleY;
				spr = actor->spr;
				if(gmFlagGetStub(FLAG_CHOOSING_COMMANDS)) {
					scale[0] = fixedScale;
					scale[1] = fixedScale;
				}
				fdRenderPlaceProjectedSpr(
					spr, pos, scale, actor->facing + ang, actor->layer);
				break;
			case ACTOR_SPRITE_GROUP:
				groupScale[0] = actor->scaleX;
				groupScale[1] = actor->scaleY;
				if(gmFlagGetStub(FLAG_CHOOSING_COMMANDS)) {
					groupScale[0] = fixedScale;
					groupScale[1] = fixedScale;
				}
				/* A group's sprite field points to its four sprite pointers;
				   empty ones are null. */
				sprs = (void **)actor->spr;
				for(part = 0; part < 4; part++) {
					spr = *sprs++;
					if(spr != NULL)
						fdRenderPlaceProjectedSpr(spr, &actor->x.fixed,
							groupScale, actor->facing + ang, actor->layer);
				}
				break;
			case ACTOR_SPRITE_NONE:
				break;
			}
		}
		syHeapSlotRelease(HEAP_SLOT_TICK_SCRATCH);
	}
}
