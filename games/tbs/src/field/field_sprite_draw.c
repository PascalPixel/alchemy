/// @file
/// Drawing sprites: composing their animations and placing them on screen.
// Part of field.c.

#include "field_internal.h"

/// Draws one sprite animation's picture into a sprite's pixel buffer
/// (Render_DecodePicture, run from RAM).
typedef void (*SpriteDrawFn)(const void *src, void *dst, s32 param);
/// Uploads a finished sprite buffer to sprite VRAM (Tile_CopyStrided).
typedef void (*SpriteUploadFn)(const void *src, u32 w, u32 h, void *vram);

/// The last of the animation commands from ANIMCMD_WAIT + 1 that this game
/// skips, and the command The Lost Age gives 252 (it plays a sound there),
/// skipped here too. 251 is not a command: it shows as a picture.
#define ANIMCMD_UNUSED_LAST 250
#define ANIMCMD_SOUND 252

/// A facing table entry's picture offset from the base picture; its
/// FACING_MIRRORED_BIT mirrors the picture.
#define ANIM_FACING_PICTURE_MASK 7
/// The sector a facing angle lies in, a turn cut into `sectors`.
#define FACING_SECTOR(dir, sectors) ((dir) / (TRIG_TURN / (sectors)))
/// Facing table `table`'s entry for facing angle `dir`, the turn cut into as
/// many sectors as the table has entries.
#define FACING_TABLE_ENTRY(table, dir)                                         \
  ((table)[FACING_SECTOR(dir, ARRAY_COUNT(table))])
/// The eighth of a turn a facing angle lies nearest: its top three bits once
/// it is turned half an eighth on, worked in the upper halfword so the sum
/// wraps.
#define FACING_NEAREST_EIGHTH_SECTOR(dir)                                      \
  (((u32)((dir) << 16) + (FACING_STEP << 16)) >> 29)

/// A sprite's shadow, 16 by 8 pixels: its half width and half height, both
/// doubled when the sprite is drawn double-size.
#define SHADOW_HALF_WIDTH 8
#define SHADOW_HALF_HEIGHT 4

/// How a sprite part is drawn (its OAM affine bits): as it is, through an
/// affine matrix, or through one in a box twice its size, so that a part
/// scaled up past full size is not cut off.
enum SpritePartAffine {
  SPRITE_PART_PLAIN = 0,
  SPRITE_PART_AFFINE = 1,
  SPRITE_PART_AFFINE_DOUBLE = 3
};
/// A plain part's matrix field holds OAM's horizontal flip bit instead.
#define SPRITE_PART_MIRRORED 8

/// Advances a sprite's animations by one frame, picks each sprite animation's
/// picture for the facing dir, and when any picture changed redraws the sprite:
/// the sprite animations composed by priority into one pixel buffer, an
/// optional outline, and the result uploaded to the sprite's VRAM tiles.
/// Returns 1 when the first sprite animation's picture is to be mirrored.
s32 Sprite_ComposePicture(struct Sprite *sprite, u16 dir) {
  s32 mirrored;
  u32 size;
  struct ActorSystemState *system;
  SpriteDrawFn decodePicture;
  s32 decoderWasLoaded;
  struct SpriteAnimation *layer;
  void *code;
  u8 *pixels;
  u8 *unpacked;
  void *decoded;
  u16 drawOrder[ANIM_LAYER_COUNT];
  s32 i;
  s32 k;
  s32 drawCount;
  u32 key;
  s32 arg;
  s32 picture;
  u32 facing;
  s32 tile;

  // The actor system's state and the picture decoder sit in the heap slot
  // table; load the decoder into RAM for this call if no one else has.
  mirrored = 0;
  system = gActorSystemState;
  decoderWasLoaded = 1;
  decodePicture = (SpriteDrawFn)HEAP_SLOT_BLOCK(HEAP_SLOT_FRAME_SCRATCH);
  if (decodePicture == 0) {
    code = HeapSlot_Alloc(HEAP_SLOT_FRAME_SCRATCH,
                          (s32)Render_DecodePictureCodeSize);
    Dma_Set(
        Render_DecodePicture, code,
        DMA_COPY_WORDS(
            ((u32)gRender_DecodePictureEnd - (u32)Render_DecodePicture) >> 2),
        REG_DMA3);
    decodePicture = (SpriteDrawFn)HEAP_SLOT_BLOCK(HEAP_SLOT_FRAME_SCRATCH);
    decoderWasLoaded = 0;
  }

  // Step each sprite animation's script: read commands until one shows a
  // picture for a while, else count the current picture's time down.
  for (i = 0; i < sprite->count; i++) {
    layer = sprite->entries[i];
    if (layer == 0) {
      continue;
    }
    if (layer->script == 0) {
      continue;
    }
  read_command:
    if ((s16)layer->timer <= 0) {
      picture = layer->script[layer->scriptOffset++];
      arg = layer->script[layer->scriptOffset++];
      switch (picture) {
      case ANIMCMD_PLAY:
        SpriteAnimation_Start(layer, arg);
        sprite->lastNo = arg;
        break;
      case ANIMCMD_JUMP:
        layer->scriptOffset = arg;
        break;
      case ANIMCMD_WAIT:
        layer->timer += arg * SPRITE_ANIMATION_NORMAL_STEP;
        break;
      case ANIMCMD_HOLD:
        layer->scriptOffset -= 2;
        goto show_picture;
      case ANIMCMD_SET_KIND:
        layer->kind = arg;
        break;
      case ANIMCMD_BLANK:
        layer->pictureBase = SPRITE_PICTURE_NONE;
        picture = SPRITE_PICTURE_NONE;
        layer->timer += arg * SPRITE_ANIMATION_NORMAL_STEP;
        goto select_direction;
      case ANIMCMD_END:
        layer->pictureBase = SPRITE_PICTURE_NONE;
        layer->script = 0;
        picture = SPRITE_PICTURE_NONE;
        // Ending any sprite animation lowers the sprite's animation count, as
        // if the last had ended.
        sprite->count--;
        goto select_direction;
      case ANIMCMD_HOLD + 1 ... ANIMCMD_WAIT - 1:
      case ANIMCMD_WAIT + 1 ... ANIMCMD_UNUSED_LAST:
      case ANIMCMD_SOUND:
        break;
      default:
        layer->pictureBase = picture;
        layer->timer += arg * SPRITE_ANIMATION_NORMAL_STEP;
        goto select_direction;
      }
      goto read_command;
    }
    // Time is left on this picture: count it down by the sprite animation's
    // step (SPRITE_ANIMATION_NORMAL_STEP takes a frame off each frame).
    layer->timer = layer->timer - layer->step;
  show_picture:
    picture = layer->pictureBase;
  select_direction:
    // The facing table of the sprite animation's kind gives an offset from the
    // base picture and whether to mirror it.
    switch (layer->kind) {
    case ANIM_FACING_ONE_PICTURE:
      // Only the first eight of its entries are read.
      facing = gAnimationFacing_OnePicture[FACING_SECTOR(dir, 8)];
      break;
    case ANIM_FACING_FRONT_BACK:
    case ANIM_FACING_TWO_PART:
      facing = FACING_TABLE_ENTRY(gAnimationFacing_FrontBack, dir);
      break;
    case ANIM_FACING_FRONT_BACK_SWAPPED:
      facing = FACING_TABLE_ENTRY(gAnimationFacing_FrontBackSwapped, dir);
      break;
    case ANIM_FACING_THREE_PICTURES:
      facing = FACING_TABLE_ENTRY(gAnimationFacing_ThreePictures, dir);
      break;
    case ANIM_FACING_FOUR_PICTURES:
      facing = FACING_TABLE_ENTRY(gAnimationFacing_FourPictures, dir);
      break;
    case ANIM_FACING_FIVE_PICTURES:
      facing = FACING_TABLE_ENTRY(gAnimationFacing_FivePictures, dir);
      break;
    case ANIM_FACING_SIX_PICTURES:
      facing = FACING_TABLE_ENTRY(gAnimationFacing_SixPictures, dir);
      break;
    case ANIM_FACING_EIGHT_PICTURES:
      facing =
          gAnimationFacing_EightPictures[FACING_NEAREST_EIGHTH_SECTOR(dir)];
      break;
    case ANIM_FACING_EIGHT_PICTURES_MIRRORED:
      facing =
          gAnimationFacing_EightPicturesMirrored[FACING_NEAREST_EIGHTH_SECTOR(
              dir)];
      break;
    default:
      facing = 0;
      break;
    }
    picture += facing & ANIM_FACING_PICTURE_MASK;
    if (i == 0 && (facing & FACING_MIRRORED_BIT) != 0) {
      mirrored = 1;
    }
    if (layer->picture != picture) {
      layer->picture = picture;
      sprite->dirty = 1;
    }
  }

  if (sprite->dirty != 0) {
    size = sprite->width * sprite->height;
    pixels = Heap_Alloc(size);
    Iwram_ClearWords(pixels, size);

    // Sort the visible sprite animations by priority (0-3), then by index, with
    // an insertion sort on keys of (priority << 8) | index.
    drawCount = -1;
    for (i = sprite->count - 1; i >= 0; i--) {
      layer = sprite->entries[i];
      if (layer == 0) {
        continue;
      }
      if (layer->pictures == 0) {
        continue;
      }
      if (layer->picture == SPRITE_PICTURE_NONE) {
        continue;
      }
      key = layer->priority;
      if (key > 3) {
        continue;
      }
      key = (key << 8) | i;
      k = drawCount;
      if (k >= 0 && drawOrder[k] > key) {
        drawOrder[k + 1] = drawOrder[k];
        for (;;) {
          k--;
          if (k < 0)
            goto insert;
          if (drawOrder[k] <= key)
            goto insert;
          drawOrder[k + 1] = drawOrder[k];
        }
      }
    insert:
      drawOrder[k + 1] = key;
      drawCount++;
    }
    drawCount++;

    // Draw the sprite animations in that order, each by its codec.
    for (i = 0; i < drawCount; i++) {
      layer = sprite->entries[*(u8 *)&drawOrder[i]];
      if (layer->mode == SPRITE_PICTURE_TAGGED_LZ) {
        Decompress_TaggedLz(layer->pictures[layer->picture], pixels);
      } else if (layer->mode == SPRITE_PICTURE_LZ) {
        if (layer->colorMode != 0) {
          unpacked = Heap_Alloc(1024);
          decodePicture(
              Decompress_Lz(layer->pictures[layer->picture], unpacked), pixels,
              layer->colorMode);
          Heap_Release(unpacked);
        } else {
          decoded = Iwram_Decompress(layer->pictures[layer->picture], pixels);
          if (decoded != 0) {
            decodePicture(decoded, pixels, 0);
          }
        }
      } else {
        decodePicture(layer->pictures[layer->picture], pixels,
                      layer->colorMode);
      }
    }

    // Outline: a pixel whose four neighbours are all drawn becomes the fill
    // colour, and any other drawn pixel the edge colour.
    if ((sprite->flags & SPRITE_FLAG_OUTLINED) != 0) {
      u8 *inside;
      u8 edge;
      u8 fill;
      u32 w;
      u32 h;
      u8 *src;
      u8 *dst;

      inside = Heap_Alloc(size);
      w = sprite->width;
      h = sprite->height;
      edge = system->edge;
      fill = system->fill;
      Iwram_ClearWords(inside, size);
      src = pixels + w + 1;
      dst = inside + w + 1;
      for (i = 1; i < h - 1; i++) {
        for (k = 1; k < w - 1; k++) {
          if (src[-1] != 0 && src[1] != 0 && *(src - w) != 0 && src[w] != 0) {
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
      for (i = 0; i < size; i++) {
        if (*dst != 0) {
          *src = fill;
        } else if (*src != 0) {
          *src = edge;
        }
        dst++;
        src++;
      }
      Heap_Release(inside);
    }

    // Upload the pixels to the sprite's tiles in VRAM.
    tile = VramCache_Load(sprite->slot, size, 0);
    {
      void *vram = (void *)(OBJ_VRAM0 + tile * TILE_SIZE_4BPP);
      ((SpriteUploadFn)gHeapSlots[HEAP_SLOT_PICTURE_UPLOADER])(
          pixels, sprite->width, sprite->height, vram);
    }
    sprite->image.tile = tile;
    sprite->dirty = 0;
    system->used += size;
    Heap_Release(pixels);
  }

  if (decoderWasLoaded == 0) {
    HeapSlot_Release(HEAP_SLOT_FRAME_SCRATCH);
  }
  return mirrored;
}

/// Positions a sprite's image (part 0) and shadow (part 1) on the screen: the
/// image centred on screenX and raised height above screenY, the shadow at
/// groundHeight. scale is the sprite's (x, y) scale (16.16); above 1.0 both
/// parts use the double-size affine mode.
void Render_PlaceSpritePartPair(struct Sprite *sprite, FIXED screenX,
                                s32 height, s32 screenY, s32 groundHeight,
                                FIXED *scale) {
  u32 halfWidth = sprite->width >> 1;
  u32 halfHeight = sprite->height / 2;
  s32 marginX = SHADOW_HALF_WIDTH;
  s32 marginY = SHADOW_HALF_HEIGHT;
  s32 affine = SPRITE_PART_AFFINE;
  s32 scaleX = *scale++;
  s32 scaleY = *scale;
  s32 x;
  s32 y;
  struct AnimationSpritePart *shadow;

  // The margins are the shadow's half width and height.
  if (scaleX > FIXED_ONE || scaleY > FIXED_ONE) {
    affine = SPRITE_PART_AFFINE_DOUBLE;
    marginX = 2 * SHADOW_HALF_WIDTH;
    marginY = 2 * SHADOW_HALF_HEIGHT;
    halfWidth <<= 1;
    halfHeight <<= 1;
  }
  x = FIXEDtoINT(screenX) - halfWidth;
  // The point is offsetY pixels above the image's bottom edge: the box is
  // raised by half the height less offsetY, scaled and rounded up to a whole
  // pixel.
  y = FIXEDtoINT(screenY - height) - halfHeight -
      FIXEDtoINT(((sprite->height / 2) - sprite->offsetY) * scaleY +
                 (FIXED_ONE - 1));
  sprite->image.affine = affine;
  sprite->image.x = x;
  sprite->image.y = y;
  // Place the shadow at ground height independently of the raised image.
  x = FIXEDtoINT(screenX) - marginX;
  y = FIXEDtoINT(screenY - groundHeight) - marginY;
  shadow = &sprite->shadow;
  shadow->affine = affine;
  shadow->x = x;
  shadow->y = y;
}

/// The two rearmost depth slots: shadows drawn under everything, and the
/// sprites drawn behind every other sprite, just in front of those shadows.
#define DEPTH_SLOT_BACK_SHADOWS 0
#define DEPTH_SLOT_BACK 1

/// Render_ApplyProjectedPlacement's draw slots: a sprite one slot further in
/// front for every two screen rows lower, on from PLACEMENT_IMAGE_SLOT, and
/// its shadow in PLACEMENT_SHADOW_SLOT, behind them.
#define PLACEMENT_IMAGE_SLOT 10
#define PLACEMENT_SHADOW_SLOT 2

/// Places a sprite for the scrolling camera and queues it for drawing.
void Render_ApplyProjectedPlacement(struct Sprite *sprite,
                                    const struct SpritePlacement *position,
                                    const struct SpriteScale *scale,
                                    s32 facing) {
  const s32 *pos = SpritePlacement_ReadValues(position);
  const s32 *scaleValues = scale->values;
  s32 matrix;
  u32 halfWidth = sprite->width >> 1;
  u32 halfHeight = sprite->height >> 1;
  s32 shadowHalfWidth = SHADOW_HALF_WIDTH;
  s32 shadowHalfHeight = SHADOW_HALF_HEIGHT;
  s32 affine;
  s32 scaleX = *scaleValues++;
  s32 screenX = *pos++;
  s32 scaleY = *scaleValues;
  s32 height = *pos++;
  s32 screenY = *pos++;
  s32 groundHeight = *pos;
  s32 imageSlot;
  /* LEFTOVER(frame): two unused locals the ROM's frame reserves and never
   * touches. */
  UNUSED s32 ground[3];
  UNUSED s32 screen[3];
  struct AffineTransform effect;
  s32 flip;
  s32 x;
  s32 y;
  s32 shadowSlot;
  struct AnimationSpritePart *part;

  // Reuse an untransformed sprite when mirroring, scale and rotation allow it.
  flip = Sprite_ComposePicture(sprite, facing);
  if (flip == 0 && scaleX == FIXED_ONE && scaleY == scaleX &&
      sprite->rotation == 0) {
    affine = SPRITE_PART_PLAIN;
    matrix = 0;
  } else {
    affine = SPRITE_PART_AFFINE;
    effect.angle = sprite->rotation;
    effect.scaleX = scaleX >> 8;
    effect.scaleY = scaleY >> 8;
    if (flip != 0)
      effect.scaleX = -effect.scaleX;
    matrix = OamMatrix_AllocFromTransform(&effect);
  }
  if (scaleX > FIXED_ONE || scaleY > FIXED_ONE) {
    affine = SPRITE_PART_AFFINE_DOUBLE;
    halfWidth <<= 1;
    halfHeight <<= 1;
    shadowHalfWidth = 2 * SHADOW_HALF_WIDTH;
    shadowHalfHeight = 2 * SHADOW_HALF_HEIGHT;
  }
  // A sprite sunk 100 pixels or more draws behind the rest.
  if (height <= -toFIXED(100)) {
    imageSlot = DEPTH_SLOT_BACK;
    shadowSlot = DEPTH_SLOT_BACK_SHADOWS;
  } else {
    imageSlot = (FIXEDtoINT(screenY) >> 1) + PLACEMENT_IMAGE_SLOT;
    shadowSlot = PLACEMENT_SHADOW_SLOT;
  }
  // Queue the shadow at ground height before the raised image.
  y = FIXEDtoINT(screenY - groundHeight) - shadowHalfHeight;
  if (sprite->flags & SPRITE_FLAG_SHADOW) {
    if (y < DISPLAY_HEIGHT) {
      part = &sprite->shadow;
      part->affine = affine;
      part->affineIndex = matrix;
      part->x = FIXEDtoINT(screenX) - shadowHalfWidth;
      part->y = y;
      Oam_AddSpriteAtDepth(part, shadowSlot);
    }
  }
  x = FIXEDtoINT(screenX) - halfWidth +
      FIXEDtoINT(sprite->offsetX * scaleX + (FIXED_ONE - 1));
  y = FIXEDtoINT(screenY - height) - halfHeight -
      FIXEDtoINT(scaleY * ((sprite->height >> 1) - sprite->offsetY) +
                 (FIXED_ONE - 1));
  // Only the right and bottom edges are tested; OAM wraps a negative
  // position.
  if (x < DISPLAY_WIDTH && y < DISPLAY_HEIGHT) {
    part = &sprite->image;
    part->x = x;
    part->y = y;
    part->affine = affine;
    part->affineIndex = matrix;
    Oam_AddSpriteAtDepth(part, imageSlot);
  }
}

/// How far past the screen's left, right and top edges a projected sprite's
/// point may lie and still be drawn, and past its bottom edge.
#define PROJECTED_MARGIN 32
#define PROJECTED_MARGIN_BELOW 48
/// A projected sprite at depth PROJECTED_MIDDLE_DEPTH draws in the middle of
/// the 256 depth slots, PROJECTED_MIDDLE_SLOT, and one slot further in front
/// for every two units nearer.
#define PROJECTED_MIDDLE_DEPTH 512
#define PROJECTED_MIDDLE_SLOT 128

/// Places a sprite at 3D point for the fixed camera and queues it for drawing:
/// projected to the screen and scaled by its distance (unless useOwnScale keeps
/// its own scale), with an affine matrix as needed and its shadow on the ground
/// below. depth 0 sorts it by distance; any other value is its draw slot.
void Render_PlaceProjectedSprite(struct Sprite *sprite, s32 *point, s32 *scale,
                                 s32 facing, s32 depth) {
  s32 flip;
  u32 halfHeight;
  u32 size;
  s32 affine;
  s32 halfWidth;
  s32 ground[3];
  s32 screen[3];
  struct AffineTransform effect;
  FIXED z;
  s32 matrix;
  s32 roundedScale;
  FIXED scaleX;
  FIXED scaleY;
  FIXED offsetX;
  FIXED offsetY;
  s32 slot;

  // Project first, then discard sprites beyond the padded screen bounds.
  affine = SPRITE_PART_AFFINE;
  if (gActorSystemState->suspended == 0) {
    z = Matrix_ProjectPoint(point, screen);
    if (screen[2] != 0 && screen[0] >= -PROJECTED_MARGIN &&
        screen[0] <= DISPLAY_WIDTH + PROJECTED_MARGIN &&
        screen[1] >= -PROJECTED_MARGIN &&
        screen[1] <= DISPLAY_HEIGHT + PROJECTED_MARGIN_BELOW) {
      if (sprite->useOwnScale)
        z = sprite->scale;
      else
        z = Iwram_MulFX(z, sprite->scale);
      halfWidth = sprite->width >> 1;
      halfHeight = sprite->height >> 1;
      size = SHADOW_HALF_WIDTH;
      flip = Sprite_ComposePicture(sprite, facing);
      // Round the perspective scale to the nearest 1/32 before
      // combining it with the sprite's own scale.
      roundedScale = (z + FIXED_ONE / 64) & -(FIXED_ONE / 32);
      scaleX = Iwram_MulFX(roundedScale, *scale++);
      scaleY = Iwram_MulFX(roundedScale, *scale);
      // Hold each scale below 2 (FIXED_ONE * 63 / 32, 2 less 1/32): a scaled
      // sprite is drawn in a double-size box, which the picture exactly fills
      // at 2 and would overflow, cut off at its edges, beyond it.
      if (scaleX >= FIXED_ONE * 63 / 32)
        scaleX = FIXED_ONE * 63 / 32;
      if (scaleY >= FIXED_ONE * 63 / 32)
        scaleY = FIXED_ONE * 63 / 32;
      offsetX = Iwram_MulFX(sprite->offsetX, scaleX);
      offsetY = -Iwram_MulFX((sprite->height >> 1) - sprite->offsetY, scaleY);
      if (scaleX > FIXED_ONE || scaleY > FIXED_ONE) {
        affine = SPRITE_PART_AFFINE_DOUBLE;
        halfWidth <<= 1;
        halfHeight <<= 1;
        size = 2 * SHADOW_HALF_WIDTH;
      } else if (scaleX == FIXED_ONE && sprite->rotation == 0 &&
                 scaleY == scaleX) {
        affine = SPRITE_PART_PLAIN;
      }
      // Mirroring uses negative affine scale when a matrix is needed.
      if (affine != SPRITE_PART_PLAIN) {
        effect.angle = sprite->rotation;
        effect.scaleX = scaleX >> 8;
        if (flip != 0) {
          effect.scaleX = -effect.scaleX;
          offsetX = -offsetX;
        }
        effect.scaleY = scaleY >> 8;
        matrix = OamMatrix_AllocFromTransform(&effect);
      } else if (flip != 0) {
        matrix = SPRITE_PART_MIRRORED;
        offsetX = -offsetX;
      } else {
        matrix = 0;
      }
      {
        struct AnimationSpritePart *image = &sprite->image;

        image->x = screen[0] - halfWidth + offsetX;
        image->y = screen[1] - halfHeight + offsetY;
        image->affine = affine;
        image->affineIndex = matrix;
      }
      // The slot falls as the projected depth screen[2] rises, and stays in
      // front of the shadows' slot.
      if (depth == 0) {
        slot = (PROJECTED_MIDDLE_DEPTH - screen[2]) / 2 + PROJECTED_MIDDLE_SLOT;
        if (slot <= 0)
          slot = DEPTH_SLOT_BACK;
        Oam_AddSpriteAtDepth(sprite, slot);
      } else {
        Oam_AddSpriteAtDepth(sprite, depth);
      }
      // Project the ground point separately so jumping does not lift the
      // shadow.
      if (sprite->flags & SPRITE_FLAG_SHADOW) {
        struct AnimationSpritePart *shadow;

        SET_VECTOR(ground, point[0], 0, point[2]);
        Matrix_ProjectPoint(ground, screen);
        shadow = &sprite->shadow;
        shadow->x = screen[0] - size;
        // Centred 2 pixels below the ground point, so the feet stand on the
        // shadow's upper half (the World Map's leader, render probe
        // --scene WorldMap).
        shadow->y = screen[1] - (size >> 1) + 2;
        shadow->affine = affine;
        shadow->affineIndex = matrix;
        if (depth == 0)
          Oam_AddSpriteAtDepth(shadow, DEPTH_SLOT_BACK_SHADOWS);
        else
          Oam_AddSpriteAtDepth(shadow, depth);
      }
      return;
    }
  }
  // Invisible sprites release cached tiles unless their display flags hold
  // them; marked dirty, the sprite is redrawn into new VRAM when it next
  // comes on screen.
  if (!sprite->retainCacheEntry) {
    VramCache_ReleaseVram(sprite->slot);
    sprite->dirty = 1;
  }
}

/// Sets the colour mode (SpriteColorMode) of each of a sprite's animations to
/// colorMode, except sprite animations drawn hidden (SPRITE_COLOR_HIDDEN), and
/// marks the sprite for redrawing.
void Sprite_SetAnimationColorModes(void *rawSprite, u32 colorMode) {
  struct Sprite *sprite = rawSprite;
  if (sprite != 0) {
    u32 count = sprite->count;

    if (count != 0) {
      struct SpriteAnimation **entry = sprite->entries;
      u32 remaining = count;
      do {
        struct SpriteAnimation *layer = *entry++;
        if (layer->colorMode != SPRITE_COLOR_HIDDEN)
          layer->colorMode = colorMode;
        remaining--;
      } while (remaining != 0);
    }
    sprite->dirty = 1;
  }
}
