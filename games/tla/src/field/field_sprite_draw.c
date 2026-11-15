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

/// Runs each of the player's animation channels on to its next stop (an
/// end, hold, jump or play command), taking the frames and
/// settings on the way, and leaves the stop to run next; marks the player and
/// each channel changed. The battle's motion calls it before it releases a
/// borrowed sprite buffer.
void Animation_SkipToStop(struct Sprite *player) {
  s32 i;

  for (i = 0; i < player->count; i++) {
    struct SpriteAnimation *channel = player->entries[i];
    u8 command;
    u8 argument;

    if (channel == 0 || channel->script == 0)
      continue;
    for (;;) {
      command = channel->script[channel->scriptOffset++];
      argument = channel->script[channel->scriptOffset++];
      switch (command) {
      case ANIMCMD_SET_KIND:
        channel->kind = argument;
        continue;
      case ANIMCMD_OFFSET_X:
        player->offsetX = argument;
        continue;
      case ANIMCMD_OFFSET_Y:
        player->offsetY = argument;
        continue;
      case ANIMCMD_BLANK:
        channel->pictureBase = SPRITE_PICTURE_NONE;
        continue;
      case 244:
      case ANIMCMD_WAIT ... 250:
      case ANIMCMD_SOUND:
        continue;
      default:
        channel->pictureBase = command;
        continue;
      case ANIMCMD_END:
      case ANIMCMD_HOLD:
      case ANIMCMD_JUMP:
      case ANIMCMD_PLAY:
        break;
      }
      break;
    }
    channel->scriptOffset -= 2;
    player->dirty = 1;
    channel->timer = 1;
  }
}

/// Advances a sprite's animations by one frame, picks each sprite animation's
/// picture for the facing dir, and when any picture changed redraws the sprite:
/// the sprite animations composed by priority into one pixel buffer, an
/// optional outline, and the result uploaded to the sprite's VRAM tiles.
s32 Sprite_ComposePicture(struct Sprite *obj, u32 dir) {
  s32 mirrored;
  u32 size;
  struct ObjectSystemColours *system;
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
  s32 arg;
  s32 picture;
  u32 facing;
  s32 tile;
  void *vram;

  mirrored = 0;
  system = (struct ObjectSystemColours *)Ram_HeapSlots->actorSystem;
  decoderWasLoaded = 1;
  decodePicture = Ram_HeapSlots->frameScratch;
  if (decodePicture == NULL) {
    Render_LoadDecoder();
    decodePicture = Ram_HeapSlots->frameScratch;
    decoderWasLoaded = 0;
  }

  for (i = 0; i < obj->count; i++) {
    layer = obj->entries[i];
    if (layer == NULL)
      continue;
    if (layer->script == NULL)
      continue;
  read_command:
    if ((s16)layer->timer <= 0) {
      picture = layer->script[layer->scriptOffset++];
      arg = layer->script[layer->scriptOffset++];
      switch (picture) {
      case ANIMCMD_PLAY:
        SpriteAnimation_Start(layer, arg);
        obj->lastNo = arg;
        break;
      case ANIMCMD_JUMP:
        layer->scriptOffset = arg;
        break;
      case ANIMCMD_SOUND:
        Sound_PlayFar(arg);
        break;
      case ANIMCMD_HOLD:
        layer->scriptOffset -= 2;
        picture = layer->pictureBase;
        goto select_direction;
      case ANIMCMD_SET_KIND:
        layer->kind = arg;
        break;
      case ANIMCMD_OFFSET_X:
        obj->offsetX = arg;
        break;
      case ANIMCMD_OFFSET_Y:
        obj->offsetY = arg;
        break;
      case ANIMCMD_BLANK:
        layer->pictureBase = SPRITE_PICTURE_NONE;
        // Falls through: its time counts as ANIMCMD_WAIT's does.
      case ANIMCMD_WAIT:
        layer->timer += arg << 4;
        break;
      case ANIMCMD_END:
        layer->pictureBase = SPRITE_PICTURE_NONE;
        layer->script = NULL;
        obj->count--;
        picture = SPRITE_PICTURE_NONE;
        goto select_direction;
      // Unused commands, skipped. 251 is not among them: it shows as a picture.
      case 244:
      case ANIMCMD_WAIT + 1 ... 250:
        break;
      default:
        layer->pictureBase = picture;
        layer->timer += arg << 4;
        break;
      }
      goto read_command;
    }
    layer->timer = layer->timer - layer->step;
    picture = layer->pictureBase;
  select_direction:
    switch (layer->kind) {
    case ANIM_FACING_ONE_PICTURE:
      facing = gAnimationFacing_OnePicture[(dir >> 13) & 7];
      break;
    case ANIM_FACING_FRONT_BACK:
    case ANIM_FACING_TWO_PART:
      facing = gAnimationFacing_FrontBack[(dir >> 13) & 7];
      break;
    case ANIM_FACING_FRONT_BACK_SWAPPED:
      facing = gAnimationFacing_FrontBackSwapped[(dir >> 13) & 7];
      break;
    case ANIM_FACING_THREE_PICTURES:
      facing = gAnimationFacing_ThreePictures[(dir >> 12) & 15];
      break;
    case ANIM_FACING_SIX_PICTURES:
      facing = gAnimationFacing_SixPictures[(dir >> 10) & 63];
      break;
    case ANIM_FACING_EIGHT_PICTURES:
      facing = gAnimationFacing_EightPictures[((dir + FACING_STEP) >> 13) & 7];
      break;
    default:
      facing = 0;
      break;
    }
    picture += facing & 7;
    if (i == 0 && (facing >> 7) != 0)
      mirrored = 1;
    if (layer->picture != picture) {
      layer->picture = picture;
      obj->dirty = 1;
    }
  }

  if (obj->flags & 0x20) {
    if (gAnimationFacing_FrontBack[(dir >> 13) & 7] & 0x80)
      mirrored = 1;
  } else if (obj->dirty != 0) {
    size = obj->width * obj->height;
    pixels = Heap_Alloc(size);
    Iwram_ClearWords(pixels, size);

    drawCount = -1;

    for (i = obj->count - 1; i >= 0; i--) {
      layer = obj->entries[i];
      if (layer == 0)
        continue;
      if (layer->pictures == 0)
        continue;
      if (layer->picture == 255)
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
      layer = obj->entries[*(u8 *)&drawOrder[i]];
      if (layer->mode == 1) {
        Decompress_TaggedLz(layer->pictures[layer->picture], pixels);
      } else if (layer->mode == 3) {
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

    if (obj->flags & 14) {
      if (obj->flags & 2) {
        OutlineFn outline;

        RamCode_Load(outline, Grid_DrawOutlineCode, Grid_DrawOutlineCodeSize);
        outline(obj->width, obj->height, pixels, system->edge, system->fill);
        Heap_Release(outline);
      }
      if (obj->flags & 8) {
        s32 height;
        s32 width;
        s32 amount;
        s32 limit;
        u32 *dst;
        u32 *src;

        // Rows to sink, held in picture: see the header.
        picture = obj->sunkRows;
        height = obj->height;
        width = obj->width;
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
      } else if (obj->flags & 4) {
        u32 rows = obj->sunkRows;
        u32 height = obj->height;
        u32 width = obj->width;
        const u8 *shade = gSpriteColorShadeTables;
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

    tile = VramCache_Load(obj->slot, size, 0);
    vram = (void *)((tile << 5) + 0x06010000);
    ((UploadFn)Ram_HeapSlots->pictureUploader)(pixels, obj->width, obj->height,
                                               vram);
    obj->image.tile = tile;
    obj->dirty = 0;
    system->used += size;
    Heap_Release(pixels);
  }

  if (decoderWasLoaded == 0)
    HeapSlot_Release(HEAP_SLOT_FRAME_SCRATCH);
  return mirrored;
}

// The height read through a u8 copy: see the FAKEMATCH tag below.
#define HEIGHT_COPY(sprite)                                                    \
  ({                                                                           \
    u8 heightCopy = (sprite)->height;                                          \
    heightCopy;                                                                \
  })

/// Positions a sprite's image (part 0) and shadow (part 1) on the screen: the
/// image centred on screenX and raised height above screenY, the shadow at
/// groundHeight. scale is the sprite's (x, y) scale (16.16); above 1.0 both
/// parts use the double-size affine mode.
void Render_PlaceSpritePartPair(struct Sprite *sprite, s32 screenX, s32 height,
                                s32 screenY, s32 groundHeight, s32 *scale) {
  u32 halfWidth = sprite->width >> 1;
  u32 halfHeight = HEIGHT_COPY(sprite) >> 1;
  s32 marginX = 8;
  s32 marginY = 4;
  s32 affine = 1;
  s32 scaleX = *scale++;
  s32 scaleY = *scale;
  s32 x;
  s32 y;
  struct AnimationSpritePart *shadow;

  /* FAKEMATCH(temporary): the height read through HEIGHT_COPY, a u8 copy, at
   * both uses. */
  if (scaleX > 0x10000 || scaleY > Q16_ONE) {
    affine = 3;
    halfWidth <<= 1;
    halfHeight <<= 1;
    marginX = 16;
    marginY = 8;
  }
  x = FIELD_PIXEL(screenX) - halfWidth;
  y = FIELD_PIXEL(screenY - height) - halfHeight -
      ((((HEIGHT_COPY(sprite) >> 1) - sprite->offsetY) * scaleY + 0xffff) >>
       16);
  sprite->image.affine = affine;
  sprite->image.x = x;
  sprite->image.y = y;
  x = FIELD_PIXEL(screenX) - marginX;
  y = FIELD_PIXEL(screenY - groundHeight) - marginY;
  shadow = &sprite->shadow;
  shadow->affine = affine;
  shadow->x = x;
  shadow->y = y;
}

// The object casts a shadow.
#define OBJECT_SHADOW 1

/// Places an animation object's sprite (and its shadow) at the screen point
/// of a projected position (x, y, z, ground), scaled by scale[0] and
/// scale[1]; the sprites go into the depth list by their depth.
/// Places a sprite for the scrolling camera and queues it for drawing.
void Render_ApplyProjectedPlacement(struct Sprite *object, s32 *pos, s32 *scale,
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
  // Unused: gives the stack frame its 24 bytes.
  s32 ground[3];
  s32 screen[3];
  s32 flip;
  struct AffineTransform effect;
  s32 anchorX;
  s32 anchorY;
  s32 x;
  s32 y;
  struct AnimationSpritePart *part;

  halfWidth = object->width >> 1;
  halfHeight = object->height >> 1;
  scaleX = *scale++;
  scaleY = *scale;
  px = *pos++;
  py = *pos++;
  pz = *pos++;
  pw = *pos;
  flip = Sprite_ComposePicture(object, mode);
  if (flip == 0 && scaleX == 0x10000 && scaleY == scaleX &&
      object->rotation == 0) {
    affine = 0;
    matrix = 0;
  } else {
    affine = 1;
    effect.angle = object->rotation;
    effect.scaleX = scaleX >> 8;
    effect.scaleY = scaleY >> 8;
    if (flip != 0)
      effect.scaleX = -effect.scaleX;
    matrix = OamMatrix_AllocFromTransform(&effect);
  }
  if (scaleX > Q16_ONE || scaleY > Q16_ONE) {
    affine = 3;
    halfWidth <<= 1;
    halfHeight <<= 1;
  }

  // The shadow: an 8 by 4 sprite on the ground below the object.
  y = FIELD_PIXEL(pz - pw) - 4;
  if ((object->flags & OBJECT_SHADOW) && y < 160) {
    part = &object->shadow;
    part->affine = 0;
    part->x = FIELD_PIXEL(px) - 8;
    part->y = y;
    Oam_AddSpriteAtDepthClamped(part, FIELD_PIXEL(pz + pw) + 60);
  }

  if (scaleX == 0 || scaleY == 0)
    return;

  // The anchor, scaled, and turned with the object.
  if (object->rotation == 0) {
    anchorX = object->offsetX * scaleX;
    anchorY = ((object->height >> 1) - object->offsetY) * scaleY;
  } else {
    s32 sin = Trig_Sin(object->rotation);
    s32 cos = Trig_Cos(object->rotation);
    s32 offsetX = object->offsetX * scaleX;
    s32 offsetY = ((object->height >> 1) - object->offsetY) * scaleY;

    anchorX = Iwram_MulQ16(cos, offsetX) + Iwram_MulQ16(sin, offsetY);
    anchorY = Iwram_MulQ16(cos, offsetY) - Iwram_MulQ16(sin, offsetX);
  }
  x = FIELD_PIXEL(px) - halfWidth + FIELD_PIXEL(anchorX + 0xffff);
  y = FIELD_PIXEL(pz - py) - halfHeight - FIELD_PIXEL(anchorY + 0xffff);
  if (x < 240 && y < 160) {
    part = &object->image;
    part->x = x;
    part->y = y;
    part->affine = affine;
    part->affineIndex = matrix;
    Oam_AddSpriteAtDepthClamped(part, FIELD_PIXEL(py + pz) + 62);
  }
}

/// Render_ApplyProjectedPlacement into the given draw slot: the sprite and its
/// shadow both go to depth, and mode chooses the picture as facing does there.
void Render_ApplyProjectedPlacementAtDepth(struct Sprite *object, s32 *pos,
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
  // Unused: gives the stack frame its 24 bytes.
  s32 ground[3];
  s32 screen[3];
  s32 flip;
  struct AffineTransform effect;
  s32 anchorX;
  s32 anchorY;
  s32 x;
  s32 y;
  struct AnimationSpritePart *part;

  halfWidth = object->width >> 1;
  {
    u8 height = object->height;

    halfHeight = height >> 1;
  }
  extentX = 8;
  extentY = 4;
  scaleX = *scale++;
  scaleY = *scale;
  px = *pos++;
  py = *pos++;
  pz = *pos++;
  pw = *pos;
  flip = Sprite_ComposePicture(object, mode);
  if (flip == 0 && scaleX == 0x10000 && scaleY == scaleX &&
      object->rotation == 0) {
    affine = 0;
    matrix = 0;
  } else {
    affine = 1;
    effect.angle = object->rotation;
    effect.scaleX = scaleX >> 8;
    effect.scaleY = scaleY >> 8;
    if (flip != 0)
      effect.scaleX = -effect.scaleX;
    matrix = OamMatrix_AllocFromTransform(&effect);
  }
  if (scaleX > 0x10000 || scaleY > Q16_ONE) {
    affine = 3;
    halfWidth <<= 1;
    halfHeight <<= 1;
    extentX = 16;
    extentY = 8;
  }

  y = FIELD_PIXEL(pz - pw) - extentY;
  if ((object->flags & OBJECT_SHADOW) && y < 160) {
    part = &object->shadow;
    part->affine = 0;
    part->x = FIELD_PIXEL(px) - extentX;
    part->y = y;
    Oam_AddSpriteAtDepth(part, depth);
  }

  if (scaleX == 0 || scaleY == 0)
    return;

  if (object->rotation == 0) {
    anchorX = object->offsetX * scaleX;
    anchorY = ((object->height >> 1) - object->offsetY) * scaleY;
  } else {
    s32 sin = Trig_Sin(object->rotation);
    s32 cos = Trig_Cos(object->rotation);

    // extentX and extentY are reused for the anchor's offset; locals of their
    // own differ.
    extentX = object->offsetX * scaleX;
    {
      u8 height = object->height;

      extentY = ((height >> 1) - object->offsetY) * scaleY;
    }

    anchorX = Iwram_MulQ16(cos, extentX) + Iwram_MulQ16(sin, extentY);
    anchorY = Iwram_MulQ16(cos, extentY) - Iwram_MulQ16(sin, extentX);
  }
  x = FIELD_PIXEL(px) - halfWidth + FIELD_PIXEL(anchorX + 0xffff);
  y = FIELD_PIXEL(pz - py) - halfHeight - FIELD_PIXEL(anchorY + 0xffff);
  if (x < 240 && y < 160) {
    part = &object->image;
    part->x = x;
    part->y = y;
    part->affine = affine;
    part->affineIndex = matrix;
    Oam_AddSpriteAtDepth(part, depth);
  }
}

/// Places a sprite at a 3D point for the fixed camera and queues it for
/// drawing, projected to the screen and scaled by its distance, as The Broken
/// Seal's Render_PlaceProjectedSprite does.
void Render_PlaceProjectedSprite(struct Sprite *sprite, s32 *point, s32 *scale,
                                 s32 facing, s32 depth) {
  s32 screen[3];
  s32 ground[3];
  s32 z;
  s32 flip;
  s32 rounded;
  s32 scaleX;
  s32 scaleY;
  s32 affine;
  s32 offsetX;
  s32 offsetY;
  u32 halfWidth;
  u32 halfHeight;
  u32 matrix;
  u32 size;

  if ((u16)Ram_HeapSlots->actorSystem->suspended == 0) {
    z = Render_ProjectPoint(point, screen);
    if (screen[2] != 0 && (u32)(screen[0] + 32) <= 304 &&
        (u32)(screen[1] + 32) <= 240) {
      if (sprite->useOwnScale)
        z = 0x10000;
      z = Iwram_MulQ16(z, sprite->scale);
      flip = Sprite_ComposePicture(sprite, facing);
      z >>= 10;
      rounded = ((z + 1) >> 1) << 11;
      scaleX = Iwram_MulQ16(rounded, scale[0]);
      scaleY = Iwram_MulQ16(rounded, scale[1]);
      if (scaleX > 0x1f800)
        scaleX = 0x1f800;
      if (scaleY > 0x1f800)
        scaleY = 0x1f800;
      halfHeight = sprite->height >> 1;
      offsetY = Iwram_MulQ16(sprite->offsetY - halfHeight, scaleY);
      halfWidth = sprite->width >> 1;
      if (scaleX > 0x10000 || scaleY > Q16_ONE) {
        affine = 3;
        halfWidth <<= 1;
        halfHeight <<= 1;
      } else {
        affine = 1;
        if (scaleX == 0x10000 && sprite->rotation == 0 && scaleY == scaleX)
          affine = 0;
      }
      if (flip)
        scaleX = -scaleX;
      if (affine)
        matrix = AffineMatrix_Build(sprite->rotation, scaleX >> 8, scaleY >> 8);
      else {
        matrix = (u32)scaleX >> 31;
        matrix <<= 3;
      }
      if (scaleX != 0 && scaleY != 0) {
        offsetX = Iwram_MulQ16(sprite->offsetX, scaleX);
        ((u32 *)&sprite->image)[1] =
            (((u32 *)&sprite->image)[1] & 0xc000fc00) | (affine << 8) |
            (matrix << 25) |
            ((u32)(screen[0] - halfWidth + offsetX) << 23 >> 7) |
            (u8)(screen[1] - halfHeight + offsetY);
        if (depth == 0) {
          s32 slot = 0x180 - (screen[2] >> 1);

          if (slot <= 0)
            slot = 1;
          Oam_AddSpriteAtDepthClamped(sprite, slot);
        } else {
          Oam_AddSpriteAtDepthClamped(sprite, depth);
        }
      }
      if (sprite->flags & 1) {
        struct AnimationSpritePart *shadow;

        ground[0] = point[0];
        ground[1] = 0;
        ground[2] = point[2];
        Render_ProjectPoint(ground, screen);
        size = 8;
        if (affine == 3)
          size = 16;
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
  if (Ram_HeapSlots->battleWork == NULL && !sprite->retainCacheEntry) {
    VramCache_ReleaseVram(sprite->slot);
    sprite->dirty = 1;
  }
}

void Animation_ApplyChildValueToFirst(struct Sprite *record, u32 value) {
  if (record != NULL) {
    record->entries[0]->colorMode = value;
    record->dirty = 1;
  }
}

void Animation_ApplyChildValuesToRecord(struct Sprite *record, u32 value) {
  s32 i;

  if (record != NULL) {
    for (i = 0; i < record->count; i++) {
      if (record->entries[i]->colorMode != 15)
        record->entries[i]->colorMode = value;
    }
    record->dirty = 1;
  }
}
