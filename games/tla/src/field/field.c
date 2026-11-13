/// @file
/// The field object, the field bank's C as The Broken Seal's field.c begins:
/// resource metadata and LZ decompression, then composing sprite pictures and
/// placing projected sprites. field_internal.h holds what the parts share; the
/// parts under field/ are included here in the order their code has in the
/// ROM. Camelot's own files are not recoverable: the ROM shows one object, and
/// these parts are ours, cut where The Broken Seal's are. The rest of the
/// field bank's C follows in ROM order: the resource slots (parts under
/// resource_slot/), the tile animation and the terrain.

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
#include "module/field/object.h"
#include "module/system/system.h"
#include "task.h"

/// Runs each of the player's animation channels on to its next stop (a
/// wait, end or jump: 0xef, 0xf1, 0xfd or 0xfe), taking the frames and
/// settings on the way, and leaves the stop to run next; marks the player and
/// each channel changed. The battle's motion calls it before it releases a
/// borrowed sprite buffer.
void Animation_SkipToStop(struct ScriptPlayer *player) {
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
// the layer fields. Render_ApplyProjectedPlacementAtDepth writes OAM
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

/// Places an animation object's sprite (and its shadow) at the screen point
/// of a projected position (x, y, z, ground), scaled by scale[0] and
/// scale[1]; the sprites go into the depth list by their depth.
/* VIEW(field): routine Render_ApplyProjectedPlacement as void
 * Render_ApplyProjectedPlacement(struct LayeredSprite *object, s32 *pos, s32
 * *scale, s32 mode); field/field.h has void Render_ApplyProjectedPlacement(void
 * *sprite, s32 *position, s32 *scale, u16 angle); tried as the definition's
 * void Render_ApplyProjectedPlacement(struct LayeredSprite *object, s32 *pos,
 * s32 *scale, s32 mode), which the object already declares, it does not compile
 * in tla-ja: games/tla/include/field/field.h:107: conflicting types for
 * `Render_ApplyProjectedPlacement'. */
void Render_ApplyProjectedPlacement__field(
    struct LayeredSprite *object, s32 *pos, s32 *scale,
    s32 mode) __asm__("Render_ApplyProjectedPlacement");
#define Render_ApplyProjectedPlacement Render_ApplyProjectedPlacement__field
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
    Oam_AddSpriteAtDepthClamped(part, ((pz + pw) >> 16) + 60);
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
    Oam_AddSpriteAtDepthClamped(part, ((py + pz) >> 16) + 62);
  }
}

static inline u8 PlacedObject_Height(struct LayeredSprite *object) {
  return object->height;
}

/// Render_ApplyProjectedPlacement into the given draw slot: the sprite and its
/// shadow both go to depth, and mode chooses the picture as facing does there.
void Render_ApplyProjectedPlacementAtDepth(struct LayeredSprite *object,
                                           s32 *pos, s32 *scale, s32 mode,
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

/// Places a sprite at a 3D point for the fixed camera and queues it for
/// drawing, projected to the screen and scaled by its distance, as The Broken
/// Seal's Render_PlaceProjectedSprite does.
void Render_PlaceProjectedSprite(struct LayeredSprite *sprite, s32 *point,
                                 s32 *scale, s32 facing, s32 depth) {
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

#undef Render_ApplyProjectedPlacement
// ---- field/resource_slot.c ----

/// The resource_slot object, written as parts in resource_slot/.

#include "resource_slot_internal.h"

// The parts, in ROM order.
#include "resource_slot/sprite_animation.c"
#include "resource_slot/actors.c"
#include "resource_slot/actor_motion.c"
#include "resource_slot/script_commands.c"
#include "resource_slot/player_control.c"
#include "resource_slot/pad_movement.c"
#include "resource_slot/move_by_keys.c"
#include "resource_slot/map_grid.c"
#include "resource_slot/map_layers.c"
#include "resource_slot/world_map.c"

#undef InitializeAnimationObjects
#undef ResourceMetadata_Register
#undef Animation_ApplyChildArgument
#undef Animation_ApplyChildValue
#undef SpriteAnimation_Alloc
#undef Render_ApplyProjectedPlacementAtDepth
#undef MapGrid_CheckCollision

// ---- field/map/tile_animation.c ----

/// Timed map changes (the map animations, the blend script and the palette
/// queue), then the ground heights and the collision of cells and the world
/// map, as The Broken Seal's tile_animation.c and terrain.c have them.

#include "map.h"
#include "graphics.h"
#include "module/field/map.h"

/* VIEW(tile_animation): variable gWorldMapWork as u8 gWorldMapWork[];
 * battle/battle.h has struct WorldMapWork gWorldMapWork; tried as the 3
 * declarations' extern u8 gWorldMapWork[], it does not compile in tla-ja:
 * games/tla/src/field/field.c:9883: `gWorldMapWork' has an incomplete type. */
extern u8 gWorldMapWork__tile_animation[] __asm__("gWorldMapWork");
#define gWorldMapWork gWorldMapWork__tile_animation

void WorldMap_PreloadCurrentTileBlock(void) {
  struct MapTileWindow *window = (struct MapTileWindow *)Ram_HeapSlots->mapWork;
  s32 x = 0;
  s32 y = 0;
  s32 x0;
  s32 y0;
  s32 layer;
  s32 row;
  s32 col;
  s32 tile;
  struct Data02020000 *block;
  const u32 *resources;
  s32 graphicsSet;
  s32 value;
  u8 *buffer;

  if (window->origin != NULL) {
    s32 *p = window->origin;

    // origin[0] and origin[2], read through a stepped pointer: the only
    // spelling that gives the native's post-increment load (4 of 277 as
    // indexes).
    x = *p++;
    y = p[1];
  }
  x0 = (x - 0x1000000) >> 25;
  y0 = (y - 0x1400000) >> 25;
  for (layer = 0; layer <= 1; layer++) {
    for (row = 0; row <= 1; row++) {
      for (col = 0; col <= 1; col++) {
        tile = (((y0 + row) & 31) << 5) + ((x0 + col) & 31);
        tile = window->tileIds[tile];
        tile += layer << 10;
        if (WorldMap_WriteLayerCellTile(layer, x0 + col, y0 + row, tile, 0) !=
            0)
          return;
      }
    }
  }
  block = (struct Data02020000 *)Ram_MapBlocks +
          (((x / 0x200000) & 31) + (((y / 0x200000) & 31) << 5));
  WorldMap_UpdateRegionTint(block->region, 1);
  // One local holds the region, then ScreenTransition_IsScanlineActiveFar's
  // result: set twice, it reads the region into r3 for the divide, as the
  // original does.
  value = block->region;
  switch (value / 10) {
  case 3:
    graphicsSet = 3;
    break;
  case 2:
    graphicsSet = 2;
    break;
  case 4:
    graphicsSet = 1;
    break;
  default:
    graphicsSet = 0;
    break;
  }
  resources = gWorldMap_GraphicsResources[graphicsSet];
  if (resources != window->resources) {
    window->resources = resources;
    window->reloadSteps = 12;
  }
  value = ScreenTransition_IsScanlineActiveFar();
  if (window->reloadSteps == 0)
    return;
  if (value != 0)
    return;
  buffer = Heap_Alloc(0x200);
  switch (window->reloadSteps) {
  case 12: {
    s16 backdropColor = *(s16 *)0x05000000;

    Resource_DecodeByteLzInRam(Resource_GetTableEntry(resources[0]), buffer);
    *(s16 *)buffer = backdropColor;
    Dma_Set(buffer, (void *)0x05000000, 0x84000070, REG_DMA3);
    {
      u8 *anim = Data_0202d000;
      BattleEffect_LoadBufferPaletteFar(buffer);
      Decompress_TaggedLz(Resource_GetTableEntry(resources[1]), anim);
      TilesetAnim_StartAll((const u16 *)anim);
    }
    Decompress_TaggedLz(Resource_GetTableEntry(resources[2]), gMapCollision);
    break;
  }
  case 11:
    Decompress_TaggedLz(Resource_GetTableEntry(resources[3]), gData_02038000);
    break;
  case 10:
    Dma_Set(gData_02038000, (void *)0x06008000, 0x84000800, REG_DMA3);
    break;
  case 9:
    Decompress_TaggedLz(Resource_GetTableEntry(resources[4]), gData_0203a000);
    break;
  case 8:
    Dma_Set(gData_0203a000, (void *)0x0600a000, 0x84000800, REG_DMA3);
    break;
  case 7:
    Decompress_TaggedLz(Resource_GetTableEntry(resources[5]), gData_0203c000);
    break;
  case 6:
    Dma_Set(gData_0203c000, (void *)0x0600c000, 0x84000800, REG_DMA3);
    break;
  case 5:
    Decompress_TaggedLz(Resource_GetTableEntry(resources[6]), gData_0203e000);
    break;
  case 4:
    Dma_Set(gData_0203e000, (void *)0x0600e000, 0x84000800, REG_DMA3);
    break;
  case 3:
    Decompress_TaggedLz(Resource_GetTableEntry(resources[7]), gData_02028000);
    break;
  case 1:
    Decompress_TaggedLz(Resource_GetTableEntry(resources[8]), gWorldMapWork);
    break;
  }
  window->reloadSteps--;
  Heap_Release(buffer);
}

#include "system.h"
#include "frame.h"

void MapAnimation_PresentPicture(void) {
  s32 control;
  volatile u32 *dma;

  control = 0xa80b;
  REG_BG3CNT = control;
  control = 0xaa0f;
  REG_BG2CNT = control;
  control = 0x682;
  REG_BG1CNT = control;
  dma = REG_DMA3;
  /* FAKEMATCH(barrier): an empty asm on dma, which loads the DMA channel's
   * address before the transfer's operands; without it, 2 of 24 instructions
   * differ in tla-en. */
  __asm__ volatile("" : "+l"(dma));
  Dma_Set(Ram_MapCellBuffer, (void *)0x06006000, DMA_COPY_WORDS(0xa000 / 4),
          dma);
}

void MapAnimation_Start(void) {
  u8 *pages = Ram_HeapSlots->unk1c[0];
  struct MapViewWork *work = (struct MapViewWork *)Ram_HeapSlots->mapWork;
  u32 *resources;
  s32 one;
  s8 *active;

  active = &work->animationActive;
  one = 1;
  *active = one;
  Task_SuspendAll(TilesetAnim_Update);
  work->picturePending = one;
  resources = work->resources;
  Decompress_TaggedLz(Resource_GetTableEntry(resources[3]), gData_02038000);
  if (work->extraTiles != 0)
    Decompress_TaggedLz(Resource_GetTableEntry(resources[4]), gData_0203a000);
  work->extraTiles = 0;
  Dma_Set((void *)0x06004000, gDecodeBuffer, DMA_COPY_WORDS(0x2000 / 4),
          REG_DMA3);
  wait(1);
  TileMap_DrawRowsFromRam(pages + (gFrameCount & one) * 0x1400 + 0xc80,
                          Ram_MapCellBuffer);
  work->windowTop = 200;
  work->windowBottom = 255;
  gVBlankCallback = MapAnimation_PresentPicture;
  work->pictureShown = one;
}

void Map_ShowBg1FromBuffer(void) {
  s32 control;
  volatile u32 *dma;

  control = 0xa80a;
  REG_BG3CNT = control;
  control = 0xaa0e;
  REG_BG2CNT = control;
  control = 0x501;
  REG_BG1CNT = control;
  dma = REG_DMA3;
  /* FAKEMATCH(barrier): an empty asm on dma, which loads the DMA channel's
   * address before the transfer's operands; without it, 2 of 24 instructions
   * differ in tla-en. */
  __asm__ volatile("" : "+l"(dma));
  Dma_Set(gData_02038000, (void *)0x06008000, DMA_COPY_WORDS(0x8000 / 4), dma);
}

s32 WorldMap_LoadAreaGraphics(void) {
  struct MapViewWork *state;
  u32 *resources;
  u8 *buffer;
  s16 value;

  state = (struct MapViewWork *)Ram_HeapSlots->mapWork;
  buffer = Ram_MapCellBuffer;
  resources = state->resources;
  value = *(s16 *)0x05000000;
  Resource_DecodeByteLzInRam(Resource_GetTableEntry(resources[0]), buffer);
  *(s16 *)buffer = value;
  Dma_Set(buffer, (void *)BG_PLTT, DMA_COPY_WORDS(112), REG_DMA3);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[3]), gData_02038000);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[4]), gData_0203a000);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[5]), gData_0203c000);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[6]), gData_0203e000);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[7]), gData_02028000);
  Decompress_TaggedLz(Resource_GetTableEntry(resources[8]), gWorldMapWork);
  gVBlankCallback = Map_ShowBg1FromBuffer;
  state->windowTop = 0;
  state->windowBottom = 159;
  wait(1);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_DefaultMapCells),
                      buffer);
  WorldMap_UpdateCurrentTileBlock();
  state->animationActive = 0;
  Task_ResumeAll(TilesetAnim_Update);
  state->picturePending = 0;
  state->pictureShown = 0;
  WorldMap_UpdateRegionTint(state->area, 0);
  wait(1);
}

void WorldMap_LoadDefaultCellsAndUpdateBlock(void) {
  struct MapViewWork *work = (struct MapViewWork *)Ram_HeapSlots->mapWork;

  gVBlankCallback = Map_ShowBg1FromBuffer;
  work->windowTop = 0;
  work->windowBottom = 0x9f;
  wait(1);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_DefaultMapCells),
                      Ram_MapCellBuffer);
  WorldMap_UpdateCurrentTileBlock();
  Task_ResumeAll(TilesetAnim_Update);
  wait(1);
}

void TilesetAnim_Update(void) {
  struct MapWork *state = Ram_HeapSlots->mapWork;
  u32 i;

  for (i = 0; i <= 15; i++) {
    const u16 *script;
    u32 op;
    u32 count;
    u32 dst;

    if ((state->anim + i)->start == NULL || (state->anim + i)->paused != 0)
      continue;
  next:
    if ((state->anim + i)->timer == 0) {
      script = (state->anim + i)->cursor;
      op = *script++;
      if (op == 0xffff) {
        (state->anim + i)->cursor = (state->anim + i)->start;
        goto next;
      }
      if ((op & 0xff00) == 0xfe00) {
        if ((op & 0xff) == 0xff)
          continue;
        (state->anim + i)->cursor = (state->anim + i)->start + (op & 0xff) * 2;
        goto next;
      }
      count = *script++;
      dst = script[0];
      (state->anim + i)->timer = script[1];
      if (state->wideTiles == 0) {
        if (op >= 0x600)
          Dma_Set(Ram_DecodeBuffer + op * 32, (void *)(dst * 32 + 0x06004000),
                  DMA_COPY_WORDS(count * 8), REG_DMA3);
        else
          Dma_Set((void *)(op * 32 + 0x06004000),
                  (void *)(dst * 32 + 0x06004000), DMA_COPY_WORDS(count * 8),
                  REG_DMA3);
      } else {
        if (op >= 0x200)
          Dma_Set(Ram_MapBlocks + op * 64, (void *)(dst * 64 + 0x06008000),
                  DMA_COPY_WORDS(count * 16), REG_DMA3);
        else
          Dma_Set((void *)(op * 64 + 0x06008000),
                  (void *)(dst * 64 + 0x06008000), DMA_COPY_WORDS(count * 16),
                  REG_DMA3);
      }
      (state->anim + i)->cursor += 4;
      goto next;
    } else {
      (state->anim + i)->timer--;
    }
  }
}

void Map_ClearLayerEntryFlag(u32 no) {
  struct MapWork *state = Ram_HeapSlots->mapWork;
  struct MapAnimation *anim = &state->anim[no];

  anim->paused = 0;
}

void Map_SetLayerEntryFlag(u32 no) {
  struct MapWork *state = Ram_HeapSlots->mapWork;
  struct MapAnimation *anim = &state->anim[no];

  anim->paused = 1;
}

void TilesetAnim_StartAll(const u16 *script) {
  struct MapWork *state = Ram_HeapSlots->mapWork;
  s32 count;
  u32 command;

  count = 0;
  Iwram_ClearWordsEntry(state->anim, sizeof(state->anim));
  command = *script++;
  while (command != 0xffff) {
    if ((command & 0xff00) == 0xfd00) {
      struct MapAnimation *anim;
      u32 idx = command & 15;
      u32 flag = 0;

      if (command & 0x80)
        flag = 1;
      anim = &state->anim[idx];
      anim->start = script;
      anim->cursor = script;
      anim->timer = 0;
      anim->paused = flag;
      count++;
    }
    command = *script++;
  }
  if (count != 0 && Task_Find(TilesetAnim_Update) == -1)
    Task_Add(TilesetAnim_Update, TASK_PASS_VBLANK);
}

void TilesetAnim_SuspendAll(void) {
  if (((struct MapViewWork *)Ram_HeapSlots->mapWork)->animationActive == 0)
    Task_SuspendAll(TilesetAnim_Update);
}

void TilesetAnim_ResumeAll(void) {
  if (((struct MapViewWork *)Ram_HeapSlots->mapWork)->animationActive == 0)
    Task_ResumeAll(TilesetAnim_Update);
}

void BlendScript_Run(void) {
  struct MapWork *work = Ram_HeapSlots->mapWork;
  struct BlendScriptState *state;
  u16 *cursor;
  u32 command;
  u32 value;
  u16 test;

  state = &work->blend;
  if (state->script == 0)
    return;
  if (state->paused != 0)
    return;

again:
  value = state->delay;
  test = value;
  if (test != 0)
    goto count_down;

  cursor = state->cursor;
  command = *cursor;
  cursor++;
  if (command == 0xffff) {
    state->cursor = state->script;
    goto again;
  }

  if ((command & 0xff00) == 0xfe00) {
    value = command & 0xff;
    if (value == 0xff)
      return;
    state->cursor = state->script + value * 2;
    goto again;
  }

  if ((command & 0xf000) == 0x3000) {
    REG_BLDCNT = command;
    work->blendControl = command;
    state->cursor++;
    goto again;
  }

  if ((work->blendControl & 0xc0) == 0x40)
    REG_BLDALPHA = command;
  else
    REG_BLDY = command;
  state->delay = cursor[0];
  state->cursor += 2;
  goto again;

count_down:
  state->delay = value + 0xffff;
}

void BlendScript_Start(u16 *script) {
  struct BlendScriptState *state;
  s32 started;
  volatile u32 zero;
  u32 first;

  started = 0;
  zero = 0;
  state = &Ram_HeapSlots->mapWork->blend;
  Dma_Set((const void *)&zero, state,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 3, REG_DMA3);
  first = *script;
  if (first != 0xffff) {
    state->script = script;
    state->cursor = script;
    state->delay = 0;
    state->paused = 0;
    started = 1;
  }
  if (started)
    Task_Add((s32)(BlendScript_Run), TASK_PASS_VBLANK);
}

void BlendScript_Suspend(void) { Task_SuspendAll((u32)BlendScript_Run); }

void BlendScript_Resume(void) { Task_ResumeAll((u32)BlendScript_Run); }

#include "palette_snapshots.h"

void PaletteCycle_Initialize(void) {
  struct PaletteSnapshotWork *queue;
  struct PaletteSnapshot *entry;
  u16 i;
  u16 j;

  queue = HeapSlot_AllocEwram(112, sizeof(struct PaletteSnapshotWork));
  entry = queue->entries;
  for (i = 0; i != 4; i++) {
    entry->source = 0;
    entry->position = 0;
    entry->timer = 0;
    entry->delay = 0;
    entry->count = 0;
    for (j = 0; j != 16; j++) {
      entry->colors[j] = 0;
    }
    entry++;
  }
  queue->count = 0;
}

#define gPaletteWork ((struct PaletteSnapshotWork *)Ram_HeapSlots->unk70[0])

#include "palqueue_add.c"

void PaletteQueue_Stop(void) {
  Task_Remove(PaletteCycle_Step);
  HeapSlot_Release(112);
}

void PaletteCycle_Start(void) { Task_Add(PaletteCycle_Step, TASK_PASS_VBLANK); }

void PaletteCycle_Step(void) {
  struct PaletteSnapshotWork *work = gPaletteWork;
  u8 i;
  u16 count = work->count & 3;
  u16 buf[16];

  for (i = 0; i < count; i++) {
    struct PaletteSnapshot *cycle = &work->entries[i];
    if (cycle->timer == 0) {
      u16 start = cycle->position;
      u16 len = cycle->count;
      void *dest = cycle->source;
      u16 *src = cycle->colors;
      u8 j;
      for (j = len - start; j < len; j++)
        buf[j] = *src++;
      for (j = 0; j < len - start; j++)
        buf[j] = *src++;
      Dma_Set(buf, dest, DMA_COPY_HALFWORDS(len), REG_DMA3);
      start++;
      if (start >= len)
        start = 0;
      cycle->position = start;
      cycle->timer = cycle->delay;
    } else {
      cycle->timer--;
    }
  }
}

#include "io_write_queue.h"
#include "flag_call.h"

/// Tints the world map for the encounter region under the party: region 9
/// brightens it by 12 steps, region 18 darkens it by 6 until flag 0x8ef is
/// set, any other clears the tint. Gradually, the tint moves one step every 16
/// frames; at once, it jumps there. Flag 0x106 holds the tint as it is.
void WorldMap_UpdateRegionTint(s32 region, s32 gradual) {
  struct WorldTint *work = (struct WorldTint *)Ram_HeapSlots->mapWork;

  if (work->region != region) {
    work->region = region;
    if (region == 9)
      work->target = 12;
    else if (region == 18 && !GameFlag_Get(0x8ef))
      work->target = -6;
    else
      work->target = 0;
  }
  if (GameFlag_Get(0x106))
    return;
  if (gradual == 0) {
    work->tint = work->target;
  } else {
    region = work->target - work->tint;
    if (region == 0)
      return;
    if (gFrameCount & 15)
      return;
    if (region < 0)
      region = -1;
    else
      region = 1;
    work->tint += region;
  }
  if (work->tint > 0) {
    IO_WRITE_QUEUE_PUSH(0x4000050, 0x3f8e, 0x20000);
    {
      struct IoWriteQueue *q = &gIoWriteQueue;
      u32 saved;
      s32 count;

      IRQ_LOCK(saved);
      count = q->count;
      if (count <= 31) {
        u32 *entry;
        /* FAKEMATCH(temporary): the entry's byte offset held in offset and
         * scaled after the count store; as u32 *entry = q->entries[count]
         * before the store, 21 of 210 instructions differ in tla-en. */
        s32 offset = count * 3;

        q->count = count + 1;
        offset *= sizeof(u32);
        /* FAKEMATCH(barrier): an empty asm keeps the tint's load after the
         * offset's scaling; without it, 2 of 210 instructions differ in
         * tla-en. */
        asm("");
        entry = (u32 *)((u8 *)q->entries + offset);
        *entry++ = (u16)work->tint;
        *entry++ = 0x4000054;
        *entry = 0x20000;
      }
      IRQ_UNLOCK(saved);
    }
  } else {
    IO_WRITE_QUEUE_PUSH(0x4000050, 0x3fce, 0x20000);
    IO_WRITE_QUEUE_PUSH(0x4000054, (u16)-work->tint, 0x20000);
  }
}

#include "curve.h"

s32 Terrain_GetFlatHeight(const s8 *samples) {
  return samples[0] << CURVE_VALUE_SHIFT;
}

s32 Terrain_GetSlopeXHeight(const s8 *samples, s32 position) {
  s32 start;
  s32 delta;

  start = samples[0] << CURVE_VALUE_SHIFT;
  delta = ((samples[1] << CURVE_VALUE_SHIFT) - start) * position;
  if (delta < 0) {
    delta += CURVE_FULL_STEPS - 1;
  }
  return start + (delta >> 4);
}

s32 Terrain_GetSlopeYHeight(const s8 *samples, s32 unused, s32 position) {
  s32 base;
  s32 offset;

  base = samples[0] << CURVE_VALUE_SHIFT;
  offset = ((samples[1] << CURVE_VALUE_SHIFT) - base) * position;
  if (offset < 0) {
    offset += CURVE_FULL_STEPS - 1;
  }
  return base + (offset >> 4);
}

s32 Terrain_GetStepSumDiagonalHeight(const s8 *samples, s32 start, s32 end) {
  s32 first;
  s32 second;
  s32 maximum;
  u32 position;

  first = samples[0] << CURVE_VALUE_SHIFT;
  second = samples[1] << CURVE_VALUE_SHIFT;
  maximum = first;
  if (second > first) {
    maximum = second;
  }
  position = start + end;
  if (position == CURVE_FULL_STEPS - 1) {
    return maximum;
  }
  if (position < CURVE_FULL_STEPS - 1) {
    second = first;
  }
  return second;
}

s32 Terrain_GetStepDifferenceDiagonalHeight(const s8 *samples, s32 start,
                                            s32 end) {
  s32 first;
  s32 second;
  s32 maximum;
  s32 difference;
  u32 position = start;

  first = samples[0] << CURVE_VALUE_SHIFT;
  second = samples[1] << CURVE_VALUE_SHIFT;
  maximum = first;
  if (second > first) {
    maximum = second;
  }
  difference = end - position;
  position = difference + CURVE_FULL_STEPS - 1;
  if (position == CURVE_FULL_STEPS - 1) {
    return maximum;
  }
  if (position < CURVE_FULL_STEPS - 1) {
    second = first;
  }
  return second;
}

s32 Terrain_GetBendSumDiagonalHeight(const s8 *samples, s32 start, s32 end) {
  s32 first;
  s32 middle;
  s32 last;
  s32 result;

  first = *samples++ << CURVE_VALUE_SHIFT;
  middle = *samples << CURVE_VALUE_SHIFT;
  last = samples[1] << CURVE_VALUE_SHIFT;
  start = start + end;
  if ((u32)start == CURVE_FULL_STEPS - 1) {
    result = middle;
  } else if ((u32)start < CURVE_FULL_STEPS - 1) {
    result = first + (middle - first) * start / (CURVE_FULL_STEPS - 1);
  } else {
    start = start - (CURVE_FULL_STEPS - 1);
    result = middle + (last - middle) * start / (CURVE_FULL_STEPS - 1);
  }
  return result;
}

s32 Terrain_GetBendDifferenceDiagonalHeight(const s8 *samples, s32 start,
                                            s32 end) {
  s32 first;
  s32 second;
  s32 third;
  s32 difference;
  s32 delta;

  first = *samples++ << CURVE_VALUE_SHIFT;
  second = samples[0] << CURVE_VALUE_SHIFT;
  third = samples[1];
  difference = end - start;
  start = difference + CURVE_FULL_STEPS - 1;
  third <<= CURVE_VALUE_SHIFT;

  if (start == CURVE_FULL_STEPS - 1)
    return second;

  if ((u32)start < CURVE_FULL_STEPS - 1) {
    delta = second - first;
    difference = delta * start;
    if (difference < 0)
      difference += CURVE_FULL_STEPS - 1;
    difference >>= 4;
    return first + difference;
  } else {
    difference *= third - second;
    if (difference < 0)
      difference += CURVE_FULL_STEPS - 1;
    difference >>= 4;
    return second + difference;
  }
}

s32 Terrain_GetRoundedSquareHeight(const s8 *samples, s32 position, s32 row) {
  s32 start;
  s32 end;
  s32 weight;
  u8 *table;

  start = samples[0];
  end = samples[1];
  table = gTerrain_LerpWeightTable;
  /* FAKEMATCH(barrier): an empty asm reading the table's address keeps its
   * load after both samples' and before the row's scaling; without it, 15 of
   * 15 instructions differ in tla-en. */
  __asm__("" : : "r"(table));
  position += row * CURVE_FULL_STEPS;
  weight = table[position];
  start <<= CURVE_VALUE_SHIFT;
  return start + (((end << CURVE_VALUE_SHIFT) - start) * weight);
}

s32 Terrain_GetStepXHeight(const s8 *samples, u32 position) {
  s32 value;
  if (position < CURVE_HALF_STEPS)
    value = samples[0];
  else
    value = samples[1];
  return value << CURVE_VALUE_SHIFT;
}

s32 Terrain_GetStepYHeight(const s8 *samples, u32 unused, u32 position) {
  s32 value;
  if (position < CURVE_HALF_STEPS)
    value = samples[0];
  else
    value = samples[1];
  return value << CURVE_VALUE_SHIFT;
}

/// As Terrain_GetSampleTableHeight with x and y swapped.
s32 Terrain_GetSampleTableTransposedHeight(const s8 *samples, u32 pixelX,
                                           u32 pixelY) {
  return samples[gTerrain_SampleIndexTable[pixelY + (pixelX << 4)]]
         << CURVE_VALUE_SHIFT;
}

/// As Terrain_GetSampleTableMirroredHeight with x and y swapped.
s32 Terrain_GetSampleTableTransposedMirroredHeight(const s8 *samples, u32 row,
                                                   u32 position) {
  const u8 *table = gTerrain_SampleIndexTable;
  u32 index;

  index = (row << 4) - position;
  index += CURVE_FULL_STEPS - 1;
  return samples[table[index]] << CURVE_VALUE_SHIFT;
}

/// The sample gTerrain_SampleIndexTable picks for (x, y): a byte per pixel
/// of the cell, row by row.
s32 Terrain_GetSampleTableHeight(const s8 *samples, u32 position, u32 row) {
  return samples[gTerrain_SampleIndexTable[position + (row << 4)]]
         << CURVE_VALUE_SHIFT;
}

/// The same, with the table mirrored left to right.
s32 Terrain_GetSampleTableMirroredHeight(const s8 *samples, u32 position,
                                         u32 row) {
  const u8 *table = gTerrain_SampleIndexTable;
  u32 index;

  index = (row << 4) - position;
  index += CURVE_FULL_STEPS - 1;
  return samples[table[index]] << CURVE_VALUE_SHIFT;
}

/// Above y 8 a step along x from the first sample to the second; below, the
/// third.
s32 Terrain_GetSplitTopStepHeight(const s8 *samples, u32 column, u32 row) {
  s32 value;

  if (row < CURVE_HALF_STEPS) {
    if (column < CURVE_HALF_STEPS)
      value = samples[0];
    else
      value = samples[1];
  } else {
    value = samples[2];
  }
  return value << CURVE_VALUE_SHIFT;
}

/// Above y 8 the first sample; below, a step along x from the second to the
/// third.
s32 Terrain_GetSplitBottomStepHeight(const s8 *samples, u32 column, u32 row) {
  s32 value;

  if (row < CURVE_HALF_STEPS)
    value = samples[0];
  else if (column < CURVE_HALF_STEPS)
    value = samples[1];
  else
    value = samples[2];
  return value << CURVE_VALUE_SHIFT;
}

typedef s32 (*TerrainHeightFn)(u8 *heights, s32 x, s32 z);

extern TerrainHeightFn gMapGrid_TerrainHeightFunctions[16];

/// Returns the ground height of a layer at (x, z) in 16.16 pixels, as The
/// Broken Seal's: the height function of the cell's terrain kind applied to
/// its samples.
s32 MapGrid_GetTerrainHeight(s32 layer, s32 x, s32 z) {
  struct MapWork *work = Ram_HeapSlots->mapWork;
  u8 *grid;
  u8 *code;

  x >>= 16;
  z >>= 16;
  if (work != NULL)
    grid = work->layers[layer & 3].terrainIds;
  else
    grid = gMapShapeGrid;
  grid += x / 16 + (z / 16 << 7);
  code = Ram_MapCollision + *grid * 4;
  return gMapGrid_TerrainHeightFunctions[*code++ & 15](code, x & 15, z & 15);
}

#include "edition.h"

/// Returns the terrain kind of a layer's cell at (x, z) in 16.16 pixels, as
/// The Broken Seal's.
s32 MapGrid_GetCellAttributeLowNibble(s32 layer, s32 x, s32 z) {
  struct MapWork *work = Ram_HeapSlots->mapWork;
  u8 *grid;

  x >>= 16;
  z >>= 16;
  if (work != NULL)
    grid = work->layers[layer & 3].terrainIds;
  else
    grid = gMapShapeGrid;
  return Ram_MapCollision[grid[x / 16 + ((z / 16) << 7)] * 4] & 15;
}

/// As MapGrid_GetCellAttributeLowNibble, but a rounded square whose two
/// samples differ (a real bump or hollow, not a flat cell) reads as 99.
s32 MapGrid_GetCellAttributeLowNibbleOrBump(s32 layer, s32 x, s32 z) {
  struct MapWork *work = Ram_HeapSlots->mapWork;
  u8 *grid;
  u8 *code;
  s32 kind;

  x >>= 16;
  z >>= 16;
  if (work != NULL)
    grid = work->layers[layer & 3].terrainIds;
  else
    grid = gMapShapeGrid;
  code = &Ram_MapCollision[grid[x / 16 + ((z / 16) << 7)] * 4];
  kind = code[0];
  if (kind == 7 && code[1] != code[2])
    kind = 99;
  return kind;
}

/// The highest ground at (x, z) and at the four points `reach` away from it
/// along x and z, all in 16.16 pixels.
s32 MapGrid_GetHighestTerrainHeightAround(s32 layer, s32 x, s32 z, s32 reach) {
  s32 best;
  s32 height;

  best = MapGrid_GetTerrainHeight(layer, x, z);
  height = MapGrid_GetTerrainHeight(layer, x - reach, z);
  if (best < height)
    best = height;
  height = MapGrid_GetTerrainHeight(layer, x + reach, z);
  if (best < height)
    best = height;
  height = MapGrid_GetTerrainHeight(layer, x, z - reach);
  if (best < height)
    best = height;
  height = MapGrid_GetTerrainHeight(layer, x, z + reach);
  if (best < height)
    best = height;
  return best;
}

/// Gives the cell at (x, z) of layer the ground height `height`, in 8-pixel
/// steps, by tagging its shape with terrain kind 0 at that height.
void MapGrid_SetCellHeight(s32 x, s32 z, s32 layer, s32 height) {
  struct MapWork *work = Ram_HeapSlots->mapWork;
  u8 *shape;

  shape = gMapShapeGrid +
          (work->layers[layer & 3].cells - (struct MapCell *)Ram_MapCellBuffer);
  shape += (x >> 20) + ((z >> 20) << 7);
  *shape = MapCollision_FindOrAddTagFar(layer, 0, height);
}

/// The ground height at (x, z) of layer, in 8-pixel steps.
s32 MapGrid_GetCellHeight(s32 x, s32 z, s32 layer) {
  return MapGrid_GetTerrainHeight(layer, x, z) >> 19;
}

/// As MapGrid_GetCellAttributeLowNibble, a second copy.
s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z) {
  struct MapWork *work = Ram_HeapSlots->mapWork;
  u8 *grid;

  x >>= 16;
  z >>= 16;
  if (work != NULL)
    grid = work->layers[layer & 3].terrainIds;
  else
    grid = gMapShapeGrid;
  return Ram_MapCollision[grid[x / 16 + ((z / 16) << 7)] * 4] & 15;
}

u8 MapGrid_GetCellBehaviorAt(s32 layer, s32 x, s32 y) {
  struct MapWork *state;
  struct MapCell *cells;

  state = Ram_HeapSlots->mapWork;
  x >>= 20;
  y >>= 20;
  cells = (struct MapCell *)gMapCellBuffer;
  if (state != NULL) {
    cells = state->layers[layer & 3].cells;
  }
  cells += x + (y << 7);
  return cells->collisionCode;
}

void MapGrid_SetCellBehaviorAt(u32 layer, s32 x, s32 y, u32 collisionCode) {
  struct MapWork *state = Ram_HeapSlots->mapWork;

  x >>= 20;
  y >>= 20;
  if (state != NULL) {
    struct MapCell *cells = state->layers[layer & 3].cells;

    cells += x + (y << 7);
    cells->collisionCode = collisionCode;
  }
}

/// The height of the ground at (px, pz) (16.16 pixels) on the given layer,
/// less base; a layer past 2 reads the shape grid. With no map, or while the
/// event mode is 3, it is 0.
s32 MapGrid_GetTerrainHeightAbove(s32 px, s32 pz, s32 layer, s32 base) {
  s32 x = px >> 16;
  s32 z = pz >> 16;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  u8 *event = Ram_HeapSlots->eventWork;
  u8 *shapes;
  u8 *code;

  if (map == NULL)
    return 0;
  if (*(s8 *)(event + 0x18a) == 3)
    return 0;
  if (layer <= 2)
    shapes = map->layers[layer].terrainIds;
  else
    shapes = gMapShapeGrid;
  code = Ram_MapCollision + shapes[x / 16 + ((z / 16) << 7)] * 4;
  return gMapGrid_TerrainHeightFunctions[*code++ & 15](code, x & 15, z & 15) -
         base;
}

/// How the ground at (px, pz) (16.16 pixels) on the given layer stands to
/// height: 2 when the cell has no tile, 1 when the ground rises more than 8
/// pixels above it, -1 when it falls more than 12 below, else 0. A layer past
/// 2 reads the cell buffer and shape grid. With no map, or while the event
/// mode is 3, every point is level (0).
s32 MapGrid_CheckCollisionAt(s32 px, s32 pz, s32 layer, s32 height) {
  s32 x = px >> 16;
  s32 z = pz >> 16;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  u8 *event = Ram_HeapSlots->eventWork;
  u8 *cells;
  u8 *shapes;
  u8 *code;
  s32 index;
  s32 rise;

  if (map == NULL)
    return 0;
  if (*(s8 *)(event + 0x18a) == 3)
    return 0;
  if (layer <= 2) {
    cells = (u8 *)map->layers[layer].cells;
    shapes = map->layers[layer].terrainIds;
  } else {
    cells = gMapCellBuffer;
    shapes = gMapShapeGrid;
  }
  index = x / 16 + ((z / 16) << 7);
  cells += index * 4;
  shapes += index;
  if (cells[2] == 255)
    return 2;
  code = Ram_MapCollision + *shapes * 4;
  rise = gMapGrid_TerrainHeightFunctions[*code++ & 15](code, x & 15, z & 15) -
         height;
  if (rise > 8 << 16)
    return 1;
  if (rise < -(12 << 16))
    return -1;
  return 0;
}

/// As MapGrid_CheckCollisionAt for the point position (16.16 pixels, read as
/// its whole-pixel halves) on the actor's layer, against its ground height.
s32 MapGrid_CheckCollision(struct Actor *actor, s16 *position) {
  s32 x = position[1];
  s32 z = position[5];
  struct MapWork *map = Ram_HeapSlots->mapWork;
  u8 *event = Ram_HeapSlots->eventWork;
  u8 *cells;
  u8 *shapes;
  u8 *code;
  s32 index;
  s32 rise;

  if (map == NULL)
    return 0;
  if (*(s8 *)(event + 0x18a) == 3)
    return 0;
  if (actor->terrainId <= 2) {
    cells = (u8 *)map->layers[actor->terrainId].cells;
    shapes = map->layers[actor->terrainId].terrainIds;
  } else {
    cells = gMapCellBuffer;
    shapes = gMapShapeGrid;
  }
  index = x / 16 + ((z / 16) << 7);
  cells += index * 4;
  shapes += index;
  if (cells[2] == 255)
    return 2;
  code = Ram_MapCollision + *shapes * 4;
  rise = gMapGrid_TerrainHeightFunctions[*code++ & 15](code, x & 15, z & 15) -
         actor->terrainHeight;
  if (rise > 8 << 16)
    return 1;
  if (rise < -(12 << 16))
    return -1;
  return 0;
}

/// As MapGrid_CheckCollision, against the actor's height and with no -1: 2
/// when the cell has no tile, 1 when the ground rises more than 8 pixels above
/// it, else 0.
s32 MapGrid_CheckCollisionAbove(struct Actor *actor, s16 *position) {
  s32 x = position[1];
  s32 z = position[5];
  struct MapWork *map = Ram_HeapSlots->mapWork;
  u8 *event = Ram_HeapSlots->eventWork;
  u8 *cells;
  u8 *shapes;
  u8 *code;
  s32 index;

  if (map == NULL)
    return 0;
  if (*(s8 *)(event + 0x18a) == 3)
    return 0;
  if (actor->terrainId <= 2) {
    cells = (u8 *)map->layers[actor->terrainId].cells;
    shapes = map->layers[actor->terrainId].terrainIds;
  } else {
    cells = gMapCellBuffer;
    shapes = gMapShapeGrid;
  }
  index = x / 16 + ((z / 16) << 7);
  cells += index * 4;
  shapes += index;
  if (cells[2] == 255)
    return 2;
  code = Ram_MapCollision + *shapes * 4;
  if (gMapGrid_TerrainHeightFunctions[*code++ & 15](code, x & 15, z & 15) -
          actor->y >
      8 << 16)
    return 1;
  return 0;
}

s32 MapGrid_IsWallAt(s32 *position) {
  s32 x = position[0] / 65536;
  s32 z = (u32)((position[2] & 0xfff00000) - position[1]) >> 16;
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  u8 *cell;

  if (map == NULL)
    return 0;
  cell = *(u8 **)(map + 0x1a8);
  cell += (x / 16 + (z / 16) * 128) * 4;
  return (cell[2] != 255) - 1;
}

/* VIEW(f02c58c): variable gMapBlocks as struct WorldMapBlock gMapBlocks[];
 * module/tla.h has u32 gMapBlocks[]; tried as the 2 declarations' extern u8
 * gMapBlocks[], it does not compile in tla-ja:
 * games/tla/src/field/field.c:10899: conflicting types for `gMapBlocks'. */
extern struct WorldMapBlock gMapBlocks__f02c58c[] __asm__("gMapBlocks");

/// Byte 2 of the world map block under position (blocks 32 pixels square on a
/// 32 by 32 grid): the place the world map's searches look for among the
/// scene's events of kind 2, and 255 where the ship may not go.
// XXX: what the byte numbers besides 255; the world map scene's events of
// kind 2 against the blocks in gMapBlocks would settle it.
s32 WorldMap_GetBlockPlace(struct WorldPosition *position) {
  s32 z = position->y;
  s32 x = position->x;

  if (x < 0)
    x += 0x1fffff;
  x = x >> 21 & 31;
  if (z < 0)
    z += 0x1fffff;
  z = z >> 21 & 31;
  return gMapBlocks__f02c58c[x + z * 32].unk2;
}

s32 WorldMap_GetCollision(struct WorldPosition *position) {
  s32 x_step = position->x >> 17;
  s32 y = position->y;
  s32 y_step = y >> 17;
  u32 cell;
  u32 pixel_offset;
  u8 tile;
  u8 packed_pixels;
  u32 result;

  cell = (((y_step / 8) & 63) << 6) + ((x_step / 8) & 63);
  pixel_offset = (((y_step / 2) & 3) << 1) + ((x_step / 4) & 1);

  tile = BG_SCREEN_BLOCK(10)[cell];
  packed_pixels = *(Ram_MapCollision + 0x800 + (tile << 3) + pixel_offset);
  if (packed_pixels != 0) {
    if (x_step & 2)
      result = packed_pixels >> 4;
    else
      result = packed_pixels & 15;
    if (result != 0)
      return result;
  }

  tile = BG_SCREEN_BLOCK(8)[cell];
  packed_pixels = *(Ram_MapCollision + (tile << 3) + pixel_offset);
  if (packed_pixels != 0) {
    if (x_step & 2)
      result = packed_pixels >> 4;
    else
      result = packed_pixels & 15;
    if (result != 0)
      return result;
  }

  return 7;
}

s32 WorldMap_IsWalkable(void *object, struct WorldPosition *position) {
  if (WorldMap_GetBlockPlace(position) != 255) {
    if ((u32)(WorldMap_GetCollision(position) - 5) <= 7)
      return 0;
  }
  return -1;
}

/// Returns 0 where the ship can sail (collision 1 to 4, or 6), else -1.
s32 WorldMap_IsSailable(void *object, struct WorldPosition *position) {
  if (WorldMap_GetBlockPlace(position) != 255) {
    s32 collision = WorldMap_GetCollision(position);
    u8 *work = Ram_HeapSlots->eventWork;
    s32 mode;

    mode = *(s8 *)(work + 0xd3a);
    if (mode == 0) {
      if ((u32)(collision - 1) <= 3 || collision == 6)
        return 0;
    } else if (mode == 1) {
      if ((u32)(collision - 1) <= 3 || collision == 6)
        return 0;
    } else if (mode == 2) {
      if ((u32)(collision - 1) <= 3 || collision == 6)
        return 0;
    }
  }
  return -1;
}

/// Returns 0 where the winged ship can go, else -1: low down, where it can
/// sail; higher, more of the ground by the kind it last flew over.
s32 WorldMap_IsFlyable(struct WorldMover *mover,
                       struct WorldPosition *position) {
  if (WorldMap_GetBlockPlace(position) != 255) {
    s32 collision = WorldMap_GetCollision(position);
    u8 *work = Ram_HeapSlots->eventWork;

    s32 height = mover->height;

    if (height < 0x40000) {
      if ((u32)(collision - 1) > 3 && collision != 6)
        return -1;
      return 0;
    } else {
      s32 mode = *(s8 *)(work + 0xd3a);

      if (mode == 0) {
        if ((u32)(collision - 1) <= 5)
          return 0;
      } else if (mode == 1) {
        if ((u32)(collision - 1) <= 7)
          return 0;
      } else {
        if (mode == 2) {
          if ((u32)(collision - 1) <= 7)
            return 0;
        } else {
          if ((u32)(collision - 4) <= 4)
            return 0;
        }
        if (collision == 10)
          return 0;
      }
    }
  }
  return -1;
}

#undef gWorldMapWork
// ---- field/map/terrain.c ----

#include "battle.h"
#include "field.h"
#include "game.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "ram_code.h"
#include "module/field/resource_slot.h"

extern u32 Data_02018000[];

u8 WorldMap_GetBattleEnvironment(struct WorldPosition *position,
                                 s32 *terrainKind) {
  s32 selector = WorldMap_GetCollision(position);
  s32 x = position->x;
  s32 y;
  s32 flag = 0;
  u32 tileX;
  u32 index;
  u32 *tile;
  u32 kind;

  if (x < 0)
    x += 0x1fffff;
  tileX = (x >> 21) & 31;
  y = position->y;
  if (y < 0)
    y += 0x1fffff;

  index = tileX + (((y >> 21) & 31) << 5);
  tile = gMapBlocks + index;
  if (((u8 *)tile)[3] & 0x80)
    flag = 0x10;

  kind = (*tile << 2) >> 26;
  *terrainKind = kind;
  if (kind > 40)
    flag = 0x20;
  if (kind == 36)
    flag = 0x40;
  if (kind == 12 || kind == 15)
    flag = 0x30;

  return gWorldMap_TerrainBehaviorTable[flag + selector];
}

void MapGrid_SetScreenShake(s32 value0, s32 value1, s32 value2) {
  struct MapWork *work;

  work = Ram_HeapSlots->mapWork;
  if (value0 >= 0) {
    work->shakeX = value0;
  }
  if (value1 >= 0) {
    work->shakeY = value1;
  }
  if (value2 >= 0) {
    work->shakeDecay = value2;
  }
}

#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void MapGrid_WaitForShakeToSettle(void) {
  s32 *work;
  s32 cnt;

  work = (s32 *)Ram_HeapSlots->mapWork;
  cnt = 0;
  if (work[1] > 255 || work[2] > 255) {
    goto body;
  body:
    wait(1);
    ++cnt;
    if (cnt >= 300) {
      goto done;
    }
    if (work[1] > 255) {
      goto body;
    }
    if (work[2] > 255) {
      goto body;
    }
  }
done:
  work[3] = 0;
}

void TileMap_DrawRowsFromRam(s32 arg0, s32 arg1) {
  void (*routine)(s32, s32, u8 *, u8 *);
  u8 *tiles = gDecodeBuffer;

  RamCode_Load(routine, Render_DrawMapCode, Render_DrawMapCodeSize);
  routine(arg0, arg1, gData_0203c000, tiles + 0x1000);
  Heap_Release(routine);
}

// An inline function over the fixed address, as ☀️'s iwram_call.h reaches
// this entry (☀️'s returns the entry's s32).
#undef Iwram_TransformVector
static inline void Iwram_TransformVector(const void *vector, s32 *result) {
  Iwram_TransformVectorEntry(vector, result);
}

void WorldMap_BuildScanlineTable(s32 pitchSlope, s32 *position,
                                 struct ScanlineRow *row) {
  s32 vector[3];
  s32 result[3];
  s32 height;
  s32 nearZ;
  s32 i;
  s32 z;
  s32 ratio;
  s32 dx;
  s32 sum;
  s32 zero;

  zero = 0;
  vector[0] = position[0];
  vector[1] = zero;
  vector[2] = position[2];
  /* FAKEMATCH(wrapper): the call through this file's inline; as the direct
   * Iwram_TransformVector macro call, 41 of 132 instructions differ in
   * tla-en. */
  Iwram_TransformVector(vector, result);
  height = result[1] - Iwram_MulQ16(result[2], pitchSlope);
  nearZ = -gCameraSceneParameters[0];
  for (i = 0; i < 160; i++) {
    s32 distance;

    z = Iwram_RatioMulQ14(nearZ, (gCameraSceneParameters[4] - i) * Q16_ONE);
    distance = z - pitchSlope;
    if (distance == 0)
      distance = 1;
    ratio = Iwram_RatioMulQ14(distance, height);
    if (ratio < 0) {
      row->scale = Iwram_RatioMulQ14(gCameraSceneParameters[0],
                                     Iwram_MulQ16(-ratio, Q16_ONE / 2));
      z = Iwram_MulQ16(ratio, z);
      dx = (result[2] - ratio) >> 4;
      z -= result[1];
      sum = Iwram_MulQ16(dx, dx);
      z >>= 4;
      sum += Iwram_MulQ16(z, z);
      {
        s32 length = Iwram_Sqrt(sum) << 12;

        if (z < 0)
          length = -length;
        row->depth = Iwram_MulQ16(length, Q16_ONE / 2);
      }
    } else {
      row->scale = 0;
      row->depth = 0;
    }
    row->offsetX = 0;
    row->offsetY = 0;
    row++;
  }
}

// The debug sprite viewer.

#include "input.h"

// The native reads the held keys afresh at each test.
#define HELD (*(volatile u32 *)&gInput.held)

/// The debug sprite viewer, in The Broken Seal's form with this game's
/// changes; it never returns.
void SpriteViewer_Run(void) {
  struct SpriteViewerLayer layers[4];
  volatile u32 zero;
  s32 x;
  s32 y;
  u32 editMode;
  s32 shadowsEnabled;
  u32 backdrop;
  void *ring;
  u32 selectedLayer;
  u32 i;
  s32 spriteId;
  s32 changed;

  x = 144;
  y = 96;
  editMode = 1;
  selectedLayer = 0;
  shadowsEnabled = 1;
  backdrop = 0;
  ring = HeapSlot_AllocEwram(36, 160);
  gDecodeFillByte = 3;
  Dma_Fill32Cell(zero, 0, ring, 4);
  Dma_Fill32Cell(zero, 0, layers, sizeof(layers));
  spriteId = SpriteViewer_FindNextSpriteWithPictures(-1, 1);
  for (i = 0; i < 4; i++) {
    layers[i].animationIndex = 1;
    layers[i].priority = 1;
    layers[i].spriteId = spriteId;
  }
  layers[0].enabled = 1;
  gDebugMode = 2;
  REG_BLDCNT = 0x3f42;
  *(volatile u16 *)0x05000000 = 0x7c00;
  REG_DISPCNT = 0x1140;
  ScreenFade_FromBlack(1);

restart:
  Heap_Initialize();
  Task_ResetAll();
  ring = HeapSlot_AllocEwram(36, 160);
  VramCache_Reset();
  ActorSystem_Initialize(2);
  SpritePictureCache_Load(0, gMapCellBuffer, layers[0].spriteId, 0);
  if (Resource_GetMetadataRecordFar(layers[0].spriteId)->drawKind == 20)
    SpritePictureCache_Load(1, Data_02018000, layers[0].spriteId + 1, 0);
  for (i = 0; i < 10; i++) {
    struct ViewerSprite *sprite;
    s32 part = 0;

    if (Resource_GetMetadataRecordFar(layers[0].spriteId)->drawKind == 20 &&
        (i & 1))
      part = 1;
    sprite = Sprite_Create(layers[0].spriteId + part + (part << 12));
    ResourceMetadata_Register(sprite, layers[1].spriteId);
    ResourceMetadata_Register(sprite, layers[2].spriteId);
    ResourceMetadata_Register(sprite, layers[3].spriteId);
    sprite->flags = shadowsEnabled;
  }
  for (i = 0; i < 4; i++) {
    if (layers[i].enabled != 0)
      SpriteViewer_SetGridColumnPriority(i, layers[i].priority);
    else
      SpriteViewer_SetGridColumnPriority(i, 8);
    SpriteViewer_SetGridColumnColorMode(i, layers[i].colorMode);
    {
      s32 a;

      // Once-only block: the animation index is read before i moves into r0.
      do {
        a = layers[i].animationIndex;
      } while (0);
      SpriteViewer_StartStaggeredAnimations(i, a);
    }
  }
  SpriteViewer_BuildProbeRing(x, y, ring);
  Task_Add(SpriteViewer_PlaceRing, 0x480);

next:
  wait(1);
  for (;;) {
    if (HELD & 8) {
      if (HELD & 32)
        x--;
      if (HELD & 16)
        x++;
      if (HELD & 64)
        y--;
      if (HELD & 128)
        y++;
      SpriteViewer_BuildProbeRing(x, y, ring);
    } else {
      // DEAD: the repeated keys are read once and the value goes unused.
      (void)gInput.repeat;
      if (gInput.repeat & 64)
        selectedLayer = (selectedLayer - 1) & 3;
      if (gInput.repeat & 128)
        selectedLayer = (selectedLayer + 1) & 3;
      if (gInput.repeat & 32)
        editMode = (editMode - 1) & 3;
      if (gInput.repeat & 16)
        editMode = (editMode + 1) & 3;
    }

    if (gInput.pressed & 8) {
      struct ViewerSprite *sprites = Ram_HeapSlots->unk0c[1];

      shadowsEnabled ^= 1;
      for (i = 0; i < 10; i++) {
        sprites->flags = shadowsEnabled;
        sprites++;
      }
    }

    switch (editMode) {
    case 0:
      if (selectedLayer == 1)
        break;
      if (gInput.repeat & 0x200) {
        layers[selectedLayer].animationIndex--;
        if (layers[selectedLayer].animationIndex < 0)
          layers[selectedLayer].animationIndex = 0;
        if (selectedLayer == 0)
          layers[1].animationIndex = layers[0].animationIndex;
        goto next;
      }
      if (!(gInput.repeat & 0x100))
        break;
      layers[selectedLayer].animationIndex++;
      if (layers[selectedLayer].animationIndex > 99)
        layers[selectedLayer].animationIndex = 99;
      if (selectedLayer == 0)
        layers[1].animationIndex = layers[0].animationIndex;
      goto next;

    case 1: {
      s32 steps;

      changed = 0;
      steps = 1;
      if (HELD & 8)
        steps = 10;
      if (gInput.repeat & 0x200) {
        layers[selectedLayer].spriteId =
            SpriteViewer_FindNextSpriteWithPictures(
                layers[selectedLayer].spriteId, -steps);
        changed = 1;
      }
      if (gInput.repeat & 0x100) {
        layers[selectedLayer].spriteId =
            SpriteViewer_FindNextSpriteWithPictures(
                layers[selectedLayer].spriteId, steps);
        changed = 1;
      }
      if (changed == 0)
        break;
      if (selectedLayer == 0)
        goto restart;
      if (layers[selectedLayer].enabled != 0) {
        SpriteViewer_SetGridColumnNumber(selectedLayer,
                                         layers[selectedLayer].spriteId);
        SpriteViewer_SetGridColumnPriority(selectedLayer,
                                           layers[selectedLayer].priority);
        SpriteViewer_SetGridColumnColorMode(selectedLayer,
                                            layers[selectedLayer].colorMode);
        SpriteViewer_StartStaggeredAnimations(
            selectedLayer, layers[selectedLayer].animationIndex);
      }
      goto next;
    }

    case 2:
      changed = 0;
      if (gInput.repeat & 0x200) {
        layers[selectedLayer].priority--;
        if (layers[selectedLayer].priority < 0)
          layers[selectedLayer].priority = 3;
        changed = 1;
      }
      if (gInput.repeat & 0x100) {
        layers[selectedLayer].priority++;
        if (layers[selectedLayer].priority > 3)
          layers[selectedLayer].priority = 0;
        changed = 1;
      }
      if (changed == 0)
        break;
      if (layers[selectedLayer].enabled != 0)
        SpriteViewer_SetGridColumnPriority(selectedLayer,
                                           layers[selectedLayer].priority);
      goto next;

    case 3:
      changed = 0;
      if (gInput.repeat & 0x200) {
        layers[selectedLayer].colorMode--;
        if (layers[selectedLayer].colorMode < 0)
          layers[selectedLayer].colorMode = 15;
        changed = 1;
      }
      if (gInput.repeat & 0x100) {
        layers[selectedLayer].colorMode++;
        if (layers[selectedLayer].colorMode > 15)
          layers[selectedLayer].colorMode = 0;
        changed = 1;
      }
      if (changed == 0)
        break;
      if (layers[selectedLayer].enabled != 0)
        SpriteViewer_SetGridColumnColorMode(selectedLayer,
                                            layers[selectedLayer].colorMode);
      goto next;
    }

    if (gInput.repeat & 1) {
      if (editMode == 0) {
        if (selectedLayer != 1) {
          if (layers[selectedLayer].enabled != 0) {
            SpriteViewer_StartStaggeredAnimations(
                selectedLayer, layers[selectedLayer].animationIndex);
            if (selectedLayer == 0)
              SpriteViewer_StartStaggeredAnimations(1,
                                                    layers[1].animationIndex);
          }
          goto next;
        }
      } else if (selectedLayer != 0) {
        layers[selectedLayer].enabled ^= 1;
        SpriteViewer_SetGridColumnPriority(selectedLayer,
                                           layers[selectedLayer].enabled != 0
                                               ? layers[selectedLayer].priority
                                               : 8);
        goto next;
      }
    }
    if (gInput.repeat & 4) {
      Task_ResetAll();
      Stub_ReturnZero((s32)&ResourceId_Data012, gResource_Data012);
      goto restart;
    }
    if (gInput.pressed & 2) {
      volatile u16 *palette = (volatile u16 *)0x05000000;
      const u16 *colors = gData_0802f054;
      backdrop++;
      backdrop &= 7;
      // Spelled as an address sum: colors[backdrop] swaps the load's registers.
      *palette = *(const u16 *)((u32)colors + (backdrop << 1));
    }
    wait(1);
  }
}

extern const u16 gData_0802f064[];

/// Steps a number from 0 to 715 by delta: a step of one wraps around the
/// range, a larger step forward goes to the next stop above value and one back
/// to the last stop below it (gData_0802f064, ended by 0xffff, from which a
/// step forward wraps to 0). A delta of 0 returns value unchanged.
s32 SpriteViewer_FindNextSpriteWithPictures(s32 value, s32 delta) {
  s32 step;

  if (delta != 0) {
    step = delta;
    if (delta < 0)
      step = -delta;
    if (step == 1) {
      value += delta;
      if (value < 0)
        value = 715;
      if (value >= 716)
        value = 0;
    } else if (delta > 0) {
      u32 i;

      for (i = 0; i < 10; i++) {
        if (value < gData_0802f064[i]) {
          value = gData_0802f064[i];
          if (value == 0xffff)
            value = 0;
          break;
        }
      }
    } else if (value == 0) {
      value = gData_0802f064[8];
    } else {
      s32 i;

      for (i = 8; i >= 0; i--) {
        if (value > gData_0802f064[i]) {
          value = gData_0802f064[i];
          break;
        }
      }
    }
    return value;
  }
#ifdef UBFIX
  return value;
#else
  // UB: no return statement for a delta of 0; the native leaves value in r0.
#endif
}

#include "render_placement.h"

/// Lays out the sprite viewer's ring around pixel (x, z): one position per
/// facing of the first sprite's facing kind (6, 8, 10 or 4, or two opposite
/// pairs for ANIM_FACING_TWO_PART), 56 pixels out (40 for a two-part
/// sprite). B held turns the ring. Called by SpriteViewer_Run when it starts
/// and while Start moves the ring.
void SpriteViewer_BuildProbeRing(s32 centerX, s32 centerY,
                                 struct SpritePlacement *positions) {
  struct SpritePlacement *pair;
  s32 angle;
  s32 i;
  u16 turn;
  u8 kind;

  angle = 0;
  kind = ((struct ProbeObjects *)Ram_HeapSlots->unk0c[1])->shape->kind;
  if ((gInput.held & 2) != 0)
    angle = (s16)(gVBlankCount << 8);
  switch ((u32)kind) {
  case 3:
    for (i = 0; i < 6; i++) {
      angle <<= 16;
      positions->x = centerX << 16;
      angle = (u32)angle >> 16;
      positions->height = 0;
      positions->y = centerY << 16;
      Vector_AddPolarOffset(0x380000, (u16)angle, &positions->x);
      positions++;
      angle = (s16)(angle + 0x2aaa);
    }
    for (; i < 10; i++)
      positions++;
    break;
  case 5:
  case 8:
  case 44:
  case 88:
    for (i = 0; i < 8; i++) {
      angle <<= 16;
      positions->x = centerX << 16;
      angle = (u32)angle >> 16;
      positions->height = 0;
      positions->y = centerY << 16;
      Vector_AddPolarOffset(0x380000, (u16)angle, &positions->x);
      positions++;
      angle = (s16)(angle + 0x2000);
    }
    for (; i < 10; i++)
      positions++;
    break;
  case 4:
  case 6:
    for (i = 0; i < 10; i++) {
      angle <<= 16;
      positions->x = centerX << 16;
      angle = (u32)angle >> 16;
      positions->height = 0;
      positions->y = centerY << 16;
      Vector_AddPolarOffset(0x380000, (u16)angle, &positions->x);
      positions++;
      angle = (s16)(angle + 0x1999);
    }
    break;
  case 20:
    turn = angle;
    angle = (s16)(turn + 0x4000);
    i = 0;
    pair = positions;
    while (i < 2) {
      pair[i * 2].x = centerX << 16;
      pair[i * 2].height = 0;
      pair[i * 2].y = centerY << 16;
      angle <<= 16;
      angle = (u32)angle >> 16;
      Vector_AddPolarOffset(0x280000, angle, &positions[0].x);
      pair[i * 2 + 1].x = centerX << 16;
      {
        s32 *point = &positions[1].x;
        pair[i * 2 + 1].height = 0;
        pair[i * 2 + 1].y = centerY << 16;
        Vector_AddPolarOffset(0x280000, (u16)angle, point);
      }
      angle = (angle + 0x8000) << 16;
      i++;
      positions += 2;
      angle >>= 16;
    }
    break;
  default:
    angle = ((angle << 16) + 0x20000000) >> 16;
    for (i = 0; i < 4; i++) {
      angle <<= 16;
      angle = (u32)angle >> 16;
      positions->x = centerX << 16;
      positions->height = 0;
      positions->y = centerY << 16;
      Vector_AddPolarOffset(0x380000, (u16)angle, &positions->x);
      positions++;
      angle = (s16)(angle + 0x4000);
    }
    for (i = 4; i < 10; i++)
      positions++;
    break;
  }
}
#undef FIELD_AT_OFFSET
#undef HELD

void SpriteViewer_SetGridColumnColorMode(s32 slot, s32 value) {
  u8 *base = Ram_HeapSlots->unk0c[1];
  s32 offset = (slot & 3) * 4 + 40;
  s32 count = 9;

  do {
    u8 *entry = *(u8 **)(base + offset);

    count--;
    entry[5] = value;
    base += 56;
  } while (count >= 0);
}

void SpriteViewer_SetGridColumnPriority(s32 slot, s32 value) {
  u8 *base = Ram_HeapSlots->unk0c[1];
  s32 offset = (slot & 3) * 4 + 40;
  s32 count = 9;

  do {
    u8 *entry = *(u8 **)(base + offset);

    count--;
    entry[6] = value;
    base += 56;
  } while (count >= 0);
}

void SpriteViewer_StartStaggeredAnimations(s32 slot, s32 value) {
  s32 index;
  s32 count;
  s32 offset;
  struct AnimationMetadata *metadata;
  struct UiGridEntry *entry;
  u8 *work;

  work = Ram_HeapSlots->unk0c[1];
  count = 0;
  offset = ((3 & slot) * 4) + 0x28;
  index = 0;
  do {
    entry = *(struct UiGridEntry **)(work + offset);
    if (entry->table0c != 0) {
      metadata = Resource_GetMetadataRecordFar(entry->no);
      if (value < metadata->animationCount) {
        entry->value04 = metadata->drawKind;
        entry->value10 = entry->table0c[value];
        entry->x = count * 0x10;
        entry->value15 = 0x10;
        entry->value14 = index;
        entry->value17 = index;
        entry->value16 = 0xFF;
      }
      work[23] = (u8)metadata->adjustY;
      *(s16 *)(work + 18) = index;
    }
    count += 1;
    work += 0x38;
  } while (count <= 9);
}

void SpriteViewer_SetGridColumnNumber(s32 slot, s32 no) {
  u8 *base = Ram_HeapSlots->unk0c[1];
  s32 offset;
  s32 count;

  Resource_GetMetadataRecordFar(no);
  offset = (slot & 3) * 4 + 40;
  count = 9;
  do {
    u8 *entry = *(u8 **)(base + offset);

    count--;
    *(u16 *)entry = no;
    SpriteAnimation_Reset(entry);
    base += 56;
  } while (count >= 0);
}

extern struct Vec2 gBattle_FormationPlacementScale;

void SpriteViewer_PlaceRing(void) {
  u8 *actor = Ram_HeapSlots->unk0c[1];
  u32 kind = (*(u8 **)(actor + 40))[4];
  struct Vec2 scale;
  u8 *table;
  u16 angle;
  u16 step;
  u16 odd = 0;
  u32 cnt;
  u32 i;

  scale = gBattle_FormationPlacementScale;
  table = (u8 *)Ram_HeapSlots->battleWork;

  switch (kind) {
  case 3:
    angle = 0;
    step = 0x2aaa;
    cnt = 6;
    break;
  case 5:
  case 8:
  case 44:
  case 88:
    angle = 0;
    step = 0x2000;
    cnt = 8;
    break;
  case 4:
  case 6:
    angle = 0;
    step = 0x1999;
    cnt = 10;
    break;
  case 20:
    angle = 0;
    step = 0;
    odd = 0x8000;
    cnt = 4;
    break;
  default:
    angle = 0x2000;
    step = 0x4000;
    cnt = 4;
    break;
  }

  for (i = 0; i < cnt; i++) {
    Render_ApplyProjectedPlacement(actor, (s32 *)(table + i * 16),
                                   (s32 *)&scale, angle);
    actor += 56;
    angle += step;
    if (i & 1)
      angle += odd;
  }
}
