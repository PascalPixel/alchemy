/// @file
/// The field object, the field bank's C as The Broken Seal's field.c begins:
/// resource metadata and LZ decompression, then composing sprite pictures and
/// placing projected sprites. field_internal.h holds what the parts share; the
/// parts under field/ are included here in the order their code has in the
/// ROM. Camelot's own files are not recoverable: the ROM shows one object, and
/// these parts are ours, cut where The Broken Seal's are.

/// @file
/// What every part of the field object shares: its includes, in their
/// original order, and the names two or more parts use. field.c includes it
/// before the parts, and each part includes it too, so that a part read on its
/// own still resolves. It places no bytes.

#include "types.h"
#include "scene.h"
#include "sprite.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "dma.h"
#include "heap_release.h"
#include "iwram_call.h"
#include "resource.h"
#include <affine.h>
#include "sprite_metadata.h"
#include "actor_view.h"

// The parts, in ROM order (The Broken Seal's src/README.md, "Objects and
// parts").
#include "module/field/field.h"

struct AnimationMetadata *Resource_GetMetadataRecordFar(u32 id) {
  id &= 0xfff;
  if (id >= 716)
    id = 0;
  return &gResource_Data012[id];
}

/// Decompress source into destination and return destination.
/// source[0-1]: offset to commands; 0 means uncompressed, returning source + 2.
/// source + 2 contains literal bytes. In the command stream,
/// read each control byte from its least significant bit:
/// 1: copy one literal byte.
/// 0: read two bytes, high then low; zero terminates.
/// Low 12 bits give distance; high 4 bits + 2 give length, or the next byte +
/// 18 if those 4 bits are zero. Copy from that distance behind source + 2.
u8 *Decompress_Lz(const u8 *source, u8 *destination) {
  s32 token;
  u32 length;
  u32 sentinel;
  u32 control;
  u8 byte;
  u8 *start;
  const u8 *base;
  const u8 *command;
  const u8 *copy;
  const u8 *literal;

  token = source[0] | (source[1] << 8);
  literal = source + 2;
  if (token == 0)
    return (u8 *)literal;
  command = source + token;
  control = *command;
  sentinel = 128;
  sentinel <<= 1; // Marker indicating all eight bits were read.
  base = literal;
  start = destination;
  command++;
  control |= sentinel;
  goto dispatch;

back_reference:
  length = token >> 12;
  if (length == 0) {
    length = *command + 16;
    command++;
  }
  length += 2;
  copy = base - (token & 0x0fff);
  while (length != 0) {
    *destination++ = *copy++;
    length--;
  }

shift:
  control >>= 1;
  if (control == 0) {
    control = *command;
    sentinel = 128;
    sentinel <<= 1;
    command++;
    control |= sentinel;
  }

dispatch:
  if ((control & 1) != 0) {
    if (control !=
        1) // Read the next control byte when only the marker remains.
      *destination++ = *literal++;
    goto shift;
  }

  byte = *command++;
  token = byte << 8;
  token |= *command++;
  if (token != 0)
    goto back_reference;
  return start;
}
#include "actor.h"
#include "affine.h"
#include "fixed_math.h"
#include "module/field/field.h"
#include "module/field/object.h"
#include "module/system/system.h"
#include "task.h"

void Func_080219cc(struct ScriptPlayer *player) {
  s32 i;

  for (i = 0; i < player->channelCount; i++) {
    struct ScriptChannel *channel = player->channels[i];
    u8 command;
    u8 argument;

    if (channel == 0 || channel->script == 0)
      continue;
    for (;;) {
      command = channel->script[channel->position++];
      argument = channel->script[channel->position++];
      switch (command) {
      case 0xf0:
        channel->unk04 = argument;
        continue;
      case 0xf3:
        player->unk16 = argument;
        continue;
      case 0xf2:
        player->unk17 = argument;
        continue;
      case 0xff:
        channel->unk17 = 0xff;
        continue;
      case 0xf4:
      case 0xf5:
      case 0xf6:
      case 0xf7:
      case 0xf8:
      case 0xf9:
      case 0xfa:
      case 0xfc:
        continue;
      default:
        channel->unk17 = command;
        continue;
      case 0xef:
      case 0xf1:
      case 0xfd:
      case 0xfe:
        break;
      }
      break;
    }
    channel->position -= 2;
    player->unk19 = 1;
    channel->unk02 = 1;
  }
}

// The sprite object: The Broken Seal's struct Sprite with the shadow after
// the layer fields. Render_PlaceProjectedSprite's twin writes OAM
// attributes 0 and 1 as one word.
struct LayeredSprite {
  union {
    struct AnimationSpritePart image;
    struct {
      u8 link[4];
      u32 attributes;
    };
  };
  s32 scale;
  u8 slot;
  u8 retainCacheEntry : 1;
  u8 useOwnScale : 1;
  u8 depth : 6;
  u16 rotation;
  u8 width;
  u8 height;
  s8 offsetX;
  s8 offsetY;
  u8 lastNo;
  u8 dirty;
  u8 flags;
  u8 count;
  struct AnimationSpritePart shadow;
  struct AnimationEntry *entries[4];
};

s32 Sprite_ComposePicture(struct LayeredSprite *obj, u32 dir) {
  s32 mirrored;
  u32 size;
  struct ObjectSystemColours *system;
  DrawFn decodePicture;
  s32 decoderWasLoaded;
  struct AnimationEntry *layer;
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
  system = (struct ObjectSystemColours *)Ram_HeapSlots->objectSystem;
  decoderWasLoaded = 1;
  decodePicture = Ram_HeapSlots->unk4c[1];
  if (decodePicture == NULL) {
    Dma_Set(Render_DecodePictureCode,
            HeapSlot_Alloc(80, (s32)Render_DecodePictureBlockSize),
            0x84000000 | ((u32)(Render_DecodePictureCopyEnd -
                                Render_DecodePictureCode) >>
                          2),
            REG_DMA3);
    decodePicture = Ram_HeapSlots->unk4c[1];
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
      picture = layer->script[layer->pos++];
      arg = layer->script[layer->pos++];
      switch (picture) {
      case 254:
        SpriteAnimation_Start(layer, arg);
        obj->lastNo = arg;
        break;
      case 253:
        layer->pos = arg;
        break;
      case 252:
        Sound_PlayFar(arg);
        break;
      case 241:
        layer->pos -= 2;
        picture = layer->pictureBase;
        goto select_direction;
      case 240:
        layer->kind = arg;
        break;
      case 243:
        obj->offsetX = arg;
        break;
      case 242:
        obj->offsetY = arg;
        break;
      case 255:
        layer->pictureBase = 255;
      case 245:
        layer->timer += arg << 4;
        break;
      case 239:
        layer->pictureBase = 255;
        layer->script = NULL;
        obj->count--;
        picture = 255;
        goto select_direction;
      case 244:
      case 246:
      case 247:
      case 248:
      case 249:
      case 250:
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
    case 1:
      facing = gAnimationFacing_OnePicture[(dir >> 13) & 7];
      break;
    case 2:
    case 20:
      facing = gAnimationFacing_FrontBack[(dir >> 13) & 7];
      break;
    case 22:
      facing = gAnimationFacing_FrontBackSwapped[(dir >> 13) & 7];
      break;
    case 3:
      facing = gAnimationFacing_ThreePictures[(dir >> 12) & 15];
      break;
    case 5:
      facing = gAnimationFacing_SixPictures[(dir >> 10) & 63];
      break;
    case 8:
      facing = gData_0802eb44[((dir + 0x1000) >> 13) & 7];
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
    Iwram_ClearWordsEntry(pixels, size);

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
        if (layer->param != 0) {
          unpacked = Heap_Alloc(0x400);
          decodePicture(
              Decompress_Lz(layer->pictures[layer->picture], unpacked), pixels,
              layer->param);
          Heap_Release(unpacked);
        } else {
          decoded = Iwram_Decompress(layer->pictures[layer->picture], pixels);
          if (decoded != 0)
            decodePicture(decoded, pixels, 0);
        }
      } else {
        decodePicture(layer->pictures[layer->picture], pixels, layer->param);
      }
    }

    if (obj->flags & 14) {
      if (obj->flags & 2) {
        u32 codeSize = (u32)Grid_DrawOutlineCodeSize;
        OutlineFn outline = Heap_Alloc(codeSize);
        u32 outlineHeight;

        Dma_Set(Grid_DrawOutlineCode, outline, 0x84000000 | (codeSize >> 2),
                REG_DMA3);
        {
          /* FAKEMATCH(temporary): the width read into a local before the
           * asm below; as obj->width in the call, 2 of 578 instructions
           * differ in tla-en. */
          u32 outlineWidth = obj->width;

          /* FAKEMATCH(barrier): an empty asm after the width, which keeps
           * its load ahead of the system pointer and the height ahead of the
           * edge; without it, 4 of 578 instructions differ in tla-en. */
          asm("");
          outlineHeight = obj->height;
          outline(outlineWidth, outlineHeight, pixels, system->edge,
                  system->fill);
        }
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
        picture = obj->depth;
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
        u32 rows = obj->depth;
        u32 height = obj->height;
        u32 width = obj->width;
        const u8 *shade = Data_080209a8;
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
    ((UploadFn)Ram_HeapSlots->unk4c[2])(pixels, obj->width, obj->height, vram);
    obj->image.tile = tile;
    obj->dirty = 0;
    system->used += size;
    Heap_Release(pixels);
  }

  if (decoderWasLoaded == 0)
    HeapSlot_Release(80);
  return mirrored;
}

// The height read through a u8 copy: see the FAKEMATCH tag below.
#define HEIGHT_COPY(sprite)                                                    \
  ({                                                                           \
    u8 heightCopy = (sprite)->height;                                          \
    heightCopy;                                                                \
  })

void Render_PlaceSpritePartPair(struct LayeredSprite *sprite, s32 screenX,
                                s32 height, s32 screenY, s32 groundHeight,
                                s32 *scale) {
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
     both uses; as sprite->height at both, 3 of 107 instructions differ in
     tla-en. */
  if (scaleX > 0x10000 || scaleY > 0x10000) {
    affine = 3;
    halfWidth <<= 1;
    halfHeight <<= 1;
    marginX = 16;
    marginY = 8;
  }
  x = (screenX >> 16) - halfWidth;
  y = ((screenY - height) >> 16) - halfHeight -
      ((((HEIGHT_COPY(sprite) >> 1) - sprite->offsetY) * scaleY + 0xffff) >>
       16);
  sprite->image.affine = affine;
  sprite->image.x = x;
  sprite->image.y = y;
  x = (screenX >> 16) - marginX;
  y = ((screenY - groundHeight) >> 16) - marginY;
  shadow = &sprite->shadow;
  shadow->affine = affine;
  shadow->x = x;
  shadow->y = y;
}

// The object casts a shadow.
#define OBJECT_SHADOW 1

// Places an animation object's sprite (and its shadow) at the screen point
// of a projected position (x, y, z, ground), scaled by scale[0] and
// scale[1]; the sprites go into the depth list by their depth.
void Render_ApplyProjectedPlacement(struct LayeredSprite *object, s32 *pos,
                                    s32 *scale, s32 mode) {
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
  if (scaleX > 0x10000 || scaleY > 0x10000) {
    affine = 3;
    halfWidth <<= 1;
    halfHeight <<= 1;
  }

  // The shadow: an 8 by 4 sprite on the ground below the object.
  y = ((pz - pw) >> 16) - 4;
  if ((object->flags & OBJECT_SHADOW) && y < 160) {
    part = &object->shadow;
    part->affine = 0;
    part->x = (px >> 16) - 8;
    part->y = y;
    Func_080140d8(part, ((pz + pw) >> 16) + 60);
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
  x = (px >> 16) - halfWidth + ((anchorX + 0xffff) >> 16);
  y = ((pz - py) >> 16) - halfHeight - ((anchorY + 0xffff) >> 16);
  if (x < 240 && y < 160) {
    part = &object->image;
    part->x = x;
    part->y = y;
    part->affine = affine;
    part->affineIndex = matrix;
    Func_080140d8(part, ((py + pz) >> 16) + 62);
  }
}

static inline u8 PlacedObject_Height(struct LayeredSprite *object) {
  return object->height;
}

void Func_08022318(struct LayeredSprite *object, s32 *pos, s32 *scale, s32 mode,
                   s32 depth) {
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
  /* FAKEMATCH(wrapper): the height read through the inline
   * PlacedObject_Height; as object->height, 4 of 262 instructions differ in
   * tla-en. */
  halfHeight = PlacedObject_Height(object) >> 1;
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
  if (scaleX > 0x10000 || scaleY > 0x10000) {
    affine = 3;
    halfWidth <<= 1;
    halfHeight <<= 1;
    extentX = 16;
    extentY = 8;
  }

  y = ((pz - pw) >> 16) - extentY;
  if ((object->flags & OBJECT_SHADOW) && y < 160) {
    part = &object->shadow;
    part->affine = 0;
    part->x = (px >> 16) - extentX;
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
    /* FAKEMATCH(wrapper): the height read through the inline
     * PlacedObject_Height; as object->height, 2 of 262 instructions differ in
     * tla-en. */
    extentY = ((PlacedObject_Height(object) >> 1) - object->offsetY) * scaleY;

    anchorX = Iwram_MulQ16(cos, extentX) + Iwram_MulQ16(sin, extentY);
    anchorY = Iwram_MulQ16(cos, extentY) - Iwram_MulQ16(sin, extentX);
  }
  x = (px >> 16) - halfWidth + ((anchorX + 0xffff) >> 16);
  y = ((pz - py) >> 16) - halfHeight - ((anchorY + 0xffff) >> 16);
  if (x < 240 && y < 160) {
    part = &object->image;
    part->x = x;
    part->y = y;
    part->affine = affine;
    part->affineIndex = matrix;
    Oam_AddSpriteAtDepth(part, depth);
  }
}

s32 Render_ProjectPoint(const void *point, s32 *screen);

void Func_0802254c(struct LayeredSprite *sprite, s32 *point, s32 *scale,
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

  if ((u16)Ram_HeapSlots->objectSystem->suspended == 0) {
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
      if (scaleX > 0x10000 || scaleY > 0x10000) {
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
        sprite->attributes =
            (sprite->attributes & 0xc000fc00) | (affine << 8) | (matrix << 25) |
            ((u32)(screen[0] - halfWidth + offsetX) << 23 >> 7) |
            (u8)(screen[1] - halfHeight + offsetY);
        if (depth == 0) {
          s32 slot = 0x180 - (screen[2] >> 1);

          if (slot <= 0)
            slot = 1;
          Func_080140d8(sprite, slot);
        } else {
          Func_080140d8(sprite, depth);
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
          Func_080140d8(shadow, 0);
        else
          Func_080140d8(shadow, depth);
      }
      return;
    }
  }
  if (Ram_HeapSlots->battleWork == NULL && !sprite->retainCacheEntry) {
    VramCache_ReleaseVram(sprite->slot);
    sprite->dirty = 1;
  }
}

void Animation_ApplyChildValueToFirst(struct ActorSpriteView *record,
                                      u32 value) {
  if (record != NULL) {
    record->entries[0]->param = value;
    record->dirty = 1;
  }
}

void Animation_ApplyChildValuesToRecord(struct ActorSpriteView *record,
                                        u32 value) {
  s32 i;

  if (record != NULL) {
    for (i = 0; i < record->count; i++) {
      if (record->entries[i]->param != 15)
        record->entries[i]->param = value;
    }
    record->dirty = 1;
  }
}
