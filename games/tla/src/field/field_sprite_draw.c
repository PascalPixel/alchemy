/* Drawing sprites: composing their animations' pictures and placing projected
   sprites on screen. Part of field.c. */

#include "field/object.h"
#include "macros.h"

/* SPR_ANIMATION.mode: how a layer's pictures are packed, each with its
   decoder. */
typedef enum SPR_PICTURE_CODEC {
	SPRITE_PICTURE_RAW = 0,
	SPRITE_PICTURE_TAGGED_LZ = 1, /* syDecompressTaggedLZ's format */
	SPRITE_PICTURE_LZ = 3		  /* LZ-compressed */
} SPR_PICTURE_CODEC;

/* Facing table `table`'s entry for facing angle `direction`, the turn cut into
   as many sectors as the table has entries. */
#define FACING_TABLE_ENTRY(table, direction) \
	((table)[FACING_SECTOR(direction, ARRAY_COUNT(table))])

/* Runs each of the sprite's sprite animations on to its next stop (an end,
   hold, jump or play command), taking the pictures and settings on the way, and
   leaves the stop to run next; marks the sprite and each sprite animation
   changed. The battle's motion calls it before it releases a borrowed sprite
   buffer. */
void fdSpriteSkipToStop(SPR *sprite)
{
	Sint32 i;

	for(i = 0; i < sprite->count; i++) {
		SPR_ANIMATION *animation = sprite->entries[i];
		Uint8		   command;
		Uint8		   argument;

		if(animation == 0 || animation->script == 0)
			continue;
		for(;;) {
			command = animation->script[animation->scriptOffset++];
			argument = animation->script[animation->scriptOffset++];
			switch(command) {
			case ANIMCMD_SET_KIND:
				animation->kind = argument;
				continue;
			case ANIMCMD_OFFSET_X:
				sprite->offsetX = argument;
				continue;
			case ANIMCMD_OFFSET_Y:
				sprite->offsetY = argument;
				continue;
			case ANIMCMD_BLANK:
				animation->pictureBase = SPRITE_PICTURE_NONE;
				continue;
			case ANIMCMD_UNUSED_FIRST ... ANIMCMD_UNUSED_LAST:
			case ANIMCMD_SOUND:
				continue;
			default:
				animation->pictureBase = command;
				continue;
			case ANIMCMD_END:
			case ANIMCMD_HOLD:
			case ANIMCMD_JUMP:
			case ANIMCMD_PLAY:
				break;
			}
			break;
		}
		animation->scriptOffset -= 2;
		sprite->dirty = 1;
		animation->timer = 1;
	}
}

/* Advances a sprite's animations by one tick, picks each sprite animation's
   picture for the facing direction, and when any picture changed redraws the
   sprite: the sprite animations composed by priority into one pixel buffer, an
   optional outline, and the result uploaded to the sprite's VRAM tiles. */
Sint32 fdSpriteComposePicture(SPR *sprite, Uint32 direction)
{
	Bool				mirrored;
	Uint32				size;
	ACTOR_SYSTEM_STATE *system;
	DRAW_FN				decodePicture;
	Bool				decoderWasLoaded;
	SPR_ANIMATION	   *layer;
	Uint8			   *pixels;
	Uint8			   *unpacked;
	void			   *decoded;
	Uint16				drawOrder[4];
	Sint32				i;
	Sint32				k;
	Sint32				drawCount;
	Uint32				key;
	Sint32				argument;
	Sint32				picture;
	Uint32				facing;
	Sint32				tile;
	void			   *vram;

	mirrored = FALSE;
	system = RamHeapSlots->actorSystem;
	decoderWasLoaded = TRUE;
	decodePicture = RamHeapSlots->frameScratch;
	if(decodePicture == NULL) {
		btRenderLoadDecoder();
		decodePicture = RamHeapSlots->frameScratch;
		decoderWasLoaded = FALSE;
	}

	for(i = 0; i < sprite->count; i++) {
		layer = sprite->entries[i];
		if(layer == NULL)
			continue;
		if(layer->script == NULL)
			continue;
	read_command:
		if((Sint16)layer->timer <= 0) {
			picture = layer->script[layer->scriptOffset++];
			argument = layer->script[layer->scriptOffset++];
			switch(picture) {
			case ANIMCMD_PLAY:
				fdSpriteAnimationStart(layer, argument);
				sprite->lastNo = argument;
				break;
			case ANIMCMD_JUMP:
				layer->scriptOffset = argument;
				break;
			case ANIMCMD_SOUND:
				sySoundPlayStub(argument);
				break;
			case ANIMCMD_HOLD:
				layer->scriptOffset -= 2;
				picture = layer->pictureBase;
				goto select_direction;
			case ANIMCMD_SET_KIND:
				layer->kind = argument;
				break;
			case ANIMCMD_OFFSET_X:
				sprite->offsetX = argument;
				break;
			case ANIMCMD_OFFSET_Y:
				sprite->offsetY = argument;
				break;
			case ANIMCMD_BLANK:
				layer->pictureBase = SPRITE_PICTURE_NONE;
				/* Falls through: its time counts as ANIMCMD_WAIT's does. */
			case ANIMCMD_WAIT:
				layer->timer += argument << 4;
				break;
			case ANIMCMD_END:
				layer->pictureBase = SPRITE_PICTURE_NONE;
				layer->script = NULL;
				sprite->count--;
				picture = SPRITE_PICTURE_NONE;
				goto select_direction;
			case ANIMCMD_UNUSED_FIRST:
			case ANIMCMD_WAIT + 1 ... ANIMCMD_UNUSED_LAST:
				break;
			default:
				layer->pictureBase = picture;
				layer->timer += argument << 4;
				break;
			}
			goto read_command;
		}
		layer->timer = layer->timer - layer->step;
		picture = layer->pictureBase;
	select_direction:
		switch(layer->kind) {
		case ANIM_FACING_ONE_PICTURE:
			/* Only the first eight of its entries are read. */
			facing = AnimationFacingOnePicture[FACING_SECTOR(direction, 8)];
			break;
		case ANIM_FACING_FRONT_BACK:
		case ANIM_FACING_TWO_PART:
			facing = FACING_TABLE_ENTRY(AnimationFacingFrontBack, direction);
			break;
		case ANIM_FACING_FRONT_BACK_SWAPPED:
			facing =
				FACING_TABLE_ENTRY(AnimationFacingFrontBackSwapped, direction);
			break;
		case ANIM_FACING_THREE_PICTURES:
			facing =
				FACING_TABLE_ENTRY(AnimationFacingThreePictures, direction);
			break;
		case ANIM_FACING_FIVE_PICTURES:
			facing = FACING_TABLE_ENTRY(AnimationFacingFivePictures, direction);
			break;
		case ANIM_FACING_EIGHT_PICTURES:
			/* Half a sector on, so each sector is centred on its direction. */
			facing = AnimationFacingEightPictures[FACING_SECTOR(
				direction + FACING_STEP, 8)];
			break;
		default:
			facing = 0;
			break;
		}
		picture += facing & ANIM_FACING_PICTURE_MASK;
		if(i == 0 && (facing & ANIM_FACING_MIRRORED) != 0)
			mirrored = TRUE;
		if(layer->picture != picture) {
			layer->picture = picture;
			sprite->dirty = 1;
		}
	}

	if(sprite->flags & SPRITE_FLAG_KEEP_PICTURE) {
		if(FACING_TABLE_ENTRY(AnimationFacingFrontBack, direction) &
			ANIM_FACING_MIRRORED)
			mirrored = TRUE;
	} else if(sprite->dirty != 0) {
		size = sprite->width * sprite->height;
		pixels = syHeapAllocate(size);
		syWordsClear(pixels, size);

		drawCount = -1;

		for(i = sprite->count - 1; i >= 0; i--) {
			layer = sprite->entries[i];
			if(layer == 0)
				continue;
			if(layer->pictures == 0)
				continue;
			if(layer->picture == SPRITE_PICTURE_NONE)
				continue;
			key = layer->priority;
			if(key > 3)
				continue;
			key = (key << 8) | i;
			k = drawCount;
			if(k >= 0 && drawOrder[k] > key) {
				drawOrder[k + 1] = drawOrder[k];
				for(;;) {
					k--;
					if(k < 0)
						goto insert;
					if(drawOrder[k] <= key)
						goto insert;
					drawOrder[k + 1] = drawOrder[k];
				}
			}
		insert:
			drawOrder[k + 1] = key;
			drawCount++;
		}
		drawCount++;

		for(i = 0; i < drawCount; i++) {
			layer = sprite->entries[*(Uint8 *)&drawOrder[i]];
			if(layer->mode == SPRITE_PICTURE_TAGGED_LZ) {
				syDecompressTaggedLZ(layer->pictures[layer->picture], pixels);
			} else if(layer->mode == SPRITE_PICTURE_LZ) {
				if(layer->colorMode != 0) {
					unpacked = syHeapAllocate(1024);
					decodePicture(
						fdDecompressLZ(
							layer->pictures[layer->picture], unpacked),
						pixels, layer->colorMode);
					syHeapRelease(unpacked);
				} else {
					decoded = syDecompressSpritePicture(
						layer->pictures[layer->picture], pixels);
					if(decoded != 0)
						decodePicture(decoded, pixels, 0);
				}
			} else {
				decodePicture(
					layer->pictures[layer->picture], pixels, layer->colorMode);
			}
		}

		if(sprite->flags &
			(SPRITE_FLAG_OUTLINED | SPRITE_FLAG_SHADED | SPRITE_FLAG_SUNK)) {
			if(sprite->flags & SPRITE_FLAG_OUTLINED) {
				OUTLINE_FN outline;

				RAM_CODE_LOAD(
					outline, fdGridDrawOutlineCode, GridDrawOutlineCodeSize);
				outline(sprite->width, sprite->height, pixels, system->edge,
					system->fill);
				syHeapRelease(outline);
			}
			if(sprite->flags & SPRITE_FLAG_SUNK) {
				Sint32	height;
				Sint32	width;
				Sint32	amount;
				Sint32	limit;
				Uint32 *dest;
				Uint32 *src;

				/* picture holds the rows to sink. */
				picture = sprite->sunkRows;
				height = sprite->height;
				width = sprite->width;
				if(picture > height)
					picture = height;
				dest = (Uint32 *)(pixels + width * height);
				dest--;
				limit = width;
				for(i = 0; i < limit; i++)
					*dest-- = 0;
				amount = width * picture;
				src = dest - amount / 4;
				limit = (height - picture - 4) * width / 4;
				for(i = 0; i < limit; i++)
					*dest-- = *src--;
				limit = amount / 4;
				for(i = 0; i < limit; i++)
					*dest-- = 0;
			} else if(sprite->flags & SPRITE_FLAG_SHADED) {
				Uint32		 rows = sprite->sunkRows;
				Uint32		 height = sprite->height;
				Uint32		 width = sprite->width;
				const Uint8 *shade = SpriteColorTintTables;
				Uint8		*p;
				Sint32		 n;

				if(rows > height)
					rows = height;
				n = width * rows;
				p = pixels + (height - rows) * width;
				for(i = 0; i < n; i++) {
					*p = shade[*p];
					p++;
				}
			}
		}

		tile = syVRAMCacheLoad(sprite->slot, size, 0);
		vram = (void *)(tile * TILE_SIZE_4BPP + OBJ_VRAM0);
		((UPLOAD_FN)RamHeapSlots->pictureUploader)(
			pixels, sprite->width, sprite->height, vram);
		sprite->image.tile = tile;
		sprite->dirty = 0;
		system->used += size;
		syHeapRelease(pixels);
	}

	if(decoderWasLoaded == 0)
		syHeapSlotRelease(HEAP_SLOT_TICK_SCRATCH);
	return mirrored;
}

/* Positions a sprite's image (part 0) and shadow (part 1) on the screen: the
   image centred on screenX and raised height above screenY, the shadow at
   groundHeight. scale is the sprite's (x, y) scale (16.16); above 1.0 both
   parts use the double-size affine mode. */
void fdRenderPlaceSpritePartPair(SPR *sprite, FIXED screenX, Sint32 height,
	Sint32 screenY, Sint32 groundHeight, FIXED *scale)
{
	Uint32				halfWidth = sprite->width >> 1;
	Uint32				halfHeight = sprite->height / 2;
	Sint32				marginX = SHADOW_HALF_WIDTH;
	Sint32				marginY = SHADOW_HALF_HEIGHT;
	Sint32				affine = OAM_AFFINE_ON;
	Sint32				scaleX = *scale++;
	Sint32				scaleY = *scale;
	Sint32				x;
	Sint32				y;
	ANIMATION_SPR_PART *shadow;

	if(scaleX > toFIXED(1) || scaleY > toFIXED(1)) {
		affine = OAM_AFFINE_DOUBLE_SIZE;
		halfWidth <<= 1;
		halfHeight <<= 1;
		marginX = 2 * SHADOW_HALF_WIDTH;
		marginY = 2 * SHADOW_HALF_HEIGHT;
	}
	x = FIXEDtoINT(screenX) - halfWidth;
	y = FIXEDtoINT(screenY - height) - halfHeight -
		FIXEDtoINT(((sprite->height / 2) - sprite->offsetY) * scaleY +
			(toFIXED(1) - 1));
	sprite->image.affine = affine;
	sprite->image.x = x;
	sprite->image.y = y;
	x = FIXEDtoINT(screenX) - marginX;
	y = FIXEDtoINT(screenY - groundHeight) - marginY;
	shadow = &sprite->shadow;
	shadow->affine = affine;
	shadow->x = x;
	shadow->y = y;
}

/* The depth slot fdRenderApplyProjectedPlacement gives a sprite's shadow: two
   behind the sprite's own, PLACEMENT_IMAGE_DEPTH (field/field.h). */
#define PLACEMENT_SHADOW_DEPTH (PLACEMENT_IMAGE_DEPTH - 2)

/* Places a sprite for the scrolling camera and queues it for drawing: the
   sprite at the screen point of a projected position (x, height, y, ground),
   scaled by scale[0] and scale[1] and turned by its rotation, and its shadow on
   the ground below; both go into the depth list by their screen row. */
void fdRenderApplyProjectedPlacement(
	SPR *sprite, Sint32 *pos, Sint32 *scale, Sint32 mode)
{
	Sint32 matrix;
	Uint32 halfWidth;
	Uint32 halfHeight;
	Sint32 affine;
	Sint32 px;
	Sint32 py;
	Sint32 pz;
	Sint32 pw;
	Sint32 scaleX;
	Sint32 scaleY;
	/* LEFTOVER(frame): two unused locals the ROM's frame reserves and never
	 * touches. */
	UNUSED Sint32		ground[3];
	UNUSED Sint32		screen[3];
	Sint32				flip;
	AFFINE_TRANSFORM	effect;
	Sint32				anchorX;
	Sint32				anchorY;
	Sint32				x;
	Sint32				y;
	ANIMATION_SPR_PART *part;

	halfWidth = sprite->width >> 1;
	halfHeight = sprite->height >> 1;
	scaleX = *scale++;
	scaleY = *scale;
	px = *pos++;
	py = *pos++;
	pz = *pos++;
	pw = *pos;
	flip = fdSpriteComposePicture(sprite, mode);
	if(flip == 0 && scaleX == toFIXED(1) && scaleY == scaleX &&
		sprite->rotation == 0) {
		affine = OAM_AFFINE_OFF;
		matrix = 0;
	} else {
		affine = OAM_AFFINE_ON;
		effect.angle = sprite->rotation;
		effect.scaleX = scaleX >> 8;
		effect.scaleY = scaleY >> 8;
		if(flip != 0)
			effect.scaleX = -effect.scaleX;
		matrix = syOAMMatrixAllocateFromTransform(&effect);
	}
	if(scaleX > toFIXED(1) || scaleY > toFIXED(1)) {
		affine = OAM_AFFINE_DOUBLE_SIZE;
		halfWidth <<= 1;
		halfHeight <<= 1;
	}

	/* The shadow, centred on the ground below the sprite. */
	y = FIXEDtoINT(pz - pw) - SHADOW_HALF_HEIGHT;
	if((sprite->flags & SPRITE_FLAG_SHADOW) && y < DISPLAY_HEIGHT) {
		part = &sprite->shadow;
		part->affine = OAM_AFFINE_OFF;
		part->x = FIXEDtoINT(px) - SHADOW_HALF_WIDTH;
		part->y = y;
		syOAMAddSpriteAtDepthClamped(
			part, FIXEDtoINT(pz + pw) + PLACEMENT_SHADOW_DEPTH);
	}

	if(scaleX == 0 || scaleY == 0)
		return;

	/* The anchor, scaled, and turned with the sprite. */
	if(sprite->rotation == 0) {
		anchorX = sprite->offsetX * scaleX;
		anchorY = ((sprite->height >> 1) - sprite->offsetY) * scaleY;
	} else {
		FIXED  sin = sySin(sprite->rotation);
		FIXED  cos = syCos(sprite->rotation);
		Sint32 offsetX = sprite->offsetX * scaleX;
		Sint32 offsetY = ((sprite->height >> 1) - sprite->offsetY) * scaleY;

		anchorX = syMulFX(cos, offsetX) + syMulFX(sin, offsetY);
		anchorY = syMulFX(cos, offsetY) - syMulFX(sin, offsetX);
	}
	x = FIXEDtoINT(px) - halfWidth + FIXEDtoINT(anchorX + (toFIXED(1) - 1));
	y = FIXEDtoINT(pz - py) - halfHeight -
		FIXEDtoINT(anchorY + (toFIXED(1) - 1));
	/* Only the right and bottom edges are tested; OAM wraps a negative
	   position. */
	if(x < DISPLAY_WIDTH && y < DISPLAY_HEIGHT) {
		part = &sprite->image;
		part->x = x;
		part->y = y;
		part->affine = affine;
		part->affineIndex = matrix;
		syOAMAddSpriteAtDepthClamped(
			part, FIXEDtoINT(py + pz) + PLACEMENT_IMAGE_DEPTH);
	}
}

/* fdRenderApplyProjectedPlacement into the given draw slot: the sprite and its
   shadow both go to depth, and mode chooses the picture as facing does there.
   It also moves a double-size sprite's shadow by twice its half size, and its
   tick and locals differ, so the two stay separate functions. */
void fdRenderApplyProjectedPlacementAtDepth(
	SPR *sprite, Sint32 *pos, Sint32 *scale, Sint32 mode, Sint32 depth)
{
	Sint32 matrix;
	Uint32 halfWidth;
	Uint32 halfHeight;
	Sint32 extentX;
	Sint32 affine;
	Sint32 px;
	Sint32 py;
	Sint32 pz;
	Sint32 pw;
	Sint32 scaleX;
	Sint32 scaleY;
	Sint32 extentY;
	/* LEFTOVER(frame): two unused locals the ROM's frame reserves and never
	 * touches. */
	UNUSED Sint32		ground[3];
	UNUSED Sint32		screen[3];
	Sint32				flip;
	AFFINE_TRANSFORM	effect;
	Sint32				anchorX;
	Sint32				anchorY;
	Sint32				x;
	Sint32				y;
	ANIMATION_SPR_PART *part;

	halfWidth = sprite->width >> 1;
	{
		Uint8 height = sprite->height;

		halfHeight = height >> 1;
	}
	extentX = SHADOW_HALF_WIDTH;
	extentY = SHADOW_HALF_HEIGHT;
	scaleX = *scale++;
	scaleY = *scale;
	px = *pos++;
	py = *pos++;
	pz = *pos++;
	pw = *pos;
	flip = fdSpriteComposePicture(sprite, mode);
	if(flip == 0 && scaleX == toFIXED(1) && scaleY == scaleX &&
		sprite->rotation == 0) {
		affine = OAM_AFFINE_OFF;
		matrix = 0;
	} else {
		affine = OAM_AFFINE_ON;
		effect.angle = sprite->rotation;
		effect.scaleX = scaleX >> 8;
		effect.scaleY = scaleY >> 8;
		if(flip != 0)
			effect.scaleX = -effect.scaleX;
		matrix = syOAMMatrixAllocateFromTransform(&effect);
	}
	if(scaleX > toFIXED(1) || scaleY > toFIXED(1)) {
		affine = OAM_AFFINE_DOUBLE_SIZE;
		halfWidth <<= 1;
		halfHeight <<= 1;
		extentX = 2 * SHADOW_HALF_WIDTH;
		extentY = 2 * SHADOW_HALF_HEIGHT;
	}

	y = FIXEDtoINT(pz - pw) - extentY;
	if((sprite->flags & SPRITE_FLAG_SHADOW) && y < DISPLAY_HEIGHT) {
		part = &sprite->shadow;
		part->affine = OAM_AFFINE_OFF;
		part->x = FIXEDtoINT(px) - extentX;
		part->y = y;
		syOAMAddSpriteAtDepth(part, depth);
	}

	if(scaleX == 0 || scaleY == 0)
		return;

	if(sprite->rotation == 0) {
		anchorX = sprite->offsetX * scaleX;
		anchorY = ((sprite->height >> 1) - sprite->offsetY) * scaleY;
	} else {
		FIXED sin = sySin(sprite->rotation);
		FIXED cos = syCos(sprite->rotation);

		/* extentX and extentY are reused for the anchor's offset. */
		extentX = sprite->offsetX * scaleX;
		{
			Uint8 height = sprite->height;

			extentY = ((height >> 1) - sprite->offsetY) * scaleY;
		}

		anchorX = syMulFX(cos, extentX) + syMulFX(sin, extentY);
		anchorY = syMulFX(cos, extentY) - syMulFX(sin, extentX);
	}
	x = FIXEDtoINT(px) - halfWidth + FIXEDtoINT(anchorX + (toFIXED(1) - 1));
	y = FIXEDtoINT(pz - py) - halfHeight -
		FIXEDtoINT(anchorY + (toFIXED(1) - 1));
	if(x < DISPLAY_WIDTH && y < DISPLAY_HEIGHT) {
		part = &sprite->image;
		part->x = x;
		part->y = y;
		part->affine = affine;
		part->affineIndex = matrix;
		syOAMAddSpriteAtDepth(part, depth);
	}
}

/* How far past the screen's left, right and top edges a projected sprite's
   point may lie and still be drawn, and past its bottom edge. */
#define PROJECTED_MARGIN		32
#define PROJECTED_MARGIN_BELOW	48
/* The depth slot of a projected sprite at depth 0; the slot falls by one for
   each two units of depth. */
#define PROJECTED_NEAREST_DEPTH 384
/* The size, shape, colours, mosaic and mode of a sprite's OAM attributes 0 and
   1, which placing it keeps. */
#define OAM_ATTR01_KEPT_BITS	0xc000fc00

/* Places a sprite at a 3D point for the fixed camera and queues it for drawing:
   projected to the screen and scaled by its distance (unless useOwnScale keeps
   its own scale), with an affine matrix as needed and its shadow on the ground
   below. depth 0 sorts it by distance; any other value is its draw slot. */
void fdRenderPlaceProjectedSprite(
	SPR *sprite, Sint32 *point, Sint32 *scale, Sint32 facing, Sint32 depth)
{
	Sint32 screen[3];
	Sint32 ground[3];
	Sint32 z;
	Sint32 flip;
	Sint32 rounded;
	FIXED  scaleX;
	FIXED  scaleY;
	Sint32 affine;
	FIXED  offsetX;
	FIXED  offsetY;
	Uint32 halfWidth;
	Uint32 halfHeight;
	Uint32 matrix;
	Uint32 size;

	if(RamHeapSlots->actorSystem->suspended == 0) {
		z = syConvert3Dto2D(point, screen);
		if(screen[2] != 0 &&
			(Uint32)(screen[0] + PROJECTED_MARGIN) <=
				DISPLAY_WIDTH + 2 * PROJECTED_MARGIN &&
			(Uint32)(screen[1] + PROJECTED_MARGIN) <=
				DISPLAY_HEIGHT + PROJECTED_MARGIN + PROJECTED_MARGIN_BELOW) {
			if(sprite->useOwnScale)
				z = toFIXED(1);
			z = syMulFX(z, sprite->scale);
			flip = fdSpriteComposePicture(sprite, facing);
			/* The perspective scale rounded to the nearest 1/32. */
			z >>= 10;
			rounded = ((z + 1) >> 1) * (toFIXED(1) / 32);
			scaleX = syMulFX(rounded, scale[0]);
			scaleY = syMulFX(rounded, scale[1]);
			/* Held below 2 (2 less 1/32): a scaled sprite is drawn in a
			   double-size box, which the picture fills at 2 and would overflow
			   beyond it. */
			if(scaleX > toFIXED(63) / 32)
				scaleX = toFIXED(63) / 32;
			if(scaleY > toFIXED(63) / 32)
				scaleY = toFIXED(63) / 32;
			halfHeight = sprite->height >> 1;
			offsetY = syMulFX(sprite->offsetY - halfHeight, scaleY);
			halfWidth = sprite->width >> 1;
			if(scaleX > toFIXED(1) || scaleY > toFIXED(1)) {
				affine = OAM_AFFINE_DOUBLE_SIZE;
				halfWidth <<= 1;
				halfHeight <<= 1;
			} else {
				affine = OAM_AFFINE_ON;
				if(scaleX == toFIXED(1) && sprite->rotation == 0 &&
					scaleY == scaleX)
					affine = OAM_AFFINE_OFF;
			}
			if(flip)
				scaleX = -scaleX;
			if(affine)
				matrix = syAffineMatrixBuild(
					sprite->rotation, scaleX >> 8, scaleY >> 8);
			else {
				/* A plain sprite has no matrix: 8 in the matrix field is OAM's
				   horizontal flip bit, set while scaleX is negative. */
				matrix = scaleX < 0;
				matrix <<= 3;
			}
			if(scaleX != 0 && scaleY != 0) {
				offsetX = syMulFX(sprite->offsetX, scaleX);
				((Uint32 *)&sprite->image)[1] =
					(((Uint32 *)&sprite->image)[1] & OAM_ATTR01_KEPT_BITS) |
					(affine << 8) | OBJ_AFFINE_MATRIX(matrix) |
					/* OBJ_POSITION as shifts: x's 9 bits into attribute 1, y's
					   8 into attribute 0. */
					((Uint32)(screen[0] - halfWidth + offsetX) << 23 >> 7) |
					(Uint8)(screen[1] - halfHeight + offsetY);
				if(depth == 0) {
					Sint32 slot = PROJECTED_NEAREST_DEPTH - (screen[2] >> 1);

					if(slot <= 0)
						slot = 1;
					syOAMAddSpriteAtDepthClamped(sprite, slot);
				} else {
					syOAMAddSpriteAtDepthClamped(sprite, depth);
				}
			}
			if(sprite->flags & SPRITE_FLAG_SHADOW) {
				ANIMATION_SPR_PART *shadow;

				ground[0] = point[0];
				ground[1] = 0;
				ground[2] = point[2];
				syConvert3Dto2D(ground, screen);
				size = SHADOW_HALF_WIDTH;
				if(affine == OAM_AFFINE_DOUBLE_SIZE)
					size = 2 * SHADOW_HALF_WIDTH;
				shadow = &sprite->shadow;
				shadow->x = screen[0] - size;
				shadow->y = screen[1] - (size >> 1) + 2;
				shadow->affine = affine;
				shadow->affineIndex = matrix;
				if(depth == 0)
					syOAMAddSpriteAtDepthClamped(shadow, 0);
				else
					syOAMAddSpriteAtDepthClamped(shadow, depth);
			}
			return;
		}
	}
	if(RamHeapSlots->battleState == NULL && !sprite->retainCacheEntry) {
		syVRAMCacheReleaseVRAM(sprite->slot);
		sprite->dirty = 1;
	}
}

/* Sets the colour mode (SPR_COL_MODE) of the sprite's first sprite animation
   and has it drawn again. */
void fdSpriteSetFirstAnimationColorMode(SPR *sprite, Uint32 colorMode)
{
	if(sprite != NULL) {
		sprite->entries[0]->colorMode = colorMode;
		sprite->dirty = 1;
	}
}

/* Sets the colour mode of every sprite animation of the sprite that is not
   hidden (SPRITE_COLOR_HIDDEN) and has it drawn again. */
void fdSpriteSetAnimationColorModes(SPR *sprite, Uint32 colorMode)
{
	Sint32 i;

	if(sprite != NULL) {
		for(i = 0; i < sprite->count; i++) {
			if(sprite->entries[i]->colorMode != SPRITE_COLOR_HIDDEN)
				sprite->entries[i]->colorMode = colorMode;
		}
		sprite->dirty = 1;
	}
}

#undef FACING_TABLE_ENTRY
