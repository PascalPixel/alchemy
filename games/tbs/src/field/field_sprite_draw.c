/* Drawing sprites: composing their animations and placing them on screen. Part
   of field.c. */

#include "field_internal.h"

/* Draws one sprite animation's picture into a sprite's pixel buffer
   (fdRenderDecodePicture, run from RAM). */
typedef void (*SPR_DRAW_FN)(const void *src, void *dst, Sint32 param);
/* Uploads a finished sprite buffer to sprite VRAM (fdTileCopyStrided). */
typedef void (*SPR_UPLOAD_FN)(const void *src, Uint32 w, Uint32 h, void *vram);

/* The last of the animation commands from ANIMCMD_WAIT + 1 that this game
   skips, and the command The Lost Age gives 252 (it plays a sound there),
   skipped here too. 251 is not a command: it shows as a picture. */
#define ANIMCMD_UNUSED_LAST 250
#define ANIMCMD_SOUND		252

/* A facing table entry's picture offset from the base picture; its
   FACING_MIRRORED_BIT mirrors the picture. */
#define ANIM_FACING_PICTURE_MASK	7
/* The sector a facing angle lies in, a turn cut into `sectors`. */
#define FACING_SECTOR(dir, sectors) ((dir) / (TRIG_TURN / (sectors)))
/* Facing table `table`'s entry for facing angle `dir`, the turn cut into as
   many sectors as the table has entries. */
#define FACING_TABLE_ENTRY(table, dir) \
	((table)[FACING_SECTOR(dir, ARRAY_COUNT(table))])
/* The eighth of a turn a facing angle lies nearest: its top three bits once it
   is turned half an eighth on, worked in the upper halfword so the sum
   wraps. */
#define FACING_NEAREST_EIGHTH_SECTOR(dir) \
	(((Uint32)((dir) << 16) + (FACING_STEP << 16)) >> 29)

/* A sprite's shadow, 16 by 8 pixels: its half width and half height, both
   doubled when the sprite is drawn double-size. */
#define SHADOW_HALF_WIDTH  8
#define SHADOW_HALF_HEIGHT 4

/* How a sprite part is drawn (its OAM affine bits): as it is, through an affine
   matrix, or through one in a box twice its size, so that a part scaled up past
   full size is not cut off. */
typedef enum SPR_PART_AFFINE {
	SPRITE_PART_PLAIN = 0,
	SPRITE_PART_AFFINE = 1,
	SPRITE_PART_AFFINE_DOUBLE = 3
} SPR_PART_AFFINE;

/* A plain part's matrix field holds OAM's horizontal flip bit instead. */
#define SPRITE_PART_MIRRORED 8

/* Advances a sprite's animations by one frame, picks each sprite animation's
   picture for the facing dir, and when any picture changed redraws the sprite:
   the sprite animations composed by priority into one pixel buffer, an optional
   outline, and the result uploaded to the sprite's VRAM tiles. Returns 1 when
   the first sprite animation's picture is to be mirrored. */
Sint32 fdSprComposePicture(SPR *sprite, Uint16 dir)
{
	Bool				mirrored;
	Uint32				size;
	ACTOR_SYSTEM_STATE *system;
	SPR_DRAW_FN			decodePicture;
	Bool				decoderWasLoaded;
	SPR_ANIMATION	   *layer;
	void			   *code;
	Uint8			   *pixels;
	Uint8			   *unpacked;
	void			   *decoded;
	Uint16				drawOrder[ANIM_LAYER_COUNT];
	Sint32				i;
	Sint32				k;
	Sint32				drawCount;
	Uint32				key;
	Sint32				arg;
	Sint32				picture;
	Uint32				facing;
	Sint32				tile;

	/* The actor system's state and the picture decoder sit in the heap slot
	   table; load the decoder into RAM for this call if no one else has. */
	mirrored = FALSE;
	system = ActorSystemStateWork;
	decoderWasLoaded = TRUE;
	decodePicture = (SPR_DRAW_FN)HEAP_SLOT_BLOCK(HEAP_SLOT_FRAME_SCRATCH);
	if(decodePicture == 0) {
		code = syHeapSlotAlloc(
			HEAP_SLOT_FRAME_SCRATCH, (Sint32)Render_DecodePictureCodeSize);
		syDMAXCopy(fdRenderDecodePicture, code,
			DMA_COPY_WORDS(((Uint32)fdRenderDecodePictureEnd -
							   (Uint32)fdRenderDecodePicture) >>
				2),
			REG_DMA3);
		decodePicture = (SPR_DRAW_FN)HEAP_SLOT_BLOCK(HEAP_SLOT_FRAME_SCRATCH);
		decoderWasLoaded = FALSE;
	}

	/* Step each sprite animation's script: read commands until one shows a
	   picture for a while, else count the current picture's time down. */
	for(i = 0; i < sprite->cnt; i++) {
		layer = sprite->entries[i];
		if(layer == 0) {
			continue;
		}
		if(layer->script == 0) {
			continue;
		}
	read_command:
		if((Sint16)layer->tmr <= 0) {
			picture = layer->script[layer->scriptOfs++];
			arg = layer->script[layer->scriptOfs++];
			switch(picture) {
			case ANIMCMD_PLAY:
				fdSprAnimationStart(layer, arg);
				sprite->lastNo = arg;
				break;
			case ANIMCMD_JUMP:
				layer->scriptOfs = arg;
				break;
			case ANIMCMD_WAIT:
				layer->tmr += arg * SPRITE_ANIMATION_NORMAL_STEP;
				break;
			case ANIMCMD_HOLD:
				layer->scriptOfs -= 2;
				goto show_picture;
			case ANIMCMD_SET_KIND:
				layer->kind = arg;
				break;
			case ANIMCMD_BLANK:
				layer->pictureBase = SPRITE_PICTURE_NONE;
				picture = SPRITE_PICTURE_NONE;
				layer->tmr += arg * SPRITE_ANIMATION_NORMAL_STEP;
				goto select_direction;
			case ANIMCMD_END:
				layer->pictureBase = SPRITE_PICTURE_NONE;
				layer->script = 0;
				picture = SPRITE_PICTURE_NONE;
				/* Ending any sprite animation lowers the sprite's animation
				   count, as if the last had ended. */
				sprite->cnt--;
				goto select_direction;
			case ANIMCMD_HOLD + 1 ... ANIMCMD_WAIT - 1:
			case ANIMCMD_WAIT + 1 ... ANIMCMD_UNUSED_LAST:
			case ANIMCMD_SOUND:
				break;
			default:
				layer->pictureBase = picture;
				layer->tmr += arg * SPRITE_ANIMATION_NORMAL_STEP;
				goto select_direction;
			}
			goto read_command;
		}
		/* Time is left on this picture: count it down by the sprite animation's
		   step (SPRITE_ANIMATION_NORMAL_STEP takes a frame off each frame). */
		layer->tmr = layer->tmr - layer->step;
	show_picture:
		picture = layer->pictureBase;
	select_direction:
		/* The facing table of the sprite animation's kind gives an offset from
		   the base picture and whether to mirror it. */
		switch(layer->kind) {
		case ANIM_FACING_ONE_PICTURE:
			/* Only the first eight of its entries are read. */
			facing = AnimationFacing_OnePicture[FACING_SECTOR(dir, 8)];
			break;
		case ANIM_FACING_FRONT_BACK:
		case ANIM_FACING_TWO_PART:
			facing = FACING_TABLE_ENTRY(AnimationFacing_FrontBack, dir);
			break;
		case ANIM_FACING_FRONT_BACK_SWAPPED:
			facing = FACING_TABLE_ENTRY(AnimationFacing_FrontBackSwapped, dir);
			break;
		case ANIM_FACING_THREE_PICTURES:
			facing = FACING_TABLE_ENTRY(AnimationFacing_ThreePictures, dir);
			break;
		case ANIM_FACING_FOUR_PICTURES:
			facing = FACING_TABLE_ENTRY(AnimationFacing_FourPictures, dir);
			break;
		case ANIM_FACING_FIVE_PICTURES:
			facing = FACING_TABLE_ENTRY(AnimationFacing_FivePictures, dir);
			break;
		case ANIM_FACING_SIX_PICTURES:
			facing = FACING_TABLE_ENTRY(AnimationFacing_SixPictures, dir);
			break;
		case ANIM_FACING_EIGHT_PICTURES:
			facing = AnimationFacing_EightPictures[FACING_NEAREST_EIGHTH_SECTOR(
				dir)];
			break;
		case ANIM_FACING_EIGHT_PICTURES_MIRRORED:
			facing = AnimationFacing_EightPicturesMirrored
				[FACING_NEAREST_EIGHTH_SECTOR(dir)];
			break;
		default:
			facing = 0;
			break;
		}
		picture += facing & ANIM_FACING_PICTURE_MASK;
		if(i == 0 && (facing & FACING_MIRRORED_BIT) != 0) {
			mirrored = TRUE;
		}
		if(layer->picture != picture) {
			layer->picture = picture;
			sprite->dirty = 1;
		}
	}

	if(sprite->dirty != 0) {
		size = sprite->width * sprite->height;
		pixels = syHeapAlloc(size);
		syWordsClear(pixels, size);

		/* Sort the visible sprite animations by priority (0-3), then by index,
		   with an insertion sort on keys of (priority << 8) | index. */
		drawCount = -1;
		for(i = sprite->cnt - 1; i >= 0; i--) {
			layer = sprite->entries[i];
			if(layer == 0) {
				continue;
			}
			if(layer->pictures == 0) {
				continue;
			}
			if(layer->picture == SPRITE_PICTURE_NONE) {
				continue;
			}
			key = layer->pri;
			if(key > 3) {
				continue;
			}
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

		/* Draw the sprite animations in that order, each by its codec. */
		for(i = 0; i < drawCount; i++) {
			layer = sprite->entries[*(Uint8 *)&drawOrder[i]];
			if(layer->mode == SPRITE_PICTURE_TAGGED_LZ) {
				syDecompressTaggedLZ(layer->pictures[layer->picture], pixels);
			} else if(layer->mode == SPRITE_PICTURE_LZ) {
				if(layer->colMode != 0) {
					unpacked = syHeapAlloc(1024);
					decodePicture(
						fdDecompressLZ(
							layer->pictures[layer->picture], unpacked),
						pixels, layer->colMode);
					syHeapRelease(unpacked);
				} else {
					decoded = syDecompressSprPicture(
						layer->pictures[layer->picture], pixels);
					if(decoded != 0) {
						decodePicture(decoded, pixels, 0);
					}
				}
			} else {
				decodePicture(
					layer->pictures[layer->picture], pixels, layer->colMode);
			}
		}

		/* Outline: a pixel whose four neighbours are all drawn becomes the fill
		   colour, and any other drawn pixel the edge colour. */
		if((sprite->flg & SPRITE_FLAG_OUTLINED) != 0) {
			Uint8 *inside;
			Uint8  edge;
			Uint8  fill;
			Uint32 w;
			Uint32 h;
			Uint8 *src;
			Uint8 *dst;

			inside = syHeapAlloc(size);
			w = sprite->width;
			h = sprite->height;
			edge = system->edge;
			fill = system->fill;
			syWordsClear(inside, size);
			src = pixels + w + 1;
			dst = inside + w + 1;
			for(i = 1; i < h - 1; i++) {
				for(k = 1; k < w - 1; k++) {
					if(src[-1] != 0 && src[1] != 0 && *(src - w) != 0 &&
						src[w] != 0) {
						*dst = 1;
					}
					dst++;
					src++;
				}
				dst += 2;
				src += 2;
			}
			src = pixels;
			dst = inside;
			for(i = 0; i < size; i++) {
				if(*dst != 0) {
					*src = fill;
				} else if(*src != 0) {
					*src = edge;
				}
				dst++;
				src++;
			}
			syHeapRelease(inside);
		}

		/* Upload the pixels to the sprite's tiles in VRAM. */
		tile = syVRAMCacheLoad(sprite->slot, size, 0);
		{
			void *vram = (void *)(OBJ_VRAM0 + tile * TILE_SIZE_4BPP);
			((SPR_UPLOAD_FN)HeapSlots[HEAP_SLOT_PICTURE_UPLOADER])(
				pixels, sprite->width, sprite->height, vram);
		}
		sprite->image.tile = tile;
		sprite->dirty = 0;
		system->used += size;
		syHeapRelease(pixels);
	}

	if(decoderWasLoaded == 0) {
		syHeapSlotRelease(HEAP_SLOT_FRAME_SCRATCH);
	}
	return mirrored;
}

/* Positions a sprite's image (part 0) and shadow (part 1) on the screen: the
   image centred on screenX and raised height above screenY, the shadow at
   groundHeight. scale is the sprite's (x, y) scale (16.16); above 1.0 both
   parts use the double-size affine mode. */
void fdRenderPlaceSprPartPair(SPR *sprite, FIXED screenX, Sint32 height,
	Sint32 screenY, Sint32 groundHeight, FIXED *scale)
{
	Uint32				halfWidth = sprite->width >> 1;
	Uint32				halfHeight = sprite->height / 2;
	Sint32				marginX = SHADOW_HALF_WIDTH;
	Sint32				marginY = SHADOW_HALF_HEIGHT;
	Sint32				affine = SPRITE_PART_AFFINE;
	Sint32				scaleX = *scale++;
	Sint32				scaleY = *scale;
	Sint32				x;
	Sint32				y;
	ANIMATION_SPR_PART *shadow;

	/* The margins are the shadow's half width and height. */
	if(scaleX > toFIXED(1) || scaleY > toFIXED(1)) {
		affine = SPRITE_PART_AFFINE_DOUBLE;
		marginX = 2 * SHADOW_HALF_WIDTH;
		marginY = 2 * SHADOW_HALF_HEIGHT;
		halfWidth <<= 1;
		halfHeight <<= 1;
	}
	x = FIXEDtoINT(screenX) - halfWidth;
	/* The point is offsetY pixels above the image's bottom edge: the box is
	   raised by half the height less offsetY, scaled and rounded up to a whole
	   pixel. */
	y = FIXEDtoINT(screenY - height) - halfHeight -
		FIXEDtoINT(
			((sprite->height / 2) - sprite->ofsY) * scaleY + (toFIXED(1) - 1));
	sprite->image.affine = affine;
	sprite->image.x = x;
	sprite->image.y = y;
	/* Place the shadow at ground height independently of the raised image. */
	x = FIXEDtoINT(screenX) - marginX;
	y = FIXEDtoINT(screenY - groundHeight) - marginY;
	shadow = &sprite->shadow;
	shadow->affine = affine;
	shadow->x = x;
	shadow->y = y;
}

/* The two rearmost depth slots: shadows drawn under everything, and the sprites
   drawn behind every other sprite, just in front of those shadows. */
#define DEPTH_SLOT_BACK_SHADOWS 0
#define DEPTH_SLOT_BACK			1

/* fdRenderApplyProjectedPlacement's draw slots: a sprite one slot further in
   front for every two screen rows lower, on from PLACEMENT_IMAGE_SLOT, and its
   shadow in PLACEMENT_SHADOW_SLOT, behind them. */
#define PLACEMENT_IMAGE_SLOT  10
#define PLACEMENT_SHADOW_SLOT 2

/* Places a sprite for the scrolling camera and queues it for drawing. */
void fdRenderApplyProjectedPlacement(SPR *sprite, const SPR_PLACEMENT *position,
	const SPR_SCALE *scale, Sint32 facing)
{
	const Sint32 *pos = sySprPlacementReadValues(position);
	const Sint32 *scaleValues = scale->values;
	Sint32		  matrix;
	Uint32		  halfWidth = sprite->width >> 1;
	Uint32		  halfHeight = sprite->height >> 1;
	Sint32		  shadowHalfWidth = SHADOW_HALF_WIDTH;
	Sint32		  shadowHalfHeight = SHADOW_HALF_HEIGHT;
	Sint32		  affine;
	Sint32		  scaleX = *scaleValues++;
	Sint32		  screenX = *pos++;
	Sint32		  scaleY = *scaleValues;
	Sint32		  height = *pos++;
	Sint32		  screenY = *pos++;
	Sint32		  groundHeight = *pos;
	Sint32		  imageSlot;
	/* LEFTOVER(frame): two unused locals the ROM's frame reserves and never
	 * touches. */
	UNUSED Sint32		ground[3];
	UNUSED Sint32		screen[3];
	AFFINE_TRANSFORM	effect;
	Sint32				flip;
	Sint32				x;
	Sint32				y;
	Sint32				shadowSlot;
	ANIMATION_SPR_PART *part;

	/* Reuse an untransformed sprite when mirroring, scale and rotation allow
	   it. */
	flip = fdSprComposePicture(sprite, facing);
	if(flip == 0 && scaleX == toFIXED(1) && scaleY == scaleX &&
		sprite->rotation == 0) {
		affine = SPRITE_PART_PLAIN;
		matrix = 0;
	} else {
		affine = SPRITE_PART_AFFINE;
		effect.ang = sprite->rotation;
		effect.scaleX = scaleX >> 8;
		effect.scaleY = scaleY >> 8;
		if(flip != 0)
			effect.scaleX = -effect.scaleX;
		matrix = syOAMMatrixAllocFromTransform(&effect);
	}
	if(scaleX > toFIXED(1) || scaleY > toFIXED(1)) {
		affine = SPRITE_PART_AFFINE_DOUBLE;
		halfWidth <<= 1;
		halfHeight <<= 1;
		shadowHalfWidth = 2 * SHADOW_HALF_WIDTH;
		shadowHalfHeight = 2 * SHADOW_HALF_HEIGHT;
	}
	/* A sprite sunk 100 pixels or more draws behind the rest. */
	if(height <= -toFIXED(100)) {
		imageSlot = DEPTH_SLOT_BACK;
		shadowSlot = DEPTH_SLOT_BACK_SHADOWS;
	} else {
		imageSlot = (FIXEDtoINT(screenY) >> 1) + PLACEMENT_IMAGE_SLOT;
		shadowSlot = PLACEMENT_SHADOW_SLOT;
	}
	/* Queue the shadow at ground height before the raised image. */
	y = FIXEDtoINT(screenY - groundHeight) - shadowHalfHeight;
	if(sprite->flg & SPRITE_FLAG_SHADOW) {
		if(y < DISPLAY_HEIGHT) {
			part = &sprite->shadow;
			part->affine = affine;
			part->affineIdx = matrix;
			part->x = FIXEDtoINT(screenX) - shadowHalfWidth;
			part->y = y;
			syOAMAddSprAtDepth(part, shadowSlot);
		}
	}
	x = FIXEDtoINT(screenX) - halfWidth +
		FIXEDtoINT(sprite->ofsX * scaleX + (toFIXED(1) - 1));
	y = FIXEDtoINT(screenY - height) - halfHeight -
		FIXEDtoINT(
			scaleY * ((sprite->height >> 1) - sprite->ofsY) + (toFIXED(1) - 1));
	/* Only the right and bottom edges are tested; OAM wraps a negative
	   position. */
	if(x < DISPLAY_WIDTH && y < DISPLAY_HEIGHT) {
		part = &sprite->image;
		part->x = x;
		part->y = y;
		part->affine = affine;
		part->affineIdx = matrix;
		syOAMAddSprAtDepth(part, imageSlot);
	}
}

/* How far past the screen's left, right and top edges a projected sprite's
   point may lie and still be drawn, and past its bottom edge. */
#define PROJECTED_MARGIN	   32
#define PROJECTED_MARGIN_BELOW 48
/* A projected sprite at depth PROJECTED_MIDDLE_DEPTH draws in the middle of the
   256 depth slots, PROJECTED_MIDDLE_SLOT, and one slot further in front for
   every two units nearer. */
#define PROJECTED_MIDDLE_DEPTH 512
#define PROJECTED_MIDDLE_SLOT  128

/* Places a sprite at 3D point for the fixed camera and queues it for drawing:
   projected to the screen and scaled by its distance (unless useOwnScale keeps
   its own scale), with an affine matrix as needed and its shadow on the ground
   below. depth 0 sorts it by distance; any other value is its draw slot. */
void fdRenderPlaceProjectedSpr(
	SPR *sprite, Sint32 *point, Sint32 *scale, Sint32 facing, Sint32 depth)
{
	Sint32			 flip;
	Uint32			 halfHeight;
	Uint32			 size;
	Sint32			 affine;
	Sint32			 halfWidth;
	Sint32			 ground[3];
	Sint32			 screen[3];
	AFFINE_TRANSFORM effect;
	FIXED			 z;
	Sint32			 matrix;
	Sint32			 roundedScale;
	FIXED			 scaleX;
	FIXED			 scaleY;
	FIXED			 offsetX;
	FIXED			 offsetY;
	Sint32			 slot;

	/* Project first, then discard sprites beyond the padded screen bounds. */
	affine = SPRITE_PART_AFFINE;
	if(ActorSystemStateWork->suspended == 0) {
		z = syConvert3Dto2D(point, screen);
		if(screen[2] != 0 && screen[0] >= -PROJECTED_MARGIN &&
			screen[0] <= DISPLAY_WIDTH + PROJECTED_MARGIN &&
			screen[1] >= -PROJECTED_MARGIN &&
			screen[1] <= DISPLAY_HEIGHT + PROJECTED_MARGIN_BELOW) {
			if(sprite->useOwnScale)
				z = sprite->scale;
			else
				z = syMulFX(z, sprite->scale);
			halfWidth = sprite->width >> 1;
			halfHeight = sprite->height >> 1;
			size = SHADOW_HALF_WIDTH;
			flip = fdSprComposePicture(sprite, facing);
			/* Round the perspective scale to the nearest 1/32 before combining
			   it with the sprite's own scale. */
			roundedScale = (z + toFIXED(1) / 64) & -(toFIXED(1) / 32);
			scaleX = syMulFX(roundedScale, *scale++);
			scaleY = syMulFX(roundedScale, *scale);
			/* Hold each scale below 2 (toFIXED(63) / 32, 2 less 1/32): a scaled
			   sprite is drawn in a double-size box, which the picture exactly
			   fills at 2 and would overflow, cut off at its edges, beyond
			   it. */
			if(scaleX >= toFIXED(63) / 32)
				scaleX = toFIXED(63) / 32;
			if(scaleY >= toFIXED(63) / 32)
				scaleY = toFIXED(63) / 32;
			offsetX = syMulFX(sprite->ofsX, scaleX);
			offsetY = -syMulFX((sprite->height >> 1) - sprite->ofsY, scaleY);
			if(scaleX > toFIXED(1) || scaleY > toFIXED(1)) {
				affine = SPRITE_PART_AFFINE_DOUBLE;
				halfWidth <<= 1;
				halfHeight <<= 1;
				size = 2 * SHADOW_HALF_WIDTH;
			} else if(scaleX == toFIXED(1) && sprite->rotation == 0 &&
				scaleY == scaleX) {
				affine = SPRITE_PART_PLAIN;
			}
			/* Mirroring uses negative affine scale when a matrix is needed. */
			if(affine != SPRITE_PART_PLAIN) {
				effect.ang = sprite->rotation;
				effect.scaleX = scaleX >> 8;
				if(flip != 0) {
					effect.scaleX = -effect.scaleX;
					offsetX = -offsetX;
				}
				effect.scaleY = scaleY >> 8;
				matrix = syOAMMatrixAllocFromTransform(&effect);
			} else if(flip != 0) {
				matrix = SPRITE_PART_MIRRORED;
				offsetX = -offsetX;
			} else {
				matrix = 0;
			}
			{
				ANIMATION_SPR_PART *image = &sprite->image;

				image->x = screen[0] - halfWidth + offsetX;
				image->y = screen[1] - halfHeight + offsetY;
				image->affine = affine;
				image->affineIdx = matrix;
			}
			/* The slot falls as the projected depth screen[2] rises, and stays
			   in front of the shadows' slot. */
			if(depth == 0) {
				slot = (PROJECTED_MIDDLE_DEPTH - screen[2]) / 2 +
					PROJECTED_MIDDLE_SLOT;
				if(slot <= 0)
					slot = DEPTH_SLOT_BACK;
				syOAMAddSprAtDepth(sprite, slot);
			} else {
				syOAMAddSprAtDepth(sprite, depth);
			}
			/* Project the ground point separately so jumping does not lift the
			   shadow. */
			if(sprite->flg & SPRITE_FLAG_SHADOW) {
				ANIMATION_SPR_PART *shadow;

				SET_VECTOR(ground, point[0], 0, point[2]);
				syConvert3Dto2D(ground, screen);
				shadow = &sprite->shadow;
				shadow->x = screen[0] - size;
				/* Centred 2 pixels below the ground point, so the feet stand on
				   the shadow's upper half (the World Map's leader, render probe
				   --scene WorldMap). */
				shadow->y = screen[1] - (size >> 1) + 2;
				shadow->affine = affine;
				shadow->affineIdx = matrix;
				if(depth == 0)
					syOAMAddSprAtDepth(shadow, DEPTH_SLOT_BACK_SHADOWS);
				else
					syOAMAddSprAtDepth(shadow, depth);
			}
			return;
		}
	}
	/* Invisible sprites release cached tiles unless their display flags hold
	   them; marked dirty, the sprite is redrawn into new VRAM when it next
	   comes on screen. */
	if(!sprite->retainCacheEntry) {
		syVRAMCacheReleaseVRAM(sprite->slot);
		sprite->dirty = 1;
	}
}

/* Sets the colour mode (SPR_COL_MODE) of each of a sprite's animations to
   colorMode, except sprite animations drawn hidden (SPRITE_COLOR_HIDDEN), and
   marks the sprite for redrawing. */
void fdSprSetAnimationColModes(void *rawSprite, Uint32 colorMode)
{
	SPR *sprite = rawSprite;
	if(sprite != 0) {
		Uint32 count = sprite->cnt;

		if(count != 0) {
			SPR_ANIMATION **entry = sprite->entries;
			Uint32			remaining = count;
			do {
				SPR_ANIMATION *layer = *entry++;
				if(layer->colMode != SPRITE_COLOR_HIDDEN)
					layer->colMode = colorMode;
				remaining--;
			} while(remaining != 0);
		}
		sprite->dirty = 1;
	}
}
