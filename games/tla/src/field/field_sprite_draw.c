/// @file
/// Drawing sprites: composing their animations' pictures and placing projected
/// sprites on screen.
// Part of field.c.

#include "field_internal.h"

#include "field/actor.h"
#include "system/affine.h"
#include "fixed_math.h"
#include "field/object.h"
#include "system/system.h"
#include "system/task.h"
#include "macros.h"

/// SpriteAnimation.mode: how a layer's pictures are packed, each with its
/// decoder.
enum SpritePictureCodec {
  SPRITE_PICTURE_RAW = 0,
  SPRITE_PICTURE_TAGGED_LZ = 1, ///< Decompress_TaggedLz's format
  SPRITE_PICTURE_LZ = 3         ///< LZ-compressed
};

/// An OAM entry's affine mode (AnimationSpritePart.affine): a plain sprite, a
/// scaled or turned one, or one drawn in a box twice its size so that a scale
/// above 1 is not cut off.
enum OamAffineMode {
  OAM_AFFINE_OFF = 0,
  OAM_AFFINE_ON = 1,
  OAM_AFFINE_DOUBLE_SIZE = 3,
};

/// Facing table `table`'s entry for facing angle `dir`, the turn cut into as
/// many sectors as the table has entries.
#define FACING_TABLE_ENTRY(table, dir)                                         \
  ((table)[FACING_SECTOR(dir, ARRAY_COUNT(table))])

/// Runs each of the sprite's sprite animations on to its next stop (an end,
/// hold, jump or play command), taking the pictures and settings on the way,
/// and leaves the stop to run next; marks the sprite and each sprite animation
/// changed. The battle's motion calls it before it releases a borrowed sprite
/// buffer.
void Sprite_SkipToStop(struct Sprite *sprite) {
  s32 i;

  for (i = 0; i < sprite->count; i++) {
    struct SpriteAnimation *animation = sprite->entries[i];
    u8 command;
    u8 argument;

    if (animation == 0 || animation->script == 0)
      continue;
    for (;;) {
      command = animation->script[animation->scriptOffset++];
      argument = animation->script[animation->scriptOffset++];
      switch (command) {
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

/// Advances a sprite's animations by one frame, picks each sprite animation's
/// picture for the facing dir, and when any picture changed redraws the sprite:
/// the sprite animations composed by priority into one pixel buffer, an
/// optional outline, and the result uploaded to the sprite's VRAM tiles.
s32 Sprite_ComposePicture(struct Sprite *sprite, u32 dir) {
  s32 mirrored;
  u32 size;
  struct ActorSystemState *system;
  DrawFn decodePicture;
  s32 decoderWasLoaded;
  struct SpriteAnimation *layer;
  u8 *pixels;
  u8 *unpacked;
  void *decoded;
  u16 drawOrder[4];
  s32 i;
  s32 k;
  s32 drawCount;
  u32 key;
  s32 argument;
  s32 picture;
  u32 facing;
  s32 tile;
  void *vram;

  mirrored = 0;
  system = Ram_HeapSlots->actorSystem;
  decoderWasLoaded = 1;
  decodePicture = Ram_HeapSlots->frameScratch;
  if (decodePicture == NULL) {
    Render_LoadDecoder();
    decodePicture = Ram_HeapSlots->frameScratch;
    decoderWasLoaded = 0;
  }

  for (i = 0; i < sprite->count; i++) {
    layer = sprite->entries[i];
    if (layer == NULL)
      continue;
    if (layer->script == NULL)
      continue;
  read_command:
    if ((s16)layer->timer <= 0) {
      picture = layer->script[layer->scriptOffset++];
      argument = layer->script[layer->scriptOffset++];
      switch (picture) {
      case ANIMCMD_PLAY:
        SpriteAnimation_Start(layer, argument);
        sprite->lastNo = argument;
        break;
      case ANIMCMD_JUMP:
        layer->scriptOffset = argument;
        break;
      case ANIMCMD_SOUND:
        Sound_PlayFar(argument);
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
        // Falls through: its time counts as ANIMCMD_WAIT's does.
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
    case ANIM_FACING_FIVE_PICTURES:
      facing = FACING_TABLE_ENTRY(gAnimationFacing_FivePictures, dir);
      break;
    case ANIM_FACING_EIGHT_PICTURES:
      // Half a sector on, so each sector is centred on its direction.
      facing =
          gAnimationFacing_EightPictures[FACING_SECTOR(dir + FACING_STEP, 8)];
      break;
    default:
      facing = 0;
      break;
    }
    picture += facing & ANIM_FACING_PICTURE_MASK;
    if (i == 0 && (facing & ANIM_FACING_MIRRORED) != 0)
      mirrored = 1;
    if (layer->picture != picture) {
      layer->picture = picture;
      sprite->dirty = 1;
    }
  }

  if (sprite->flags & SPRITE_FLAG_KEEP_PICTURE) {
    if (FACING_TABLE_ENTRY(gAnimationFacing_FrontBack, dir) &
        ANIM_FACING_MIRRORED)
      mirrored = 1;
  } else if (sprite->dirty != 0) {
    size = sprite->width * sprite->height;
    pixels = Heap_Alloc(size);
    Iwram_ClearWords(pixels, size);

    drawCount = -1;

    for (i = sprite->count - 1; i >= 0; i--) {
      layer = sprite->entries[i];
      if (layer == 0)
        continue;
      if (layer->pictures == 0)
        continue;
      if (layer->picture == SPRITE_PICTURE_NONE)
        continue;
      key = layer->priority;
      if (key > 3)
        continue;
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
          if (decoded != 0)
            decodePicture(decoded, pixels, 0);
        }
      } else {
        decodePicture(layer->pictures[layer->picture], pixels,
                      layer->colorMode);
      }
    }

    if (sprite->flags &
        (SPRITE_FLAG_OUTLINED | SPRITE_FLAG_SHADED | SPRITE_FLAG_SUNK)) {
      if (sprite->flags & SPRITE_FLAG_OUTLINED) {
        OutlineFn outline;

        RamCode_Load(outline, Grid_DrawOutlineCode, Grid_DrawOutlineCodeSize);
        outline(sprite->width, sprite->height, pixels, system->edge,
                system->fill);
        Heap_Release(outline);
      }
      if (sprite->flags & SPRITE_FLAG_SUNK) {
        s32 height;
        s32 width;
        s32 amount;
        s32 limit;
        u32 *dst;
        u32 *src;

        // picture holds the rows to sink.
        picture = sprite->sunkRows;
        height = sprite->height;
        width = sprite->width;
        if (picture > height)
          picture = height;
        dst = (u32 *)(pixels + width * height);
        dst--;
        limit = width;
        for (i = 0; i < limit; i++)
          *dst-- = 0;
        amount = width * picture;
        src = dst - amount / 4;
        limit = (height - picture - 4) * width / 4;
        for (i = 0; i < limit; i++)
          *dst-- = *src--;
        limit = amount / 4;
        for (i = 0; i < limit; i++)
          *dst-- = 0;
      } else if (sprite->flags & SPRITE_FLAG_SHADED) {
        u32 rows = sprite->sunkRows;
        u32 height = sprite->height;
        u32 width = sprite->width;
        const u8 *shade = gSpriteColorTintTables;
        u8 *p;
        s32 n;

        if (rows > height)
          rows = height;
        n = width * rows;
        p = pixels + (height - rows) * width;
        for (i = 0; i < n; i++) {
          *p = shade[*p];
          p++;
        }
      }
    }

    tile = VramCache_Load(sprite->slot, size, 0);
    vram = (void *)(tile * TILE_SIZE_4BPP + OBJ_VRAM0);
    ((UploadFn)Ram_HeapSlots->pictureUploader)(pixels, sprite->width,
                                               sprite->height, vram);
    sprite->image.tile = tile;
    sprite->dirty = 0;
    system->used += size;
    Heap_Release(pixels);
  }

  if (decoderWasLoaded == 0)
    HeapSlot_Release(HEAP_SLOT_FRAME_SCRATCH);
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
  s32 affine = OAM_AFFINE_ON;
  s32 scaleX = *scale++;
  s32 scaleY = *scale;
  s32 x;
  s32 y;
  struct AnimationSpritePart *shadow;

  if (scaleX > FIXED_ONE || scaleY > FIXED_ONE) {
    affine = OAM_AFFINE_DOUBLE_SIZE;
    halfWidth <<= 1;
    halfHeight <<= 1;
    marginX = 2 * SHADOW_HALF_WIDTH;
    marginY = 2 * SHADOW_HALF_HEIGHT;
  }
  x = FIXEDtoINT(screenX) - halfWidth;
  y = FIXEDtoINT(screenY - height) - halfHeight -
      FIXEDtoINT(((sprite->height / 2) - sprite->offsetY) * scaleY +
                 (FIXED_ONE - 1));
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

/// The depth slot Render_ApplyProjectedPlacement gives a sprite's shadow: two
/// behind the sprite's own, PLACEMENT_IMAGE_DEPTH (field/field.h).
#define PLACEMENT_SHADOW_DEPTH (PLACEMENT_IMAGE_DEPTH - 2)

/// Places a sprite for the scrolling camera and queues it for drawing: the
/// sprite at the screen point of a projected position (x, height, y, ground),
/// scaled by scale[0] and scale[1] and turned by its rotation, and its shadow
/// on the ground below; both go into the depth list by their screen row.
void Render_ApplyProjectedPlacement(struct Sprite *sprite, s32 *pos, s32 *scale,
                                    s32 mode) {
  s32 matrix;
  u32 halfWidth;
  u32 halfHeight;
  s32 affine;
  s32 px;
  s32 py;
  s32 pz;
  s32 pw;
  s32 scaleX;
  s32 scaleY;
  /* LEFTOVER(frame): two unused locals the ROM's frame reserves and never
   * touches. */
  UNUSED s32 ground[3];
  UNUSED s32 screen[3];
  s32 flip;
  struct AffineTransform effect;
  s32 anchorX;
  s32 anchorY;
  s32 x;
  s32 y;
  struct AnimationSpritePart *part;

  halfWidth = sprite->width >> 1;
  halfHeight = sprite->height >> 1;
  scaleX = *scale++;
  scaleY = *scale;
  px = *pos++;
  py = *pos++;
  pz = *pos++;
  pw = *pos;
  flip = Sprite_ComposePicture(sprite, mode);
  if (flip == 0 && scaleX == FIXED_ONE && scaleY == scaleX &&
      sprite->rotation == 0) {
    affine = OAM_AFFINE_OFF;
    matrix = 0;
  } else {
    affine = OAM_AFFINE_ON;
    effect.angle = sprite->rotation;
    effect.scaleX = scaleX >> 8;
    effect.scaleY = scaleY >> 8;
    if (flip != 0)
      effect.scaleX = -effect.scaleX;
    matrix = OamMatrix_AllocFromTransform(&effect);
  }
  if (scaleX > FIXED_ONE || scaleY > FIXED_ONE) {
    affine = OAM_AFFINE_DOUBLE_SIZE;
    halfWidth <<= 1;
    halfHeight <<= 1;
  }

  // The shadow, centred on the ground below the sprite.
  y = FIXEDtoINT(pz - pw) - SHADOW_HALF_HEIGHT;
  if ((sprite->flags & SPRITE_FLAG_SHADOW) && y < DISPLAY_HEIGHT) {
    part = &sprite->shadow;
    part->affine = OAM_AFFINE_OFF;
    part->x = FIXEDtoINT(px) - SHADOW_HALF_WIDTH;
    part->y = y;
    Oam_AddSpriteAtDepthClamped(part,
                                FIXEDtoINT(pz + pw) + PLACEMENT_SHADOW_DEPTH);
  }

  if (scaleX == 0 || scaleY == 0)
    return;

  // The anchor, scaled, and turned with the sprite.
  if (sprite->rotation == 0) {
    anchorX = sprite->offsetX * scaleX;
    anchorY = ((sprite->height >> 1) - sprite->offsetY) * scaleY;
  } else {
    FIXED sin = Trig_Sin(sprite->rotation);
    FIXED cos = Trig_Cos(sprite->rotation);
    s32 offsetX = sprite->offsetX * scaleX;
    s32 offsetY = ((sprite->height >> 1) - sprite->offsetY) * scaleY;

    anchorX = Iwram_MulFX(cos, offsetX) + Iwram_MulFX(sin, offsetY);
    anchorY = Iwram_MulFX(cos, offsetY) - Iwram_MulFX(sin, offsetX);
  }
  x = FIXEDtoINT(px) - halfWidth + FIXEDtoINT(anchorX + (FIXED_ONE - 1));
  y = FIXEDtoINT(pz - py) - halfHeight - FIXEDtoINT(anchorY + (FIXED_ONE - 1));
  // Only the right and bottom edges are tested; OAM wraps a negative
  // position.
  if (x < DISPLAY_WIDTH && y < DISPLAY_HEIGHT) {
    part = &sprite->image;
    part->x = x;
    part->y = y;
    part->affine = affine;
    part->affineIndex = matrix;
    Oam_AddSpriteAtDepthClamped(part,
                                FIXEDtoINT(py + pz) + PLACEMENT_IMAGE_DEPTH);
  }
}

/// Render_ApplyProjectedPlacement into the given draw slot: the sprite and its
/// shadow both go to depth, and mode chooses the picture as facing does there.
/// It also moves a double-size sprite's shadow by twice its half size, and its
/// frame and locals differ, so the two stay separate routines.
void Render_ApplyProjectedPlacementAtDepth(struct Sprite *sprite, s32 *pos,
                                           s32 *scale, s32 mode, s32 depth) {
  s32 matrix;
  u32 halfWidth;
  u32 halfHeight;
  s32 extentX;
  s32 affine;
  s32 px;
  s32 py;
  s32 pz;
  s32 pw;
  s32 scaleX;
  s32 scaleY;
  s32 extentY;
  /* LEFTOVER(frame): two unused locals the ROM's frame reserves and never
   * touches. */
  UNUSED s32 ground[3];
  UNUSED s32 screen[3];
  s32 flip;
  struct AffineTransform effect;
  s32 anchorX;
  s32 anchorY;
  s32 x;
  s32 y;
  struct AnimationSpritePart *part;

  halfWidth = sprite->width >> 1;
  {
    u8 height = sprite->height;

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
  flip = Sprite_ComposePicture(sprite, mode);
  if (flip == 0 && scaleX == FIXED_ONE && scaleY == scaleX &&
      sprite->rotation == 0) {
    affine = OAM_AFFINE_OFF;
    matrix = 0;
  } else {
    affine = OAM_AFFINE_ON;
    effect.angle = sprite->rotation;
    effect.scaleX = scaleX >> 8;
    effect.scaleY = scaleY >> 8;
    if (flip != 0)
      effect.scaleX = -effect.scaleX;
    matrix = OamMatrix_AllocFromTransform(&effect);
  }
  if (scaleX > FIXED_ONE || scaleY > FIXED_ONE) {
    affine = OAM_AFFINE_DOUBLE_SIZE;
    halfWidth <<= 1;
    halfHeight <<= 1;
    extentX = 2 * SHADOW_HALF_WIDTH;
    extentY = 2 * SHADOW_HALF_HEIGHT;
  }

  y = FIXEDtoINT(pz - pw) - extentY;
  if ((sprite->flags & SPRITE_FLAG_SHADOW) && y < DISPLAY_HEIGHT) {
    part = &sprite->shadow;
    part->affine = OAM_AFFINE_OFF;
    part->x = FIXEDtoINT(px) - extentX;
    part->y = y;
    Oam_AddSpriteAtDepth(part, depth);
  }

  if (scaleX == 0 || scaleY == 0)
    return;

  if (sprite->rotation == 0) {
    anchorX = sprite->offsetX * scaleX;
    anchorY = ((sprite->height >> 1) - sprite->offsetY) * scaleY;
  } else {
    FIXED sin = Trig_Sin(sprite->rotation);
    FIXED cos = Trig_Cos(sprite->rotation);

    // extentX and extentY are reused for the anchor's offset.
    extentX = sprite->offsetX * scaleX;
    {
      u8 height = sprite->height;

      extentY = ((height >> 1) - sprite->offsetY) * scaleY;
    }

    anchorX = Iwram_MulFX(cos, extentX) + Iwram_MulFX(sin, extentY);
    anchorY = Iwram_MulFX(cos, extentY) - Iwram_MulFX(sin, extentX);
  }
  x = FIXEDtoINT(px) - halfWidth + FIXEDtoINT(anchorX + (FIXED_ONE - 1));
  y = FIXEDtoINT(pz - py) - halfHeight - FIXEDtoINT(anchorY + (FIXED_ONE - 1));
  if (x < DISPLAY_WIDTH && y < DISPLAY_HEIGHT) {
    part = &sprite->image;
    part->x = x;
    part->y = y;
    part->affine = affine;
    part->affineIndex = matrix;
    Oam_AddSpriteAtDepth(part, depth);
  }
}

/// How far past the screen's left, right and top edges a projected sprite's
/// point may lie and still be drawn, and past its bottom edge.
#define PROJECTED_MARGIN 32
#define PROJECTED_MARGIN_BELOW 48
/// The depth slot of a projected sprite at depth 0; the slot falls by one for
/// each two units of depth.
#define PROJECTED_NEAREST_DEPTH 384
/// The size, shape, colours, mosaic and mode of a sprite's OAM attributes 0
/// and 1, which placing it keeps.
#define OAM_ATTR01_KEPT_BITS 0xc000fc00

/// Places a sprite at a 3D point for the fixed camera and queues it for
/// drawing: projected to the screen and scaled by its distance (unless
/// useOwnScale keeps its own scale), with an affine matrix as needed and its
/// shadow on the ground below. depth 0 sorts it by distance; any other value
/// is its draw slot.
void Render_PlaceProjectedSprite(struct Sprite *sprite, s32 *point, s32 *scale,
                                 s32 facing, s32 depth) {
  s32 screen[3];
  s32 ground[3];
  s32 z;
  s32 flip;
  s32 rounded;
  FIXED scaleX;
  FIXED scaleY;
  s32 affine;
  FIXED offsetX;
  FIXED offsetY;
  u32 halfWidth;
  u32 halfHeight;
  u32 matrix;
  u32 size;

  if (Ram_HeapSlots->actorSystem->suspended == 0) {
    z = Matrix_ProjectPoint(point, screen);
    if (screen[2] != 0 &&
        (u32)(screen[0] + PROJECTED_MARGIN) <=
            DISPLAY_WIDTH + 2 * PROJECTED_MARGIN &&
        (u32)(screen[1] + PROJECTED_MARGIN) <=
            DISPLAY_HEIGHT + PROJECTED_MARGIN + PROJECTED_MARGIN_BELOW) {
      if (sprite->useOwnScale)
        z = FIXED_ONE;
      z = Iwram_MulFX(z, sprite->scale);
      flip = Sprite_ComposePicture(sprite, facing);
      // The perspective scale rounded to the nearest 1/32.
      z >>= 10;
      rounded = ((z + 1) >> 1) * (FIXED_ONE / 32);
      scaleX = Iwram_MulFX(rounded, scale[0]);
      scaleY = Iwram_MulFX(rounded, scale[1]);
      // Held below 2 (2 less 1/32): a scaled sprite is drawn in a double-size
      // box, which the picture fills at 2 and would overflow beyond it.
      if (scaleX > FIXED_ONE * 63 / 32)
        scaleX = FIXED_ONE * 63 / 32;
      if (scaleY > FIXED_ONE * 63 / 32)
        scaleY = FIXED_ONE * 63 / 32;
      halfHeight = sprite->height >> 1;
      offsetY = Iwram_MulFX(sprite->offsetY - halfHeight, scaleY);
      halfWidth = sprite->width >> 1;
      if (scaleX > FIXED_ONE || scaleY > FIXED_ONE) {
        affine = OAM_AFFINE_DOUBLE_SIZE;
        halfWidth <<= 1;
        halfHeight <<= 1;
      } else {
        affine = OAM_AFFINE_ON;
        if (scaleX == FIXED_ONE && sprite->rotation == 0 && scaleY == scaleX)
          affine = OAM_AFFINE_OFF;
      }
      if (flip)
        scaleX = -scaleX;
      if (affine)
        matrix = AffineMatrix_Build(sprite->rotation, scaleX >> 8, scaleY >> 8);
      else {
        // A plain sprite has no matrix: 8 in the matrix field is OAM's
        // horizontal flip bit, set while scaleX is negative.
        matrix = scaleX < 0;
        matrix <<= 3;
      }
      if (scaleX != 0 && scaleY != 0) {
        offsetX = Iwram_MulFX(sprite->offsetX, scaleX);
        ((u32 *)&sprite->image)[1] =
            (((u32 *)&sprite->image)[1] & OAM_ATTR01_KEPT_BITS) |
            (affine << 8) | OBJ_AFFINE_MATRIX(matrix) |
            // OBJ_POSITION as shifts: x's 9 bits into attribute 1, y's 8 into
            // attribute 0.
            ((u32)(screen[0] - halfWidth + offsetX) << 23 >> 7) |
            (u8)(screen[1] - halfHeight + offsetY);
        if (depth == 0) {
          s32 slot = PROJECTED_NEAREST_DEPTH - (screen[2] >> 1);

          if (slot <= 0)
            slot = 1;
          Oam_AddSpriteAtDepthClamped(sprite, slot);
        } else {
          Oam_AddSpriteAtDepthClamped(sprite, depth);
        }
      }
      if (sprite->flags & SPRITE_FLAG_SHADOW) {
        struct AnimationSpritePart *shadow;

        ground[0] = point[0];
        ground[1] = 0;
        ground[2] = point[2];
        Matrix_ProjectPoint(ground, screen);
        size = SHADOW_HALF_WIDTH;
        if (affine == OAM_AFFINE_DOUBLE_SIZE)
          size = 2 * SHADOW_HALF_WIDTH;
        shadow = &sprite->shadow;
        shadow->x = screen[0] - size;
        shadow->y = screen[1] - (size >> 1) + 2;
        shadow->affine = affine;
        shadow->affineIndex = matrix;
        if (depth == 0)
          Oam_AddSpriteAtDepthClamped(shadow, 0);
        else
          Oam_AddSpriteAtDepthClamped(shadow, depth);
      }
      return;
    }
  }
  if (Ram_HeapSlots->battleState == NULL && !sprite->retainCacheEntry) {
    VramCache_ReleaseVram(sprite->slot);
    sprite->dirty = 1;
  }
}

/// Sets the colour mode (enum SpriteColorMode) of the sprite's first sprite
/// animation and has it drawn again.
void Sprite_SetFirstAnimationColorMode(struct Sprite *sprite, u32 colorMode) {
  if (sprite != NULL) {
    sprite->entries[0]->colorMode = colorMode;
    sprite->dirty = 1;
  }
}

/// Sets the colour mode of every sprite animation of the sprite that is not
/// hidden (SPRITE_COLOR_HIDDEN) and has it drawn again.
void Sprite_SetAnimationColorModes(struct Sprite *sprite, u32 colorMode) {
  s32 i;

  if (sprite != NULL) {
    for (i = 0; i < sprite->count; i++) {
      if (sprite->entries[i]->colorMode != SPRITE_COLOR_HIDDEN)
        sprite->entries[i]->colorMode = colorMode;
    }
    sprite->dirty = 1;
  }
}

#undef FACING_TABLE_ENTRY
