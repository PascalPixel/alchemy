/* @file */
/* The actor cameras, which place every actor's sprite each frame. */
/* Part of field.c. */

#include "field_internal.h"

/* A map cell's word, as the camera reads it. */
typedef struct ACTOR_CAMERA_TILE {
	u32 block : 12; /* the number of the cell's tiles */
	u32 layer : 2;	/* if not 0, the layer of an actor standing on it, + 1 */
	u32 pri : 2;	/* if not 0, the priority of an actor standing on it */
	/* MAP_GRID_CELL's cell behavior and shape; not read here. */
	u32 behaviorAndCollision : 16;
} ACTOR_CAMERA_TILE;

/* A point the fixed camera looks from or at. */
typedef struct ACTOR_CAMERA_POINT {
	FIXED x; /* 16.16 fixed-point pixels */
	FIXED y; /* 16.16 fixed-point pixels */
	FIXED z; /* 16.16 fixed-point pixels */
} ACTOR_CAMERA_POINT;

/* The start of the 3D camera's state (heap slot HEAP_SLOT_CAMERA, */
/* BattleCameraWork, a BATTLE_CAMERA), as the fixed camera reads it: the
 */
/* eye and target it looks from and at, or the points that stand in for them. */
typedef struct ACTOR_CAMERA_FIXED {
	ACTOR_CAMERA_POINT eye;
	ACTOR_CAMERA_POINT tgt;
	ACTOR_CAMERA_POINT *followEye; /* when set, used as the eye */
	ACTOR_CAMERA_POINT *followPos; /* when set, used as the target */
} ACTOR_CAMERA_FIXED;

/* Each frame, with the scrolling camera: places every actor's sprite on the */
/* screen. */
void fdActorSystemPlaceSprs(void) {
	/* LEFTOVER(frame): three unused points the ROM's frame reserves and never
	 * touches. */
	UNUSED ACTOR_CAMERA_POINT unusedPoints[3];
	s32 actorIndex;
	MAP_GRID_SCROLL_STATE *grid;
	s32 viewX;
	s32 viewZ;
	ACTOR_SYSTEM_STATE *system;
	ACTOR *actor;
	SPR *sprite;
	u32 kind;
	FIXED dx;
	FIXED dz;
	FIXED screenTop;
	ACTOR_CAMERA_TILE *tile;
	u32 priority;
	SPR_SCALE scale;
	SPR_PLACEMENT drawPosition;
	u32 layerPlusOne;

	/* MAP_GRID_SCROLL_STATE owns all three layer cell pointers at their 48-byte
	 */
	/* stride. */
	grid = MapGridState[0];
	viewX = syMapGridScrollStateView(grid)[0] & FIELD_PIXEL_MASK;
	viewZ = syMapGridScrollStateView(grid)[1] & FIELD_PIXEL_MASK;
	system = ActorSystemStateWork;
	/* Copy the picture decoder (Render_DecodePicture) into its heap slot. */
	syRamCodeLoadSlot(HEAP_SLOT_FRAME_SCRATCH, Render_DecodePicture,
					  Render_DecodePictureCodeSize);
	system->used = 0;
	actor = ActorsWork;
	for(actorIndex = 0; actorIndex < ACTOR_COUNT; actorIndex++, actor++) {
		if(actor->script == 0)
			continue;
		/* An actor at (x, z) = (0, 0) is not placed; only its sprite's VRAM is
		 */
		/* given up. */
		if(actor->x.fixed != 0 || actor->z.fixed != 0) {
			kind = actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK;
			if(kind == ACTOR_SPRITE_NONE)
				continue;
			/* This camera places single sprites only; groups are left. */
			if(kind != ACTOR_SPRITE_ONE)
				continue;
			/* While a menu has sprite projection suspended, only held actors
			 * are */
			/* drawn. */
			if(system->suspended != 0 && actor->retainSprCache == 0) {
				SPR *frozen = actor->spr;

				syVramCacheReleaseVram(frozen->slot);
				frozen->dirty = kind;
				continue;
			}
			dx = actor->x.fixed - viewX;
			dz = actor->z.fixed - viewZ;
			screenTop = dz - actor->y.fixed;
			sprite = actor->spr;
			/* From 32 pixels left of the screen to 32 past its 240, and from 32
			 */
			/* pixels above it to 224. */
			if(dx > -toFIXED(32) && dx < toFIXED(272) &&
			   screenTop > -toFIXED(32) && screenTop < toFIXED(224)) {
				/* The cell under the actor: x and z >> 20 are 16-pixel cells,
				 * and a */
				/* row holds 128 of them. */
				tile = &((ACTOR_CAMERA_TILE *)grid->layers[actor->layer]
							 .cells)[(FIELD_CELL(actor->x.fixed)) +
									 FIELD_CELL(actor->z.fixed) * MAPGRID_SIZE];
				if(actor->priFlg & ACTOR_PRIORITY_AUTOMATIC) {
					priority = tile->pri;
					if(priority != 0) {
						sprite->image.pri = priority;
						sprite->shadow.pri = priority;
					}
				}
				layerPlusOne = tile->layer;
				if(layerPlusOne != 0)
					actor->layer = layerPlusOne - 1;
				scale.x = syMulFX(actor->scaleX, sprite->scale);
				scale.y = syMulFX(actor->scaleY, sprite->scale);
				drawPosition.x = dx;
				drawPosition.height = actor->y.fixed;
				drawPosition.y = dz;
				drawPosition.groundHeight = actor->groundHeight;
				/* ACTOR_PRIORITY_UNDERFOOT and _OVERHEAD move the point 320
				 * pixels up */
				/* or down the map with its height and ground height, so the
				 * sprite and */
				/* shadow stay where they are on screen; only the draw slot
				 * changes, */
				/* which fdRenderApplyProjectedPlacement takes from the point's
				 * y before */
				/* its height is taken off. */
				if(actor->priFlg & ACTOR_PRIORITY_UNDERFOOT) {
					drawPosition.height += -toFIXED(320);
					drawPosition.y += -toFIXED(320);
					drawPosition.groundHeight += -toFIXED(320);
				}
				if(actor->priFlg & ACTOR_PRIORITY_OVERHEAD) {
					drawPosition.height += toFIXED(320);
					drawPosition.y += toFIXED(320);
					drawPosition.groundHeight += toFIXED(320);
				}
				fdRenderApplyProjectedPlacement(sprite, &drawPosition, &scale,
												actor->facing);
				continue;
			}
			/* Off screen: the sprite's VRAM goes back to the cache, and dirty
			 * makes */
			/* it redraw when it comes back. */
			if(actor->retainSprCache == 0) {
				if(!sprite->retainCacheEntry) {
					syVramCacheReleaseVram(sprite->slot);
					sprite->dirty = 1;
				}
			}
		}
		else {
			kind = actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK;
			if(kind == ACTOR_SPRITE_ONE) {
				sprite = actor->spr;
				if(actor->retainSprCache == 0 && !sprite->retainCacheEntry) {
					syVramCacheReleaseVram(sprite->slot);
					sprite->dirty = kind;
				}
			}
		}
	}
	syHeapSlotRelease(HEAP_SLOT_FRAME_SCRATCH);
}

/* Unused: returns 1. */
UNUSED s32 fdActorCameraReturnTrue(void) { return 1; }

/* Each frame, with the fixed camera: places every actor's sprite, from the */
/* 64th actor down, facing them by the angle from the eye to the target. */
void fdActorSystemProjectSprs(void) {
	s32 i;
	ACTOR_CAMERA_FIXED *camera;
	ACTOR_SYSTEM_STATE *system;
	ACTOR_CAMERA_POINT *eye;
	ACTOR_CAMERA_POINT *target;
	ACTOR *actor;
	s32 angle;
	s32 part;
	s32 kind;
	s32 *position;
	void **sprites;
	void *sprite;
	FIXED groupScale[2];
	FIXED scale[2];
	/* LEFTOVER(frame): two unused points the ROM's frame reserves and never
	 * touches. */
	UNUSED ACTOR_CAMERA_POINT unusedPoints[2];

	camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
	system = ActorSystemStateWork;
	syRamCodeLoadSlot(HEAP_SLOT_FRAME_SCRATCH, Render_DecodePicture,
					  Render_DecodePictureCodeSize);
	eye = &camera->eye;
	target = &camera->tgt;
	if(camera->followEye != NULL)
		eye = camera->followEye;
	if(camera->followPos != NULL)
		target = camera->followPos;
	/* The camera's heading over the ground, from the eye's offset from the */
	/* target in whole pixels; each sprite's facing is drawn relative to it, so
	 */
	/* a sprite keeps its world heading as the camera turns. */
	angle = (s16)syAtan(FIXEDtoINT(eye->x - target->x),
						FIXEDtoINT(eye->z - target->z));
	system->used = 0;
	syInitMatrix();
	/* Under the fixed view matrix every sprite also faces an eighth of a turn
	 */
	/* further back. */
	if(gmFlagGetStub(FLAG_CHOOSING_COMMANDS)) {
		angle += -(DEGtoANG(45));
		syMultiMatrix(Camera_FixedViewMatrix);
		syLookAt((s32 *)eye, (s32 *)target);
	}
	else {
		syMatrixSetLookAt((s32 *)eye, (s32 *)target);
	}
	/* The actors are placed from the last down. */
	actor = ActorsWork;
	actor += ACTOR_COUNT - 1;
	{
		FIXED fixedScale = FIXED_ONE;

		for(i = ACTOR_COUNT - 1; i >= 0; i--, actor--) {
			if(actor->script == 0)
				continue;
			kind = actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK;
			switch(kind) {
			case ACTOR_SPRITE_ONE:
				position = &actor->x.fixed;
				scale[0] = actor->scaleX;
				scale[1] = actor->scaleY;
				sprite = actor->spr;
				if(gmFlagGetStub(FLAG_CHOOSING_COMMANDS)) {
					scale[0] = fixedScale;
					scale[1] = fixedScale;
				}
				fdRenderPlaceProjectedSpr(sprite, position, scale,
										  actor->facing + angle, actor->layer);
				break;
			case ACTOR_SPRITE_GROUP:
				groupScale[0] = actor->scaleX;
				groupScale[1] = actor->scaleY;
				if(gmFlagGetStub(FLAG_CHOOSING_COMMANDS)) {
					groupScale[0] = fixedScale;
					groupScale[1] = fixedScale;
				}
				/* A group's sprite field points to its four sprite pointers;
				 * empty */
				/* ones are null. */
				sprites = (void **)actor->spr;
				for(part = 0; part < 4; part++) {
					sprite = *sprites++;
					if(sprite != NULL)
						fdRenderPlaceProjectedSpr(
							sprite, &actor->x.fixed, groupScale,
							actor->facing + angle, actor->layer);
				}
				break;
			case ACTOR_SPRITE_NONE:
				break;
			}
		}
		syHeapSlotRelease(HEAP_SLOT_FRAME_SCRATCH);
	}
}
