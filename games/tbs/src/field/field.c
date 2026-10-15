#include "unit/field/facing.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "mem.h"
#include "types.h"
#include "animspr.h"
#include "global_cells.h"
#include "dma.h"
#include "ramcode.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "iowrite.h"
#include "heap_state.h"
#include "affine.h"
#include "callback_scheduler.h"
#include "scene.h"
#include "object_dispatch.h"
#include "metadata_lookup.h"
#include "edition.h"
#include "objdisp.h"
#include "object_runtime.h"
#include "battle_system.h"
#include "transform.h"
#include "script.h"
#include "script_motion.h"
#include "script_object_entry.h"
#include "script_object_runtime.h"
#include "field_obj.h"
#include "fixed_math.h"
#include "keystate.h"
#include "field_runtime.h"
#include "event_runtime.h"
#include "game_state.h"
#include "map.h"
#include "map_scroll.h"
#include "ram_buffer.h"
#include "field_sprite.h"
#include "resource_ids.h"
#include "flag_ids.h"
#include "mapcopy.h"
#include "fixed_point_position.h"
#include "mapframe.h"
#include "battle_display.h"
#include "project.h"
#include "frame.h"
#include "palqueue.h"
#include "curve.h"
#include "battle_party.h"
#include "b5_context.h"
#include "vram_tab.h"
#include "vram_block.h"
#include "unit/field/update.h"
#include "unit/field/vertices.h"
#include "unit/field/curve_data.h"
#include "unit/field/griddata.h"

// Decompresses an LZ stream from source into destination and returns
// destination. The stream starts with a little-endian halfword: the offset of
// its command bytes, or 0 for an uncompressed block, whose data (source + 2)
// is returned as it is. The literal bytes follow that halfword. Each command
// byte gives eight flags, low bit first: 1 copies the next literal byte; 0
// reads a big-endian halfword whose low 12 bits are a distance and high 4
// bits a length - 2 (or, when 0, the next byte + 16 + 2), and copies that many
// bytes from the literals' start minus the distance. A zero halfword ends the
// stream.
u8 *Resource_DecompressLz(const u8 *source, u8 *destination) {
  s32 token;
  u32 length;
  u32 control;
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
  base = literal;
  start = destination;
  command++;
  control |= 0x100; // a marker bit, shifted down as the flags are read
  for (;;) {
    if ((control & 1) != 0) {
      if (control != 1) // only the marker is left: read the next command
        *destination++ = *literal++;
    } else {
      token = *command++ << 8;
      token |= *command++;
      if (token == 0)
        break;
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
    }
    control >>= 1;
    if (control == 0) {
      control = *command;
      command++;
      control |= 0x100;
    }
  }
  return start;
}

// Draws one layer's frame into a sprite's pixel buffer (Render_DecodeFrame,
// run from RAM) and uploads a finished buffer to sprite VRAM
// (Tile_CopyStrided).
typedef void (*DrawFn)(const void *src, void *dst, s32 param);
typedef void (*UploadFn)(const void *src, u32 w, u32 h, void *vram);

// Animation script commands. A script is pairs of bytes, a command and its
// argument; a command below 239 shows that frame for `arg` frames at normal
// speed (the timer counts sixteenths, less AnimationEntry.step each frame).
enum AnimationCommand {
  ANIM_CMD_END = 239,      // blank the layer and stop its script
  ANIM_CMD_SET_KIND = 240, // use facing table `arg`
  ANIM_CMD_HOLD = 241,     // stay on this command and frame for good
  ANIM_CMD_WAIT = 245,     // keep the current frame `arg` frames longer
  ANIM_CMD_JUMP = 253,     // continue at script position `arg`
  ANIM_CMD_PLAY = 254,     // switch to animation `arg`
  ANIM_CMD_BLANK = 255     // show nothing for `arg`
};

// Advances a sprite's animation layers by one frame, picks each layer's frame
// for the facing dir, and when any frame changed redraws the sprite: the
// layers composed by priority into one pixel buffer, an optional outline,
// and the result uploaded to the sprite's VRAM tiles. Returns 1 when layer
// 0's frame for this facing is drawn mirrored.
s32 Sprite_ComposeAnimationFrame(struct AnimationObject *obj, u16 dir) {
  s32 mirrored;
  u32 size;
  struct ObjectSystemWork *system;
  DrawFn decodeFrame;
  s32 decoderWasLoaded;
  struct AnimationEntry *layer;
  void *code;
  u8 *pixels;
  u8 *unpacked;
  void *decoded;
  u16 drawOrder[4];
  s32 i;
  s32 k;
  s32 drawCount;
  u32 key;
  s32 arg;
  s32 frame;
  u32 facing;
  s32 tile;

  // The object system's work and the frame decoder sit in the heap slot
  // table; load the decoder into RAM for this call if no one else has.
  mirrored = 0;
  system = (*(void *(*)[]) & gMenuCtrlWork)[0];
  decoderWasLoaded = 1;
  decodeFrame =
      (DrawFn)(*(void *(*)[]) &
               gMenuCtrlWork)[HEAP_SLOT_FRAME_DECODER - HEAP_SLOT_MENU_CONTROL];
  if (decodeFrame == 0) {
    code = Runtime_AllocateHeapBlock(HEAP_SLOT_FRAME_DECODER,
                                     (s32)Render_DecodeFrameCodeSize);
    Dma_Set(Render_DecodeFrame, code,
            (((u32)gRender_DecodeFrameBuffer - (u32)Render_DecodeFrame) >> 2) |
                (DMA_ENABLE32 | DMA_32BIT),
            REG_DMA3);
    decodeFrame =
        (DrawFn)(*(void *(*)[]) & gMenuCtrlWork)[HEAP_SLOT_FRAME_DECODER -
                                                 HEAP_SLOT_MENU_CONTROL];
    decoderWasLoaded = 0;
  }

  // Step each layer's script: read commands until one shows a frame for a
  // while, else count the current frame's time down.
  for (i = 0; i < obj->count; i++) {
    layer = obj->entries[i];
    if (layer == 0) {
      continue;
    }
    if (layer->script == 0) {
      continue;
    }
    /* FAKEMATCH: needed to match: explicit timer-test lifetime keeps command
     * back-edges and the decoded frame value separate from the hold reload. */
  read_command:
    if ((s16)layer->timer <= 0) {
      frame = layer->script[layer->pos++];
      arg = layer->script[layer->pos++];
      switch (frame) {
      case ANIM_CMD_PLAY:
        Animation_SetWorkEntry(layer, arg);
        obj->lastNo = arg;
        break;
      case ANIM_CMD_JUMP:
        layer->pos = arg;
        break;
      case ANIM_CMD_WAIT:
        layer->timer += arg << 4;
        break;
      case ANIM_CMD_HOLD:
        layer->pos -= 2;
        goto framed;
      case ANIM_CMD_SET_KIND:
        layer->kind = arg;
        break;
      case ANIM_CMD_BLANK:
        layer->frameBase = 255;
        frame = 255;
        layer->timer += arg << 4;
        goto select_direction;
      case ANIM_CMD_END:
        layer->frameBase = 255;
        layer->script = 0;
        frame = 255;
        obj->count--;
        goto select_direction;
      // Unused commands, skipped. 251 is not among them: it shows as a frame.
      case 242:
      case 243:
      case 244:
      case 246:
      case 247:
      case 248:
      case 249:
      case 250:
      case 252:
        break;
      default:
        layer->frameBase = frame;
        layer->timer += arg << 4;
        goto select_direction;
      }
      goto read_command;
    }
    layer->timer = layer->timer - layer->step;
  framed:
    frame = layer->frameBase;
  select_direction:
    // The facing table of the layer's kind gives an offset from the base
    // frame (low three bits) and whether to mirror it (bit 7).
    switch (layer->kind) {
    case 1:
      facing = gAnimationFacing_Kind1[dir >> 13];
      break;
    case 2:
    case 20:
      facing = gAnimationFacing_Kind2[dir >> 13];
      break;
    case 22:
      facing = gAnimationFacing_Kind22[dir >> 13];
      break;
    case 3:
      facing = gAnimationFacing_Kind3[dir >> 12];
      break;
    case 4:
      facing = gAnimationFacing_Kind4[dir >> 10];
      break;
    case 5:
      facing = gAnimationFacing_Kind5[dir >> 12];
      break;
    case 6:
      facing = gAnimationFacing_Kind6[dir >> 10];
      break;
    case 8:
      facing = gAnimationFacing_Kind8[((u32)(dir << 16) + 0x10000000) >> 29];
      break;
    case 88:
      facing = gAnimationFacing_Kind88[((u32)(dir << 16) + 0x10000000) >> 29];
      break;
    default:
      facing = 0;
      break;
    }
    frame += facing & 7;
    if (i == 0 && (facing >> 7) != 0) {
      mirrored = 1;
    }
    if (layer->frame != frame) {
      layer->frame = frame;
      obj->dirty = 1;
    }
  }

  if (obj->dirty != 0) {
    size = obj->width * obj->height;
    pixels = MemAlloc(size);
    Iwram_ClearWords(pixels, size);

    // Sort the visible layers by priority (0-3), then by index, with an
    // insertion sort on keys of (priority << 8) | index.
    drawCount = -1;
    for (i = obj->count - 1; i >= 0; i--) {
      layer = obj->entries[i];
      if (layer == 0) {
        continue;
      }
      if (layer->frames == 0) {
        continue;
      }
      if (layer->frame == 255) {
        continue;
      }
      key = layer->priority;
      if (key > 3) {
        continue;
      }
      key = (key << 8) | i;
      /* FAKEMATCH: needed to match: the first shift written apart and the scan
       * left by goto keep the scan unrotated, with its step at the top. */
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

    // Draw the layers in that order. A layer's frames are stored by its
    // codec: 1 is the type-01 format, 3 LZ (unpacked first, or drawn
    // through a scratch buffer when the layer has a draw parameter), and
    // anything else raw for the decoder.
    for (i = 0; i < drawCount; i++) {
      layer = obj->entries[*(u8 *)&drawOrder[i]];
      if (layer->mode == 1) {
        Resource_DecodeType01(layer->frames[layer->frame], pixels);
      } else if (layer->mode == 3) {
        if (layer->param != 0) {
          unpacked = MemAlloc(0x400);
          decodeFrame(
              Resource_DecompressLz(layer->frames[layer->frame], unpacked),
              pixels, layer->param);
          MemFree(unpacked);
        } else {
          decoded = Iwram_Decompress(layer->frames[layer->frame], pixels);
          if (decoded != 0) {
            decodeFrame(decoded, pixels, 0);
          }
        }
      } else {
        decodeFrame(layer->frames[layer->frame], pixels, layer->param);
      }
    }

    // Outline: a pixel whose four neighbours are all drawn becomes the fill
    // colour, and any other drawn pixel the edge colour.
    if ((obj->flags & 2) != 0) {
      u8 *inside;
      u8 edge;
      u8 fill;
      u32 w;
      u32 h;
      u8 *src;
      u8 *dst;

      inside = MemAlloc(size);
      w = obj->width;
      h = obj->height;
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
      MemFree(inside);
    }

    // Upload the pixels to the sprite's tiles in VRAM.
    tile = VramBlock_LoadCached(obj->slot, size, 0);
    {
      void *vram = (void *)(0x06010000 + (tile << 5));
      ((UploadFn)gWorkSlot[HEAP_SLOT_FRAME_UPLOADER])(pixels, obj->width,
                                                      obj->height, vram);
    }
    obj->part[0].tile = tile;
    obj->dirty = 0;
    system->used += size;
    MemFree(pixels);
  }

  if (decoderWasLoaded == 0) {
    Runtime_ReleaseHeapBlock(HEAP_SLOT_FRAME_DECODER);
  }
  return mirrored;
}

// Positions a sprite's image (part 0) and shadow (part 1) on the screen: the
// image centred on screenX and raised height above screenY, the shadow at
// groundHeight. scale is the sprite's (x, y) scale (16.16); above 1.0 both
// parts use the double-size affine mode.
void Render_PlaceSpritePartPair(struct AnimationObject *sprite, s32 screenX,
                                s32 height, s32 screenY, s32 groundHeight,
                                s32 *scale) {
  u32 halfWidth = sprite->width >> 1;
  u32 halfHeight = sprite->height >> 1;
  s32 marginX = 8;
  s32 marginY = 4;
  s32 affine = 1;
  s32 scaleX = *scale++;
  s32 scaleY = *scale;
  s32 x;
  s32 y;
  struct AnimationSpritePart *shadow;

  if (scaleX > 0x10000 || scaleY > 0x10000) {
    affine = 3;
    marginX = 16;
    marginY = 8;
    halfWidth <<= 1;
    halfHeight <<= 1;
  }
  x = (screenX >> 16) - halfWidth;
  y = ((screenY - height) >> 16) - halfHeight -
      ((((sprite->height >> 1) - sprite->offsetY) * scaleY + 0xffff) >> 16);
  sprite->part[0].affine = affine;
  sprite->part[0].x = x;
  sprite->part[0].y = y;
  x = (screenX >> 16) - marginX;
  y = ((screenY - groundHeight) >> 16) - marginY;
  shadow = &sprite->part[1];
  shadow->affine = affine;
  shadow->x = x;
  shadow->y = y;
}

// Places a sprite for the scrolling camera and queues it for drawing. pos is
// (screen x, height, screen y, ground height) in 16.16 pixels, scale its
// (x, y) scale and facing its facing angle. Builds an affine matrix when the
// sprite is mirrored, scaled or rotated, queues the shadow (when the sprite
// has one) and then the image in a draw slot by depth; an object 100 pixels
// or more below the ground takes slot 1 instead, its shadow slot 0.
void Render_ApplyProjectedPlacement(struct AnimationObject *sprite, s32 *pos,
                                    s32 *scale, s32 facing) {
  s32 matrix;
  u32 halfWidth = sprite->width >> 1;
  u32 halfHeight = sprite->height >> 1;
  s32 size = 8;
  s32 sizeHalf = 4;
  s32 affine;
  s32 scaleX = *scale++;
  s32 screenX = *pos++;
  s32 scaleY = *scale;
  s32 height = *pos++;
  s32 screenY = *pos++;
  s32 groundHeight = *pos;
  s32 slot;
  /* FAKEMATCH: needed to match: the two unused arrays supply 24 bytes of the
   * 68-byte frame; removing them leaves 44 bytes. */
  s32 ground[3];
  s32 screen[3];
  struct AffineTransform effect;
  s32 flip;
  s32 x;
  s32 y;
  s32 depth;
  struct AnimationSpritePart *part;

  flip = Sprite_ComposeAnimationFrame(sprite, facing);
  if (flip == 0 && scaleX == 0x10000 && scaleY == scaleX &&
      sprite->rotation == 0) {
    affine = 0;
    matrix = 0;
  } else {
    affine = 1;
    effect.angle = sprite->rotation;
    effect.scaleX = scaleX >> 8;
    effect.scaleY = scaleY >> 8;
    if (flip != 0)
      effect.scaleX = -effect.scaleX;
    matrix = AffineMatrix_BuildForEffect(&effect);
  }
  if (scaleX > 0x10000 || scaleY > 0x10000) {
    affine = 3;
    halfWidth <<= 1;
    halfHeight <<= 1;
    size = 16;
    sizeHalf = 8;
  }
  if (height <= -0x640000) {
    slot = 1;
    depth = 0;
  } else {
    slot = (screenY >> 17) + 10;
    depth = 2;
  }
  y = ((screenY - groundHeight) >> 16) - sizeHalf;
  if (sprite->flags & 1) {
    if (y < 160) {
      part = &sprite->part[1];
      part->affine = affine;
      part->affineIndex = matrix;
      part->x = (screenX >> 16) - size;
      part->y = y;
      Runtime_PushSlotEntry(part, depth);
    }
  }
  x = (screenX >> 16) - halfWidth + ((sprite->offsetX * scaleX + 0xffff) >> 16);
  y = ((screenY - height) >> 16) - halfHeight -
      ((scaleY * ((sprite->height >> 1) - sprite->offsetY) + 0xffff) >> 16);
  if (x < 240 && y < 160) {
    part = &sprite->part[0];
    part->x = x;
    part->y = y;
    part->affine = affine;
    part->affineIndex = matrix;
    Runtime_PushSlotEntry(part, slot);
  }
}

// Places a sprite at 3D point for the fixed camera and queues it for drawing:
// projected to the screen and scaled by its distance (unless displayFlags bit
// 1 keeps its own scale), with an affine matrix as needed and its shadow on
// the ground below. depth 0 sorts it by distance; any other value is its draw
// slot. A sprite off screen gives up its VRAM cache entry unless displayFlags
// bit 0 holds it.
void Render_PlaceProjectedSprite(struct AnimationObject *sprite, s32 *point,
                                 s32 *scale, s32 facing, s32 depth) {
  s32 flip;
  u32 halfHeight;
  u32 size;
  s32 affine;
  s32 halfWidth;
  s32 ground[3];
  s32 screen[3];
  struct AffineTransform effect;
  s32 z;
  s32 roundedScale;
  s32 scaleX;
  s32 scaleY;
  s32 offsetX;
  s32 offsetY;
  s32 slot;

  affine = 1;
  if (gMenuCtrlWork->suspended == 0) {
    z = Render_ProjectPoint(point, screen);
    if (screen[2] != 0 && screen[0] >= -32 && screen[0] <= 272 &&
        screen[1] >= -32 && screen[1] <= 208) {
      if (sprite->displayFlags & 2)
        z = sprite->scale;
      else
        z = Iwram_MulQ16(z, sprite->scale);
      halfWidth = sprite->width >> 1;
      halfHeight = sprite->height >> 1;
      size = 8;
      flip = Sprite_ComposeAnimationFrame(sprite, facing);
      roundedScale = (z + 0x400) & -0x800;
      scaleX = Iwram_MulQ16(roundedScale, *scale++);
      scaleY = Iwram_MulQ16(roundedScale, *scale);
      if (scaleX > 0x1f7ff)
        scaleX = 0x1f800;
      if (scaleY > 0x1f7ff)
        scaleY = 0x1f800;
      offsetX = Iwram_MulQ16(sprite->offsetX, scaleX);
      offsetY = -Iwram_MulQ16((sprite->height >> 1) - sprite->offsetY, scaleY);
      if (scaleX > 0x10000 || scaleY > 0x10000) {
        affine = 3;
        halfWidth <<= 1;
        halfHeight <<= 1;
        size = 16;
      } else if (scaleX == 0x10000 && sprite->rotation == 0 &&
                 scaleY == scaleX) {
        affine = 0;
      }
      if (affine != 0) {
        effect.angle = sprite->rotation;
        effect.scaleX = scaleX >> 8;
        if (flip != 0) {
          effect.scaleX = -effect.scaleX;
          offsetX = -offsetX;
        }
        effect.scaleY = scaleY >> 8;
        z = AffineMatrix_BuildForEffect(&effect);
      } else if (flip != 0) {
        z = 8;
        offsetX = -offsetX;
      } else {
        z = 0;
      }
      sprite->part[0].x = screen[0] - halfWidth + offsetX;
      sprite->part->y = screen[1] - halfHeight + offsetY;
      sprite->part[0].affine = affine;
      sprite->part[0].affineIndex = z;
      if (depth == 0) {
        slot = (0x200 - screen[2]) / 2 + 128;
        if (slot <= 0)
          slot = 1;
        Runtime_PushSlotEntry(sprite, slot);
      } else {
        Runtime_PushSlotEntry(sprite, depth);
      }
      if (sprite->flags & 1) {
        struct AnimationSpritePart *shadow;

        ground[0] = point[0];
        ground[1] = 0;
        ground[2] = point[2];
        Render_ProjectPoint(ground, screen);
        shadow = &sprite->part[1];
        shadow->x = screen[0] - size;
        shadow->y = screen[1] - (size >> 1) + 2;
        shadow->affine = affine;
        shadow->affineIndex = z;
        if (depth == 0)
          Runtime_PushSlotEntry(shadow, 0);
        else
          Runtime_PushSlotEntry(shadow, depth);
      }
      return;
    }
  }
  if (!(sprite->displayFlags & 1)) {
    Resource_ActivateEntry(sprite->slot);
    sprite->dirty = 1;
  }
}

// Sets the draw parameter of each of a sprite's layers to value, except
// layers whose parameter is 15, and marks the sprite for redrawing.
void ObjectGroup_SetChildValueUnlessFifteen(void *rawSprite, u32 value) {
  struct AnimationObject *sprite = rawSprite;
  if (sprite != 0) {
    u32 count = sprite->count;

    if (count != 0) {
      struct AnimationEntry **entry = sprite->entries;
      u32 remaining = count;
      do {
        struct AnimationEntry *layer = *entry++;
        if (layer->param != 15)
          layer->param = value;
        remaining--;
      } while (remaining != 0);
    }
    sprite->dirty = 1;
  }
}

struct ResourceSlotNumber {
  u16 resource;
  s16 number;
};

enum { RESOURCE_SLOT_COUNT = 8, ANIMATION_OBJECT_COUNT = 64 };

extern struct ResourceSlotNumber gResourceSlot_NumberTable[];

// Loads the frames of animation id number into resource slot slot's buffer:
// the resource listed for it in gResourceSlot_NumberTable, decoded, its frame
// offsets turned into pointers, and for variant 1-5 its pixel colours (those
// up to 0xdf) remapped through that variant's conversion table. Returns the
// image's size in pixels (width * height), or 0 when the slot or id is not
// known.
s32 ResourceSlot_Load(u32 slot, u32 *buffer, s32 number, u32 variant) {
  struct AnimationMetadata *metadata;
  struct ObjectSystemWork *work;
  struct ResourceSlot *record;
  struct ResourceSlotNumber *entry;
  u32 *offset;
  u8 *text;
  u8 *end;
  u8 *table;
  u32 size;
  u32 count;
  u32 index;
  u16 entryNumber;
  s32 resource;

  if (slot >= RESOURCE_SLOT_COUNT)
    return 0;
  work = gMenuCtrlWork;
  record = &work->slots[slot];
  metadata = Resource_GetMetadataRecordFar(number);
  record->tag = (slot << 12) | number;
  record->buffer = buffer;

  entry = gResourceSlot_NumberTable;
  for (count = 0; count < 256; count++) {
    s16 *numbers = &entry->number;

    resource = entry->resource;
    entryNumber = *numbers;
    entry++;
    if (entryNumber == 0)
      return 0;
    if (entryNumber == number)
      break;
  }
  size = Resource_DecodeType01(Resource_GetTableEntry(resource), buffer);

  offset = buffer;
  for (count = 0; count < 256; count++) {
    if (*offset == 0)
      break;
    *offset += (u32)buffer;
    offset++;
  }
  if (variant != 0) {
    index = variant - 1;
    text = (u8 *)(offset + 1);
    end = (u8 *)buffer + size;
    if (index > 4)
      index = 0;
    table = gResourceSlot_ConversionTables[index];
    for (; text < end; text++) {
      u32 c = *text;

      if (c <= 0xdf) {
        c = table[c];
        *text = c;
      }
    }
  }
  return metadata->width * metadata->height;
}

// Returns the buffer loaded into the resource slot whose tag is key, or 0.
s32 Animation_LookupValueByKey(s32 key) {
  u32 i;
  struct ResourceSlot *slot = gMenuCtrlWork->slots;

  for (i = 0; i < RESOURCE_SLOT_COUNT; i++, slot++) {
    if (slot->tag == key)
      return (s32)slot->buffer;
  }
  return 0;
}

// Resets each layer of a sprite from its animation's metadata: its facing
// kind, frames, scripts and codec, with no frame shown and no script
// running. The first layer also sets the sprite's size, scale and offsets.
// Returns 0.
s32 Animation_InitializeObjects(struct AnimationObject *sprite) {
  s32 i;

  for (i = 0; i < sprite->count; i++) {
    struct AnimationEntry *layer = sprite->entries[i];
    struct AnimationMetadata *metadata =
        Resource_GetMetadataRecordFar(layer->animId);
    void **frames;

    if (metadata->width == 0)
      continue;

    if (i == 0) {
      sprite->width = metadata->width;
      sprite->height = metadata->height;
      sprite->scale = metadata->scale << 8;
      sprite->offsetY = metadata->adjustY;
      sprite->offsetX = metadata->adjustX;
    }

    frames = metadata->frames;
    if (frames == 0)
      frames = (void **)Animation_LookupValueByKey(layer->animId);

    layer->kind = metadata->drawKind;
    layer->frames = frames;
    layer->scripts = metadata->animation;
    layer->mode = metadata->frameCodec;
    layer->frame = 0xff;
    layer->script = 0;
    layer->pos = 0;
  }

  return 0;
}

// Resets one animation layer from its animation's metadata, as
// Animation_InitializeObjects does for each layer.
void Animation_InitWorkFromMetadata(struct AnimationEntry *layer) {
  void **frames;
  s32 zero;
  struct AnimationMetadata *metadata;

  if (layer != NULL) {
    metadata = Resource_GetMetadataRecordFar(layer->animId);
    if (metadata->width != 0) {
      frames = metadata->frames;
      if (frames == 0)
        frames = (void **)Animation_LookupValueByKey(layer->animId);
      layer->kind = metadata->drawKind;
      layer->scripts = metadata->animation;
      layer->frames = frames;
      layer->mode = metadata->frameCodec;
      zero = 0;
      layer->frame = 0xff;
      layer->script = (u8 *)zero;
      layer->pos = zero;
    }
  }
}

// Adds a layer of animation id to a sprite, in its first free layer of
// four. The first layer also sets the sprite's size, scale and offsets.
// Returns the layer, 0 when no layer record is free, or -1 when the sprite
// has four layers already.
s32 ResourceMetadata_Register(struct AnimationObject *sprite, s32 id) {
  s32 layer = (s32)sprite->entries[0];
  s32 freeIndex = 0;
  struct AnimationEntry **entry;
  struct AnimationMetadata *metadata;

  if (layer != 0) {
    entry = &sprite->entries[0];
    do {
      freeIndex++;
      if (freeIndex > 3)
        break;
      entry++;
      layer = (s32)*entry;
    } while (layer != 0);
  }
  if (freeIndex == 4)
    return -1;
  layer = (s32)AnimationObject_Allocate(id);
  if (layer == 0)
    return 0;
  sprite->entries[freeIndex] = (struct AnimationEntry *)layer;
  metadata = Resource_GetMetadataRecordFar(id);
  if (sprite->count == 0) {
    sprite->width = metadata->width;
    sprite->height = metadata->height;
    sprite->scale = metadata->scale << 8;
    sprite->offsetY = metadata->adjustY;
    sprite->offsetX = metadata->adjustX;
  }
  if (freeIndex == sprite->count)
    sprite->count = freeIndex + 1;
  return layer;
}

// Removes the layer handle from a sprite and clears it, trimming the
// sprite's layer count when no later layer remains.
void ResourceMetadata_Unregister(struct AnimationObject *sprite, s32 layer) {
  s32 laterLayers;
  u32 index;
  u32 i;

  if (sprite != NULL && layer != 0) {
    ResourceMetadata_ClearRecord(layer);
    for (index = 0; index <= 3; index++) {
      if (layer == (s32)sprite->entries[index])
        break;
    }
    if (index != 4) {
      sprite->entries[index] = 0;
      laterLayers = 0;
      for (i = index + 1; i <= 3; i++) {
        if (sprite->entries[i] != 0)
          laterLayers++;
      }
      if (laterLayers == 0)
        sprite->count = index;
    }
  }
}

// Removes and clears a sprite's layer number no (0-3), trimming the sprite's
// layer count when no later layer remains.
void ResourceMetadata_ReleaseSlot(struct AnimationObject *sprite, u32 index) {
  struct AnimationEntry *layer;
  s32 laterLayers;
  u32 i;

  if (sprite != NULL && index <= 3) {
    layer = sprite->entries[index];
    if (layer != NULL) {
      ResourceMetadata_ClearRecord(layer);
      sprite->entries[index] = NULL;
      laterLayers = 0;
      for (i = index + 1; i <= 3; i++) {
        if (sprite->entries[i] != NULL)
          laterLayers++;
      }
      if (laterLayers == 0)
        sprite->count = index;
    }
  }
}

// Starts animation no on a layer: from its first command, unless bit 7 of no
// is set. An animation number the layer's metadata does not have is ignored.
void Animation_SetWorkEntry(struct AnimationEntry *layer, s32 animation) {
  s32 keepPlace;
  struct AnimationMetadata *metadata;
  s32 script;

  keepPlace = 0x80 & animation;
  if (layer->scripts != 0) {
    metadata = Resource_GetMetadataRecordFar(layer->animId);
    if (animation < metadata->animationCount) {
      script = (s32)layer->scripts[animation];
      layer->kind = metadata->drawKind;
      layer->script = (u8 *)script;
      layer->step = 0x10;
      if (keepPlace == 0) {
        layer->pos = keepPlace;
        layer->timer = keepPlace;
      }
    }
  }
}

// Starts animation flags & 0x7f on every layer of a sprite (bit 7 keeps
// each layer's place and timer), unless the sprite plays it already. The
// first layer's animation also sets the sprite's offsets. Returns 0.
s32 AnimationObjects_SelectAnimation(struct AnimationObject *sprite,
                                     s32 animation) {
  s32 keepPlace;
  s32 i;

  keepPlace = animation & 0x80;
  animation &= 0x7f;

  /* FAKEMATCH: the i/test loop keeps an 8-byte frame; a for loop spills an
   * layer cursor and uses 12 bytes. 2026-10-13: for (i = 0; i <
   * sprite->count; i++) strength-reduces entries[i] into that cursor
   * (82 instructions for 76, differ 30). */
  if (sprite->lastNo != animation) {
    i = 0;
    goto test_entry;
  entry_loop:
    {
      struct AnimationEntry *layer = sprite->entries[i];
      struct AnimationMetadata *metadata;
      s32 selectedAnimation;

      if (layer != 0 && layer->scripts != 0) {
        metadata = Resource_GetMetadataRecordFar(layer->animId);
        if (animation < metadata->animationCount) {
          selectedAnimation = (s32)layer->scripts[animation];
          layer->kind = metadata->drawKind;
          layer->script = (u8 *)selectedAnimation;
          layer->step = 0x10;
          if (keepPlace == 0) {
            layer->pos = 0;
            layer->timer = 0;
          }
          if (i == 0) {
            sprite->offsetY = metadata->adjustY;
            sprite->offsetX = metadata->adjustX;
          }
        }
      }
    }
    i++;
  test_entry:
    if (i < sprite->count)
      goto entry_loop;
    sprite->lastNo = animation;
  }
  return 0;
}

// Sets the frame timer of each animated layer of a sprite to val frames.
// Returns 0.
s32 AnimationObjects_SetActiveFrameTimers(struct AnimationObject *sprite,
                                          s32 frames) {
  u8 count = sprite->count;
  struct AnimationEntry **layerList;
  s32 remaining;

  if (count != 0) {
    frames <<= 4;
    layerList = sprite->entries;
    remaining = count;
    do {
      struct AnimationEntry *layer = *layerList++;
      if (layer != 0 && layer->scripts != 0)
        layer->timer = frames;
      remaining--;
    } while (remaining != 0);
  }
  return 0;
}

// Sets the timer step, the animation speed, of each animated layer of a
// sprite (16 is normal speed).
void AnimationObjects_SetActiveTimerSteps(struct AnimationObject *sprite,
                                          s32 step) {
  u8 count = sprite->count;
  if (count != 0) {
    struct AnimationEntry **layerList = sprite->entries;
    s32 remaining = count;
    do {
      struct AnimationEntry *layer = *layerList++;
      if (layer != 0 && layer->scripts != 0)
        layer->step = step;
      remaining--;
    } while (remaining != 0);
  }
}

// Allocates and zeroes the 64 sprites and 64 animation layers (from the
// fixed blocks in mode 3, the heap otherwise), sets up the palette DMA,
// loads the shadow tiles into VRAM cache entry 93 and puts the sprite tile
// uploader in RAM.
void ObjectSystem_Configure(s32 mode) {
  void *sprites;
  void *layers;
  volatile u32 zero;

  if (mode == 3) {
    sprites = Runtime_AllocateBlock(4, ANIMATION_OBJECT_COUNT *
                                           sizeof(struct AnimationObject));
    layers = Runtime_AllocateBlock(3, ANIMATION_OBJECT_COUNT *
                                          sizeof(struct AnimationEntry));
  } else {
    sprites = Runtime_AllocateHeapBlock(4, ANIMATION_OBJECT_COUNT *
                                               sizeof(struct AnimationObject));
    layers = Runtime_AllocateHeapBlock(3, ANIMATION_OBJECT_COUNT *
                                              sizeof(struct AnimationEntry));
  }
  PaletteDma_LoadBlock();
  Dma_Fill32Cell(zero, 0, sprites,
                 ANIMATION_OBJECT_COUNT * sizeof(struct AnimationObject));
  Dma_Fill32Cell(zero, 0, layers,
                 ANIMATION_OBJECT_COUNT * sizeof(struct AnimationEntry));
  VramBlock_LoadCached(93, 128, gObject_ShadowTiles);
  RamCode_LoadSlot(53, Tile_CopyStrided, Tile_CopyStridedCodeSize);
}

// Takes a free animation layer record and starts it on animation id's first
// animation. Returns the layer, or NULL when none is free or the id has no
// image.
struct AnimationEntry *AnimationObject_Allocate(s32 id) {
  struct AnimationMetadata *metadata;
  struct AnimationEntry *candidate;
  struct AnimationEntry *found;
  struct AnimationEntry *layer;
  s32 i;
  void **frames;
  u8 **animation;

  found = NULL;
  metadata = Resource_GetMetadataRecordFar(id);
  candidate = gAnimationObjects[0];
  layer = NULL;

  if (metadata->width != 0) {
    for (i = 0; i < ANIMATION_OBJECT_COUNT; i++, candidate++) {
      if (candidate->kind == 0) {
        found = candidate;
        break;
      }
    }
    if (found != NULL) {
      /* FAKEMATCH: instruction order only: read as a word, the frames load does
       * not wait on the animId store, so the zero for pos and param loads first
       * as native; the typed read swaps the two (differ 2). */
      frames = (void **)*(s32 *)&metadata->frames;
      layer = found;
      layer->animId = id;
      if (frames == 0)
        frames = (void **)Animation_LookupValueByKey(id);

      animation = metadata->animation;
      layer->scripts = animation;
      layer->frames = frames;
      layer->mode = metadata->frameCodec;
      layer->frame = 0xff;
      layer->script = animation[0];
      layer->pos = 0;
      layer->kind = metadata->drawKind;
      layer->param = 0;
    }
  }
  return layer;
}

// Zeroes one animation layer (struct AnimationEntry); does nothing for NULL.
void ResourceMetadata_ClearRecord(void *dst) {
  if (dst != 0) {
    Dma_Fill32(0, dst, sizeof(struct AnimationEntry));
  }
}

// Creating and releasing animated sprites, and measuring animation lengths.

typedef char
    ResourceObjectWork_size[sizeof(struct ResourceObjectWork) == 56 ? 1 : -1];

// Creates a sprite for animation id, taking one of the 64 free sprites and a
// VRAM cache entry for its tiles. Returns the sprite, or NULL when there is
// no room or the id has no image.
struct ResourceObject *ResourceObject_Create(s32 id) {
  struct AnimationMetadata *metadata;
  struct AnimationObject *sprite;
  struct AnimationObject *freeSprite;
  u32 *attribute;
  s32 slot;
  s32 tile;
  s32 i;
  u32 shape;

  freeSprite = NULL;
  metadata = Resource_GetMetadataRecordFar(id);
  slot = Resource_FindFreeEntry();
  sprite = gSpriteObjects;
  if (metadata->width == 0)
    return NULL;
  // A width of 0 marks a free sprite.
  for (i = 0; i <= 63; i++, sprite++) {
    if (sprite->width == 0) {
      freeSprite = sprite;
      break;
    }
  }
  if (freeSprite == NULL)
    return NULL;
  if (slot == VRAM_CACHE_ENTRY_COUNT)
    return NULL;
  tile = VramBlock_LoadCached(slot, 0, 0);
  if (tile == 0)
    return NULL;
  freeSprite->slot = slot;
  freeSprite->rotation = 0;
  freeSprite->flags = 1;

  // The OAM shape and size bits (attributes 0 and 1) for the image's width
  // and height in pixels.
  switch ((u32)((metadata->width << 8) + metadata->height)) {
  case 0x0808: // 8x8
    shape = 0;
    break;
  case 0x0810: // 8x16
    shape = 0x8000;
    break;
  case 0x1008: // 16x8
    shape = 0x4000;
    break;
  case 0x1010: // 16x16
    shape = 0x40000000;
    break;
  case 0x1020: // 16x32
    shape = 0x80008000;
    break;
  case 0x2010: // 32x16
    shape = 0x80004000;
    break;
  case 0x2020: // 32x32
    shape = 0x80000000;
    break;
  case 0x2040: // 32x64
    shape = 0xc0008000;
    break;
  case 0x4020: // 64x32
    shape = 0xc0004000;
    break;
  case 0x4040: // 64x64
    shape = 0xc0000000;
    break;
  default:
    shape = 0;
    break;
  }

  // Part 0 is the image: 256 colours, priority 2. Part 1 is its shadow: a
  // 16x8, 256-colour sprite of the shadow tiles (VRAM cache 93), priority 2.
  attribute = (u32 *)freeSprite->part;
  *attribute++ = 0;
  *attribute++ = shape | 0x2000;
  *attribute++ = tile | 0x800;
  *attribute++ = 0;
  *attribute++ = 0x6000;
  *attribute = (gVramBlockCache[93].offset >> 5) | 0x800;
  if (ResourceMetadata_Register(sprite, id) == -1) {
    /* FAKEMATCH: native dead -1 after Register (movs/negs); empty arm
       after a same-byte reload keeps the carrier (differ 32 -> 0). */
    freeSprite->dirty = freeSprite->dirty;
  }
  return (struct ResourceObject *)freeSprite;
}

// Releases a sprite: its VRAM cache entry (unless bit 0 of flags is set),
// its four animation layers, and then zeroes its 56 bytes.
void ResourceObject_Release(struct ResourceObjectWork *sprite) {
  s32 remaining;
  void **layer;

  if (sprite) {
    if (!(sprite->flags & 1))
      Resource_ResetEntry(sprite->resource);
    layer = sprite->children;
    remaining = 3;
    do {
      ResourceMetadata_ClearRecord(*layer++);
    } while (--remaining >= 0);
    Dma_Fill32(0, sprite, sizeof(*sprite));
  }
}

// Returns how long the first frameCount frames of animation number animation
// of id last: the sum of the arguments of its frame, wait and blank commands,
// up to frameCount of them or the first play, hold, jump or end command.
s32 ResourceMetadata_SumCommandLengths(s32 id, u32 animation, s32 frameCount) {
  struct AnimationMetadata *metadata;
  u8 *script;
  u8 command;
  u8 arg;
  s32 sum = 0;

  metadata = Resource_GetMetadataRecordFar(id);
  if (animation >= metadata->animationCount) {
    return 0;
  }
  script = ((u8 **)metadata->animation)[animation];
  for (;;) {
    command = script[0];
    arg = script[1];
    script += 2;
    if (command == ANIM_CMD_PLAY || command == ANIM_CMD_HOLD ||
        command == ANIM_CMD_JUMP || command == ANIM_CMD_END) {
      break;
    }
    if (command == ANIM_CMD_WAIT || command == ANIM_CMD_BLANK ||
        command < ANIM_CMD_END) {
      sum += arg;
      frameCount--;
      if (frameCount == 0) {
        break;
      }
    }
  }
  return sum;
}

// Field objects: creating and releasing them, and drawing them each frame.

// One step of dissolving a sprite pixel by pixel. In each 8x8 tile (256
// colours) one pixel is cleared, in gMap_TileDissolveOrder's order; each
// tile runs one step ahead of the last, and only tiles whose step is 64-127
// clear a pixel. obj is the sprite (struct AnimationObject), pos the step
// (0-127).
void Map_RenderAnimatedTileFrame(u8 *obj, u32 pos) {
  u16 *tile;
  s32 tileCount;
  u32 i;
  u8 *order;
  u32 highMask;
  u32 indexMask;
  u8 offsetMask;

  struct AnimationObject *sprite = (struct AnimationObject *)obj;

  // The sprite's tiles in OBJ VRAM.
  tile = (u16 *)(0x06010000 + gVramBlockCache[sprite->slot].offset);
  tileCount = (sprite->width * sprite->height) / 64;
  i = 0;

  if (i < tileCount) {
    order = gMap_TileDissolveOrder;
    highMask = 0xff00;
    indexMask = 0x3f;
    offsetMask = 0x3e;

    for (; i < tileCount; i++, tile += 32, pos++) {
      if ((pos - 64) <= 63) {
        u8 pixel;
        u16 *pixelPair;
        u32 isOdd;

        // VRAM takes no byte writes: rewrite the pixel pair, clearing one.
        pixel = order[(pos + i * 16) & indexMask];
        pixelPair = (u16 *)((u8 *)tile + (pixel & offsetMask));
        isOdd = 1;
        isOdd &= pixel;
        if (isOdd != 0)
          *pixelPair = *(u8 *)pixelPair;
        else
          *pixelPair &= highMask;
      }
    }
  }
}

// Dissolves a sprite pixel by pixel, four steps a frame, over 32 frames.
void Map_RenderAnimatedTileFramesForObject(u8 *sprite) {
  u32 step;

  step = 0;
  do {
    Map_RenderAnimatedTileFrame(sprite, step);
    Map_RenderAnimatedTileFrame(sprite, step + 1);
    Map_RenderAnimatedTileFrame(sprite, step + 2);
    Map_RenderAnimatedTileFrame(sprite, step + 3);
    step += 4;
    WaitFrames(1);
  } while (step <= 127);
}

// Dissolves count sprites together, pixel by pixel, over 32 frames.
void Map_RenderAllAnimatedTileFrames(u8 **sprites, s32 count) {
  u8 **first;
  u8 **sprite;
  s32 remaining;
  u32 step;
  u32 step1;
  u32 step2;
  u32 step3;

  first = sprites;
  step = 0;
  do {
    if (count > 0) {
      step1 = step + 1;
      step2 = step + 2;
      sprite = first;
      step3 = step + 3;
      remaining = count;
      do {
        Map_RenderAnimatedTileFrame(*sprite, step);
        Map_RenderAnimatedTileFrame(*sprite, step1);
        Map_RenderAnimatedTileFrame(*sprite, step2);
        remaining -= 1;
        Map_RenderAnimatedTileFrame(*sprite++, step3);
      } while (remaining != 0);
    }
    WaitFrames(1U);
    step += 4;
  } while (step <= 0x7fU);
}

// Writes obj's position on the screen, in pixels, to out[0] and out[1] and
// returns 0 when it is on or near the screen; otherwise writes 0, 0 and
// returns -1.
s32 Map_GetScreenRelativePosition(struct ObjectRuntime *obj, s32 *out) {
  struct MapScrollWork *mapWork = gMapWork[0];
  s32 *view = &mapWork->viewX;
  s32 viewX;
  s32 viewZ;
  s32 x;
  s32 y;

  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  x = obj->x - viewX;
  y = obj->z - viewZ;
  if ((x >= -0x1fffff && x <= 0x10fffff) && y > 0 && y < 0xe00000) {
    *out++ = x >> 16;
    *out = y >> 16;
    return 0;
  }
  *out++ = 0;
  *out = 0;
  return -1;
}

// Sets up the object system: allocates and zeroes the 64 objects and
// registers the per-frame updates. Mode 4 moves objects with
// Object_UpdateAllMotion; modes 3 and 4 use the fixed camera.
void ObjectSystem_Initialize(s32 mode) {
  struct ObjectSystemWork *system;
  struct ObjectRuntime *objects;
  volatile u32 fill;

  system = Runtime_AllocateBlock(6, sizeof(*system));
  objects = Runtime_AllocateBlock(5, 64 * sizeof(*objects));
  ObjectSystem_Configure(mode);
  Dma_Fill32Cell(fill, 0, objects, 64 * sizeof(*objects));
  Dma_Fill32Cell(fill, 0, system, sizeof(*system));
  if (mode == 4)
    Scheduler_AddOrUpdateCallback(Object_UpdateAllMotion, 0xc8a);
  else
    Scheduler_AddOrUpdateCallback(Object_UpdateAllThumb, 0xc8a);
  if (mode >= 3 && mode <= 4) {
    Scheduler_AddOrUpdateCallback(ObjectSystem_UpdateCameraFixed, 0xc80);
  } else {
    Scheduler_AddOrUpdateCallback(ObjectSystem_UpdateCamera, 0xc80);
    gIwram_Data2 = 0;
    gIwram_Data1 = 0;
  }
  system->edge = 15;
  system->fill = 0;
}

// Does nothing.
void ResourceTable_ReservedNoOpC0C4(void) {}

// Does nothing.
void ResourceTable_ReservedNoOp2(void) {}

// Returns the first of the 64 objects not in use (with no script), or 0.
void *ObjectDispatch_FindFreeObject(void) {
  struct ObjectRuntime *obj = gObjectSlots;
  void *found = 0;
  s32 i = 0;

  while (i <= 63) {
    if (obj->script == 0) {
      found = obj;
      break;
    }
    i++;
    obj++;
  }
  return found;
}

// Releases an object's sprites and zeroes it, freeing it.
void ObjectDispatch_Release(struct DispatchObject *obj) {
  s32 remaining;
  struct ResourceObjectWork **spriteList;

  if (obj) {
    switch (obj->kind & 15) {
    case 1: // one sprite
      ResourceObject_Release(obj->target.child);
      break;
    case 2: // up to four sprites
      spriteList = (struct ResourceObjectWork **)obj->target.children;
      remaining = 3;
      do {
        struct ResourceObjectWork *sprite = *spriteList++;
        if (sprite)
          ResourceObject_Release(sprite);
      } while (--remaining >= 0);
      break;
    }
    Dma_Fill32(0, obj, sizeof(struct ObjectRuntime));
  }
}

// Creates a field object at (x, y, z), in 16.16 pixels, or returns 0 when
// none is free. id / 4096 is the kind (0: one sprite, 2: two sprites, id
// and id + 1) and the low 12 bits the sprite's animation id.
struct ObjectRuntime *FieldObject_Create(s32 id, s32 x, s32 y, s32 z) {
  struct ObjectRuntime *obj;
  struct FieldActor *actor;
  struct ScriptMotionObject *motion;
  struct ScriptObjectRuntime *scripted;
  void *sprite;
  s32 kind;
  void **spriteList;

  ObjectDispatch_FindFreeObject();
  kind = id / 4096;
  id &= 0xfff;
  obj = ObjectDispatch_FindFreeObject();
  if (obj != NULL) {
    actor = (struct FieldActor *)obj;
    actor->radius = 16;
    switch (kind) {
    case 0:
      sprite = ResourceObject_Create(id);
      if (sprite != NULL) {
        obj->animationKind = 1;
        obj->animation = sprite;
        actor->radius = Resource_GetMetadataRecordFar(id)->boxY >> 1;
      } else {
        obj->animationKind = 0;
      }
      break;
    case 2:
      // Take a zeroed list of four sprites, and fill it with id and id + 1.
      spriteList = (void **)((u32 *)gMenuCtrlWork + gMenuCtrlWork->count++ + 2);
      obj->animationKind = kind;
      obj->animation = spriteList;
      Dma_Fill32(0, spriteList, 0x10);
      sprite = ResourceObject_Create(id);
      if (sprite != NULL) {
        actor->radius = Resource_GetMetadataRecordFar(id)->boxY >> 1;
        *spriteList++ = sprite;
      }
      sprite = ResourceObject_Create(id + 1);
      if (sprite != NULL)
        *spriteList = sprite;
      break;
    }
  }
  if (obj != NULL) {
    motion = (struct ScriptMotionObject *)obj;
    scripted = (struct ScriptObjectRuntime *)obj;
    Object_SetPositionAndResetMotion(obj, x, y, z);
    obj->script = (s32 *)gObjectDispatch_DefaultScript;
    obj->speedLimit = 0x20000;
    obj->step = 0;
    actor->scaleX = 0x10000;
    actor->scaleY = 0x10000;
    obj->acceleration = 0x10000;
    obj->flags = 3;
    motion->gravity = 0x10000;
    motion->vertical.bounce = 0x4000;
    obj->unk59 = 0;
    obj->actionFlags = 1;
    actor->riseBaseY = 0;
    obj->angle = 0x4000;
    scripted->homeX = x / 65536; // its home position, in pixels
    scripted->homeZ = z / 65536;
  }
  return obj;
}

// Gives an object a script and starts it from the top.
void ObjectDispatch_Initialize(struct DispatchObject *obj, const s32 *script) {
  if (obj != 0) {
    obj->value04 = 0;
    obj->value00 = (u32)script;
    obj->value5b = 0;
    obj->value5d = 0;
    obj->value57 = 0;
  }
}

// Defines name(object, value), which calls apply(sprite, value) on an
// object's one sprite, or on each sprite of its group.
#define DEFINE_APPLY_TO_CHILDREN(name, object_type, apply)                     \
  void name(object_type rawObject, s32 value) {                                \
    struct DispatchObject *obj;                                                \
    void **sprites;                                                            \
    s32 remaining;                                                             \
    void *sprite;                                                              \
                                                                               \
    obj = rawObject;                                                           \
    if (obj != 0) {                                                            \
      switch (obj->kind & 0xf) {                                               \
      case 1:                                                                  \
        apply(obj->target.child, value);                                       \
        break;                                                                 \
      case 2:                                                                  \
        sprites = obj->target.children;                                        \
        remaining = 3;                                                         \
        do {                                                                   \
          sprite = *sprites++;                                                 \
          if (sprite != 0)                                                     \
            apply(sprite, value);                                              \
          remaining--;                                                         \
        } while (remaining >= 0);                                              \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
  }

// Switches all of an object's sprites to animation value.
DEFINE_APPLY_TO_CHILDREN(ObjectDispatch_ApplyArgumentToChildren, void *,
                         AnimationObjects_SelectAnimation)

// Sets the animation speed (timer step) of all of an object's sprites.
DEFINE_APPLY_TO_CHILDREN(ObjectDispatch_ApplyValueToChildren,
                         struct DispatchObject *,
                         AnimationObjects_SetActiveTimerSteps)

// Both of the above: switches all of an object's sprites to animation and
// sets their timer step to step.
void ObjectDispatch_ApplyPairToChildren(struct DispatchObject *obj,
                                        s32 animation, s32 step) {
  void **sprites;
  void *sprite;
  s32 i;

  if (obj != 0) {
    switch (obj->kind & 15) {
    case 1:
      AnimationObjects_SelectAnimation(obj->target.child, animation);
      AnimationObjects_SetActiveTimerSteps(obj->target.child, step);
      break;
    case 2:
      sprites = obj->target.children;
      for (i = 3; i >= 0; i--) {
        sprite = *sprites++;
        if (sprite != 0) {
          AnimationObjects_SelectAnimation(sprite, animation);
          AnimationObjects_SetActiveTimerSteps(sprite, step);
        }
      }
      break;
    }
  }
}

// Sets the rotation of an object's sprite, when it has just one.
void ObjectDispatch_SetChildRotation(struct DispatchObject *obj, u32 rotation) {
  if (obj != 0 && (obj->kind & 0xf) == 1)
    ((struct FieldSprite *)obj->target.child)->rotation = rotation;
}

// Gives a one-sprite object's first layer animation id animId and restarts
// its layers.
// A negative animId does nothing.
void Animation_SetIndexAndInitObjects(void *rawObject, s32 animId) {
  struct DispatchObject *obj = rawObject;
  struct AnimationObject *sprite;

  if (obj != NULL && (obj->kind & 0xf) == 1) {
    sprite = obj->target.child;
    if (animId >= 0) {
      sprite->entries[0]->animId = animId;
      Animation_InitializeObjects(sprite);
    }
  }
}

// Adds a layer of animation id animId to a one-sprite object's sprite.
// A negative animId does nothing.
void ObjectDispatch_RegisterChildMetadata(struct DispatchObject *obj,
                                          s32 animId) {
  if (obj != 0 && (obj->kind & 0xf) == 1) {
    struct AnimationObject *sprite = obj->target.child;
    if (animId >= 0)
      ResourceMetadata_Register(sprite, animId);
  }
}

// Starts an object on the script that approaches its linked object
// (SC_APPROACH_LINK), linked to arg. The functions below start the engine's
// other scripts (object_script.c).
void ObjectDispatch_InitFromTable5WithArgument(struct DispatchObject *obj,
                                               s32 arg) {
  ObjectDispatch_Initialize(obj, gObjectDispatch_Table5Script);
  obj->argument = arg;
}

// Starts an object on the script that runs SC_KEY_COUNT every frame.
void ObjectDispatch_InitFromTable0(struct DispatchObject *obj) {
  ObjectDispatch_Initialize(obj, gObjectDispatch_Table0Script);
}

// Starts an object on the script that moves it by the keys (SC_KEYS).
void ObjectDispatch_InitFromTable1(struct DispatchObject *obj) {
  ObjectDispatch_Initialize(obj, gObjectDispatch_Table1Script);
}

// Starts an object on the player control script (SC_PLAYER).
void ObjectDispatch_InitFromTable2(struct DispatchObject *obj) {
  ObjectDispatch_Initialize(obj, gObjectDispatch_Table2Script);
}

// Starts an object on the world map control script (SC_WORLD).
void ObjectDispatch_InitFromTable3(struct DispatchObject *obj) {
  ObjectDispatch_Initialize(obj, gObjectDispatch_Table3Script);
}

// Starts an object on the idle script (a lone SC_END).
void ObjectDispatch_InitFromTable6(struct DispatchObject *obj) {
  ObjectDispatch_Initialize(obj, gObjectDispatch_Table6Script);
}

// Starts an object on the camera script (SC_CAMERA). With a leader to
// follow (arg), it also gets a speed limit of 4 pixels a frame and an
// acceleration of 0.5 (value30 and value34 are those fields).
void ObjectDispatch_InitFromTable4WithArgument(struct DispatchObject *obj,
                                               s32 arg) {
  ObjectDispatch_Initialize(obj, gObjectDispatch_Table4Script);
  if (arg != 0) {
    obj->value34 = 0x8000;
    obj->value30 = 0x40000;
    obj->argument = arg;
    obj->value64 = 0;
  }
}

// Waits until the object's script reaches SC_END (0x10), at most 300 frames.
void ObjectDispatch_WaitForValue16(void *rawObject) {
  struct ScriptInterpreter *interpreter = rawObject;
  s32 frames;

  frames = 0;
  if (interpreter->script[interpreter->cursor] != 0x10) {
    do {
      WaitFrames(1);
      frames++;
      if (frames > 299)
        break;
    } while (interpreter->script[interpreter->cursor] != 0x10);
  }
}

// Sets the flags of a one-sprite object's sprite (bit 0: draw its shadow).
void ObjectDispatch_SetSingleChildFlags(struct DispatchObject *obj, u32 flags) {
  if (obj != NULL) {
    if ((obj->kind & 0xf) == 1)
      ((struct AnimationObject *)obj->target.child)->flags = flags;
  }
}

// Sets the OAM blend mode of a one-sprite object's sprite.
void ObjectDispatch_SetChildBlendMode(struct DispatchObject *obj, u32 mode) {
  if (obj != 0 && obj->kind == 1) {
    struct FieldSprite *sprite = obj->target.child;
    sprite->blendMode = mode;
  }
}

// Sets bit 1 of a one-sprite object's displayFlags: when set, the fixed
// camera draws it at its own scale, not scaled by distance.
void ObjectDispatch_SetChildDisplayFlag1(struct DispatchObject *obj,
                                         u32 fixedScale) {
  if (obj != 0 && obj->kind == 1) {
    struct FieldSprite *sprite = obj->target.child;
    sprite->unk1d1 = fixedScale;
  }
}

// Sets the draw parameter of a one-sprite object's layers (see
// ObjectGroup_SetChildValueUnlessFifteen).
void Animation_ApplyChildValues(struct DispatchObject *obj, u32 param) {
  if (obj != NULL && obj->kind == 1)
    ObjectGroup_SetChildValueUnlessFifteen(obj->target.child, param);
}

// Resumes the object camera updates, starts the palette back to normal and,
// a frame later, shows BG0 and the sprites with BG1-3 hidden.
void Graphics_EnableObjLayerAndCallbacks(void) {
  Scheduler_EnableCallbacks(ObjectSystem_UpdateCamera);
  Scheduler_EnableCallbacks(ObjectSystem_UpdateCameraFixed);
  BattleFx_ApplyColorToTargetBufferFar(0x10000, 1);
  BattleFx_StartBufferInterpolationFar(1);
  WaitFrames(1);
  REG_DISPCNT = (REG_DISPCNT & 0xf1ff) | DISPCNT_OBJ_ON;
}

// Stops the object camera updates and hides BG1-3 and the sprites.
void ObjectDispatch_StopCallbacksAndHideLayers(void) {
  Scheduler_DisableCallbacks(ObjectSystem_UpdateCamera);
  Scheduler_DisableCallbacks(ObjectSystem_UpdateCameraFixed);
  REG_DISPCNT &= 0xe1ff;
}

// Returns 1.
s32 ObjectDispatch_ReturnTrue(void) { return 1; }

// A map cell's word, as the camera reads it.
struct CameraTile {
  u32 block : 12;   // the block (metatile) number
  u32 layer : 2;    // if not 0, the layer of an object standing on it, + 1
  u32 priority : 2; // if not 0, the priority of an object standing on it
  u32 unk10 : 16;
};

// An object's sprite, with its OAM attributes, as the camera reads it.
struct CameraSprite {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 flipX : 1;
  u16 flipY : 1;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[0x0a];
  u16 secondTile : 10;
  u16 secondPriority : 2;
  u16 secondPalette : 4;
  u8 unk16[2];
  s32 scale;
  u8 resource; // its VRAM cache entry
  u8 flags1d;
  u8 unk1e[7];
  u8 dirty; // redraw when next placed
};

// A field object, as the camera reads it.
struct CameraObject {
  u32 active;
  u16 unk04;
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  s32 height;
  s32 scaleX;
  s32 scaleY;
  u8 unk20[2];
  u8 layer;
  u8 flags;
  u8 unk24[0x2c];
  void *sprite;
  u8 kind; // 1: one sprite, 2: up to four
  u8 unk55[7];
  u8 held;
  u8 unk5d[0x13];
};

struct CameraLayer {
  struct CameraTile *tiles;
  u8 unk04[44];
};

// The map work, as the camera reads it.
struct CameraState {
  u8 unk000[0xe4];
  s32 x; // the view's position
  s32 z;
  u8 unk0ec[0x44];
  struct CameraLayer layers[1];
};

struct CameraPoint {
  s32 x;
  s32 y;
  s32 z;
};

// The fixed camera.
struct FixedCamera {
  struct CameraPoint eye;
  struct CameraPoint target;
  struct CameraPoint *eyeOverride;    // when set, used as the eye
  struct CameraPoint *targetOverride; // when set, used as the target
};

// Each frame, with the scrolling camera: places every object's sprite on the
// screen. An object on or near the screen takes its layer and priority from
// the cell under it; one off screen gives up its VRAM cache entry.
void ObjectSystem_UpdateCamera(void) {
  /* FAKEMATCH: frame local: unused; the native frame keeps room for
     three points above the position. Measured (2026-10-13): without it
     the frame is 44 bytes for 80 (differ 4, the two sp adjustments). */
  struct CameraPoint spare[3];
  s32 i;
  struct CameraState *mapWork;
  s32 viewX;
  s32 viewZ;
  struct ObjectSystemWork *system;
  struct CameraObject *obj;
  struct CameraSprite *sprite;
  s32 *view;
  u32 kind;
  s32 dx;
  s32 dz;
  s32 screenTop;
  struct CameraTile *tile;
  u32 priority;
  s32 scale[2];
  s32 pos[4];
  u32 layerPlusOne;

  mapWork = *(struct CameraState **)(((u8 *)&gObjectSlots) + 12);
  view = &mapWork->x;
  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  system = *(struct ObjectSystemWork **)(((u8 *)&gObjectSlots) + 4);
  // Copy the frame decoder (Render_DecodeFrame) into its heap slot.
  RamCode_LoadSlot(52, Render_DecodeFrame, Render_DecodeFrameCodeSize);
  system->used = 0;
  obj = *(struct CameraObject **)((u8 *)&gObjectSlots);
  for (i = 0; i < 64; i++, obj++) {
    if (obj->active == 0)
      continue;
    if (obj->x != 0 || obj->z != 0) {
      kind = obj->kind & 15;
      if (kind == 0)
        continue;
      if (kind != 1)
        continue;
      if (system->suspended != 0 && obj->held == 0) {
        struct CameraSprite *frozen = obj->sprite;

        Resource_ActivateEntry(frozen->resource);
        frozen->dirty = kind;
        continue;
      }
      dx = obj->x - viewX;
      dz = obj->z - viewZ;
      screenTop = dz - obj->y;
      sprite = obj->sprite;
      if (dx > -0x200000 && dx < 0x1100000 && screenTop > -0x200000 &&
          screenTop < 0xe00000) {
        // The cell under the object.
        tile = &mapWork->layers[obj->layer]
                    .tiles[(obj->x >> 20) + ((obj->z >> 20) << 7)];
        if (obj->flags & 1) {
          priority = tile->priority;
          if (priority != 0) {
            sprite->priority = priority;
            sprite->secondPriority = priority;
          }
        }
        layerPlusOne = tile->layer;
        if (layerPlusOne != 0)
          obj->layer = layerPlusOne - 1;
        scale[0] = Iwram_MulQ16(obj->scaleX, sprite->scale);
        scale[1] = Iwram_MulQ16(obj->scaleY, sprite->scale);
        pos[0] = dx;
        pos[1] = obj->y;
        pos[2] = dz;
        pos[3] = obj->height;
        if (obj->flags & 2) {
          pos[1] += 0xfec00000; // -20 pixels
          pos[2] += 0xfec00000;
          pos[3] += 0xfec00000;
        }
        if (obj->flags & 4) {
          pos[1] += 0x1400000; // +20 pixels
          pos[2] += 0x1400000;
          pos[3] += 0x1400000;
        }
        Render_ApplyProjectedPlacement(sprite, pos, scale, obj->angle);
        continue;
      }
      if (obj->held == 0) {
        if (!(sprite->flags1d & 1)) {
          Resource_ActivateEntry(sprite->resource);
          sprite->dirty = 1;
        }
      }
    } else {
      kind = obj->kind & 15;
      if (kind == 1) {
        sprite = obj->sprite;
        if (obj->held == 0 && !(sprite->flags1d & 1)) {
          Resource_ActivateEntry(sprite->resource);
          sprite->dirty = kind;
        }
      }
    }
  }
  Runtime_ReleaseHeapBlock(52);
}

// Returns 1.
s32 ObjectCamera_ReturnTrue(void) { return 1; }

// Each frame, with the fixed camera: places every object's sprite, from the
// 64th object down, facing them by the angle from the eye to the target.
// While battle commands are chosen (flag 363, set by
// BattlePresentation_BuildActions) it uses the fixed view matrix and 1.0 scale.
void ObjectSystem_UpdateCameraFixed(void) {
  s32 i;
  struct FixedCamera *camera;
  struct ObjectSystemWork *system;
  struct CameraPoint *eye;
  struct CameraPoint *target;
  struct CameraObject *obj;
  s32 angle;
  s32 p;
  s32 kind;
  s32 *pos;
  s32 unitScale;
  void **sprites;
  void *sprite;
  s32 groupScale[2];
  s32 scale[2];
  /* FAKEMATCH: frame local: unused; the native frame keeps room for two
     points above the scales. Measured (2026-10-13): without it the frame
     is 28 bytes for 52 (differ 4, the two sp adjustments). */
  struct CameraPoint spare[2];

  camera = *(struct FixedCamera **)(((u8 *)&gObjectSlots) + 0x1c);
  system = *(struct ObjectSystemWork **)(((u8 *)&gObjectSlots) + 4);
  RamCode_LoadSlot(52, Render_DecodeFrame, Render_DecodeFrameCodeSize);
  eye = &camera->eye;
  target = &camera->target;
  if (camera->eyeOverride != NULL)
    eye = camera->eyeOverride;
  if (camera->targetOverride != NULL)
    target = camera->targetOverride;
  angle = (s16)ArcTan2((eye->x - target->x) >> 16, (eye->z - target->z) >> 16);
  system->used = 0;
  Render_ResetTransformState();
  if (GameFlag_TestFar(363)) {
    angle += 0xffffe000;
    Iwram_TransformMatrix(gCamera_FixedViewMatrix);
    Graphics_PrepareTransferAndRun(eye, target);
  } else {
    Graphics_PrepareTransferInIwramWork(eye, target);
  }
  obj = *(struct CameraObject **)((u8 *)&gObjectSlots);
  obj += 63;
  unitScale = 0x10000;
  for (i = 63; i >= 0; i--, obj--) {
    if (obj->active == 0)
      continue;
    kind = obj->kind & 15;
    switch (kind) {
    case 1:
      pos = &obj->x;
      scale[0] = obj->scaleX;
      scale[1] = obj->scaleY;
      sprite = obj->sprite;
      if (GameFlag_TestFar(363)) {
        scale[0] = unitScale;
        scale[1] = unitScale;
      }
      Render_PlaceProjectedSprite(sprite, pos, scale, obj->angle + angle,
                                  obj->layer);
      break;
    case 2:
      groupScale[0] = obj->scaleX;
      groupScale[1] = obj->scaleY;
      if (GameFlag_TestFar(363)) {
        groupScale[0] = unitScale;
        groupScale[1] = unitScale;
      }
      sprites = obj->sprite;
      for (p = 3; p >= 0; p--) {
        sprite = *sprites++;
        if (sprite != NULL)
          Render_PlaceProjectedSprite(sprite, &obj->x, groupScale,
                                      obj->angle + angle, obj->layer);
      }
      break;
    case 0:
      break;
    }
  }
  Runtime_ReleaseHeapBlock(52);
}

// Object script commands that move the cursor and set byte54.

// SC_BASE: restarts the script at the next word, and returns 1 to run on.
s32 Script_AdvancePastCursor(struct ScriptInterpreter *interpreter) {
  s32 zero;

  // The word after the command becomes the script's start.
  interpreter->script = interpreter->script + interpreter->cursor + 1;
  zero = 0;
  interpreter->cursor = zero;
  return 1;
}

// SC_B54_OFF: clears byte54 and moves on to the next command.
s32 Script_ClearByte54(struct ScriptInterpreter *interpreter) {
  interpreter->byte54 = 0;
  interpreter->cursor++;
  return 1;
}

// SC_B54_ON: sets byte54 and moves on to the next command.
s32 Script_SetByte54(struct ScriptInterpreter *interpreter) {
  interpreter->byte54 = 1;
  interpreter->cursor++;
  return 1;
}

// Waits until the object has arrived at its target (Object_IsTargetUnset),
// at most 600 frames.
void Script_WaitForEventTimeout(s32 object) {
  s32 frames;

  frames = 0;
  while (frames <= 0x257 &&
         Object_IsTargetUnset((struct ObjectRuntime *)object) == 0) {
    WaitFrames(1);
    frames++;
  }
}

// Returns 1 when the object has no movement target: none of its target
// coordinates set, or for an object moved by the ground (any motion flag)
// neither its target x nor z.
s32 Object_IsTargetUnset(struct ObjectRuntime *object) {
  s32 first;
  s32 second;

  if (object->flags == 0) {
    second = object->targetX;
    if (second != 0x80000000)
      return 0;
    first = object->targetY;
  } else {
    first = object->targetX;
    second = 0x80000000;
  }
  if (first != second || object->targetZ != first)
    return 0;
  return 1;
}

#define TARGET_UNSET ((s32)0x80000000)

typedef s32 (*ScriptCommandFn)(struct ScriptMotionObject *);

extern ScriptCommandFn gScriptObject_CommandTable[];

// value squared.
static __inline__ s32 Square(s32 value) { return value * value; }

// Advances all 64 field objects by one frame, as Object_UpdateAll does for
// the eight script objects, with the field's extras: free flight in 3D,
// terrain following, floating, and solid objects that refuse to overlap.
void Object_UpdateAllThumb(void) {
  struct ScriptMotionObject *obj;
  const struct ScriptMotionWords *script;
  s32 i;
  s32 x;
  s32 y;
  s32 z;
  s32 ground;
  s32 arrived;
  s32 pos[3];
  s32 dx;
  s32 dy;
  s32 dz;
  s32 vx;
  s32 vy;
  s32 vz;
  s32 dist;
  s32 len;
  s32 ratio;
  s32 slowed;
  s32 maxClimb;
  s32 turn;
  u32 command;
  u32 phase;

  obj = gObjectSlots;
  for (i = 63; i >= 0; i--, obj++) {
    arrived = 0;

    script = obj->script;
    if (script == NULL)
      continue;

    if (obj->hook != NULL) {
      obj->hook(obj);
      script = obj->script;
    }
    if (script == NULL)
      continue;
    if (obj->paused != 0)
      continue;

    // Count down a wait, or run commands until one yields for this frame.
    if (obj->wait != 0) {
      obj->wait--;
    } else {
      for (;;) {
        command = script->word[(s16)obj->scriptCursor];
        if (command <= 63) {
          if (gScriptObject_CommandTable[command](obj) == 0)
            break;
        } else {
          obj->scriptCursor++;
        }
        script = obj->script;
      }
      if (obj->script == NULL)
        continue;
    }

    x = obj->x;
    y = obj->y;
    z = obj->z;

    if (obj->frozen == 0) {
      // Free flight (no motion flags): steer in 3D towards the target,
      // capped at the speed limit, or brake when there is none.
      if (obj->motionFlags == 0) {
        if (obj->targetX != TARGET_UNSET) {
          dx = (obj->targetX - x) / 65536;
          dy = (obj->targetY - y) / 65536;
          dz = (obj->targetZ - z) / 65536;
          dist = Iwram_Sqrt(Square(dx) + Square(dy) + Square(dz));
          if (dist == 0) {
            x = obj->targetX;
            y = obj->targetY;
            z = obj->targetZ;
          } else {
            ratio = Iwram_RatioMulQ14(dist << 16, obj->acceleration);
            obj->velocityX += dx * ratio;
            obj->velocityY += dy * ratio;
            obj->velocityZ += dz * ratio;
            vx = obj->velocityX;
            vy = obj->velocityY;
            vz = obj->velocityZ;
            len = FixedSqrt(Iwram_MulQ16(vx, vx) + Iwram_MulQ16(vy, vy) +
                            Iwram_MulQ16(vz, vz));
            if (len > obj->speedLimit) {
              ratio = Iwram_RatioMulQ14(len, obj->speedLimit);
              obj->velocityX = Iwram_MulQ16(vx, ratio);
              obj->velocityY = Iwram_MulQ16(vy, ratio);
              obj->velocityZ = Iwram_MulQ16(vz, ratio);
            }
          }
        } else {
          dx = obj->velocityX;
          dy = obj->velocityY;
          dz = obj->velocityZ;
          len = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                          Iwram_MulQ16(dz, dz));
          if (len != 0) {
            slowed = len - obj->acceleration;
            if (slowed < 0)
              slowed = 0;
            ratio = Iwram_RatioMulQ14(len, slowed);
            obj->velocityX = Iwram_MulQ16(dx, ratio);
            obj->velocityY = Iwram_MulQ16(dy, ratio);
            obj->velocityZ = Iwram_MulQ16(dz, ratio);
          } else {
            obj->velocityX = 0;
            obj->velocityY = 0;
            obj->velocityZ = 0;
          }
        }
      } else {
        // On the ground: steer in the XZ plane the same way.
        if (obj->targetX != TARGET_UNSET) {
          dx = (obj->targetX - x) / 65536;
          dz = (obj->targetZ - z) / 65536;
          dist = Iwram_Sqrt(Square(dx) + Square(dz)) << 16;
          if (dist <= 0x00ffffff) {
            dx = obj->targetX - x;
            dz = obj->targetZ - z;
            dist = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
          }
          if (dist == 0) {
            x = obj->targetX;
            z = obj->targetZ;
          } else {
            ratio = Iwram_RatioMulQ14(dist, obj->acceleration);
            obj->velocityX += Iwram_MulQ16(dx, ratio);
            obj->velocityZ += Iwram_MulQ16(dz, ratio);
            vx = obj->velocityX;
            vz = obj->velocityZ;
            len = FixedSqrt(Iwram_MulQ16(vx, vx) + Iwram_MulQ16(vz, vz));
            if (len > obj->speedLimit) {
              ratio = Iwram_RatioMulQ14(len, obj->speedLimit);
              obj->velocityX = Iwram_MulQ16(vx, ratio);
              obj->velocityZ = Iwram_MulQ16(vz, ratio);
            }
          }
        } else {
          dx = obj->velocityX;
          dz = obj->velocityZ;
          len = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
          if (len != 0) {
            slowed = len - obj->acceleration;
            if (slowed < 0)
              slowed = 0;
            ratio = Iwram_RatioMulQ14(len, slowed);
            obj->velocityX = Iwram_MulQ16(dx, ratio);
            obj->velocityZ = Iwram_MulQ16(dz, ratio);
          } else {
            obj->velocityX = 0;
            obj->velocityZ = 0;
          }
        }

        // Follow the terrain: take the ground height where the object is
        // going and move with its change, unless the object is 4 or more
        // pixels above the ground. Unless bit 4 is set, a climb or drop also
        // slows it, by three times the change (at most half its
        // acceleration).
        if ((obj->motionFlags & 1) != 0) {
          vx = x + obj->velocityX;
          vz = z + obj->velocityZ;
          ground = Map_GetTerrainHeight(obj->terrainId, vx, vz);
          vy = ground - obj->terrainHeight;
          if (ground - y > -0x40000)
            y += vy;
          if (vy < 0)
            vy = -vy;
          maxClimb = obj->acceleration / 2;
          if (vy > maxClimb)
            vy = maxClimb;
          vy *= 3;
          if (vy != 0 && (obj->motionFlags & 0x10) == 0) {
            dx = obj->velocityX;
            dy = obj->velocityY;
            dz = obj->velocityZ;
            len = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                            Iwram_MulQ16(dz, dz));
            if (len != 0) {
              slowed = len - vy;
              if (slowed < 0)
                slowed = 0;
              ratio = Iwram_RatioMulQ14(len, slowed);
              obj->velocityX = Iwram_MulQ16(dx, ratio);
              obj->velocityY = Iwram_MulQ16(dy, ratio);
              obj->velocityZ = Iwram_MulQ16(dz, ratio);
            }
          }
          obj->terrainHeight = ground;
        }

        // Gravity, and a bounce that loses energy on landing.
        if ((obj->motionFlags & 2) != 0) {
          ground = obj->terrainHeight;
          if (y > ground) {
            obj->velocityY -= obj->gravity;
          } else if (obj->velocityY < 0) {
            y = ground;
            obj->velocityY =
                -Iwram_MulQ16(obj->velocityY, obj->vertical.bounce);
            if ((obj->velocityY < 0 ? -obj->velocityY : obj->velocityY) <=
                obj->gravity)
              obj->velocityY = 0;
          }
        }

        // Float: bob up and down on a triangle wave (bit 3 makes it bigger).
        if ((obj->motionFlags & 4) != 0) {
          phase = (obj->vertical.phase & 0x3f);
          if ((obj->motionFlags & 8) != 0)
            obj->velocityY =
                gScriptObject_CycleTable[phase >> 1] * obj->gravity / 16;
          else
            obj->velocityY =
                gScriptObject_CycleTable[phase >> 1] * obj->gravity / 64;
          obj->vertical.phase++;
        }
      }
    }

    x += obj->velocityX;
    y += obj->velocityY;
    z += obj->velocityZ;

    // A solid object does not move into another; count the refusals.
    if ((obj->collisionFlags & 0x80) != 0) {
      pos[0] = x;
      pos[1] = y;
      pos[2] = z;
      if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)obj, pos) !=
          0) {
        obj->refused++;
        continue;
      }
      obj->refused = 0;
    }

    // Arrived: the watched axis reached the target, or crossed it.
    switch (obj->arrivalAxis) {
    case ARRIVAL_AXIS_X:
      if (x == obj->targetX ||
          ((obj->x - obj->targetX) ^ (x - obj->targetX)) < 0)
        arrived = 1;
      break;
    case ARRIVAL_AXIS_Y:
      if (y == obj->targetY ||
          ((obj->y - obj->targetY) ^ (y - obj->targetY)) < 0)
        arrived = 1;
      break;
    case ARRIVAL_AXIS_Z:
      if (z == obj->targetZ ||
          ((obj->z - obj->targetZ) ^ (z - obj->targetZ)) < 0)
        arrived = 1;
      break;
    default:
      break;
    }

    if (arrived != 0) {
      if (obj->snapToTarget != 0) {
        x = obj->targetX;
        obj->velocityX = 0;
        z = obj->targetZ;
        obj->velocityZ = 0;
        if (obj->motionFlags == 0) {
          y = obj->targetY;
          obj->velocityY = 0;
        }
      }
      obj->targetX = TARGET_UNSET;
      obj->targetY = TARGET_UNSET;
      obj->targetZ = TARGET_UNSET;
      obj->arrivalAxis = 0;
    }

    obj->x = x;
    obj->y = y;
    obj->z = z;

    // Turn to face the direction of travel, at most 0x1000 a frame.
    if ((obj->steeringFlags & 1) != 0) {
      x = obj->velocityX;
      z = obj->velocityZ;
      if (x != 0 || z != 0) {
        s32 limit;

        turn = (s16)(ArcTan2(z, x) - obj->facing);
        limit = 0x1000;
        if (turn > limit)
          turn = limit;
        limit = -0x1000;
        if (turn < limit)
          turn = limit;
        obj->facing += turn;
      }
    }
  }
}

// Places obj at (x, y, z), in 16.16 pixels, clears its movement target and
// stops it.
void Object_SetPositionAndResetMotion(struct ObjectRuntime *obj, s32 x, s32 y,
                                      s32 z) {
  obj->z = z;
  obj->targetX = TARGET_UNSET;
  obj->targetY = TARGET_UNSET;
  obj->targetZ = TARGET_UNSET;
  obj->x = x;
  obj->y = y;
  obj->velocityX = 0;
  obj->velocityY = 0;
  obj->velocityZ = 0;
}

// Sends an object towards (x, y, z), in 16.16 pixels. Within a pixel it
// arrives at once. Unless it snaps to its target, the target is pulled in
// by half its braking distance (speed limit squared over acceleration), or
// to halfway when that is further, as the object coasts on. Picks the
// axis to watch for arrival: the one with the furthest to go (y only for a
// free-flying object).
void Object_SetMoveTarget(struct ObjectRuntime *object, s32 x, s32 y, s32 z) {
  s32 dx;
  s32 dist;
  s32 dz;
  s32 total;
  u8 *axis;

  dx = (x - object->x) / 0x10000;
  dist = (y - object->y) / 0x10000;
  dz = (z - object->z) / 0x10000;
  total = dz * dz;
  dist = Iwram_Sqrt(dx * dx + dist * dist + total) << 16;
  if (dist < 0x100000) {
    dx = x - object->x;
    dist = y - object->y;
    dz = z - object->z;
    dist = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dist, dist) +
                     Iwram_MulQ16(dz, dz));
  }
  if (dist < 0x10000) {
    object->x = x;
    object->y = y;
    object->z = z;
    object->targetX = TARGET_UNSET;
    object->targetY = TARGET_UNSET;
    object->targetZ = TARGET_UNSET;
    return;
  }
  if (object->snapToTarget == 0) {
    s32 brake;

    brake =
        Iwram_RatioMulQ14(object->acceleration,
                          Iwram_MulQ16(object->speedLimit, object->speedLimit));
    if (dist > brake)
      brake = dist - brake / 2;
    else
      brake = dist / 2;
    dist = Iwram_RatioMulQ14(dist, brake);
    x = object->x + Iwram_MulQ16(x - object->x, dist);
    y = object->y + Iwram_MulQ16(y - object->y, dist);
    z = object->z + Iwram_MulQ16(z - object->z, dist);
  }
  object->targetX = x;
  object->targetY = y;
  object->targetZ = z;
  dx = x - object->x;
  dist = y - object->y;
  dz = z - object->z;
  axis = &object->arrivalAxis;
  *axis = ARRIVAL_AXIS_X;
  if ((dx < 0 ? -dx : dx) < (dz < 0 ? -dz : dz)) {
    *axis = ARRIVAL_AXIS_Z;
    dx = dz;
  }
  if (object->flags == 0) {
    if (dx < 0)
      dx = -dx;
    if (dx < (dist < 0 ? -dist : dist))
      *axis = ARRIVAL_AXIS_Y;
  }
}

// Copies Object_UpdateAll (ARM code) to the heap, runs it once and frees it.
void Object_RunUpdateAllFromHeap(void) {
  void (*routine)(void);

  RamCode_Load(routine, Object_UpdateAll, Object_UpdateAllCodeSize);
  routine();
  MemFree(routine);
}

// Advances the first 14 objects' motion by one frame, without running their
// scripts: steering towards a target or braking, gravity and bouncing,
// arrival, and turning to face the direction of travel.
void Object_UpdateAllMotion(void) {
  struct ScriptMotionObject *obj;
  s32 cnt;
  s32 y;
  s32 arrived;
  s32 x;
  s32 z;
  s32 dx;
  s32 dz;
  s32 vx;
  s32 dist;
  s32 ratio;
  s32 turn;

  obj = gObjectSlots;
  for (cnt = 13; cnt >= 0; cnt--, obj++) {
    if (obj->script == 0)
      continue;
    x = obj->x;
    y = obj->y;
    z = obj->z;
    if (obj->frozen == 0) {
      arrived = 0;
      if (obj->targetX != TARGET_UNSET) {
        dx = (obj->targetX - x) / 65536;
        dz = (obj->targetZ - z) / 65536;
        dist = Iwram_Sqrt(dx * dx + dz * dz) << 16;
        if (dist <= 0xffffff) {
          dx = obj->targetX - x;
          dz = obj->targetZ - z;
          dist = Iwram_Sqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz)) << 8;
        }
        if (dist == 0) {
          x = obj->targetX;
          z = obj->targetZ;
        } else {
          ratio = Iwram_RatioMulQ14(dist, obj->acceleration);
          vx = obj->velocityX + Iwram_MulQ16(dx, ratio);
          obj->velocityX = vx;
          dx = obj->velocityZ + Iwram_MulQ16(dz, ratio);
          obj->velocityZ = dx;
          dist = Iwram_Sqrt(Iwram_MulQ16(vx, vx) + Iwram_MulQ16(dx, dx)) << 8;
          if (dist > obj->speedLimit) {
            ratio = Iwram_RatioMulQ14(dist, obj->speedLimit);
            obj->velocityX = Iwram_MulQ16(vx, ratio);
            obj->velocityZ = Iwram_MulQ16(dx, ratio);
          }
        }
      } else {
        dx = obj->velocityX;
        dz = obj->velocityZ;
        if ((dx | dz) != 0) {
          dist = Iwram_Sqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz)) << 8;
          if (dist != 0) {
            if (dist - obj->acceleration < 0) {
              obj->velocityX = 0;
              obj->velocityZ = 0;
            } else {
              ratio = Iwram_RatioMulQ14(dist, dist - obj->acceleration);
              obj->velocityX = Iwram_MulQ16(dx, ratio);
              obj->velocityZ = Iwram_MulQ16(dz, ratio);
            }
          } else {
            obj->velocityX = 0;
            obj->velocityZ = 0;
          }
        }
      }
      if (obj->motionFlags & 2) {
        if (y > obj->terrainHeight) {
          obj->velocityY -= obj->gravity;
        } else if (obj->velocityY < 0) {
          y = obj->terrainHeight;
          obj->velocityY = -Iwram_MulQ16(obj->velocityY, obj->vertical.bounce);
          if ((obj->velocityY < 0 ? -obj->velocityY : obj->velocityY) <=
              obj->gravity)
            obj->velocityY = 0;
        }
      }
    }
    x += obj->velocityX;
    y += obj->velocityY;
    z += obj->velocityZ;
    if (obj->arrivalAxis != 0)
      switch (obj->arrivalAxis) {
      case 16:
        if (x == obj->targetX ||
            ((obj->x - obj->targetX) ^ (x - obj->targetX)) < 0)
          arrived = 1;
        break;
      case 17:
        if (y == obj->targetY ||
            ((obj->y - obj->targetY) ^ (y - obj->targetY)) < 0)
          arrived = 1;
        break;
      case 18:
        if (z == obj->targetZ ||
            ((obj->z - obj->targetZ) ^ (z - obj->targetZ)) < 0)
          arrived = 1;
        break;
      }
    if (arrived) {
      if (obj->snapToTarget) {
        obj->velocityX = 0;
        obj->velocityZ = 0;
        x = obj->targetX;
        z = obj->targetZ;
        if (obj->motionFlags == 0) {
          y = obj->targetY;
          obj->velocityY = 0;
        }
      }
      obj->targetX = TARGET_UNSET;
      obj->targetY = TARGET_UNSET;
      obj->targetZ = TARGET_UNSET;
      obj->arrivalAxis = 0;
    }
    obj->x = x;
    obj->y = y;
    obj->z = z;
    if (obj->steeringFlags & 1) {
      x = obj->velocityX;
      z = obj->velocityZ;
      if (x != 0 || z != 0) {
        turn = (s16)(ArcTan2(z, x) - obj->facing);
        if (turn > 0x1000)
          turn = 0x1000;
        if (turn < -0x1000)
          turn = -0x1000;
        obj->facing += turn;
      }
    }
  }
}

// SC_WAIT(n): waits n frames; this frame counts as the first. Returns 0 to
// end the object's commands for this frame.
s32 Script_StoreLookupResult(struct ScriptInterpreter *interpreter) {
  s16 cursor = interpreter->cursor;

  interpreter->lookupResult = interpreter->script[cursor + 1] - 1;
  interpreter->cursor += 2;
  return 0;
}

// SC_SYNC: waits until the object arrives at its target, or until a solid
// object has blocked it for 60 frames (delay is the blocked count, which
// the update raises). Returns 1 (run on) once it has, 0 (wait) until then.
s32 Script_WaitForEvent(struct ScriptInterpreter *interpreter) {
  if (interpreter->delay > 0x3B) {
    interpreter->delay = 0;
    interpreter->cursor++;
    return 1;
  }
  if (Object_IsTargetUnset((struct ObjectRuntime *)interpreter) != 0) {
    interpreter->cursor++;
    return 1;
  }
  return 0;
}

// SC_CALL(fn): calls fn with the object. A non-zero result waits and calls
// it again next frame; zero moves on, past the command unless fn moved the
// cursor itself.
s32 Script_InvokeCallback(struct ScriptInterpreter *interpreter) {
  s16 initial = interpreter->cursor;
  ScriptCommand callback = (ScriptCommand)interpreter->script[initial + 1];

  if (callback(interpreter) != 0)
    return 0;
  if (interpreter->cursor == initial)
    interpreter->cursor = interpreter->cursor + 2;
  return 1;
}

// Returns the cursor just past the script's label key (SC_LABEL or SC_TO,
// matched without the bit that tells them apart), searching its first 1024
// words, or 0 (the start) when key is 0 or not found. Clears the wait.
s32 Script_FindLabel(struct ScriptInterpreter *interpreter, u32 key) {
  u32 *entries;
  s32 index;
  s16 *field = &interpreter->lookupResult;
  s32 zero = 0;

  *field = zero;
  if (key == 0) {
    return 0;
  }

  key &= 0xBFFFFFFF;
  entries = (u32 *)interpreter->script;
  index = 0;
  do {
    if (*entries++ == key) {
      return index + 1;
    }
    index++;
  } while (index <= 0x3FF);
  return 0;
}

// SC_LOOP(n, to): jumps to label to until it has done so n - 1 times, then
// moves on; with n = SC_FOREVER it always jumps.
s32 Script_RepeatOrJump(struct ScriptInterpreter *interpreter) {
  const s32 *arguments;
  s32 repeatLimit;
  s32 jumpKey;
  u8 *repeatCount;
  s32 nextCount;

  arguments = &interpreter->script[interpreter->cursor + 1];
  repeatLimit = *arguments++;
  jumpKey = *arguments;
  if (repeatLimit == 0xFFFF) {
    interpreter->cursor = Script_FindLabel(interpreter, jumpKey);
  } else {
    repeatCount = &interpreter->repeatCount;
    nextCount = *repeatCount + 1;
    *repeatCount = nextCount;
    if ((u8)nextCount < (s16)repeatLimit) {
      interpreter->cursor = Script_FindLabel(interpreter, jumpKey);
    } else {
      *repeatCount = 0;
      interpreter->cursor += 3;
    }
  }
  return 1;
}

// SC_JUMP(to): jumps to label to.
s32 Script_JumpToLabel(struct ScriptInterpreter *interpreter) {
  interpreter->cursor = Script_FindLabel(
      interpreter, interpreter->script[interpreter->cursor + 1]);
  return 1;
}

// SC_IF(to): jumps to label to when the last test was true.
u32 Script_JumpIfTrue(struct ScriptInterpreter *interpreter) {
  s32 index = interpreter->cursor;
  const s32 *table = interpreter->script;
  u32 value = table[index + 1];

  if (interpreter->conditionResult != 0) {
    interpreter->cursor = Script_FindLabel(interpreter, value);
  } else {
    interpreter->cursor += 2;
  }
  return 1;
}

// SC_IFNOT(to): jumps to label to when the last test was false.
u32 Script_JumpIfFalse(struct ScriptInterpreter *interpreter) {
  s32 index = interpreter->cursor;
  const s32 *table = interpreter->script;
  u32 value = table[index + 1];

  if (interpreter->conditionResult == 0) {
    interpreter->cursor = Script_FindLabel(interpreter, value);
  } else {
    interpreter->cursor += 2;
  }
  return 1;
}

// SC_END: hands the object back to the idle script. Returns 0.
s32 Script_LoadMainScript(struct ScriptInterpreter *interpreter) {
  s32 result;

  interpreter->script = gScript_MainScript;
  interpreter->cursor = (result = 0);
  return result;
}

// SC_FLAG_CHECK(f): tests game flag f.
s32 Script_TestFlag(struct ScriptInterpreter *interpreter) {
  interpreter->conditionResult =
      GameFlag_TestFar(interpreter->script[interpreter->cursor + 1]);
  interpreter->cursor += 2;
  return 1;
}

// Defines a flag command: stores the flag's current value in conditionResult,
// then changes the flag with change.
#define DEFINE_FLAG_AND_TEST(name, change)                                     \
  s32 name(struct ScriptInterpreter *interpreter) {                            \
    s32 value;                                                                 \
                                                                               \
    value = interpreter->script[interpreter->cursor + 1];                      \
    interpreter->conditionResult = GameFlag_TestFar(value);                    \
    change(value);                                                             \
    interpreter->cursor += 2;                                                  \
    return 1;                                                                  \
  }

// SC_FLAG_ON(f) and SC_FLAG_OFF(f): test game flag f, then set or clear it.
DEFINE_FLAG_AND_TEST(Script_SetFlagAndTest, GameFlag_SetBitFar)
DEFINE_FLAG_AND_TEST(Script_ClearFlagAndTest, GameFlag_ClearBitFar)

// SC_FLAG_REV(f): tests game flag f, then flips it.
s32 Script_ToggleFlagAndTest(struct ScriptInterpreter *interpreter) {
  s32 value;
  s32 result;

  value = interpreter->script[interpreter->cursor + 1];
  result = GameFlag_TestFar(value);
  interpreter->conditionResult = result;
  if ((result << 0x18) == 0x01000000) {
    GameFlag_ClearBitFar(value);
  } else {
    GameFlag_SetBitFar(value);
  }
  interpreter->cursor += 2;
  return 1;
}

// SC_ARG(v): switches the object's sprites to animation v.
s32 Script_ApplyObjectArgument(struct ScriptInterpreter *interpreter) {
  ObjectDispatch_ApplyArgumentToChildren(
      interpreter, interpreter->script[interpreter->cursor + 1]);
  interpreter->cursor += 2;
  return 1;
}

// SC_FREE: releases the object. Returns 0.
s32 ObjectDispatch_RunHookAndReturnZero(struct ScriptInterpreter *interpreter) {
  ObjectDispatch_Release((struct DispatchObject *)interpreter);
  return 0;
}

// SC_NOP(v): does nothing with its operand.
s32 Script_SkipCommand(struct ScriptInterpreter *interpreter) {
  interpreter->cursor += 2;
  return 1;
}

// SC_SE(n): sends sound command n (a sound effect).
s32 Script_PlayAudioCue(struct ScriptInterpreter *interpreter) {
  AudioCommand_PlayFar(interpreter->script[interpreter->cursor + 1]);
  interpreter->cursor += 2;
  return 1;
}

// Returns -1 when an object at position values would overlap a solid
// object (bit 0 of unk59), each radius less 2 pixels; otherwise 0.
s32 ScriptObject_CheckOverlap(struct ScriptObjectEntry *object, s32 *values) {
  s32 index;
  u8 *flags;
  struct ObjectRuntime *entry = gObjectSlots;

  flags = &entry->unk59;
  for (index = 0; index < 64; index++, entry++, flags += sizeof(*entry)) {
    if (entry->script != NULL && (1 & *flags) &&
        entry != (struct ObjectRuntime *)object) {
      if (Runtime_CheckRadiusOverlap(
              &entry->x, ((struct FieldActor *)entry)->radius - 2, values,
              ((struct FieldActor *)object)->radius - 2) >= 0)
        return -1;
    }
  }
  return 0;
}

struct ScriptObjectEntry *
// Returns the first solid object that an object at position values would
// overlap, as ScriptObject_CheckOverlap tests, or NULL.
ScriptObject_FindOverlappingEntry(struct ScriptObjectEntry *object,
                                  s32 *values) {
  s32 index;
  u8 *flags;
  struct ObjectRuntime *entry = gObjectSlots;

  flags = &entry->unk59;
  for (index = 0; index < 64; index++, entry++, flags += sizeof(*entry)) {
    if (entry->script != NULL && (1 & *flags) &&
        entry != (struct ObjectRuntime *)object) {
      if (Runtime_CheckRadiusOverlap(
              &entry->x, ((struct FieldActor *)entry)->radius - 2, values,
              ((struct FieldActor *)object)->radius - 2) >= 0)
        return (struct ScriptObjectEntry *)entry;
    }
  }
  return NULL;
}

// Passes the three words after the command to place as a position.
static __inline__ s32 ApplyPosition(struct ScriptInterpreter *interpreter,
                                    void (*place)(struct ObjectRuntime *object,
                                                  s32 x, s32 y, s32 z)) {
  s32 first;
  s32 second;
  s32 third;
  const s32 *argument;

  argument = interpreter->script + interpreter->cursor;
  argument++;
  first = *argument;
  argument++;
  second = *argument;
  argument++;
  third = *argument;
  place((struct ObjectRuntime *)interpreter, first, second, third);
  interpreter->cursor += 4;
  return 1;
}

// SC_POS(x, y, z): places the object at (x, y, z) and stops it.
s32 Script_SetPositionAndResetMotion(struct ScriptInterpreter *interpreter) {
  return ApplyPosition(interpreter, Object_SetPositionAndResetMotion);
}

// SC_MOVE(x, y, z): sends the object towards (x, y, z).
s32 Script_ApplyAbsolutePosition(struct ScriptInterpreter *interpreter) {
  return ApplyPosition(interpreter, Object_SetMoveTarget);
}

// SC_MOVE_BY(x, y, z): sends the object towards its position plus (x, y, z).
s32 Script_ApplyRelativePosition(struct ScriptObjectRuntime *object) {
  u8 *entry = (u8 *)(object->script + (s16)object->scriptCursor);
  s32 *cursor = (s32 *)(entry + 4);
  s32 first = *cursor++;
  s32 second = *cursor++;
  s32 third = *cursor;

  Object_SetMoveTarget((struct ObjectRuntime *)object, object->x + first,
                       object->y + second, object->z + third);
  object->scriptCursor += 4;
  return 1;
}

// SC_FACE_LINK: stores the angle from the object to its linked object in
// scriptValue.
s32 Script_StoreAngleToLinkedObject(struct ScriptObjectRuntime *object) {
  struct ScriptObjectRuntime *target;

  target = object->linkedObject;
  object->scriptValue =
      ArcTan2((target->z - object->z), (target->x - object->x));
  object->scriptCursor++;
  return 1;
}

// SC_MOVE_LINK: sends the object towards its linked object.
s32 Script_ApplyLinkedObjectPosition(struct ScriptObjectRuntime *object) {
  struct ScriptObjectRuntime *target;

  target = object->linkedObject;
  Object_SetMoveTarget((struct ObjectRuntime *)object, target->x, target->y,
                       target->z);
  object->scriptCursor++;
  return 1;
}

// SC_MOVE_LOCAL(a, b): sends the object towards its position plus an offset
// that is never filled in; the operands are skipped unread.
s32 Script_ApplyLocalOffsetPosition(struct ScriptObjectRuntime *object) {
  s32 offset[3];

  // BUG: offset is never set, so the target is the position plus whatever
  // the stack held.
#ifdef UBFIX
  offset[0] = offset[1] = offset[2] = 0;
#endif
  Object_SetMoveTarget((struct ObjectRuntime *)object, object->x + offset[0],
                       object->y + offset[1], object->z + offset[2]);
  object->scriptCursor += 3;
  return 1;
}

struct MapScrollBounds {
  u8 unk00[0xec];
  s32 left;
  s32 top;
  s32 right;
  s32 bottom;
};

// SC_CAMERA: keeps the camera object on its leader (linkedObject), clamped
// to 120 pixels inside the map's left and right edges and a band above and
// below the leader. It closes an eighth of the distance a frame, at most
// its speed limit, and a quarter of a large height difference; when action
// is set it jumps there at once.
s32 Camera_FollowLeaderInBounds(struct ObjectRuntime *obj) {
  struct ObjectRuntime *leader;
  struct MapScrollBounds *bounds;
  s32 minX;
  s32 minZ;
  s32 maxX;
  s32 maxZ;
  s32 x;
  s32 y;
  s32 z;
  s32 dz;
  s32 dy;
  s32 dx;
  s32 dist;
  s32 step;

  bounds = gMapWork[0];
  leader = obj->linkedObject;
  minX = bounds->left + 0x780000;
  minZ = bounds->top + leader->y + 0x600000;
  maxX = bounds->right - 0x780000;
  maxZ = bounds->bottom + leader->y - 0x400000;
  obj->flags = 0;
  if (leader != NULL && leader->script != NULL) {
    x = leader->x;
    y = leader->y;
    z = leader->z;
    obj->targetX = TARGET_UNSET;
    obj->targetY = TARGET_UNSET;
    obj->targetZ = TARGET_UNSET;
    if (x < minX)
      x = minX;
    if (z < minZ)
      z = minZ;
    if (x > maxX)
      x = maxX;
    if (z > maxZ)
      z = maxZ;
    if (obj->action != 0) {
      obj->x = x;
      obj->y = y;
      obj->z = z;
    } else {
      // The distance in the XZ plane: dy is computed as in
      // Object_SetMoveTarget but not used for the distance.
      dx = (x - obj->x) / 0x10000;
      /* FAKEMATCH: dead code: this dy is overwritten below and the compiler
       * drops it, but its references rank y above the bounds in global
       * allocation, keeping y in r5 as native; without it 114 rows differ in
       * the English build (y in r9). The form is the sibling
       * Object_SetMoveTarget's three-axis delta. */
      dy = (y - obj->y) / 0x10000;
      dz = (z - obj->z) / 0x10000;
      dist = Iwram_Sqrt(dx * dx + dz * dz) << 16;
      dx = x - obj->x;
      dy = y - obj->y;
      dz = z - obj->z;
      if (dist < 0x400000)
        dist = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
      if ((step = dist / 8) > obj->speedLimit)
        step = obj->speedLimit;
      if (dist < 0x4000) {
        obj->x = x;
        obj->z = z;
      } else {
        if (dist > step) {
          dx = Iwram_MulQ16(Iwram_RatioMulQ14(dist, dx), step);
          dz = Iwram_MulQ16(Iwram_RatioMulQ14(dist, dz), step);
        }
        obj->x += dx;
        obj->z += dz;
      }
      if ((dy < 0 ? -dy : dy) > 0x8000)
        dy /= 4;
      obj->y += dy;
    }
  }
  obj->step++;
  return 1;
}

extern u8 IwramSqrt[];
s32 _call_via_r3(s32, s32, s32, s32);

// SC_APPROACH_LINK: follows the linked object at its speed. More than 16
// pixels away it walks to 16 pixels short of it (animation 2) and runs on;
// within 16 it stands (animation 1) and waits for the next frame.
s32 Object_ApproachLinkedObject(struct ObjectRuntime *object) {
  s32 len;
  s32 mz;
  s32 dx;
  s32 dz;
  s32 dzh;
  s32 dxh;
  s32 n;
  s32 mx;
  s32 dx2;
  struct ObjectRuntime *link;

  link = object->linkedObject;
  object->speedLimit = link->speedLimit;
  object->acceleration = link->acceleration;
  dx = link->x - object->x;
  dz = link->z - object->z;
  dxh = dx >> 0x10;
  dzh = dz >> 0x10;
  len = _call_via_r3((dxh * dxh) + (dzh * dzh), dx, dzh, (u32)IwramSqrt);
  if (len > 0x10) {
    dx2 = dx;
    n = len - 0x10;
    mx = dx2 * n / len;
    mz = dz * n / len;
    Object_SetMoveTarget(object, object->x + mx, object->y, object->z + mz);
    ObjectDispatch_ApplyArgumentToChildren(object, 2);
    object->step = (object->step + 1);
    return 1;
  }
  ObjectDispatch_ApplyArgumentToChildren(object, 1);
  return 0;
}

// SC_WANDER(base, range, limit): walks to a random point base to base +
// range away, within 90 degrees either side of its heading (scriptValue),
// that no solid object or wall blocks, with clear ground 8 pixels further at
// 45 degrees either side, and within limit pixels of home. After seven
// failed tries it turns around (turnedBack) and waits for the next frame.
s32 Object_Wander(struct ScriptObjectRuntime *object) {
  struct FieldPosition pos;
  struct FieldPosition probe;
  s32 base;
  const s32 *args;
  s32 range;
  s32 limit;
  s32 radius;
  s32 heading;
  s32 tries;
  s32 dx;
  s32 dz;

  args = &object->script[(s16)object->scriptCursor + 1];
  base = *args++;
  range = *args++;
  limit = *args / 0x10000;
  tries = 0;
  limit = limit * limit;
retry:
  tries++;
  if (tries <= 7) {
    pos.x = object->x;
    pos.y = object->y;
    pos.z = object->z;
    radius = base + Iwram_MulQ16(Random16(), range);
    heading = object->scriptValue + (Random16() >> 2) - (Random16() >> 2);
    Vector_AddPolarOffset(radius, heading, &pos);
    if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)object,
                                  (s32 *)&pos) != 0 ||
        Map_CheckObjectStep((struct ObjectRuntime *)object, &pos) != 0)
      goto retry;
    radius += 0x80000;
    probe.x = object->x;
    probe.y = object->y;
    probe.z = object->z;
    Vector_AddPolarOffset(radius, heading, &probe);
    probe.x = object->x;
    probe.y = object->y;
    probe.z = object->z;
    Vector_AddPolarOffset(radius, heading + 0x2000, &probe);
    if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probe) != 0)
      goto retry;
    probe.x = object->x;
    probe.y = object->y;
    probe.z = object->z;
    Vector_AddPolarOffset(radius, heading - 0x2000, &probe);
    if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probe) != 0)
      goto retry;
    /* FAKEMATCH: one-pass block: it ends cse2's block, so the division works on
     * the loaded pos.x and the copy outlives it into the move target call. As
     * plain statements cse2 swaps them (differ 16), also with dx/dz split or
     * casts. The retry gotos stay: as for(;;) with continue, loop.c hoists &pos
     * and &probe, which the native recomputes each try (differ 128). */
    do {
      dx = pos.x / 0x10000 - object->homeX;
      dz = pos.z / 0x10000 - object->homeZ;
    } while (0);
    if (dx * dx + dz * dz > limit)
      goto retry;
  } else {
    object->scriptValue += 0x8000;
    object->turnedBack = 1;
    return 0;
  }
  Object_SetMoveTarget((struct ObjectRuntime *)object, pos.x, pos.y, pos.z);
  object->scriptCursor += 4;
  return 1;
}

// SC_WANDER_HOME(base, range, limit): wanders as SC_WANDER does, testing
// more ground around the point, while it is within limit pixels of home;
// outside that, or when no point works, it heads back towards home.
s32 ScriptObject_WanderNearHome(struct ScriptObjectRuntime *object) {
  struct FieldPosition pos;
  struct FieldPosition probe;
  s32 base;
  const s32 *args;
  s32 range;
  s32 limit;
  s32 radius;
  u16 heading;
  u16 angle;
  s32 tries;
  s32 dx;
  s32 dz;

  args = &object->script[(s16)object->scriptCursor + 1];
  base = *args++;
  range = *args++;
  limit = *args / 0x10000;
  angle = object->scriptValue;
  limit *= limit;
  tries = 0;
  dx = object->x / 0x10000 - object->homeX;
  dz = object->z / 0x10000 - object->homeZ;
  if (dx * dx + dz * dz <= limit) {
  roam:
    tries++;
    if (tries <= 7) {
      radius = base + Iwram_MulQ16(Random16(), range);
      heading = angle + (Random16() >> 2) - (Random16() >> 2);
      pos.x = object->x;
      pos.y = object->y;
      pos.z = object->z;
      Vector_AddPolarOffset(0x80000, heading, &pos);
      if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)object,
                                    (s32 *)&pos) != 0)
        goto roam;
      pos.x = object->x;
      pos.y = object->y;
      pos.z = object->z;
      Vector_AddPolarOffset(radius, heading, &pos);
      if (Map_CheckObjectStep((struct ObjectRuntime *)object, &pos) != 0)
        goto roam;
      probe.x = object->x;
      probe.y = object->y;
      radius += 0x80000;
      probe.z = object->z;
      Vector_AddPolarOffset(radius, heading, &probe);
      if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probe) != 0)
        goto roam;
      probe.x = object->x;
      probe.y = object->y;
      probe.z = object->z;
      Vector_AddPolarOffset(radius, heading + 0x2000, &probe);
      if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probe) != 0)
        goto roam;
      probe.x = object->x;
      probe.y = object->y;
      probe.z = object->z;
      Vector_AddPolarOffset(radius, heading - 0x2000, &probe);
      if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probe) != 0)
        goto roam;
      probe.x = object->x;
      probe.y = object->y;
      probe.z = object->z;
      Vector_AddPolarOffset(radius, heading + 0x4000, &probe);
      if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probe) != 0)
        goto roam;
      probe.x = object->x;
      probe.y = object->y;
      probe.z = object->z;
      Vector_AddPolarOffset(radius, heading - 0x4000, &probe);
      if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probe) != 0)
        goto roam;
      dx = pos.x / 0x10000 - object->homeX;
      dz = pos.z / 0x10000 - object->homeZ;
      if (dx * dx + dz * dz > limit)
        goto roam;
      object->flags59 |= 2;
      Object_SetMoveTarget((struct ObjectRuntime *)object, pos.x, pos.y, pos.z);
      goto done;
    }
  }
  tries = 0;
  angle = (u16)ArcTan2(dz, dx) + 0x8000;
back:
  tries++;
  if (tries <= 7) {
    radius = base + Iwram_MulQ16(Random16(), range);
    heading = angle + (Random16() >> 2) - (Random16() >> 2);
    pos.x = object->x;
    pos.y = object->y;
    pos.z = object->z;
    Vector_AddPolarOffset(0x80000, heading, &pos);
    if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)object,
                                  (s32 *)&pos) != 0)
      goto back;
    pos.x = object->x;
    pos.y = object->y;
    pos.z = object->z;
    Vector_AddPolarOffset(radius, heading, &pos);
    if (Map_CheckObjectStep((struct ObjectRuntime *)object, &pos) != 0)
      goto back;
    object->flags59 &= ~2;
    Object_SetMoveTarget((struct ObjectRuntime *)object, pos.x, pos.y, pos.z);
  }
done:
  object->scriptCursor += 4;
  return 1;
}

enum ScriptOperandOperation {
  SCRIPT_OPERAND_SET,
  SCRIPT_OPERAND_ADD,
  SCRIPT_OPERAND_COMPARE
};

typedef void (*OperandFunc)(struct ScriptOperands *, s32, s32);

extern OperandFunc gScript_OperandHandlerTable[];

// Defines the handler for a 32-bit work value: set it, add to it, or compare
// it (into comparisonResult).
#define DEFINE_OPERAND_WORD(name, field)                                       \
  void name(struct ScriptOperands *state, s32 operation, s32 value) {          \
    if (operation == SCRIPT_OPERAND_SET) {                                     \
      state->field = value;                                                    \
      return;                                                                  \
    }                                                                          \
    if (operation == SCRIPT_OPERAND_ADD) {                                     \
      state->field += value;                                                   \
      return;                                                                  \
    }                                                                          \
    state->comparisonResult = state->field == value;                           \
  }

// The same for an address: an add steps by value words (value * 4 bytes).
#define DEFINE_OPERAND_ADDRESS(name, field)                                    \
  void name(struct ScriptOperands *state, s32 operation, s32 value) {          \
    if (operation == SCRIPT_OPERAND_SET) {                                     \
      state->field = value;                                                    \
      return;                                                                  \
    }                                                                          \
    if (operation == SCRIPT_OPERAND_ADD) {                                     \
      state->field += value * 4;                                               \
      return;                                                                  \
    }                                                                          \
    state->comparisonResult = state->field == value;                           \
  }

// The same for an 8- or 16-bit value: a compare narrows value to its type.
#define DEFINE_OPERAND_NARROW(name, field, type)                               \
  void name(struct ScriptOperands *state, s32 operation, s32 value) {          \
    if (operation == SCRIPT_OPERAND_SET) {                                     \
      state->field = value;                                                    \
    } else if (operation == SCRIPT_OPERAND_ADD) {                              \
      state->field += value;                                                   \
    } else {                                                                   \
      state->comparisonResult = state->field == (type)value;                   \
    }                                                                          \
  }

// The work value handlers, one for each entry of enum ScriptWork (see
// gScript_OperandHandlerTable in operand.c); each names the object field it
// sets, adds to or compares.
DEFINE_OPERAND_ADDRESS(Script_SetOrCompareAddress, scriptAddress)

// Work value SW_CURSOR: sets, adds to or compares the script cursor.
void Script_SetOrCompareCursor(struct ScriptOperands *state, s32 operation,
                               s32 value) {
  if (operation == SCRIPT_OPERAND_SET) {
    state->cursor = value;
  } else if (operation == SCRIPT_OPERAND_ADD) {
    state->cursor = ((s16)state->cursor + value);
  } else {
    state->comparisonResult = (s16)state->cursor == (s16)value;
  }
}

DEFINE_OPERAND_NARROW(Script_SetOrCompareUnsignedHalfword, unsignedHalfword,
                      u16)
DEFINE_OPERAND_WORD(Script_SetOrCompareX, word08)
DEFINE_OPERAND_WORD(Script_SetOrCompareY, word0c)
DEFINE_OPERAND_WORD(Script_SetOrCompareZ, word10)

// Work value SW_RADIUS: sets, adds to or compares the object's radius.
void Script_SetOrCompareRadius(struct ScriptOperands *state, s32 operation,
                               s32 value) {
  if (operation == SCRIPT_OPERAND_SET) {
    state->halfword20 = value;
    return;
  }
  if (operation == SCRIPT_OPERAND_ADD) {
    state->halfword20 = (state->halfword20 + value);
    return;
  }
  state->comparisonResult = state->halfword20 == (s16)value;
}

DEFINE_OPERAND_WORD(Script_SetOrCompareScaleX, word18)
DEFINE_OPERAND_WORD(Script_SetOrCompareScaleY, word1c)
DEFINE_OPERAND_WORD(Script_SetOrCompareVelocityX, word24)
DEFINE_OPERAND_WORD(Script_SetOrCompareVelocityY, word28)
DEFINE_OPERAND_WORD(Script_SetOrCompareVelocityZ, word2c)
DEFINE_OPERAND_WORD(Script_SetOrCompareSpeedLimit, word30)
DEFINE_OPERAND_WORD(Script_SetOrCompareAcceleration, word34)
DEFINE_OPERAND_WORD(Script_SetOrCompareTargetX, word38)
DEFINE_OPERAND_WORD(Script_SetOrCompareTargetY, word3c)
DEFINE_OPERAND_WORD(Script_SetOrCompareTargetZ, word40)
DEFINE_OPERAND_WORD(Script_SetOrCompareVerticalMotion, word44)
DEFINE_OPERAND_WORD(Script_SetOrCompareGravity, word48)
DEFINE_OPERAND_WORD(Script_SetOrCompareTerrainHeight, word14)
DEFINE_OPERAND_WORD(Script_SetOrCompareWord4c, word4c)
DEFINE_OPERAND_ADDRESS(Script_SetOrCompareAddress50, address50)
DEFINE_OPERAND_NARROW(Script_SetOrCompareByte54, byte54, u8)
DEFINE_OPERAND_NARROW(Script_SetOrCompareMotionFlags, byte55, u8)
DEFINE_OPERAND_NARROW(Script_SetOrCompareArrivalAxis, byte56, u8)
DEFINE_OPERAND_NARROW(Script_SetOrCompareComparisonResult, comparisonResult, u8)
DEFINE_OPERAND_NARROW(Script_SetOrCompareSnapToTarget, byte58, u8)
DEFINE_OPERAND_NARROW(Script_SetOrCompareCollisionFlags, byte59, u8)
DEFINE_OPERAND_NARROW(Script_SetOrCompareSteeringFlags, byte5a, u8)
DEFINE_OPERAND_NARROW(Script_SetOrComparePaused, byte5b, u8)
DEFINE_OPERAND_NARROW(Script_SetOrCompareRepeatCount, byte5d, u8)

// Work value SW_WAIT: sets, adds to or compares the wait counter.
void Script_SetOrCompareHalfword5e(struct ScriptOperands *work, s32 operation,
                                   s32 value) {
  if (operation == SCRIPT_OPERAND_SET) {
    work->halfword5e = value;
    return;
  }
  if (operation == SCRIPT_OPERAND_ADD) {
    work->halfword5e += value;
    return;
  }
  work->comparisonResult = (s16)work->halfword5e == (s16)value;
}

DEFINE_OPERAND_NARROW(Script_SetOrCompareSignedHalfword64, signedHalfword64,
                      s16)
DEFINE_OPERAND_NARROW(Script_SetOrCompareSignedHalfword66, signedHalfword66,
                      s16)
DEFINE_OPERAND_WORD(Script_SetOrCompareWord68, word68)
DEFINE_OPERAND_WORD(Script_SetOrCompareUpdateAddress, word6c)
DEFINE_OPERAND_NARROW(Script_SetOrCompareByte62, byte62, u8)
DEFINE_OPERAND_NARROW(Script_SetOrCompareByte63, byte63, u8)

// Reads the two words after the command (a work value number and a value)
// and passes operation to that value's handler.
static __inline__ s32 ApplyOperand(struct ScriptOperands *work, s32 operation) {
  s16 index = work->cursor;
  const s32 *command = (const s32 *)work->scriptAddress + index + 1;
  OperandFunc handler = gScript_OperandHandlerTable[command[0]];

  if (handler != 0)
    handler(work, operation, command[1]);
  work->cursor += 3;
  return 1;
}

// SC_SET(w, v): sets work value w to v.
s32 Script_ApplyOperandSet(struct ScriptOperands *work) {
  return ApplyOperand(work, SCRIPT_OPERAND_SET);
}

// SC_ADD(w, v): adds v to work value w.
s32 Script_ApplyOperandAdd(struct ScriptOperands *work) {
  return ApplyOperand(work, SCRIPT_OPERAND_ADD);
}

// SC_CMP(w, v): tests whether work value w equals v.
s32 Script_ApplyOperandCompare(struct ScriptOperands *work) {
  return ApplyOperand(work, SCRIPT_OPERAND_COMPARE);
}

// Stores value as a halfword.
static __inline__ void StoreHalfword(s16 *address, s32 value) {
  *address = value;
}

// Stores value in the word that the object's unk6c points at.
void ObjectDispatch_SetUpdateValue(void *raw, s32 value) {
  struct ObjectRuntime *object = raw;

  if (object != NULL)
    *(s32 *)object->unk6c = value;
}

#if EDITION_INTERNATIONAL

#define ASSIGNED_KEYS gKeyState
#else

#define ASSIGNED_KEYS gKeysHeld
#endif

// Handles a shortcut key's assignment: the top two bits of value choose
// what it requests (0 a Psynergy, 1 the other kind, stored in unk180) and
// the low 14 bits which one. With flag 0x107 set, or in mode 3 (where L and
// R give 0xfc88 and 0xfc87), it raises a fixed trigger instead. Returns the
// request.
u32 Field_StoreAssignedKeyValue(u32 value) {
  u32 selector = value >> 14;
  u32 request = 0x3FFF & value;
  struct FieldStepWork *work = (struct FieldStepWork *)gEventWork;

  if (GameFlag_TestFar(0x107) != 0) {
    StoreHalfword((s16 *)&work->raisedTrigger, 0xFA);
  } else if (work->mode == 3) {
    if (ASSIGNED_KEYS & 0x100) {
      StoreHalfword((s16 *)&work->raisedTrigger, 0xFC88);
    } else if (ASSIGNED_KEYS & 0x200) {
      StoreHalfword((s16 *)&work->raisedTrigger, 0xFC87);
    }
  } else {
    switch (selector) {
    case 0:
      StoreHalfword(&work->psynergyRequest, request);
      break;
    case 1:
      StoreHalfword((s16 *)work->unk180, request);
      break;
    }
  }

  return request;
}

// Checks the keys the player has configured: Select, A and Start set their
// flag in the event work and return 1; the two shortcut keys pass on their
// assignment (Field_StoreAssignedKeyValue). Returns 0 when none is pressed.
s32 Field_CheckConfiguredKeys(void) {
  struct EventRuntime *work = gEventWork;
  s32 result = 0;

  if (work == NULL)
    return 0;
  if (gKeyState & gGameState.selectKeys) {
    work->unk172[0] = 1;
    result = 1;
  } else if (gKeyState & gGameState.aKeys) {
    work->unk172[1] = 1;
    result = 1;
  } else if (gKeyState & gGameState.startKeys) {
    work->unk172[2] = 1;
    result = 1;
  } else if (gKeyState & gGameState.firstShortcutKeys) {
    result = Field_StoreAssignedKeyValue(gGameState.firstShortcut);
  } else if (gKeyState & gGameState.secondShortcutKeys) {
    result = Field_StoreAssignedKeyValue(gGameState.secondShortcut);
  }
  return result;
}

// Returns 0 when two spheres overlap: at positions a and b (16.16 pixels)
// with radii radiusA and radiusB (pixels); -1 otherwise.
s32 Runtime_CheckRadiusOverlap(s32 *a, s32 radiusA, s32 *b, s32 radiusB) {
  const struct FieldPosition *first = (const struct FieldPosition *)a;
  const struct FieldPosition *second = (const struct FieldPosition *)b;
  const s32 *firstCoord = &first->x;
  const s32 *secondCoord = &second->x;
  s32 dx = (*firstCoord++ - *secondCoord++) >> 16;
  s32 dy = (*firstCoord++ - *secondCoord++) >> 16;
  s32 dz = (*firstCoord - *secondCoord) >> 16;
  s32 radius = radiusA + radiusB;

  if (!(dx > 0x400000) && !(dz > 0x400000) &&
      dx * dx + dy * dy + dz * dz < radius * radius)
    return 0;
  return -1;
}

struct WorldMapSprite {
  struct FieldSprite sprite;
  u8 unk28[4];
  u8 *shadow;
};

// SC_PLAYER: moves the player on the field by the direction pad: walking or
// running, sliding around walls, pushing movable objects, turning, playing
// the matching animation and leaving footprints. Also checks the configured
// keys. Returns 1.
s32 FieldObject_UpdatePlayerControl(struct ObjectRuntime *object) {
  struct FieldPosition position;
  struct FieldPosition testPosition;
  struct FieldPosition contact;
  s16 angleOffsets[22];
  s32 collision;
  s32 pushed;
  s16 heading;
  s32 angle;
  s32 mode;
  u16 direction;
  s32 rotation;
  s32 angleIndex;
  s32 count;
  struct ObjectRuntime *other;

  collision = 0;
  pushed = 0;
  // Debug (flag 350): beep when fewer than 136 of the 512 resource blocks
  // are free.
  if (gDebugMode != 0 && GameFlag_TestFar(350) != 0) {
    s32 free;
    s32 block;

    free = 0;
    for (block = 0; block < 512; block++) {
      if (gResourceBlockOwners[block] == 0xff)
        free++;
    }
    if (free - 136 < 0)
      AudioCommand_PlayFar(135);
  }
  /* FAKEMATCH: the volatile read puts gDebugMode's address in r5 here and at
   * the test before it, as the bytes do; read plainly both take r3 (differ 12,
   * 808 instructions either way). */
  if (*(volatile u8 *)&gDebugMode != 0) {
    s32 mask;

    mask = KEY_L;
    if (gKeysHeld & mask) {
      s32 wait;

      wait = mask;
    wait_a:
      --wait;
      if (wait != 0)
        goto wait_a;
      wait = 95;
    wait_b:
      wait--;
      if (wait >= 0)
        goto wait_b;
      wait = 63;
    wait_c:
      wait--;
      if (wait >= 0)
        goto wait_c;
      wait = 63;
    wait_d:
      wait--;
      if (wait >= 0)
        goto wait_d;
    }
  }
  // Walk, or run with the dash keys (animation 5 instead of 2); flag 0x17f
  // and B move much faster.
  if (gKeysHeld & gGameState.dashKeys) {
    object->speedLimit = 0x18000;
    object->acceleration = 0x4000;
    mode = 5;
  } else {
    object->speedLimit = 0x10000;
    object->acceleration = 0x4000;
    mode = 2;
  }
  if (GameFlag_TestFar(0x17f) != 0 && (gKeysHeld & KEY_B)) {
    object->speedLimit = 0x40000;
    object->acceleration = 0x10000;
    mode = 5;
  }

  // Step 8 pixels the way the direction pad points, when that step and the
  // ones 22.5 and 45 degrees either side are clear (debug L walks through
  // walls); otherwise try headings up to 67.5 degrees off. Bit 2 of
  // collision means no direction is held, bit 0 that every heading is
  // blocked.
  angle = gWorldMap_KeyHeadings[(gKeysHeld >> 4) & 15];
  if ((u16)angle == 0xffff) {
    collision |= 4;
  } else {
    collision = 0;
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Vector_AddPolarOffset(0x80000, (u16)angle, &position);
    if (gDebugMode != 0 && (gKeysHeld & KEY_L)) {
      heading = angle;
    } else {
      if (Map_CheckObjectStep(object, &position) == 0) {
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)angle + 0x1000, &testPosition);
        if (Map_CheckObjectStep(object, &testPosition) == 0) {
          testPosition.x = object->x;
          testPosition.y = object->y;
          testPosition.z = object->z;
          Vector_AddPolarOffset(0x80000, (u16)angle - 0x1000, &testPosition);
          if (Map_CheckObjectStep(object, &testPosition) == 0) {
            testPosition.x = object->x;
            testPosition.y = object->y;
            testPosition.z = object->z;
            Vector_AddPolarOffset(0x80000, (u16)angle + 0x2000, &testPosition);
            if (Map_CheckObjectStep(object, &testPosition) == 0) {
              testPosition.x = object->x;
              testPosition.y = object->y;
              testPosition.z = object->z;
              Vector_AddPolarOffset(0x80000, (u16)angle - 0x2000,
                                    &testPosition);
              if (Map_CheckObjectStep(object, &testPosition) == 0) {
                heading = angle;
                goto push_objects;
              }
            }
          }
        }
      }
      direction = angle;
      count = 0;
      angleOffsets[count++] = direction + 0x1000;
      angleOffsets[count++] = direction - 0x1000;
      angleOffsets[count++] = direction + 0x2000;
      angleOffsets[count++] = direction - 0x2000;
      angleOffsets[count++] = direction + 0x3000;
      angleOffsets[count++] = direction - 0x3000;
      for (angleIndex = 0; angleIndex < count; angleIndex++) {
        heading = angleOffsets[angleIndex];
        position.x = object->x;
        position.y = object->y;
        position.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)heading, &position);
        if (Map_CheckObjectStep(object, &position) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)heading + 0x1000, &testPosition);
        if (Map_CheckObjectStep(object, &testPosition) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)heading - 0x1000, &testPosition);
        if (Map_CheckObjectStep(object, &testPosition) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)heading + 0x2000, &testPosition);
        if (Map_CheckObjectStep(object, &testPosition) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(0x80000, (u16)heading - 0x2000, &testPosition);
        if (Map_CheckObjectStep(object, &testPosition) == 0)
          goto push_objects;
      }
      position.x = object->x;
      position.y = object->y;
      position.z = object->z;
      collision |= 1;

    push_objects:
      // Push the movable objects (unk59 bit 1) just ahead 4 pixels further,
      // when they have room; any other solid object there blocks the step
      // (bit 1 of collision). Pushing slows the player down.
      contact.x = object->x;
      contact.y = object->y;
      contact.z = object->z;
      Vector_AddPolarOffset(0x40000, (u16)heading, &contact);
      other = gObjectSlots;
      for (angleIndex = 63; angleIndex >= 0; angleIndex -= 1, other++) {
        s32 radius;

        radius = object->radius - 2;
        if (other->script == 0)
          continue;
        if ((other->unk59 & 1) == 0)
          continue;
        if (other == object)
          continue;
        if (Runtime_CheckRadiusOverlap(&other->x, radius, &contact.x,
                                       other->radius - 2) < 0)
          continue;
        if ((other->unk59 & 2) != 0 && other->movementState == 0) {
          direction = ArcTan2(other->z - object->z, other->x - object->x);
          testPosition.x = other->x;
          testPosition.y = other->y;
          testPosition.z = other->z;
          heading = direction;
          Vector_AddPolarOffset(0x4000, direction, &testPosition);
          if (!ScriptObject_CheckOverlap((struct ScriptObjectEntry *)other,
                                         (s32 *)&testPosition)) {
            testPosition.x = other->x;
            testPosition.y = other->y;
            testPosition.z = other->z;
            Vector_AddPolarOffset(0xa0000, direction, &testPosition);
            if (Map_CheckObjectStep(other, &testPosition) == 0) {
              testPosition.x = other->x;
              testPosition.y = other->y;
              testPosition.z = other->z;
              Vector_AddPolarOffset(0xa0000, direction + 0x1000, &testPosition);
              if (Map_CheckObjectStep(other, &testPosition) == 0 &&
                  Map_CheckObjectStep(other, &testPosition) == 0) {
                testPosition.x = other->x;
                testPosition.y = other->y;
                testPosition.z = other->z;
                Vector_AddPolarOffset(0xa0000, direction - 0x1000,
                                      &testPosition);
                if (Map_CheckObjectStep(other, &testPosition) == 0) {
                  Vector_AddPolarOffset(0x4000, direction,
                                        (struct FieldPosition *)&other->x);
                  other->targetX = TARGET_UNSET;
                  other->targetY = TARGET_UNSET;
                  other->targetZ = TARGET_UNSET;
                  pushed |= 1;
                  continue;
                }
              }
            }
          }
        }
        collision |= 2;
      }
      if (collision == 0 && pushed != 0) {
        object->speedLimit = 0x4000;
        object->acceleration = 0x2000;
      }
    }
  }
  // Count the blocked steps, and pick the animation: push (8), bump (9, or
  // 22 when the leader is down) or walk / run.
  if (gEventWork != NULL) {
    if (collision & 3)
      gEventWork->blockedSteps++;
    else
      gEventWork->blockedSteps = 0;
  }
  if (pushed != 0) {
    ObjectDispatch_ApplyArgumentToChildren(object, 8);
  } else if (collision != 0) {
    s32 action;

    action = 9;
    if (UnitGetFar(gGameState.selectedActor)->hp == 0)
      action = 22;
    ObjectDispatch_ApplyArgumentToChildren(object, action);
  } else {
    ObjectDispatch_ApplyArgumentToChildren(object, mode);
  }

  // Blocked: stop and turn towards the pad, at most 0x1000 a frame.
  // Otherwise move to the step, keeping the speed but along the heading.
  if (collision != 0) {
    object->targetX = TARGET_UNSET;
    object->targetY = TARGET_UNSET;
    object->targetZ = TARGET_UNSET;
    object->velocityX = 0;
    object->velocityZ = 0;
    if (collision & 3) {
      s32 difference;

      difference = (u16)angle - object->angle;
      difference = (s16)difference;
      if (difference > 0x1000)
        difference = 0x1000;
      if (difference < -0x1000)
        difference = -0x1000;
      object->angle += difference;
    }
    object->action = 0;
    object->unk66 = 2;
  } else {
    s32 square;

    Object_SetMoveTarget(object, position.x, position.y, position.z);
    square = FixedSqrt(Iwram_MulQ16(object->velocityX, object->velocityX) +
                       Iwram_MulQ16(object->velocityZ, object->velocityZ));
    object->velocityX = 0;
    object->velocityZ = 0;
    Vector_AddPolarOffset(square, (u16)heading,
                          (struct FieldPosition *)&object->velocityX);
    if (object->action != 0)
      object->action--;
  }

  // Leave a footprint (object kind 25) every 18 walking or 12 running
  // frames on maps that show them, alternating left and right.
  rotation = (u16)angle;
  if (((struct MapScrollWork *)gMapWork[0])->footprints != 0 &&
      object->action == 0 && collision == 0) {
    struct ObjectRuntime *effect;

    effect = FieldObject_Create(25, object->x, object->y, object->z);
    if (effect != 0) {
      struct FieldSprite *animation;

      effect->terrainHeight = object->terrainHeight;
      animation = effect->animation;
      ObjectDispatch_Initialize((struct DispatchObject *)effect,
                                (u32)gWorldMap_FootprintActions);
      effect->unk23 = 2;
      effect->flags = collision;
      if (animation != 0) {
        AnimationObjects_SelectAnimation((struct AnimationObject *)animation,
                                         1);
        animation->flags = collision;
        animation->rotation = rotation + 0x4000;
        animation->priority = 3;
      }
      if (object->unk66 == 2) {
        AnimationObjects_SelectAnimation((struct AnimationObject *)animation,
                                         2);
        object->unk66 = collision;
      }
      if (object->unk66 != 0)
        effect->angle = 0x8000;
      if (mode == 5)
        object->action = 12;
      else
        object->action = 18;
      object->unk66 ^= 1;
    }
  }
  Field_CheckConfiguredKeys();
  object->step++;
  return 1;
}

#if EDITION_INTERNATIONAL
#define WORLD_PROBE_RADIUS 0x70000
#else
#define WORLD_PROBE_RADIUS 0x80000
#endif

// SC_WORLD: moves the player on the world map by the direction pad, as
// SC_PLAYER does on the field, and turns the world map camera with the pad.
// Shallow water (terrain 6) leaves a ripple and terrain 9 hides the shadow.
s32 Object_MoveOnWorldMap(struct ObjectRuntime *object) {
  struct FieldPosition position;
#if EDITION_INTERNATIONAL
  struct FieldPosition testPosition;
  s16 angleOffsets[28];
#else
  struct FieldPosition leftPosition;
  struct FieldPosition rightPosition;
  s16 angleOffsets[22];
#endif
  s32 collision;
  s32 angle;
  u16 direction;
  s32 angleIndex;
  s32 mode;
  s32 count;
  s32 difference;
  s32 collisionKind;
  s32 square;
  s32 rate;
  struct PerspectiveWork *work;
  s32 step;
  struct FieldSprite *sprite;
  struct ObjectRuntime *effect;
  struct FieldSprite *animation;

  mode = 2;
  collision = 0;
#if !EDITION_INTERNATIONAL
  count = 0;
#endif
  if (gKeysHeld & gGameState.dashKeys) {
    object->speedLimit = 0x10000;
    object->acceleration = 0x14000;
    mode = 5;
  } else {
    object->speedLimit = 0x8000;
    object->acceleration = 0x4000;
  }
  if ((gKeysRepeat & KEY_L) != 0)
    object->speedLimit = 0x40000;

  angle = gWorldMap_KeyHeadings[(gKeysHeld >> 4) & 15];
  direction = angle;
  if (direction == 0xffff) {
    collision |= 4;
  } else {
    collision = 0;
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction, &position);
    if (gDebugMode == 0 || (gKeysHeld & KEY_L) == 0) {
#if EDITION_INTERNATIONAL
      if (CheckWorldMapCollisionRange((s32)object,
                                      (struct WorldPosition *)&position) == 0) {
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction + 0x1000,
                              &testPosition);
        if (CheckWorldMapCollisionRange(
                (s32)object, (struct WorldPosition *)&testPosition) == 0) {
          testPosition.x = object->x;
          testPosition.y = object->y;
          testPosition.z = object->z;
          Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction - 0x1000,
                                &testPosition);
          if (CheckWorldMapCollisionRange(
                  (s32)object, (struct WorldPosition *)&testPosition) == 0) {
            testPosition.x = object->x;
            testPosition.y = object->y;
            testPosition.z = object->z;
            Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction + 0x2000,
                                  &testPosition);
            if (CheckWorldMapCollisionRange(
                    (s32)object, (struct WorldPosition *)&testPosition) == 0) {
              testPosition.x = object->x;
              testPosition.y = object->y;
              testPosition.z = object->z;
              Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction - 0x2000,
                                    &testPosition);
              if (CheckWorldMapCollisionRange(
                      (s32)object, (struct WorldPosition *)&testPosition) == 0)
                goto update_object;
            }
          }
        }
      }
      count = 0;
      angleOffsets[count++] = direction + 0x1000;
      angleOffsets[count++] = direction - 0x1000;
      angleOffsets[count++] = direction + 0x2000;
      angleOffsets[count++] = direction - 0x2000;
      angleOffsets[count++] = direction + 0x3000;
      angleOffsets[count++] = direction - 0x3000;
      angleOffsets[count++] = direction + 0x4000;
      angleOffsets[count++] = direction - 0x4000;
      for (angleIndex = 0; angleIndex < count; angleIndex++) {
        direction = angleOffsets[angleIndex];
        position.x = object->x;
        position.y = object->y;
        position.z = object->z;
        Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction, &position);
        if (CheckWorldMapCollisionRange((s32)object,
                                        (struct WorldPosition *)&position) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction + 0x1000,
                              &testPosition);
        if (CheckWorldMapCollisionRange(
                (s32)object, (struct WorldPosition *)&testPosition) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction - 0x1000,
                              &testPosition);
        if (CheckWorldMapCollisionRange(
                (s32)object, (struct WorldPosition *)&testPosition) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction + 0x2000,
                              &testPosition);
        if (CheckWorldMapCollisionRange(
                (s32)object, (struct WorldPosition *)&testPosition) != 0)
          continue;
        testPosition.x = object->x;
        testPosition.y = object->y;
        testPosition.z = object->z;
        Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction - 0x2000,
                              &testPosition);
        if (CheckWorldMapCollisionRange(
                (s32)object, (struct WorldPosition *)&testPosition) != 0)
          continue;
        angle = (s16)direction;
        goto update_object;
      }
      collision |= 1;

#else
      leftPosition.x = object->x;
      leftPosition.y = object->y;
      leftPosition.z = object->z;
      Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction + 0x1555,
                            &leftPosition);
      rightPosition.x = object->x;
      rightPosition.y = object->y;
      rightPosition.z = object->z;
      Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction - 0x1555,
                            &rightPosition);
      if ((CheckWorldMapCollisionRange((s32)object,
                                       (struct WorldPosition *)&position) |
           CheckWorldMapCollisionRange((s32)object,
                                       (struct WorldPosition *)&leftPosition) |
           CheckWorldMapCollisionRange(
               (s32)object, (struct WorldPosition *)&rightPosition)) != 0) {
        angleOffsets[count++] = direction + 0x1000;
        angleOffsets[count++] = direction - 0x1000;
        angleOffsets[count++] = direction + 0x2000;
        angleOffsets[count++] = direction - 0x2000;
        angleOffsets[count++] = direction + 0x3000;
        angleOffsets[count++] = direction - 0x3000;
        for (angleIndex = 0; angleIndex < count; angleIndex++) {
          direction = angleOffsets[angleIndex];
          position.x = object->x;
          position.y = object->y;
          position.z = object->z;
          Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction, &position);
          leftPosition.x = object->x;
          leftPosition.y = object->y;
          leftPosition.z = object->z;
          Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction + 0x1555,
                                &leftPosition);
          rightPosition.x = object->x;
          rightPosition.y = object->y;
          rightPosition.z = object->z;
          Vector_AddPolarOffset(WORLD_PROBE_RADIUS, direction - 0x1555,
                                &rightPosition);
          if ((CheckWorldMapCollisionRange((s32)object,
                                           (struct WorldPosition *)&position) |
               CheckWorldMapCollisionRange(
                   (s32)object, (struct WorldPosition *)&leftPosition) |
               CheckWorldMapCollisionRange(
                   (s32)object, (struct WorldPosition *)&rightPosition)) != 0)
            continue;
          angle = (s16)direction;
          goto update_object;
        }
        collision |= 1;
      }
#endif
    }
  }

update_object:
  if (gEventWork != NULL) {
    if ((collision & 3) != 0)
      gEventWork->blockedSteps++;
    else
      gEventWork->blockedSteps = 0;
  }
  if (collision != 0)
    ObjectDispatch_ApplyArgumentToChildren(object, 9);
  else
    ObjectDispatch_ApplyArgumentToChildren(object, mode);

  if (collision != 0) {
    object->targetX = TARGET_UNSET;
    object->targetY = TARGET_UNSET;
    object->targetZ = TARGET_UNSET;
    object->velocityX = 0;
    object->velocityZ = 0;
    if ((collision & 3) != 0) {
      direction = angle;
      difference = (s16)(direction - object->angle);
      if (difference > 0x1000)
        difference = 0x1000;
      if (difference < -0x1000)
        difference = -0x1000;
      object->angle += difference;
    }
    object->action = 0;
    object->unk66 = 2;
  } else {
    Object_SetMoveTarget(object, position.x, position.y, position.z);
    square = FixedSqrt(Iwram_MulQ16(object->velocityX, object->velocityX) +
                       Iwram_MulQ16(object->velocityZ, object->velocityZ));
    object->velocityX = collision;
    object->velocityZ = collision;
    Vector_AddPolarOffset(square, (u16)angle,
                          (struct FieldPosition *)&object->velocityX);
    if (object->action != 0)
      object->action--;
  }

  work = gMapWork[0];
  rate = gWorldMap_KeyYaws[(gKeysHeld >> 4) & 15];
  step = (s16)(rate - work->yaw) / 8;
  if (step > 0x200)
    step = 0x200;
  if (step < -0x200)
    step = -0x200;
  if (step > -16 && step < 16)
    step = rate - work->yaw;
  work->yaw += step;

  if (object->animationKind == 1) {
    sprite = object->animation;
    collisionKind = GetWorldMapCollision((struct WorldPosition *)&object->x);
    if (collisionKind == 9) {
      ((struct WorldMapSprite *)sprite)->shadow[6] = 1;
      sprite->flags = 0;
    } else {
      ((struct WorldMapSprite *)sprite)->shadow[6] = 9;
      sprite->flags = 1;
    }
    if (collisionKind == 6 && object->action == 0 && collision == 0) {
      effect = FieldObject_Create(24, object->x, object->y, object->z);
      if (effect != 0) {
        animation = effect->animation;
        ObjectDispatch_Initialize((struct DispatchObject *)effect,
                                  (u32)(gWorldMap_FootprintActions + 12));
        effect->flags = collision;
        effect->terrainId = 1;
        if (animation != 0) {
          AnimationObjects_SelectAnimation((struct AnimationObject *)animation,
                                           1);
          animation->flags = collision;
          animation->blendMode = 1;
          animation->priority = 2;
        }
        object->action = 10;
      }
    }
  }
  Field_CheckConfiguredKeys();
  object->step++;
  return 1;
}

// SC_KEY_COUNT: checks the configured keys (Field_CheckConfiguredKeys).
// Returns 1.
s32 Field_CheckConfiguredKeysAndCount(void *work) {
  struct ObjectRuntime *object = work;

  Field_CheckConfiguredKeys();
  object->step++;
  return 1;
}

// SC_KEYS: moves an object 8 pixels at a time by the direction pad, with up
// and down changing its height (y) rather than its depth. Refuses steps
// into an occupied cell, across a change of collision code, or onto ground
// too high (up) or too low (down). Returns 1.
s32 Object_MoveByKeys(struct ObjectRuntime *object) {
  struct FieldPosition pos;
  /* FAKEMATCH: never used; its 68 bytes are the native frame space no
   * instruction touches. Without it the frame is 12 bytes, not 80 (differ 8:
   * the sp adjustments and pos's address). 68 bytes is exactly the probe locals
   * of the sibling Object_MoveOnWorldMap (testPosition and angleOffsets[28]
   * internationally, leftPosition, rightPosition and angleOffsets[22] in
   * Japan); declared here instead, they also match tbs-en and tbs-ja
   * (2026-10-13). */
  u8 unused[68];
  struct MapCell *from;
  struct MapCell *to;
  u16 angle;
  s32 motion;
  s32 blocked;
  s32 deltaZ;
  s32 height;

  object->speedLimit = 0x8000;
  object->acceleration = 0x4000;
  motion = 12;
  angle = gWorldMap_KeyHeadings[(gKeysHeld >> 4) & 15];
  if (angle == 0xffff) {
    blocked = 4;
  } else {
    motion = 14;
    if ((angle & 0xf000) != 0) {
      motion = 15;
      if ((angle & 0xf000) != 0x8000)
        motion = 10;
    }
    blocked = 0;
    pos.x = 0;
    pos.y = 0;
    pos.z = 0;
    Vector_AddPolarOffset(0x80000, angle, &pos);
    pos.x += object->x;
    deltaZ = pos.z;
    if (deltaZ < 0)
      object->angle = 0xc000;
    if (deltaZ > 0)
      object->angle = 0x4000;
    pos.y = object->y - pos.z;
    pos.z = object->z;
    from = &(
        (struct MapCell *)
            Ram_MapCellBuffer)[object->x / 0x100000 + (pos.z / 0x100000) * 128];
    to = &((struct MapCell *)
               Ram_MapCellBuffer)[pos.x / 0x100000 + (pos.z / 0x100000) * 128];
    if (CheckMapPositionCellOccupied((struct WorldPosition *)&pos) != 0 ||
        from->collisionCode != to->collisionCode) {
      blocked = 4;
      motion = 12;
    } else if (gKeysHeld & KEY_UP) {
      height = Map_GetTerrainHeight(object->terrainId, pos.x, pos.z - 0x100000);
      if (height - object->y < 0x100000)
        goto refused;
    } else if (gKeysHeld & KEY_DOWN) {
      height = Map_GetTerrainHeight(object->terrainId, pos.x, pos.z);
      if (height - object->y > -0x80000) {
      refused:
        blocked = 1;
        motion = 12;
      }
    }
  }
  if (gEventWork != NULL) {
    if (blocked & 3)
      gEventWork->blockedSteps++;
    else
      gEventWork->blockedSteps = 0;
  }
  ObjectDispatch_ApplyArgumentToChildren(object, motion);
  if (blocked != 0) {
    object->targetX = TARGET_UNSET;
    object->targetY = TARGET_UNSET;
    object->targetZ = TARGET_UNSET;
    object->velocityX = 0;
    object->velocityY = 0;
    object->velocityZ = 0;
  } else {
    Object_SetMoveTarget(object, pos.x, pos.y, pos.z);
  }
  object->step++;
  return 1;
}

// Remaps cnt bytes of buf in place through gRuntime_ByteRemapTable.
void Runtime_RemapBytesByTable(u8 *buf, s32 cnt) {
  s32 remaining;
  u8 *byte;
  u8 *table;

  byte = buf;
  table = gRuntime_ByteRemapTable;
  remaining = cnt - 1;
  if (remaining != -1) {
    do {
      remaining -= 1;
      *byte = table[*byte];
      byte += 1;
    } while (remaining != -1);
  }
}

// Unpacks the staged block data into gMapBlocks. Byte 1 of the staged buffer
// (Ram_MapCellBuffer) is the method, and the
// 16-bit data follows from byte 2. size counts the method byte too (an even
// size means no data). Methods:
//   0: copy as it is
//   1: all the high bytes, then all the low bytes; each value is XORed with
//      the one before
//   2: each value is XORed with the one before
void Tilemap_DecodeStagedBuffer(s32 size) {
  u16 *dst;
  u16 *src;
  s32 n;
  s32 i;

  src = (u16 *)(Ram_MapCellBuffer + 2);
  dst = (u16 *)gMapBlocks;
  n = (size - 1) / 2;
  if (size & 1) {
    switch (((u8 *)gMapCellBuffer)[1]) {
    case 0:
      for (i = 0; i < n; i++)
        *dst++ = *src++;
      break;
    case 1: {
      u32 prev = 0;
      u8 *hi = Ram_MapCellBuffer + 2;
      u8 *lo = hi + n;

      for (i = 0; i < n; i++) {
        u32 v = ((*hi++ << 8) | *lo++) ^ prev;
        *dst++ = v;
        prev = v;
      }
      break;
    }
    case 2: {
      s32 prev = 0;

      for (i = 0; i < n; i++) {
        s32 v = *src++ ^ prev;
        *dst++ = v;
        prev = v;
      }
      break;
    }
    }
  }
}

typedef void (*ConvertFn)(void *dst, const void *src, const void *saved);

// Gives each map cell whose block number is 0xfff (a placeholder) a number
// of its own, counting up from 0 in map order.
void Graphics_RenumberFillerEntries(void) {
  u32 *cell = (u32 *)gMapCellBuffer;
  u32 remaining = 128 << 7;
  u32 mask = 0xfff;
  s32 number = -1;

  do {
    u32 value = *cell++;
    u32 block = value & mask;

    if (block == mask) {
      if (number != block) {
        number++;
      }
      value = value + number - block;
      cell[-1] = value;
    }
    remaining--;
  } while (remaining != 0);
}

// Converts the map's cells in place with Tile_ConvertMap, run from RAM, from
// a saved copy of the staged buffer.
void Tilemap_ConvertBuffer(void) {
  u8 *saved;
  ConvertFn routine;
  u32 size;

  size = 0x8000;
  saved = Runtime_BumpAllocateAlternatePool(size);
  Iwram_CopyWords(saved, Ram_MapCellBuffer, size);
  RamCode_Load(routine, Tile_ConvertMap, Tile_ConvertMapCodeSize);
  routine(((u8 *)gMapCellBuffer + 0x8000), Ram_MapCellBuffer, saved);
  MemFree(routine);
  MemFree(saved);
}

// Copies one tile column of a block (metatile) into a BG map: its top and
// bottom tile entries, left or right by parity.
static __inline__ void CopyBlock(u32 *map, u8 *base, u32 rowmod, u32 colmod,
                                 u32 parity) {
  u16 *entry;
  u8 *destination;
  u32 index = ((*map << 20) >> 18) + parity;

  entry = (u16 *)Ram_MapBlocks;
  entry += index;
  destination = base + (rowmod + colmod + parity) * 2;
  *(u16 *)destination = *entry;
  entry = (u16 *)(Ram_MapBlocks + 4);
  entry += index;
  *(u16 *)(destination + 64) = *entry;
}

// Copies a whole block (metatile) into a BG map: its top two tile entries
// and its bottom two, a map row (64 bytes) below.
static __inline__ void CopyCameraCell(u32 *map, u8 *base, u32 rowmod,
                                      u32 colmod) {
  u32 *tiles;
  u8 *dest;
  u32 index = (*map << 20) >> 19;

  tiles = (u32 *)Ram_MapBlocks;
  tiles += index;
  dest = base + rowmod * 2 + colmod * 2;
  *(u32 *)dest = *tiles;
  tiles = (u32 *)(Ram_MapBlocks + 4);
  tiles += index;
  *(u32 *)(dest + 64) = *tiles;
}

struct SceneEntry {
  u16 resources[6];
};

extern struct SceneEntry gMap_LayeredScenes[];

struct SceneLayerSource {
  u8 x;
  u8 y;
  s8 scrollX;
  s8 scrollY;
  s8 speedX;
  s8 speedY;
  u8 periodX;
  u8 periodY;
};

struct SceneHeader {
  u8 origin[4];
  u8 priority[3];
  u8 charBase[3];
  u16 unk0a;
  struct SceneLayerSource layers[3];
  s32 tiles;
  s32 collision;
  s32 tilemap;
  s32 animation;
  s32 blend;
  s32 script;
};

// Loads layered scene index from gMap_LayeredScenes: its blocks, collision,
// cells, tile animations, blend script and event script; sets up the three
// background layers' origins, parallax and wrap from the scene header; loads
// the palette and tile graphics unless FLAG_KEEP_LAYER_GRAPHICS says they are
// loaded; and starts the layer scrolling. Returns 2.
s32 Map_LoadLayeredScene(s32 index) {
  struct SceneEntry *entry;
  struct MapScrollWork *work;
  struct SceneHeader *header;
  struct MapLayerScroll *layer;
  struct SceneLayerSource *source;
  s32 *scaleX;
  s32 *scaleY;
  s32 i;
  u8 *buf;
  s32 backdrop;
  u8 *src;
  s32 cnt;

  REG_DISPCNT &= ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON |
                   DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
  Blend_SetDarkenTarget0(0);
  entry = &gMap_LayeredScenes[index];
  work = Runtime_AllocateBlock(8, sizeof(struct MapScrollWork));
  Iwram_ClearWords(work, sizeof(struct MapScrollWork));
  header = (struct SceneHeader *)Resource_GetTableEntry(
      entry->resources[0] + (u32)&ResourceId_Map001);
  src = (u8 *)header + header->tiles;
  Tilemap_DecodeStagedBuffer(Resource_DecodeType01(src, Ram_MapCellBuffer + 1));
  src = (u8 *)header + header->collision;
  Resource_DecodeType01(src, Ram_MapCollision);
  src = (u8 *)header + header->tilemap;
  Resource_DecodeType01(src, Ram_MapCellBuffer);
  Tilemap_ConvertBuffer();
  if (header->animation != 0) {
    src = (u8 *)header + header->animation;
    Resource_DecodeType01(src, Ram_MapCollision + 0x1000);
    MapAnimation_StartChannels((const u16 *)(Ram_MapCollision + 0x1000));
  }
  if (header->blend != 0) {
    src = (u8 *)header + header->blend;
    Resource_DecodeType01(src, Ram_MapCollision + 0x1e00);
    DisplayBlend_StartScript((u16 *)(Ram_MapCollision + 0x1e00));
  }
  work->script = (u8 *)header + header->script;
  work->minX = header->origin[0] << 19;
  work->minY = header->origin[1] << 19;
  work->maxX = header->origin[2] << 19;
  work->maxY = header->origin[3] << 19;
  scaleX = &work->viewX;
  *scaleX = 0;
  scaleY = &work->viewY;
  *scaleY = 0;
  work->enabled[0] = header->priority[0];
  work->enabled[1] = header->priority[1];
  work->enabled[2] = header->priority[2];
  layer = work->layers;
  source = header->layers;
  for (i = 0; i < 3; i++) {
    u32 x = source->x;
    u32 y = source->y;
    s32 scrollX;
    s32 scrollY;
    s32 baseX;
    s32 baseY;

    layer->offsetX = baseX = x << 19;
    layer->offsetY = baseY = y << 19;
    scrollX = source->scrollX << 12;
    scrollY = source->scrollY << 12;
    layer->scaleX = scrollX;
    layer->scaleY = scrollY;
    layer->speedX = source->speedX << 12;
    layer->speedY = source->speedY << 12;
    layer->maskX = source->periodX;
    layer->maskY = source->periodY;
    layer->phaseX = 0;
    layer->phaseY = 0;
    layer->cells =
        (struct MapCell *)Ram_MapCellBuffer + (y >> 1) * 128 + (x >> 1);
    layer->x = Iwram_MulQ16(*scaleX, scrollX) + baseX;
    layer->y = Iwram_MulQ16(*scaleY, scrollY) + baseY;
    source++;
    layer++;
  }
  work->flags = DISPCNT_OBJ_ON;
  if (work->enabled[0] != 0)
    work->flags = DISPCNT_OBJ_ON | DISPCNT_BG3_ON;
  if (work->enabled[1] != 0)
    work->flags |= DISPCNT_BG2_ON;
  if (work->enabled[2] != 0)
    work->flags |= DISPCNT_BG1_ON;
  cnt = work->enabled[0] | BGCNT_CHAR_BASE(header->charBase[0]) |
        BGCNT_SCREEN_BASE(5);
  REG_BG3CNT = cnt;
  Io_Write16(work->enabled[1] | BGCNT_CHAR_BASE(header->charBase[1]) |
                 BGCNT_SCREEN_BASE(6),
             &REG_BG2CNT);
  Io_Write16(work->enabled[2] | BGCNT_CHAR_BASE(header->charBase[2]) |
                 BGCNT_SCREEN_BASE(7),
             &REG_BG1CNT);
  if (GameFlag_TestFar(FLAG_KEEP_LAYER_GRAPHICS) != 0) {
    GameFlag_ClearBitFar(FLAG_KEEP_LAYER_GRAPHICS);
  } else {
    buf = MemAlloc(0x4000);
    if (buf != NULL) {
      backdrop = *(s16 *)BG_PLTT;
      Resource_DecodeType01(
          Resource_GetTableEntry(entry->resources[1] + (u32)&ResourceId_Map001),
          buf);
      *(s16 *)buf = backdrop;
      /* FAKEMATCH: needed to match: these four copies call the fixed entry
       * itself; through Iwram_CopyWords the scene header and the work move from
       * r5 and r8 to r8 and sl (237 of 357 instructions differ), with or
       * without the function's other devices. */
      Iwram_CopyWordsEntry((void *)BG_PLTT, buf, 0x1c0);
      Resource_DecodeType2(
          Resource_GetTableEntry(entry->resources[2] + (u32)&ResourceId_Map001),
          buf);
      Iwram_CopyWordsEntry(BG_CHAR_BLOCK(1), buf, 0x4000);
      Resource_DecodeType2(
          Resource_GetTableEntry(entry->resources[3] + (u32)&ResourceId_Map001),
          buf);
      Iwram_CopyWordsEntry(BG_CHAR_BLOCK(2), buf, 0x4000);
      Resource_DecodeType2(
          Resource_GetTableEntry(entry->resources[4] + (u32)&ResourceId_Map001),
          buf);
      Iwram_CopyWordsEntry(BG_CHAR_BLOCK(3), buf, 0x4000);
      Resource_DecodeType2(
          Resource_GetTableEntry(entry->resources[5] + (u32)&ResourceId_Map001),
          gMapLayerData);
      MemFree(buf);
    }
  }
  cnt = 0;
  REG_MOSAIC = cnt;
  REG_BLDCNT = cnt;
  cnt = DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP;
  REG_DISPCNT = cnt;
  /* FAKEMATCH: Scheduler_AddOrUpdateCallback is called as void (*)(void
   * (*)(void), s32); as s32 (*)(SchedulerCallback, s32), its prototype,
   * Map_LoadLayeredScene has 6 of 868 bytes different */
  ((void (*)(void (*)(void), s32))Scheduler_AddOrUpdateCallback)(
      Map_UpdateLayerScroll, 0xc85);
  return 2;
}

// Centres the camera on the point the map follows (origin: x, height, z),
// or on (0, 0) when there is none, and scrolls the layers there.
void Map_ApplyWorkOriginAndSpan(void) {
  struct MapScrollWork *work = gMapWork[0];
  s32 x;
  s32 y;
  s32 height;
  s32 *p;

  p = work->origin;
  x = 0;
  height = 0;
  y = 0;
  if (p != NULL) {
    x = *p++;
    height = *p++;
    y = *p;
  }
  Map_SetCameraCenter(x, y - height);
  Map_UpdateLayerScroll();
}

// Draws a row of 16 blocks (32 tiles) of a layer's cells into its BG map
// (screen block 5 + layer), starting at tile (tileX, tileY); the BG map
// wraps every 32 tiles.
void Map_RenderMetatileRow(u32 layer, s32 tileX, s32 tileY) {
  u8 *dest = BG_SCREEN_BLOCK(5) + (layer << 11);
  u32 row = ((tileY / 2) & 0x7F) << 7;
  u32 rowmod = (tileY & 30) << 5;
  u32 col = (tileX / 2) & 0x7F;
  u32 colmod = tileX & 30;
  u32 i;

  for (i = 0; i <= 15; i++) {
    u32 *map = (u32 *)Ram_MapCellBuffer;
    u32 *tiles;
    u8 *cell;
    u32 index;

    map += row + col;
    index = (*map << 20) >> 19;
    tiles = (u32 *)Ram_MapBlocks;
    tiles += index;
    cell = dest + (rowmod + colmod) * 2;
    *(u32 *)cell = *tiles;
    tiles = (u32 *)(Ram_MapBlocks + 4);
    tiles += index;
    *(u32 *)(cell + 64) = *tiles;
    col = (col + 1) & 0x7F;
    colmod = (colmod + 2) & 30;
  }
}

// Draws a column of 11 blocks of a layer's cells, one tile wide, into its BG
// map, starting at tile (tileX, tileY).
void Map_RenderMetatileColumn(u32 layer, s32 tileX, s32 tileY) {
  u8 *destination = BG_SCREEN_BLOCK(5) + (layer << 11);
  u32 row = ((tileY / 2) & 0x7F) << 7;
  u32 rowmod = (tileY & 30) << 5;
  u32 col = (tileX / 2) & 0x7F;
  u32 colmod = tileX & 30;
  u32 parity = tileX & 1;
  u32 i;

  for (i = 0; i <= 10; i++) {
    u32 *map = (u32 *)Ram_MapCellBuffer;

    map += row + col;
    CopyBlock(map, destination, rowmod, colmod, parity);

    row = (row + 128) & 0x3F80;
    rowmod = (rowmod + 64) & 0x3C0;
  }
}

// Each frame on a layered map: centres the view on the point the map follows
// (origin), clamped to the map's bounds and shaken by any decaying screen
// shake; then scrolls each of the three layers by its parallax scale and
// automatic scroll (wrapping at its period), draws the block column or row
// that scrolls into view, and sets its BG scroll.
void Map_UpdateLayerScroll(void) {
  struct MapScrollWork *work;
  struct MapLayerScroll *layer;
  s32 *origin;
  s32 x, y;
  s32 minX, minY, maxX, maxY;
  s32 baseX, baseY;
  u32 i;

  work = gMapWork[0];
  origin = work->origin;
  layer = work->layers;
  if (origin == 0)
    return;

  baseX = *origin++ - 0x780000;
  {
    s32 height = *origin++;
    baseY = *origin - height - 0x600000;
  }
  minX = work->minX + work->shakeX;
  maxX = work->maxX - work->shakeX - 0xf00000;
  minY = work->minY + work->shakeY;
  maxY = work->maxY - work->shakeY - 0xa00000;
  if (minX > maxX)
    maxX = minX;
  if (minY > maxY)
    maxY = minY;
  if (baseX < minX)
    baseX = minX;
  if (baseX > maxX)
    baseX = maxX;
  if (baseY < minY)
    baseY = minY;
  if (baseY > maxY)
    baseY = maxY;

  if (work->shakeX != 0) {
    s32 first = Random16();
    s32 second = Random16();
    s32 amplitude = work->shakeX;
    baseX += Iwram_MulQ16(amplitude, first - second);
    work->shakeX = Iwram_MulQ16(amplitude, work->shakeDecay);
  }
  if (work->shakeY != 0) {
    s32 first = Random16();
    s32 second = Random16();
    s32 amplitude = work->shakeY;
    baseY += Iwram_MulQ16(amplitude, first - second);
    work->shakeY = Iwram_MulQ16(amplitude, work->shakeDecay);
  }
  work->viewX = baseX;
  work->viewY = baseY;
  for (i = 0; i < 3; i++, layer++) {
    baseX = Iwram_MulQ16(work->viewX, layer->scaleX);
    baseY = Iwram_MulQ16(work->viewY, layer->scaleY);
    if (layer->speedX != 0) {
      layer->phaseX += layer->speedX;
      baseX = (baseX + layer->phaseX) & ((layer->maskX << 19) | 0x7ffff);
    }
    if (layer->speedY != 0) {
      layer->phaseY += layer->speedY;
      baseY = (baseY + layer->phaseY) & ((layer->maskY << 19) | 0x7ffff);
    }
    baseX += layer->offsetX;
    baseY += layer->offsetY;
    x = baseX / 0x80000;
    y = baseY / 0x80000;
    if ((layer->x ^ baseX) & 0x80000) {
      if (layer->x < baseX)
        Map_RenderMetatileColumn(i, x + 30, y);
      else
        Map_RenderMetatileColumn(i, x, y);
    }
    if ((layer->y ^ baseY) & 0x100000) {
      if (layer->y < baseY)
        Map_RenderMetatileRow(i, x, y + 20);
      else
        Map_RenderMetatileRow(i, x, y);
    }
    gBgScroll[3 - i].x = baseX >> 16;
    gBgScroll[3 - i].y = baseY >> 16;
    layer->x = baseX;
    layer->y = baseY;
  }
}

// Centres the view on (x, y) in 16.16 pixels, clamped to the map's bounds,
// and redraws every enabled layer's BG map around it.
void Map_SetCameraCenter(s32 x, s32 y) {
  u32 no;
  u8 *dest;
  struct MapScrollWork *work = gMapWork[0];
  struct MapLayerScroll *layer = work->layers;

  x -= 0x780000;
  y -= 0x600000;
  if (x < work->minX)
    x = work->minX;
  if (x > work->maxX - 0xf00000)
    x = work->maxX - 0xf00000;
  if (y < work->minY)
    y = work->minY;
  if (y > work->maxY - 0xa00000)
    y = work->maxY - 0xa00000;
  work->viewX = x;
  work->viewY = y;

  for (no = 0; no < 3; no++) {
    if (work->enabled[no]) {
      u32 rows = 22;
      u32 row, col, rowmod, colmod, i, j;

      x = Iwram_MulQ16(work->viewX, layer->scaleX);
      y = Iwram_MulQ16(work->viewY, layer->scaleY);
      if (layer->speedX) {
        layer->phaseX += layer->speedX;
        x = (x + layer->phaseX) & ((layer->maskX << 19) | 0x7ffff);
      }
      if (layer->speedY) {
        layer->phaseY += layer->speedY;
        y = (y + layer->phaseY) & ((layer->maskY << 19) | 0x7ffff);
        rows = 32;
      }
      x += layer->offsetX;
      y += layer->offsetY;
      layer++;
      x /= 0x80000;
      y /= 0x80000;
      dest = BG_SCREEN_BLOCK(5) + (no << 11);
      row = ((y / 2) & 127) << 7;
      rowmod = (y & 30) << 5;
      for (i = 0; i < rows / 2; i++) {
        col = (x / 2) & 127;
        colmod = x & 30;
        for (j = 0; j <= 15; j++) {
          u32 *map = (u32 *)Ram_MapCellBuffer;
          map += row + col;
          CopyCameraCell(map, dest, rowmod, colmod);
          col = (col + 1) & 127;
          colmod = (colmod + 2) & 30;
        }
        row = (row + 128) & 0x3f80;
        rowmod = (rowmod + 64) & 0x3c0;
      }
    }
  }
}

// The top left of a layer's visible area, in cells.
struct TilePos {
  s32 x, y;
};

// Copies the block numbers of a rectangle of cells from (srcX, srcY) to
// (dstX, dstY), width by height cells, keeping each destination's
// attributes, and redraws the copied cells that are on screen.
void Map_CopyMetatileIndicesRect(s32 srcX, s32 srcY, u32 dstX, u32 dstY,
                                 u32 width, u32 height) {
  /* FAKEMATCH: needed to match: temporary sharing preserves register allocation
   * and scheduling without changing the frame or instructions. Measured
   * 2026-10-11, before the volatile casts on the two word copies were dropped
   * as unneeded: i = 0 after the offset differs 59, at the top of the body 32;
   * a fresh bytes = cell * 8 is hoisted by loop.c, 158; the scale before the
   * first destination, 4; address expressions without scr and blk, 113-153. A
   * plain body differs 214. */
  u32 *src = (u32 *)Ram_MapCellBuffer + (srcY * 128 + srcX);
  u32 *dst = (u32 *)Ram_MapCellBuffer + (dstY * 128 + dstX);
  struct MapLayerScroll *layer = ((struct MapScrollWork *)gMapWork[0])->layers;
  struct TilePos tile[3];
  s32 i;
  struct TilePos *pos = tile;
  u32 y;
  u32 x;

  for (i = 2; i >= 0; i--) {
    pos->x = layer->x >> 20;
    pos->y = layer->y >> 20;
    pos++;
    layer++;
  }
  for (y = dstY; y < dstY + height; y++) {
    for (x = dstX; x < dstX + width; x++) {
      u32 val = *src++;
      u32 cell = val & 0xfff;
      u32 offset;
      u32 tmp3;
      u32 n;
      u32 tmp6;
      u32 tmp9;
      u32 tmp12;
      u32 tmp11;
      u32 tmp13;
      u32 tmp15;
      tmp9 = *dst;
      tmp3 = tmp9 & -0x1000;
      tmp12 = x & 15;
      dst[0] = cell | tmp3;
      tmp6 = tmp12;
      dst++;
      i = 0;
      tmp15 = y & 15;
      tmp3 = tmp15;
      pos = tile;
      tmp13 = tmp3 << 5;
      tmp12 = tmp13;
      n = tmp12;
      n += tmp6;
      tmp13 = 4 * n;
      tmp11 = tmp13;
      tmp6 = tmp11;
      offset = tmp6;
      for (; i < 3; i++) {
        if (pos->x <= (s32)x && 16 + pos->x > (s32)x && pos->y <= (s32)y &&
            pos->y + 12 > (s32)y) {
          u32 scr, blk;
          u8 *dest, *tiles;

          scr = 0x06002800;
          dest = (u8 *)scr;
          dest += offset;
          tmp11 = cell * 8;
          blk = (u32)Ram_MapBlocks;
          tiles = (u8 *)blk;
          tiles += tmp11;
          *(u32 *)dest = *(u32 *)tiles;

          blk = (u32)Ram_MapBlocks + 4;
          tiles = (u8 *)blk;
          tiles += tmp11;
          scr = 0x06002840;
          dest = (u8 *)scr;
          dest += offset;
          *(u32 *)dest = *(u32 *)tiles;
          break;
        }
        offset += 0x800;
        pos++;
      }
    }
    src += 128 - width;
    dst += 128 - width;
  }
}

struct MapCopyCommand {
  u16 source;
  s16 y, width, height, delay;
} __attribute__((packed, aligned(2)));
typedef char MapCopyCommandSize[(sizeof(struct MapCopyCommand) == 10) ? 1 : -1];

// Plays a map edit sequence (struct MapCopyCommand records, ended by a
// source of 0xffff): copies each record's rectangle of block numbers to
// (destinationX, destinationY) and waits its delay in frames. Doors and
// other changing scenery are drawn this way.
void Map_PlayMetatileCopySequence(u16 *command, s32 destinationX,
                                  u32 destinationY) {
  struct MapCopyCommand *record = (struct MapCopyCommand *)command;
  u32 source = record->source;
  u32 end = 0xffff;
  u32 test = source;

  /* FAKEMATCH: the conditional cancelling updates end CSE before the
   * comparison, preserving its ip copy while source stays in r0. Both
   * updates disappear; removing them loses the copy (53 -> 52 English
   * instructions, five rows differ). */
  if (record) {
    source ^= test;
    source ^= test;
  }
  if (test != end) {
    u32 stop = end;
    s16 *args = (s16 *)(command + 1);

    while (1) {
      s32 sourceY = args[0];
      s32 width = args[1];
      s32 height = args[2];
      s32 delay = args[3];
      args += 5;

      Map_CopyMetatileIndicesRect(source, (u16)sourceY, destinationX,
                                  destinationY, (u16)width, (u16)height);
      record++;
      WaitFrames((u16)delay);
      source = record->source;
      {
        u32 next = source;

        /* FAKEMATCH: the same ip copy after the wait. 2026-10-11: the native
         * compares a copy of source held in ip at both tests; the copy's pseudo
         * must reach regclass with only the copy and the branch as its insns,
         * which CSE's copy swap or combine's substitution removes in every
         * plain form tried (best, a do-while on the field re-read with stop in
         * a local: differ 12). */
        __asm__("" : "+r"(next));
        if (next == stop)
          break;
      }
    }
  }
}

// Map: copying rectangles of cells.

#define MAP_CELLS ((u32 *)gMapCellBuffer)
#define MAP_CELL_TILE_MASK 0x00000fff      // the block (metatile) number
#define MAP_CELL_ATTRIBUTE_MASK 0xfffff000 // the cell's attributes

// Copies a rectangle of cells (16x16 pixels each) as they are, from
// (srcX, srcY) to (dstX, dstY), width by height cells. Where a copied cell
// lies in a layer's visible area (16x12 cells), its block's 2x2 tiles go to
// that layer's BG map in VRAM too.
void Map_CopyMetatileCellsRect(s32 srcX, s32 srcY, s32 width, s32 height,
                               s32 dstX, s32 dstY) {
  s32 y = dstY;
  u32 *src = (u32 *)Ram_MapCellBuffer + (srcY * 128 + srcX);
  u32 *dst = &((u32 *)Ram_MapCellBuffer)[y * 128 + dstX];
  struct MapLayerScroll *layer = ((struct MapScrollWork *)*gMapWork)->layers;
  struct TilePos view[3];
  s32 i;
  struct TilePos *pos = view;
  s32 x;

  // The three layers' visible areas: their scroll positions in cells.
  i = 2;
  while (i >= 0) {
    s32 tmp5;

    tmp5 = layer->x >> 20;
    pos->x = tmp5;
    tmp5 = (*layer).y;
    pos->y = tmp5 >> 20;
    pos++;
    layer++;
    i--;
  }
  if (y < dstY + height) {
    do {
      for (x = dstX; x < dstX + width; x++) {
        u32 val = *src++;
        u32 cell;
        u32 offset;
        u32 tmp3;
        u32 *dest;
        u32 n;
        u32 *tiles;
        u32 tmp6;
        u32 tmp12;
        u32 tmp13;

        dst[0] = val;
        cell = val & 0xfff;
        tmp12 = x & 15;
        tmp6 = tmp12;
        dst += 1;
        tmp3 = y & 15;
        tmp13 = tmp3 << 5;
        pos = view;
        tmp12 = tmp13;
        n = tmp12;
        n += tmp6;
        tmp13 = n * 4;
        i = 0;
        tmp6 = tmp13;
        offset = tmp6;
        // In a layer's visible area, write its BG map in VRAM too (0x800
        // bytes per layer).
        for (; i < 3; i += 1) {
          if (pos->x <= x && 16 + (*pos).x > x && pos->y <= y &&
              pos->y + 12 > y) {
            // The block's (8 bytes) top two tiles, and its bottom two 0x40
            // bytes further on.
            u32 *tmp;
            u32 tmp2;
            u32 tmp4;
            u32 *tmp7;
            u32 tmp10;
            u32 tmp14;
            u32 tmp16;
            /* FAKEMATCH: restoring the unscaled cell keeps loop.c's
             * shift inside the successful layer branch. The restoration
             * folds away, preserving the native 36-byte frame and registers.
             * The copy chain through tmp2 to tmp17 sets the registers and
             * stack slots: written as two plain word stores, tbs-en differs. */
            u32 bytes = cell * 8;
            u32 tmp8;
            u32 tmp17;

            cell = bytes / 8;
            tmp = (u32 *)Ram_MapBlocks;
            tmp8 = 0x06002800 + offset;
            tiles = tmp;
            tmp2 = tmp8;
            tmp = (u32 *)tmp2;
            dest = tmp;
            tiles = (u32 *)((u8 *)tiles + bytes);
            tmp16 = *tiles;
            tmp14 = tmp16;
            tmp17 = 0x06002840 + offset;
            tmp10 = tmp14;
            *dest = tmp10;
            tmp7 = (u32 *)(Ram_MapBlocks + 4);
            tiles = tmp7;
            tiles = (u32 *)((u8 *)tiles + bytes);
            tmp8 = tmp17;
            tmp14 = tmp8;
            tmp17 = tiles[0];
            tmp16 = tmp17;
            tmp10 = tmp16;
            tmp4 = tmp10;
            tmp2 = tmp4;
            tmp4 = tmp14;
            tmp7 = (u32 *)tmp4;
            dest = tmp7;
            *dest = tmp2;
            break;
          }
          n = 0x800; // the next layer's BG map
          offset += n;
          pos++;
        }
      }
      src += (128 - width);
      dst = 128 - width + dst;
      y++;
    } while (y < dstY + height);
  }
}

// Copies only the attributes of a rectangle of cells, from (srcX, srcY) to
// (dstX, dstY), width by height cells, keeping each destination cell's
// block number.
void Map_CopyCellAttributeRect(s32 srcX, s32 srcY, s32 width, s32 height,
                               s32 dstX, s32 dstY) {
  u32 *dst = MAP_CELLS;
  u32 *src = dst;
  u16 row;
  u16 col;

  dst += (dstY << 7) + dstX;
  src += (srcY << 7) + srcX;

  for (row = 0; row < height; row++) {
    u32 *dstCell = dst + (row << 7);
    u32 *srcCell = src + (row << 7);

    for (col = 0; col < width; col++) {
      *dstCell = (*dstCell & MAP_CELL_TILE_MASK) |
                 (*srcCell++ & MAP_CELL_ATTRIBUTE_MASK);
      dstCell++;
    }
  }
}

// Copies only the block numbers of a rectangle of cells, from (srcX, srcY)
// to (dstX, dstY), width by height cells, keeping each destination cell's
// attributes; cells in a visible area update the BG maps too.
void Map_CopyCellIndexRect(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX,
                           s32 dstY) {
  /* FAKEMATCH: needed to match: word/index temporaries preserve register
   * allocation; the plain body allocates differently and uses a 40-byte frame.
   * Measured 2026-10-11, before the volatile casts on the two word copies were
   * dropped as unneeded: i = 0 after pos = view differs 38; the scale into n,
   * 52; the scale before the first destination, 54. A plain body differs 204.
   */
  s32 y;
  u32 *src = (u32 *)Ram_MapCellBuffer + (srcY * 128 + srcX);
  u32 *dst = &((u32 *)Ram_MapCellBuffer)[dstY * 128 + dstX];
  struct MapLayerScroll *layer = ((struct MapScrollWork *)*gMapWork)->layers;
  struct TilePos view[3];
  s32 i;
  struct TilePos *pos = view;
  s32 x;

  // The three layers' visible areas: their scroll positions in cells.
  i = 2;
  while (i >= 0) {
    s32 tmp5;

    tmp5 = layer->x >> 20;
    pos->x = tmp5;
    tmp5 = (*layer).y;
    pos->y = tmp5 >> 20;
    pos++;
    layer++;
    i--;
  }
  y = dstY;
  if (y < dstY + height) {
    do {
      for (x = dstX; x < dstX + width; x++) {
        u32 val = *src++;
        u32 cell;
        u32 offset;
        u32 tmp3;
        u32 n;
        u32 tmp6;
        u32 tmp12;
        u32 tmp13;

        cell = val & MAP_CELL_TILE_MASK;
        dst[0] = (dst[0] & MAP_CELL_ATTRIBUTE_MASK) | cell;
        tmp12 = x & 15;
        tmp6 = tmp12;
        dst += 1;
        i = 0;
        tmp3 = y & 15;
        tmp13 = tmp3 << 5;
        pos = view;
        tmp12 = tmp13;
        n = tmp12;
        n += tmp6;
        tmp13 = n * 4;
        tmp6 = tmp13;
        offset = tmp6;
        // In a layer's visible area, write its BG map in VRAM too (0x800
        // bytes per layer).
        for (; i < 3; i += 1) {
          if (pos->x <= x && 16 + (*pos).x > x && pos->y <= y &&
              pos->y + 12 > y) {
            // The block's (8 bytes) top two tiles, and its bottom two 0x40
            // bytes further on.
            u32 scr, blk;
            u8 *dest, *tiles;

            scr = 0x06002800;
            dest = (u8 *)scr;
            dest += offset;
            tmp13 = cell * 8;
            blk = (u32)Ram_MapBlocks;
            tiles = (u8 *)blk;
            tiles += tmp13;
            *(u32 *)dest = *(u32 *)tiles;

            blk = (u32)Ram_MapBlocks + 4;
            tiles = (u8 *)blk;
            tiles += tmp13;
            scr = 0x06002840;
            dest = (u8 *)scr;
            dest += offset;
            *(u32 *)dest = *(u32 *)tiles;
            break;
          }
          n = 0x800; // the next layer's BG map
          offset += n;
          pos++;
        }
      }
      src = src + (128 - width);
      dst = 128 - width + dst;
      y++;
    } while (y < dstY + height);
  }
}

// Sets bits 9-11 of the map's flags from v, keeping the others.
void Map_SetWorkFlagBits9To11(u32 v) {
  struct MapState *state = gMapWork[0];
  u32 mask = v & 0xe00;
  u32 flags = state->flags;

  flags = (flags & 0xf1ff) | mask;
  state->flags = flags;
}

// Writes a halfword register (value first).
static __inline__ void Io_Set16(s32 value, volatile u16 *reg) { *reg = value; }

// Writes a halfword register (register first).
static __inline__ void Io_Put16(volatile u16 *reg, s32 value) { *reg = value; }

typedef s32 (*PlaneFn)(void *camera, s32 *position, void *lines, void *out);

#define ABS(v) ((v) < 0 ? -(v) : (v))

struct WorldCell {
  u32 tile : 24;
  u32 kind : 7;
  u32 flag : 1;
};

#define BG_PALETTE ((s16 *)BG_PLTT)

#define WORLD_CELLS ((struct WorldCell *)Ram_MapBlocks)

// Loads world map tile `tile` into quarter (x & 1, y & 1) of a layer's block
// buffer, decompressing it from the area's graphics, and with update set
// also draws it into the layer's BG map. Returns 0 when that quarter already
// shows the tile (and update is 0), else 1.
s32 Map_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update) {
  struct PerspectiveWork *work = gMapWork[0];
  u32 *graphics = work->tiles;
  u16 *shown;
  u16 *buffer;
  u16 *source;
  u8 *blocks;
  u16 *screen;
  u32 i;
  u32 j;
  u32 id;

  x &= 1;
  y &= 1;
  shown = &work->quarterTiles[(layer * 2 + y) * 2 + x];
  if (update == 0 && tile == *shown)
    return 0;
  *shown = tile;
  buffer = Runtime_AllocateHeapBlock(14, 0x400);
  Resource_DecodeByteLz((u8 *)graphics + graphics[tile], buffer);

  source = buffer;
  blocks = Ram_MapBlocks + (((layer * 2 + y) * 32 + x) << 6);
  for (i = 0; i < 16; i++) {
    Dma_Set(source, blocks, DMA_ENABLE32 | DMA_32BIT | 0x10, REG_DMA3);
    source += 32;
    blocks += 128;
  }

  if (update != 0) {
    screen = (u16 *)(BG_SCREEN_BLOCK(8) + ((((layer * 2 + y) << 6) + x) << 5));
    source = buffer;
    for (i = 0; i < 16; i++) {
      for (j = 0; j < 16; j++) {
        u16 *bottom;

        id = *source;
        screen[0] = *(u16 *)(Ram_MapCellBuffer + id * 4);
        bottom = (u16 *)(Ram_MapCellBuffer + 2);
        bottom = (u16 *)((u8 *)bottom + id * 4);
        screen[32] = *bottom;
        screen++;
        source += 2;
      }
      screen += 48;
    }
  }
  Runtime_ReleaseHeapBlock(14);
  return 1;
}

// Sets up the world map's perspective view: BG2 and BG3 in affine mode, the
// camera work, the projection, the per-scanline ground plane routine in RAM
// (Transform_UpdateVertices) and its first frame, and the per-frame view and
// HBlank updates.
s32 Map_InitializePerspectiveScene(void) {
  struct PerspectiveWork *work;
  struct BattleCamera *camera;
  struct MapFrameWork *tiles;
  struct MapAffinePair(*lines)[160];
  s32 *position;
  s32 *distance;
  u16 *yaw;
  u16 *pitch;
  u16 *turn;
  /* FAKEMATCH: the fill word is this function's own volatile local, filled by
   * Dma_Fill32Cell, so it takes the native's frame slot at sp+28; plain
   * Dma_Fill32's inner word lands at sp+16 and the slots after it shift (differ
   * 4 in all six editions, measured 2026-10-13). */
  volatile u32 fill;
  struct FixedPointPosition vector;
  s32 farPlane;
  s32 i;

  REG_DISPCNT &= ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON |
                   DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
  Blend_SetDarkenTarget0(0);
  work = Runtime_AllocateHeapBlock(8, sizeof(struct PerspectiveWork));
  Dma_Fill32Cell(fill, 0, work, sizeof(struct PerspectiveWork));
  work->viewX = 0;
  work->viewY = 0;
  work->scaleX = 0x200000;
  work->scaleY = 0x400000;
  work->limitX = 0x1fe00000;
  work->limitY = 0x1fe00000;
  work->script = NULL;
  work->tiles = Resource_GetTableEntry((s32)&ResourceId_PerspectiveDataA);
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_DefaultMapAnimation),
      Ram_MapCollision + 0x1000);
  MapAnimation_StartChannels((const u16 *)(Ram_MapCollision + 0x1000));
  Io_Set16(0x3f9e, &REG_BLDCNT);
  Io_Set16(0x1010, &REG_BLDALPHA);
  REG_BLDY = 0;
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_DefaultMapCells),
      Ram_MapCellBuffer);
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_DefaultMetatileAttributes),
      Ram_MapCollision);
  work->fade = 0x1f00;
  work->fadeStep = 0x80;
  Io_Set16(0xa80a, &REG_BG3CNT);
  Io_Set16(0xaa0e, &REG_BG2CNT);
  Io_Set16(0x0501, &REG_BG1CNT);
  Io_Write16(0x100, &REG_BG2PA);
  REG_BG2PB = 0;
  REG_BG2PC = 0;
  /* FAKEMATCH: one-pass block; its loop notes keep the 0x100 for BG2PD built
   * just before its store and the 12 for Runtime_AllocateBlock loaded before
   * the 76. Without it differ 6, whether the three writes are Io_Put16,
   * Io_Set16 or plain stores; Io_Write16 for BG2PD (itself a one-pass macro)
   * differ 2. */
  do {
    Io_Put16(&REG_BG2PD, 0x100);
    REG_BG2X = 0;
    REG_BG2Y = 0;
    Io_Put16(&REG_BG3PA, 0x100);
    REG_BG3PB = 0;
    REG_BG3PC = 0;
    Io_Put16(&REG_BG3PD, 0x100);
    REG_BG3X = 0;
    REG_BG3Y = 0;
  } while (0);

  camera = Runtime_AllocateBlock(12, sizeof(struct BattleCamera));
  tiles = Runtime_AllocateHeapBlock(7, sizeof(struct MapFrameWork));
  position = camera->pos;
  lines = tiles->pages;
  farPlane = 0x1fe0000;
  work->farPlane = farPlane;
  distance = &work->distance;
  *distance = farPlane;
  work->zoom = 0x10000;
  turn = &work->zoomTransitionFrames;
  camera->unk18 = 0;
  camera->followPos = 0;
  *turn = 0;
  gProjection.centerX = 120;
  gProjection.centerY = 96;
  Camera_StoreSceneParameters(farPlane, farPlane >> 1, farPlane << 1);
  position[0] = 0;
  position[1] = 0;
  position[2] = 0;
  Render_ResetTransformState();
  SceneTransform_ApplyPosition(position);
  yaw = &work->yaw;
  SceneTransform_ApplyYaw(*yaw);
  pitch = &work->pitch;
  SceneTransform_ApplyPitch(*pitch);
  vector.x = 0;
  vector.y = 0;
  vector.z = farPlane;
  Iwram_TransformVector((const s32 *)&vector, (s32 *)camera);
  Render_ResetTransformState();
  Graphics_PrepareTransferInIwramWork(camera->eye, position);
  RamCode_LoadSlot(46, Transform_UpdateVertices, &Transform_UpdateVerticesSize);
  WorldMap_BuildScanlineTable(
      Iwram_RatioMulQ14(Trig_Cos(*pitch), Trig_Sin(*pitch)), position,
      tiles->scanlines);
  gScanlineAngle = 0;
  gScanlinePitch = work->pitch;
  ((PlaneFn)gWorkSlot[46])(camera, position, tiles->scanlines,
                           lines[gFrameCount & 1]);
  position[0] = 0;
  position[1] = 0;
  position[2] = 0;
  Render_ResetTransformState();
  Io_Put16(pitch, 0xe000);
  *yaw = 0;
  Render_ResetTransformState();
  SceneTransform_ApplyPosition(position);
  SceneTransform_ApplyYaw(*yaw);
  SceneTransform_ApplyPitch(*pitch);
  vector.x = 0;
  vector.y = 0;
  vector.z = *distance + 0x10000;
  Iwram_TransformVector((s32 *)&vector, camera->eye);
  REG_MOSAIC = 0;
  Io_Write16(0x42, &REG_DISPCNT);
  gBgScroll[1].x = 0;
  gBgScroll[1].y = 0;
  gBgScroll[2].x = 0;
  gBgScroll[2].y = 0;
  gBgScroll[3].x = 0;
  gBgScroll[3].y = 0;
  work->windowTop = 0;
  work->windowBottom = 159;
  Scheduler_AddOrUpdateCallback(WorldMap_UpdateView, 0xc85);
  Scheduler_AddOrUpdateCallback(MapAnimation_ApplyAffineFrame, 0x480);
  for (i = 255; i >= 0; i--)
    work->tileIds[i] = i;
}

// Puts world map tile (x / 16, y / 16) at cell (px / 16, py / 16) of the
// tile id grid, and draws it at once when it is next to the view.
void Map_SetWindowCellTile(s32 x, s32 y, s32 px, s32 py) {
  struct PerspectiveWork *window;
  s32 *position;
  s32 originX;
  s32 originY;
  s32 tile;

  window = gMapWork[0];
  originX = 0;
  originY = 0;
  position = window->origin;
  if (position != 0) {
    originX = *position++;
    originY = position[1];
  }

  originX >>= 24;
  originY >>= 24;
  x >>= 4;
  y >>= 4;
  px >>= 3;
  py >>= 3;
  tile = (y << 4) + x;
  window->tileIds[(((py / 2) & 15) << 4) + ((px / 2) & 15)] = tile;

  if (ABS(originX - px) <= 1 && ABS(originY - py) <= 1) {
    Map_WriteLayerCellTile(0, px / 2, py / 2, tile, 1);
    Map_WriteLayerCellTile(1, px / 2, py / 2, tile + 0x140, 1);
  }
}

// Loads the world map's palette and tiles for the area at (x, z): the second
// set where the ground there is terrain kind 21. Fills the BG maps, and for
// the second set swaps in its own tiles at ids 10-12 and 26-28.
void WorldMap_LoadGraphics(s32 x, s32 z) {
  struct PerspectiveWork *state;
  u32 *resources;
  u8 *buffer;
  u8 *tiles;
  u32 *cursor;
  u32 tile;
  s32 variant;
  s32 row;
  s32 col;
  s16 value;

  variant = 0;
  buffer = MemAlloc(0x200);
  state = gMapWork[0];
  if (WORLD_CELLS[((x / 0x200000) & 31) + (((z / 0x200000) & 31) << 5)].kind ==
      21)
    variant = 1;
  resources = gWorldMap_GraphicsResources[variant];
  state->resources = resources;
  value = BG_PALETTE[0];
  Resource_DecodeByteLz(Resource_GetTableEntry(resources[0]), buffer);
  *(s16 *)buffer = value;
  Dma_Set(buffer, BG_PALETTE, DMA_ENABLE32 | DMA_32BIT | 0x70, REG_DMA3);
  tiles = gBgTileBuffer;
  Resource_DecodeType01(Resource_GetTableEntry(resources[1]), tiles);
  Dma_Set(tiles, BG_CHAR_BLOCK(2), DMA_ENABLE32 | DMA_32BIT | 0x800, REG_DMA3);
  Resource_DecodeType01(Resource_GetTableEntry(resources[2]),
                        Ram_BgTileBuffer + 0x2000);
  Dma_Set(Ram_BgTileBuffer + 0x2000, BG_CHAR_BLOCK(2) + 0x2000,
          DMA_ENABLE32 | DMA_32BIT | 0x800, REG_DMA3);
  Resource_DecodeType01(Resource_GetTableEntry(resources[3]),
                        Ram_BgTileBuffer + 0x4000);
  Dma_Set(Ram_BgTileBuffer + 0x4000, BG_CHAR_BLOCK(3),
          DMA_ENABLE32 | DMA_32BIT | 0x800, REG_DMA3);
  Resource_DecodeType01(Resource_GetTableEntry(resources[4]),
                        Ram_BgTileBuffer + 0x6000);
  Dma_Set(Ram_BgTileBuffer + 0x6000, BG_CHAR_BLOCK(3) + 0x2000,
          DMA_ENABLE32 | DMA_32BIT | 0x800, REG_DMA3);
  Resource_DecodeType01(Resource_GetTableEntry(resources[5]), gMapLayerData);
  Dma_Fill32(0xf07ff07f, BG_SCREEN_BLOCK(5), 0x600);
  cursor = (u32 *)BG_SCREEN_BLOCK(6);
  tile = 0x01a901a8;
  for (row = 0; row < 20; row++) {
    for (col = 0; col < 15; col++) {
      *cursor++ = tile;
      tile += 0x00020002;
    }
    cursor++;
  }
  if (variant == 1) {
    state->tileIds[10] = 0x10a;
    state->tileIds[11] = 0x10b;
    state->tileIds[12] = 0x10c;
    state->tileIds[26] = 0x11a;
    state->tileIds[27] = 0x11b;
    state->tileIds[28] = 0x11c;
    Map_UpdateCurrentTileBlock();
  }
  MemFree(buffer);
}

// HBlank setup each frame on the world map: copies the first scanline's BG2
// and BG3 affine parameters, starts the HBlank DMA for the rest, and turns
// the window on or off for the window's top and bottom.
void MapAnimation_ApplyAffineFrame(void) {
  struct MapAffinePair(*lines)[160] =
      ((struct MapFrameWork *)WORK_SLOT(7))->pages;
  s32 dispcnt = (s16)(REG_DISPCNT & 0xfff8);
  u32 *dst = (u32 *)&REG_BG2PA;
  struct PerspectiveWork *work = WORK_SLOT(8);
  volatile u16 *channel = REG_DMA0;
  u32 *src;
  u32 mode;
  u32 top;
  u32 control;

  channel[5] &= 0xc5ff;
  channel[5] &= 0x7fff;
  (void)channel[5];
  if (lines != NULL) {
    src = (u32 *)lines[gFrameCount & 1];
    *dst++ = *src++;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst = *src++;
    Dma_Set(src, (void *)&REG_BG2PA, 0xa6600008, (volatile u32 *)channel);
  }
  work->shownTop = work->windowTop;
  work->shownBottom = work->windowBottom;
  top = work->shownTop;
  mode = 0;
  if (top < 200) {
    u16 bottom = work->shownBottom;

    mode = (bottom != 0) << 1;
    if (top <= bottom) {
      mode = 0;
      if (top == 0)
        mode = 2;
    }
  }
  dispcnt |= mode;
  control = (u16)dispcnt;
  REG_DISPCNT = control;
  work->windowPending = 0;
}

// Copies one world map cell's two tile entries (top and bottom) to a BG map.
static __inline__ void CopyEntry(u8 *map, u8 *destination) {
  u32 palette = *(u16 *)map;
  u16 *colors = (u16 *)Ram_MapCellBuffer;

  colors += palette * 2;
  *(u16 *)destination = *colors++;
  *(u16 *)(destination + 64) = *colors;
}

// Draws world map row `value` (in tiles) into the BG map: 32 cells, then 32
// more a screen block on.
void Map_RenderPaletteMappedRow(u32 value) {
  u8 *map;
  u8 *destination;
  u32 counter;

  map = Ram_MapBlocks + ((((s32)value / 2) & 31) << 7);
  destination = (u8 *)(0x06004000 + ((value & 62) << 6));
  counter = 0;
  do {
    CopyEntry(map, destination);
    counter++;
    destination += 2;
    map += 4;
  } while (counter <= 31);

  destination += 0xfc0;
  map += 0xf80;
  counter = 0;
  do {
    CopyEntry(map, destination);
    counter++;
    destination += 2;
    map += 4;
  } while (counter <= 31);
}

// Draws world map column `value` (in tiles) into the BG map: 64 cells down.
void Map_RenderPaletteMappedColumn(u32 value) {
  u8 *map;
  u8 *destination;
  u32 counter;

  map = Ram_MapBlocks + ((((s32)value / 2) & 31) << 2);
  destination = (u8 *)(0x06004000 + (value & 62));
  counter = 0;
  do {
    CopyEntry(map, destination);
    counter++;
    destination += 128;
    map += 128;
  } while (counter <= 63);
}

// Each frame on the world map: loads tiles as the view moves, shakes and
// follows the player, draws the BG row or column that scrolls into view,
// moves the camera, and builds the ground plane's scanline table.
void WorldMap_UpdateView(void) {
  struct BattleCamera *cam = WORK_SLOT(HEAP_SLOT_CAMERA);
  struct MapFrameWork *map = WORK_SLOT(7);
  struct PerspectiveWork *view = WORK_SLOT(8);
  s32 *pos = cam->pos;
  s32 *target = view->origin;
  struct MapAffinePair(*buffer)[160] = map->pages;
  s32 distance = view->farPlane;
  s32 height = view->distance;
  struct FixedPointPosition local;

  Map_UpdateCurrentTileBlockUntilBlocked();
  if (target != NULL) {
    s32 x;
    s32 y;
    s32 col;
    s32 row;

    y = target[2];
    x = target[0];
    if (view->shakeX != 0) {
      s32 r = Random16();
      s32 amp;

      r -= Random16();
      amp = view->shakeX;
      x += Iwram_MulQ16(amp, r);
      view->shakeX = Iwram_MulQ16(amp, view->shakeDecay);
    }
    if (view->shakeY != 0) {
      s32 r = Random16();
      s32 amp;

      r -= Random16();
      amp = view->shakeY;
      y += Iwram_MulQ16(amp, r);
      view->shakeY = Iwram_MulQ16(amp, view->shakeDecay);
    }
    col = x / 0x100000;
    row = y / 0x100000;
    if ((view->viewX ^ x) & 0x100000) {
      if (view->viewX < x)
        Map_RenderPaletteMappedColumn(col + 16);
      else
        Map_RenderPaletteMappedColumn(col - 16);
    }
    if ((view->viewY ^ y) & 0x100000) {
      if (view->viewY < y)
        Map_RenderPaletteMappedRow(row + 12);
      else
        Map_RenderPaletteMappedRow(row - 18);
    }
    view->viewX = x;
    view->viewY = y;
  }
  gProjection.centerX = 120;
  gProjection.centerY = 96;
  Camera_StoreSceneParameters(distance, height / 2, height * 2);
  pos[0] = *target++;
  pos[1] = 0;
  pos[2] = target[1];
  Render_ResetTransformState();
  SceneTransform_ApplyPosition(pos);
  SceneTransform_ApplyYaw(view->yaw);
  SceneTransform_ApplyPitch(view->pitch);
  local.x = 0;
  local.y = 0;
  local.z = height + 0x10000;
  Iwram_TransformVector((s32 *)&local, (s32 *)cam);
  Render_ResetTransformState();
  Graphics_PrepareTransferInIwramWork(cam, pos);
  if (gScanlinePitch != view->pitch) {
    s32 c = Trig_Cos(view->pitch);
    s32 s = Trig_Sin(view->pitch);

    WorldMap_BuildScanlineTable(Iwram_RatioMulQ14(c, s), pos, map->scanlines);
    gScanlineAngle = 0;
    gScanlinePitch = view->pitch;
  }
  ((s32 (*)(void *, s32 *, void *, void *))gWorkSlot[46])(
      cam, pos, map->scanlines, buffer[gFrameCount & 1]);
}

// Loads and draws the 2x2 world map tiles around the player in both layers.
void Map_UpdateCurrentTileBlock(void) {
  struct PerspectiveWork *window = gMapWork[0];
  s32 x0 = 0;
  s32 y0 = 0;
  u32 layer;
  u32 row;
  u32 col;
  s32 bias;
  s32 tile;

  if (window->origin != NULL) {
    s32 *p = window->origin;

    x0 = *p++;
    y0 = p[1];
  }
  x0 = (x0 - 0x1000000) >> 25;
  y0 = (y0 - 0x1400000) >> 25;
  layer = 0;
  bias = 0;
  for (; layer < 2; layer++) {
    for (row = 0; row < 2; row++) {
      for (col = 0; col < 2; col++) {
        tile = (((y0 + row) & 15) << 4) + ((x0 + col) & 15);
        tile = window->tileIds[tile];
        tile += bias;
        Map_WriteLayerCellTile(layer, x0 + col, y0 + row, tile, 1);
      }
    }
    bias += 320;
  }
}

// Loads the first of the 2x2 world map tiles around the player that is not
// shown yet (at most one a frame), without drawing it.
void Map_UpdateCurrentTileBlockUntilBlocked(void) {
  struct PerspectiveWork *window = gMapWork[0];
  s32 x0 = 0;
  s32 y0 = 0;
  u32 layer;
  u32 row;
  u32 col;
  s32 tile;

  if (window->origin != NULL) {
    s32 *p = window->origin;

    x0 = *p++;
    y0 = p[1];
  }
  x0 = (x0 - 0x1000000) >> 25;
  y0 = (y0 - 0x1400000) >> 25;
  for (layer = 0; layer < 2; layer++) {
    for (row = 0; row < 2; row++) {
      for (col = 0; col < 2; col++) {
        tile = (((y0 + row) & 15) << 4) + ((x0 + col) & 15);
        tile = window->tileIds[tile];
        tile += layer * 320;
        if (Map_WriteLayerCellTile(layer, x0 + col, y0 + row, tile, 0) != 0)
          return;
      }
    }
  }
}

// VBlank: shows BG1 and copies the decoded frame's tiles into VRAM.
void MapAnimation_PresentFrame(void) {
  Io_Write16(0x682, &REG_BG1CNT);
  Dma_Set(gMapCellBuffer, BG_CHAR_BLOCK(1) + 0x2a00,
          DMA_ENABLE32 | DMA_32BIT | 0x2580, REG_DMA3);
}

// Starts a full-screen BG1 animation on the world map: saves BG1's tiles,
// decodes the first frame and hands the window and VBlank to it.
void MapAnimation_Start(void) {
  struct MapFrameWork *pages = WORK_SLOT(7);
  struct PerspectiveWork *work = WORK_SLOT(8);
  s32 one = 1;

  work->animationActive = one;
  Scheduler_EnableCallbacks(MapAnimation_Update);
  Dma_Set(BG_CHAR_BLOCK(1), gDecodeBuffer, DMA_ENABLE32 | DMA_32BIT | 0x800,
          REG_DMA3);
  WaitFrames(1);
  Resource_RunCopiedDecoder((s32)pages->pages[gFrameCount & one],
                            (s32)gMapCellBuffer);
  work->windowTop = 200;
  work->windowBottom = 255;
  gVBlankHook = MapAnimation_PresentFrame;
}

// VBlank: shows BG1 and copies the area's tiles back into VRAM.
void Map_ShowBg1FromBuffer(void) {
  Io_Write16(0x501, &REG_BG1CNT);
  Dma_Set(gBgTileBuffer, BG_CHAR_BLOCK(2), 0x84002000, REG_DMA3);
}

// Reloads the world map area's palette and tiles after a BG1 animation, puts
// the default cells back and stops the animation.
void Map_LoadAreaGraphics(void) {
  struct PerspectiveWork *state;
  u32 *resources;
  u8 *buffer;
  s16 value;

  state = gMapWork[0];
  buffer = (u8 *)gMapCellBuffer;
  resources = state->resources;
  value = *(s16 *)BG_PLTT;
  Resource_DecodeByteLz(Resource_GetTableEntry(resources[0]), buffer);
  *(s16 *)buffer = value;
  Dma_Set(buffer, (void *)BG_PLTT, DMA_ENABLE32 | DMA_32BIT | 0x70, REG_DMA3);
  Resource_DecodeType01(Resource_GetTableEntry(resources[1]), gBgTileBuffer);
  Resource_DecodeType01(Resource_GetTableEntry(resources[2]),
                        Ram_BgTileBuffer + 0x2000);
  Resource_DecodeType01(Resource_GetTableEntry(resources[3]),
                        Ram_BgTileBuffer + 0x4000);
  Resource_DecodeType01(Resource_GetTableEntry(resources[4]),
                        Ram_BgTileBuffer + 0x6000);
  gVBlankHook = Map_ShowBg1FromBuffer;
  state->windowTop = 0;
  state->windowBottom = 159;
  WaitFrames(1);
  Resource_DecodeType01(
      Resource_GetTableEntry((u32)&ResourceId_DefaultMapCells), buffer);
  Map_UpdateCurrentTileBlock();
  state->animationActive = 0;
  Scheduler_DisableCallbacks(MapAnimation_Update);
  WaitFrames(1);
}

// Puts the default world map cells back after a BG1 animation and stops it.
void Map_LoadDefaultCellsAndUpdateBlock(void) {
  struct PerspectiveWork *work = gMapWork[0];
  gVBlankHook = Map_ShowBg1FromBuffer;
  work->windowTop = 0;
  work->windowBottom = 0x9f;
  WaitFrames(1);
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_DefaultMapCells), gMapCellBuffer);
  Map_UpdateCurrentTileBlock();
  Scheduler_DisableCallbacks(MapAnimation_Update);
  WaitFrames(1);
}

// Each frame: runs the 16 tile animation channels. A channel script is
// groups of four halfwords (source tile, count, destination tile, frames to
// wait) that copy tiles into VRAM; 0xfeNN jumps to group NN, 0xfeff stops,
// and 0xffff loops to the start.
void MapAnimation_Update(void) {
  /* FAKEMATCH: each channel is written (state->anim + i), not through a channel
   * pointer: a pointer anim = state->anim + i (or &state->anim[i], declared in
   * either scope) differs 4, ip and lr swapping the 0xffff sentinel and the
   * VRAM base; a walking anim++ differs 8; state->anim[i]. differs 167, no
   * walking pointer at all. */
  struct MapState *state = gMapWork[0];
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
          Dma_Set(Ram_DecodeBuffer + op * 32, BG_CHAR_BLOCK(1) + dst * 32,
                  DMA_ENABLE32 | DMA_32BIT | count * 8, REG_DMA3);
        else
          Dma_Set(BG_CHAR_BLOCK(1) + op * 32, BG_CHAR_BLOCK(1) + dst * 32,
                  DMA_ENABLE32 | DMA_32BIT | count * 8, REG_DMA3);
      } else {
        if (op >= 0x200)
          Dma_Set(Ram_MapBlocks + op * 64, BG_CHAR_BLOCK(2) + dst * 64,
                  DMA_ENABLE32 | DMA_32BIT | count * 16, REG_DMA3);
        else
          Dma_Set(BG_CHAR_BLOCK(2) + op * 64, BG_CHAR_BLOCK(2) + dst * 64,
                  DMA_ENABLE32 | DMA_32BIT | count * 16, REG_DMA3);
      }
      (state->anim + i)->cursor += 4;
      goto next;
    } else {
      (state->anim + i)->timer--;
    }
  }
}

// Resumes tile animation channel no.
void Map_ClearLayerEntryFlag(u32 no) {
  struct MapState *state = gMapWork[0];
  struct MapAnimation *anim = &state->anim[no];

  anim->paused = 0;
}

// Pauses tile animation channel no.
void Map_SetLayerEntryFlag(u32 no) {
  struct MapState *state = gMapWork[0];
  struct MapAnimation *anim = &state->anim[no];

  anim->paused = 1;
}

// Starts the tile animation channels a map's animation script lists: each
// 0xfdNN marker starts channel NN & 15 there (paused when bit 7 is set).
void MapAnimation_StartChannels(const u16 *script) {
  struct MapState *state;
  s32 count;
  u32 command;

  state = gMapWork[0];
  count = 0;
  Dma_Fill32(0, state->anim, sizeof(state->anim));
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
  if (count != 0)
    Scheduler_AddOrUpdateCallback(MapAnimation_Update, 0xc80);
}

// Resumes the tile animations, unless a BG1 animation is running.
void Map_EnableUpdateCallback(void) {
  if (((struct MapScrollWork *)gMapWork[0])->animationActive == 0)
    Scheduler_EnableCallbacks(MapAnimation_Update);
}

// Stops the tile animations, unless a BG1 animation is running.
void Map_DisableUpdateCallback(void) {
  if (((struct MapScrollWork *)gMapWork[0])->animationActive == 0)
    Scheduler_DisableCallbacks(MapAnimation_Update);
}

// Each frame: runs the map's blend script. A 0x3NNN command sets BLDCNT;
// any other sets BLDALPHA (or BLDY when not alpha blending) and waits the
// halfword after it in frames; 0xfeNN jumps, 0xfeff stops and 0xffff loops.
void DisplayBlend_RunScript(void) {
  struct MapState *work = gMapWork[0];
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
  if (test == 0) {
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
  }
  state->delay = value + 0xffff;
}

// Starts a map's blend script, unless it is empty.
void DisplayBlend_StartScript(u16 *script) {
  struct BlendScriptState *state;
  s32 started;
  volatile u32 zero;

  started = 0;
  zero = 0;
  state = &((struct MapState *)gMapWork[0])->blend;
  Dma_Set((const void *)&zero, state, 0x85000003, REG_DMA3);
  if (*script != 0xffff) {
    state->script = script;
    state->cursor = script;
    state->delay = 0;
    state->paused = 0;
    started = 1;
  }
  if (started)
    Scheduler_AddOrUpdateCallback(DisplayBlend_RunScript, 0xc80);
}

// Resumes the blend script.
void DisplayBlend_EnableRunScript(void) {
  Scheduler_EnableCallbacks(DisplayBlend_RunScript);
}

// Stops the blend script.
void DisplayBlend_DisableRunScript(void) {
  Scheduler_DisableCallbacks(DisplayBlend_RunScript);
}

// Allocates the palette cycling queue (heap slot 28) with its four entries
// cleared.
void Runtime_AllocateAndClearQueue(void) {
  struct PaletteSnapshotWork *queue;
  struct PaletteSnapshot *entry;
  u16 i;
  u16 j;

  queue = Runtime_AllocateBlock(28, sizeof(struct PaletteSnapshotWork));
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

// Adds a palette cycle: count colours of background palette bank from
// colour index, rotated by one every delay + 1 frames. Returns 0, or -1
// when all four cycles are taken.
s32 PaletteQueue_Add(s16 bank, s16 index, s16 delay, s16 count) {
  struct PaletteSnapshotWork *work;
  struct PaletteSnapshot *entry;
  u16 *source;
  u32 slot;
  u16 size;

  work = gPaletteWork;
  slot = work->count;
  if (slot > 3)
    return -1;
  entry = &work->entries[slot];
  source = (u16 *)BG_PLTT + (((u16)bank << 4) + (u16)index);
  size = count;
  entry->position = 0;
  entry->timer = 0;
  entry->count = size;
  entry->source = source;
  entry->delay = delay;
  Dma_Set(source, entry->colors, 0x80000000 | size, REG_DMA3);
  work->count++;
  return 0;
}

// Stops the palette cycles and frees their queue (heap slot 28).
void Runtime_ScheduleCallbackAndReleaseBlock28(void) {
  Scheduler_RemoveCallback(PaletteQueue_StepCycles);
  Runtime_ReleaseHeapBlock(0x1C);
}

// Starts running the palette cycles each frame.
void Scheduler_ScheduleCallbackBAfterFrames(void) {
  Scheduler_AddOrUpdateCallback(PaletteQueue_StepCycles, 0xc80);
}

// Each frame: advances each palette cycle whose timer has run out by one
// colour, writing the rotated colours back to the palette.
void PaletteQueue_StepCycles(void) {
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
      Dma_Set(buf, dest, 0x80000000 | len, REG_DMA3);
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

// The terrain height functions (gMap_TerrainHeightFunctions): each takes a
// cell's height samples after its kind byte and a pixel (x, y) within the
// 16x16 cell, and returns the ground height there in 16.16 pixels. This one
// is flat: the first sample everywhere.
s32 Curve_GetFirstSampleA(const s8 *samples) {
  return samples[0] << CURVE_VALUE_SHIFT;
}

// A slope along x: from sample 0 at x = 0 towards sample 1 at x = 16.
s32 Curve_LerpTwoSamples(const s8 *samples, s32 x) {
  s32 start;
  s32 delta;

  start = samples[0] << CURVE_VALUE_SHIFT;
  delta = ((samples[1] << CURVE_VALUE_SHIFT) - start) * x;
  if (delta < 0) {
    delta += CURVE_FULL_STEPS - 1;
  }
  return start + (delta >> 4);
}

// A slope along y: from sample 0 at y = 0 towards sample 1 at y = 16.
s32 Curve_LerpTwoSamplesB(const s8 *samples, s32 unused, s32 y) {
  s32 base;
  s32 offset;

  base = samples[0] << CURVE_VALUE_SHIFT;
  offset = ((samples[1] << CURVE_VALUE_SHIFT) - base) * y;
  if (offset < 0) {
    offset += CURVE_FULL_STEPS - 1;
  }
  return base + (offset >> 4);
}

// A step across the diagonal x + y = 15: sample 0 before it, sample 1 after
// it and the higher of the two on it.
s32 Curve_StepAtSummedPosition(const s8 *samples, s32 x, s32 y) {
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
  position = x + y;
  if (position == CURVE_FULL_STEPS - 1) {
    return maximum;
  }
  if (position < CURVE_FULL_STEPS - 1) {
    second = first;
  }
  return second;
}

// A step across the diagonal x = y: sample 0 where y < x, sample 1 where
// y > x and the higher of the two on it.
s32 Curve_StepAtDifferencePosition(const s8 *samples, s32 x, s32 y) {
  s32 first;
  s32 second;
  s32 maximum;
  s32 difference;
  u32 position = x;

  first = samples[0] << CURVE_VALUE_SHIFT;
  second = samples[1] << CURVE_VALUE_SHIFT;
  maximum = first;
  if (second > first) {
    maximum = second;
  }
  difference = y - position;
  position = difference + CURVE_FULL_STEPS - 1;
  if (position == CURVE_FULL_STEPS - 1) {
    return maximum;
  }
  if (position < CURVE_FULL_STEPS - 1) {
    second = first;
  }
  return second;
}

// A slope that bends on the diagonal x + y = 15: from sample 0 at the top
// left corner to sample 1 on the diagonal to sample 2 at the bottom right.
s32 Curve_LerpThreeSamplesByRatio(const s8 *samples, s32 x, s32 y) {
  s32 first;
  s32 middle;
  s32 last;
  s32 result;

  first = *samples++ << CURVE_VALUE_SHIFT;
  middle = *samples << CURVE_VALUE_SHIFT;
  last = samples[1] << CURVE_VALUE_SHIFT;
  x += y;
  if (x == CURVE_FULL_STEPS - 1) {
    result = middle;
  } else if ((u32)x < CURVE_FULL_STEPS - 1) {
    result = first + (middle - first) * x / (CURVE_FULL_STEPS - 1);
  } else {
    x -= (CURVE_FULL_STEPS - 1);
    result = middle + (last - middle) * x / (CURVE_FULL_STEPS - 1);
  }
  return result;
}

// A slope that bends on the diagonal x = y: from sample 0 at the top right
// corner to sample 1 on the diagonal to sample 2 at the bottom left.
s32 Curve_LerpThreeSamplesAtDifference(const s8 *samples, s32 x, s32 y) {
  s32 first;
  s32 second;
  s32 third;
  s32 difference;
  s32 delta;

  first = *samples++ << CURVE_VALUE_SHIFT;
  second = samples[0] << CURVE_VALUE_SHIFT;
  third = samples[1];
  difference = y - x;
  x = difference + CURVE_FULL_STEPS - 1;
  third <<= CURVE_VALUE_SHIFT;

  if (x == CURVE_FULL_STEPS - 1)
    return second;

  if ((u32)x < CURVE_FULL_STEPS - 1) {
    delta = second - first;
    difference = delta * x;
    if (difference < 0)
      difference += CURVE_FULL_STEPS - 1;
    difference >>= 4;
    return first + difference;
  }
  difference *= third - second;
  if (difference < 0)
    difference += CURVE_FULL_STEPS - 1;
  difference >>= 4;
  return second + difference;
}

// A raised or sunken rounded square: sample 1 where gCurve_LerpWeightTable is
// 1, sample 0 in the corners.
s32 Curve_LerpTwoSamplesByTable(const s8 *samples, s32 x, s32 y) {
  s32 start;

  start = samples[0] << CURVE_VALUE_SHIFT;
  return start + (((samples[1] << CURVE_VALUE_SHIFT) - start) *
                  gCurve_LerpWeightTable[x + (y * CURVE_FULL_STEPS)]);
}

// A step along x: sample 0 for x < 8, sample 1 from x = 8.
s32 Curve_StepAtHalf(const s8 *samples, u32 x) {
  s32 value;
  if (x < CURVE_HALF_STEPS)
    value = samples[0];
  else
    value = samples[1];
  return value << CURVE_VALUE_SHIFT;
}

// A step along y: sample 0 for y < 8, sample 1 from y = 8.
s32 Curve_StepAtHalfB(const s8 *samples, u32 unused, u32 y) {
  s32 value;
  if (y < CURVE_HALF_STEPS)
    value = samples[0];
  else
    value = samples[1];
  return value << CURVE_VALUE_SHIFT;
}

// A slope along x that bends at the middle: sample 0 at x = 0, sample 1 at
// x = 8, towards sample 2 at x = 16.
s32 Curve_LerpThreeSamplesAtHalf(s8 *samples, u32 x) {
  s32 a;
  s32 b;
  s32 delta;

  a = *samples++ << CURVE_VALUE_SHIFT;
  b = *samples++ << CURVE_VALUE_SHIFT;
  if (x < CURVE_HALF_STEPS) {
    delta = (b - a) * x;
    if (delta < 0) {
      delta += CURVE_HALF_STEPS - 1;
    }
    return a + (delta >> 3);
  }

  {
    s32 pos;

    a = *samples << CURVE_VALUE_SHIFT;
    pos = x;
    a -= b;
    pos -= CURVE_HALF_STEPS;
    delta = a * pos;
    if (delta < 0) {
      delta += CURVE_HALF_STEPS - 1;
    }
    return b + (delta >> 3);
  }
}

// The same along y.
s32 Curve_LerpThreeSamplesAtHalfB(u8 *samples, u32 unused, u32 y) {
  s32 a;
  s32 b;
  s32 pos;
  s32 delta;
  s32 tail;

  a = *samples++ << CURVE_VALUE_SHIFT;
  b = *samples++ << CURVE_VALUE_SHIFT;
  if (y < CURVE_HALF_STEPS) {
    delta = (b - a) * y;
    if (delta < 0) {
      delta += CURVE_HALF_STEPS - 1;
    }
    return a + (delta >> 3);
  }

  a = *samples << CURVE_VALUE_SHIFT;
  pos = y;
  a -= b;
  pos -= CURVE_HALF_STEPS;
  tail = a * pos;
  if (tail < 0) {
    tail += CURVE_HALF_STEPS - 1;
  }
  return b + (tail >> 3);
}

// The sample gCurve_SampleIndexTable picks for (x, y).
s32 Curve_LookupSampleByTable(const s8 *samples, u32 x, u32 y) {
  return samples[gCurve_SampleIndexTable[x + (y << 4)]] << CURVE_VALUE_SHIFT;
}

// The same, with the table mirrored left to right.
s32 Curve_LookupSampleByTableReversed(const s8 *samples, u32 x, u32 y) {
  return samples[gCurve_SampleIndexTable[((y << 4) - x) + CURVE_FULL_STEPS - 1]]
         << CURVE_VALUE_SHIFT;
}

// Flat, as Curve_GetFirstSampleA.
s32 Curve_GetFirstSampleB(const s8 *samples) {
  return samples[0] << CURVE_VALUE_SHIFT;
}

// Flat, as Curve_GetFirstSampleA.
s32 Curve_GetFirstSampleC(const s8 *samples) {
  return samples[0] << CURVE_VALUE_SHIFT;
}

typedef s32 (*TerrainHeightFn)(u8 *heights, s32 x, s32 y);

extern TerrainHeightFn gMap_TerrainHeightFunctions[16];

// Returns the ground height of a layer at (x, y) in 16.16 pixels: the
// height function of the cell's terrain kind applied to its samples.
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 y) {
  struct MapState *work = gMapWork[0];
  struct MapCell *cells;
  u8 *attributes;

  x >>= 16;
  y >>= 16;
  cells = (struct MapCell *)Ram_MapCellBuffer;
  if (work != NULL) {
    cells = work->layers[layer & 3].cells;
  }
  cells += x / 16 + (y / 16 << 7);
  attributes = Ram_MapCollision + cells->attributeB * 4;
  return gMap_TerrainHeightFunctions[*attributes++ & 15](attributes, x & 15,
                                                         y & 15);
}

// Returns the terrain kind (the low four bits of the first collision byte)
// of a layer's cell at (x, y) in 16.16 pixels.
s32 Map_GetCellAttributeLowNibble(s32 index, s32 x, s32 y) {
  struct MapState *state = gMapWork[0];
  struct MapCell *map;
  s32 col;
  s32 row;
  u32 attr;

  x >>= 16;
  y >>= 16;
  map = (struct MapCell *)gMapCellBuffer;
  if (state != 0) {
    map = state->layers[index & 3].cells;
  }
  col = x / 16;
  row = y / 16;
  map += col + (row << 7);
  attr = map->attributeB;
  return *(Ram_MapCollision + attr * 4) & 15;
}

// Returns the collision code of a layer's cell at (x, y) in 16.16 pixels.
s32 GetMapCellCollision(s32 layer, s32 x, s32 y) {
  struct MapState *state;
  struct MapCell *cells;

  state = gMapWork[0];
  x >>= 20;
  y >>= 20;
  cells = (struct MapCell *)gMapCellBuffer;
  if (state != NULL) {
    cells = state->layers[layer & 3].cells;
  }
  cells += x + (y << 7);
  return cells->collisionCode;
}

// Sets the collision code of a layer's cell at (x, y) in 16.16 pixels.
void SetMapCellCollision(u32 layer, s32 x, s32 y, u32 collisionCode) {
  struct MapState *state = gMapWork[0];

  x >>= 20;
  y >>= 20;
  if (state != NULL) {
    struct MapCell *cells = state->layers[layer & 3].cells;

    cells += x + (y << 7);
    cells->collisionCode = collisionCode;
  }
}

// Returns the top two bits of the block word of the cell at pixel (x, y).
s32 Map_GetCellHighFlags(s32 x, s32 y) {
  s32 tileX = x / 16;
  s32 tileY = y / 16;
  struct MapCell *cell =
      (struct MapCell *)gMapCellBuffer + (tileX + tileY * 128);

  return ((u8 *)&cell->metatileAndFlags)[1] >> 6;
}

// Returns whether an object can step to position: 2 when the cell there is
// a wall (collision code 0xff), 1 when the ground is more than 8 pixels
// higher than the object's, -1 when it is more than 12 pixels lower, else 0.
s32 Map_CheckObjectStep(struct ObjectRuntime *object,
                        struct FieldPosition *position) {
  struct MapState *state;
  s32 x;
  s32 z;
  struct MapCell *cell;
  u8 *attributes;
  s32 height;
  s32 delta;

  x = ((union FieldCoordinate *)&position->x)->part.pixel;
  z = ((union FieldCoordinate *)&position->z)->part.pixel;
  state = gMapWork[0];
  if (state == NULL)
    return 0;
  if (object->terrainId <= 2)
    cell = state->layers[object->terrainId].cells;
  else
    cell = (struct MapCell *)Ram_MapCellBuffer;
  cell += x / 16 + (z / 16 << 7);
  if (cell->collisionCode == 0xff)
    return 2;
  attributes = Ram_MapCollision + cell->attributeB * 4;
  height = gMap_TerrainHeightFunctions[*attributes++ & 15](attributes, x & 15,
                                                           z & 15);
  delta = height - object->terrainHeight;
  if (delta > 0x80000)
    return 1;
  if (delta < -0xc0000)
    return -1;
  return 0;
}

// Returns 0 when the cell under position (its x, and y less its height) in
// layer 2 is a wall (collision code 0xff), else -1.
s32 CheckMapPositionCellOccupied(struct WorldPosition *position) {
  s32 x;
  s32 y;
  s32 tileX;
  s32 tileY;
  struct MapState *work;
  struct MapCell *cell;

  x = position->x / 65536;
  y = (position->y - position->height) / 65536;
  work = gMapWork[0];
  if (work == NULL)
    return 0;
  cell = work->layers[2].cells;
  tileX = x / 16;
  tileY = y / 16;
  cell += tileX + tileY * 128;
  return (cell->collisionCode != 0xff) - 1;
}

// Returns the world map's collision value (1-15) at position: from the upper
// collision layer where it has one, else the lower, else 7. Each layer is a
// tile map of 4-bit collision pixels.
s32 GetWorldMapCollision(struct WorldPosition *position) {
  s32 stepX = position->x >> 17;
  s32 y = position->y;
  s32 stepY = y >> 17;
  u32 cell;
  u32 pixelOffset;
  u8 tile;
  u8 pixelPair;
  u32 result;

  cell = (((stepY / 8) & 63) << 6) + ((stepX / 8) & 63);
  pixelOffset = (((stepY / 2) & 3) << 1) + ((stepX / 4) & 1);

  tile = BG_SCREEN_BLOCK(10)[cell];
  pixelPair = *(Ram_MapCollision + 0x800 + (tile << 3) + pixelOffset);
  if (pixelPair != 0) {
    if (stepX & 2)
      result = pixelPair >> 4;
    else
      result = pixelPair & 15;
    if (result != 0)
      return result;
  }

  tile = BG_SCREEN_BLOCK(8)[cell];
  pixelPair = *(Ram_MapCollision + (tile << 3) + pixelOffset);
  if (pixelPair != 0) {
    if (stepX & 2)
      result = pixelPair >> 4;
    else
      result = pixelPair & 15;
    if (result != 0)
      return result;
  }

  return 7;
}

// Returns 0 when the world map at position can be walked on (collision 5 to
// 12), else -1.
s32 CheckWorldMapCollisionRange(s32 unused, struct WorldPosition *position) {
  if ((GetWorldMapCollision(position) - 5) <= 7U) {
    return 0;
  }
  return -1;
}

// Returns the world map's terrain behaviour at position from
// gWorldMap_TerrainBehaviorTable, and stores the block's terrain kind in
// terrainKind.
s32 GetWorldMapTerrainBehavior(struct WorldPosition *position,
                               s32 *terrainKind) {
  s32 selector = GetWorldMapCollision(position);
  s32 x = position->x;
  s32 y;
  s32 flag = 0;
  u32 tileX;
  u32 index;
  u32 *tile;

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

  *terrainKind = (*tile << 1) >> 25;
  if (*terrainKind == 21)
    flag = 0x20;

  return gWorldMap_TerrainBehaviorTable[flag + selector];
}

// Shakes the screen: sets the map's x and y shake amplitude and its decay
// (16.16), leaving any negative value unchanged.
void Runtime_SetWorkTripleIfNonNegative(s32 shakeX, s32 shakeY,
                                        s32 shakeDecay) {
  struct MapScrollWork *work;

  work = gMapWork[0];
  if (shakeX >= 0) {
    work->shakeX = shakeX;
  }
  if (shakeY >= 0) {
    work->shakeY = shakeY;
  }
  if (shakeDecay >= 0) {
    work->shakeDecay = shakeDecay;
  }
}

// Waits until the screen shake has died down (both amplitudes under 1/256
// pixel), at most 300 frames, then stops it.
void Map_WaitWorkValuesBelow256(void) {
  struct MapScrollWork *work;
  s32 frames;

  work = gMapWork[0];
  frames = 0;
  if (work->shakeX > 255 || work->shakeY > 255) {
  wait:
    WaitFrames(1);
    if (++frames < 300 && (work->shakeX > 255 || work->shakeY > 255))
      goto wait;
  }
  work->shakeDecay = 0;
}

// Runs TileMap_DrawRows from RAM on a and b, with the BG tile buffer's
// second half and the decode buffer as its work areas.
void Resource_RunCopiedDecoder(s32 a, s32 b) {
  u8 *base;

  base = gDecodeBuffer;
  RamCode_LoadSlot(49, TileMap_DrawRows, TileMap_DrawRowsCodeSize);
  /* FAKEMATCH: one-pass block: its loop notes keep gBgTileBuffer's address load
   * ahead of a's move into r0, as the bytes order the call's arguments. Plain
   * forms measured 2026-10-13: the bare call differ 2 (that load one move
   * later); the tile address in a local differ 2; &gBgTileBuffer[0x4000] and
   * &base[0x1000] through WORK_SLOT differ 3; the routine in a local pointer
   * differ 8; base += 0x1000 after the load call differ 10; gDecodeBuffer +
   * 0x1000 at the declaration differ 26. */
  do {
    ((void (*)(s32, s32, void *, void *))gWorkSlot[49])(
        a, b, gBgTileBuffer + 0x4000, base + 0x1000);
  } while (0);
  Runtime_ReleaseHeapBlock(49);
}

// Builds the world map ground plane's per-scanline terms for the camera's
// pitch (depth is its cotangent): for each of the 160 lines, where its ray
// meets the ground, the line's width scale and its distance, or zeros above
// the horizon. Transform_UpdateVertices turns these into the affine
// parameters each frame.
void WorldMap_BuildScanlineTable(s32 depth, s32 *position,
                                 struct MapScanline *row) {
  s32 ground[3];
  s32 viewed[3];
  s32 horizon;
  s32 focus;
  s32 *view;
  s32 line;
  s32 distance;
  s32 difference;
  s32 scale;
  s32 x;
  s32 y;
  s32 diagonal;
  s32 vertical;
  s32 zero;
  s32 horizontal;

  zero = 0;
  ground[0] = position[0];
  ground[1] = zero;
  ground[2] = position[2];
  Iwram_TransformVector(ground, viewed);
  view = viewed;
  horizon = view[1] - Iwram_MulQ16(view[2], depth);
  focus = -gProjection.focal;
  for (line = 0; line < 160; line++) {
    distance = Iwram_RatioMulQ14(focus, (gProjection.centerY - line) << 16);
    difference = distance - depth;
    if (difference == 0)
      difference = 1;
    scale = Iwram_RatioMulQ14(difference, horizon);
    if (scale < 0) {
      diagonal = Iwram_MulQ16(-scale, 0x8000);
      row->scale = Iwram_RatioMulQ14(gProjection.focal, diagonal);
      diagonal = Iwram_MulQ16(scale, distance);
      horizontal = (view[2] - scale) >> 4;
      vertical = (diagonal - view[1]) >> 4;
      x = Iwram_MulQ16(horizontal, horizontal);
      y = Iwram_MulQ16(vertical, vertical);
      y = Iwram_Sqrt(x + y) << 12;
      if (vertical < 0)
        y = -y;
      row->depth = Iwram_MulQ16(y, 0x8000);
    } else {
      row->scale = 0;
      row->depth = 0;
    }
    row->offsetX = 0;
    row->offsetY = 0;
    row++;
  }
}

// One animation layer the sprite viewer edits.
struct IconColumn {
  s16 number;    // sprite number
  s16 animation; // animation shown
  u8 on;         // layer shown
  s8 priority;
  s8 param;
};

// A debug sprite viewer, which never returns. It shows a sprite (with up to
// three more animation layers) once for each of its facings, in a ring.
// Start with the direction pad moves the ring; otherwise up and down choose
// a layer and left and right what to edit: 0 the animation, 1 the sprite
// number (ten at a time with Start), 2 the priority, 3 the draw parameter,
// changed with L and R. A restarts the animation, or in the other modes
// turns a layer on or off. Pressing Start toggles the shadows; B turns the
// ring. Select reloads the far-call resources (the alternate set unless B is
// held) and starts over.
void Ui_RunIconMonitor(void) {
  struct IconColumn layers[4];
  struct AnimationMetadata *metadata;
  struct AnimationObject *table;
  volatile s32 zero;
  s32 number;
  s32 x;
  s32 y;
  u32 mode;
  s32 flag;
  struct AnimationPosition *ring;
  s32 slot;
  u32 steps;
  s32 alt;
  s32 moved;
  /* FAKEMATCH: volatile, as gKeysHeld and gKeysRepeat are declared
   * (keystate.h): each *held and *repeat is a load of its own. A plain held
   * differs 245, 663 instructions for 672; a plain repeat 89, 669 for 672.
   * Using gKeysHeld and gKeysRepeat directly differs 283: the native keeps only
   * these two addresses in fp and r9 and reloads every other key address from
   * the pool. */
  volatile u32 *held;
  volatile u32 *repeat;
  u32 i;

  x = 144;
  y = 96;
  mode = 1;
  flag = 1;
  slot = 0;
  ring = Runtime_AllocateBlock(9, 10 * sizeof(*ring));
  gDecodeFillByte = 3;

  Dma_Fill32Cell(zero, 0, ring, 4);
  Dma_Fill32Cell(zero, 0, layers, sizeof(layers));

  number = Ui_FindNextNumberWithMetadata(-1, 1);
  for (i = 0; i <= 3; i++) {
    layers[i].animation = 1;
    layers[i].priority = 1;
    layers[i].number = number;
  }
  layers[0].on = 1;
  gDebugMode = 2;
  REG_BLDCNT = 0x3F42;
  PLTT_BACKDROP = 0x01E0;
  REG_DISPCNT = 0x1140;
  Blend_SetDarkenTarget0(1);

restart:
  Runtime_InitializeHeap();
  Scheduler_ResetTaskTable();
  ring = Runtime_AllocateBlock(9, 10 * sizeof(*ring));
  Resource_InitializeTable();
  ObjectSystem_Initialize(2);
  ResourceSlot_Load(0, (u32 *)Ram_MapCellBuffer, layers[0].number, 0);
  metadata = Resource_GetMetadataRecordFar(layers[0].number);
  if (metadata->drawKind == 20) {
    ResourceSlot_Load(1, (u32 *)Ram_ActorSpriteSlots, layers[0].number + 1, 0);
  }

  for (i = 0; i <= 9; i++) {
    metadata = Resource_GetMetadataRecordFar(layers[0].number);
    alt = 0;
    if (metadata->drawKind == 20 && (i & 1) != 0) {
      alt = 1;
    }
    alt = (s32)ResourceObject_Create(layers[0].number + alt + (alt << 12));
    ResourceMetadata_Register((struct AnimationObject *)alt, layers[1].number);
    ResourceMetadata_Register((struct AnimationObject *)alt, layers[2].number);
    ResourceMetadata_Register((struct AnimationObject *)alt, layers[3].number);
    /* FAKEMATCH: needed to match: the native adds the flags offset to the
     * object's own register (r5) in place; a separate address takes a copy.
     * Measured 2026-10-13: ((struct AnimationObject *)alt)->flags = flag, the
     * same through a pointer local, or a byte offset from alt all differ 55; a
     * separate object variable 128 (the index and the object share r5). */
    alt = (s32)&((struct AnimationObject *)alt)->flags;
    *(u8 *)alt = flag;
  }

  for (i = 0; i <= 3; i++) {
    if (layers[i].on != 0) {
      Ui_SetGridColumnPriority(i, layers[i].priority);
    } else {
      Ui_SetGridColumnPriority(i, 8);
    }
    Ui_SetGridColumnParameter(i, layers[i].param);
    Ui_FillGridColumnFromMetadata(i, layers[i].animation);
  }

  Map_BuildProbeRing(x, y, ring);
  Scheduler_AddOrUpdateCallback(Battle_PlaceActorsByFormationKind, 3200);

next:
  WaitFrames(1);
  held = &gKeysHeld;
  repeat = &gKeysRepeat;
  for (;;) {

    if ((*held & KEY_START) != 0) {
      if ((*held & KEY_LEFT) != 0) {
        x--;
      }
      if ((*held & KEY_RIGHT) != 0) {
        x++;
      }
      if ((*held & KEY_UP) != 0) {
        y--;
      }
      if ((*held & KEY_DOWN) != 0) {
        y++;
      }
      Map_BuildProbeRing(x, y, ring);
    } else {
      (void)gKeysRepeat;
      if ((*repeat & KEY_UP) != 0) {
        slot = (slot - 1) & 3;
      }
      if ((gKeysRepeat & KEY_DOWN) != 0) {
        slot = (slot + 1) & 3;
      }
      if ((gKeysRepeat & KEY_LEFT) != 0) {
        mode = (mode - 1) & 3;
      }
      if ((gKeysRepeat & KEY_RIGHT) != 0) {
        mode++;
        mode &= 3;
      }
    }

    if ((gKeyState & KEY_START) != 0) {
      table = gSpriteObjects;
      flag ^= 1;
      for (i = 0; i <= 9; i++) {
        table->flags = flag;
        table++;
      }
    }

    switch (mode) {
    case 0:
      if (slot == 1) {
        break;
      }
      if ((gKeysRepeat & KEY_L) != 0) {
        layers[slot].animation--;
        if (layers[slot].animation < 0) {
          layers[slot].animation = 0;
        }
        if (slot == 0) {
          layers[1].animation = layers[0].animation;
        }
        goto next;
      }
      if ((gKeysRepeat & KEY_R) == 0) {
        break;
      }
      layers[slot].animation++;
      if (layers[slot].animation > 99) {
        layers[slot].animation = 99;
      }
      if (slot == 0) {
        layers[1].animation = layers[0].animation;
      }
      goto next;

    case 1:
      moved = 0;
      steps = 1;
      if ((*held & KEY_START) != 0) {
        steps = 10;
      }
      if ((*repeat & KEY_L) != 0) {
        for (i = 0; i < steps; i++) {
          layers[slot].number =
              Ui_FindNextNumberWithMetadata(layers[slot].number, -1);
        }
        moved = 1;
      }
      if ((*repeat & KEY_R) != 0) {
        for (i = 0; i < steps; i++) {
          layers[slot].number =
              Ui_FindNextNumberWithMetadata(layers[slot].number, 1);
        }
        moved = 1;
      }
      if (moved == 0) {
        break;
      }
      if (slot == 0) {
        goto restart;
      }
      if (layers[slot].on != 0) {
        Ui_SetGridColumnNumber(slot, layers[slot].number);
        Ui_SetGridColumnPriority(slot, layers[slot].priority);
        Ui_SetGridColumnParameter(slot, layers[slot].param);
        Ui_FillGridColumnFromMetadata(slot, layers[slot].animation);
      }
      goto next;

    case 2:
      moved = 0;
      if ((*repeat & KEY_L) != 0) {
        layers[slot].priority--;
        if (layers[slot].priority < 0) {
          layers[slot].priority = 3;
        }
        moved = 1;
      }
      if ((*repeat & KEY_R) != 0) {
        layers[slot].priority++;
        if (layers[slot].priority > 3) {
          layers[slot].priority = 0;
        }
        moved = 1;
      }
      if (moved == 0) {
        break;
      }
      if (layers[slot].on != 0) {
        Ui_SetGridColumnPriority(slot, layers[slot].priority);
      }
      goto next;

    case 3:
      moved = 0;
      if ((*repeat & KEY_L) != 0) {
        layers[slot].param--;
        if (layers[slot].param < 0) {
          layers[slot].param = 15;
        }
        moved = 1;
      }
      if ((*repeat & KEY_R) != 0) {
        layers[slot].param++;
        if (layers[slot].param > 15) {
          layers[slot].param = 0;
        }
        moved = 1;
      }
      if (moved == 0) {
        break;
      }
      if (layers[slot].on != 0) {
        Ui_SetGridColumnParameter(slot, layers[slot].param);
      }
      goto next;

    default:
      break;
    }

    if ((*repeat & KEY_A) != 0) {
      if (mode == 0) {
        if (slot != 1) {
          if (layers[slot].on != 0) {
            Ui_FillGridColumnFromMetadata(slot, layers[slot].animation);
            if (slot == 0) {
              Ui_FillGridColumnFromMetadata(1, layers[1].animation);
            }
          }
          goto next;
        }
      } else if (slot != 0) {
        layers[slot].on ^= 1;
        Ui_SetGridColumnPriority(
            slot, layers[slot].on != 0 ? layers[slot].priority : 8);
        goto next;
      }
    }

    if ((*repeat & KEY_SELECT) != 0) {
      Scheduler_ResetTaskTable();
      if ((*held & KEY_B) != 0) {
        RuntimeDispatch_ReturnZero((s32)&ResourceId_ResourceFarCalls,
                                   FarCall_ResourceTable);
      } else {
        RuntimeDispatch_ReturnZero((s32)&ResourceId_ResourceFarCallsAlternate,
                                   FarCall_ResourceTable);
      }
      goto restart;
    }
    WaitFrames(1);
  }
}

// Returns the next sprite number after start, stepping by step (1 or -1)
// and wrapping within 0-511, that has an image.
s32 Ui_FindNextNumberWithMetadata(s32 start, s32 step) {
  for (;;) {
    start += step;
    if (start < 0) {
      start = 0x200;
      continue;
    }
    {
      /* FAKEMATCH: the upper bound in a local compares as cmp against 0x200
       * with blt; the literal compares against 0x1ff from the pool. 2026-10-13:
       * start >= 0x200 or 0x200 <= start differs 9 of 25 (ldr r3, =0x1ff and
       * ble); the tests as one else-if chain 9, start < 0x200 guarding the
       * lookup 13. */
      s32 limit = 0x200;
      if (start >= limit) {
        start = -1;
        continue;
      }
    }
    if (Resource_GetMetadataRecordFar(start)->width) {
      return start;
    }
  }
}

// Lays out the sprite viewer's ring around pixel (x, z): one position per
// facing of the first sprite's facing kind (6, 8, 10 or 4, or two opposite
// pairs for kind 20), 56 pixels out (40 for kind 20). B held turns the ring.
void Map_BuildProbeRing(s32 x, s32 z, struct AnimationPosition *out) {
  struct AnimationPosition *pair;
  s32 angle;
  s32 i;
  u16 turn;
  u8 kind;

  angle = 0;
  kind = gSpriteObjects->entries[0]->kind;
  if ((gKeysHeld & KEY_B) != 0)
    angle = (s16)(gFrameTick << 8);
  switch ((u32)kind) {
  case 3:
    for (i = 0; i < 6; i++) {
      angle <<= 16;
      out->x = x << 16;
      angle = (u32)angle >> 16;
      out->y = 0;
      out->z = z << 16;
      Vector_AddPolarOffset(0x380000, (u16)angle, &out->x);
      out++;
      angle = (s16)(angle + 0x2aaa);
    }
    for (; i < 10; i++)
      out++;
    break;
  case 5:
  case 8:
  case 44:
  case 88:
    for (i = 0; i < 8; i++) {
      angle <<= 16;
      out->x = x << 16;
      angle = (u32)angle >> 16;
      out->y = 0;
      out->z = z << 16;
      Vector_AddPolarOffset(0x380000, (u16)angle, &out->x);
      out++;
      angle = (s16)(angle + 0x2000);
    }
    for (; i < 10; i++)
      out++;
    break;
  case 4:
  case 6:
    for (i = 0; i < 10; i++) {
      angle <<= 16;
      out->x = x << 16;
      angle = (u32)angle >> 16;
      out->y = 0;
      out->z = z << 16;
      Vector_AddPolarOffset(0x380000, (u16)angle, &out->x);
      out++;
      angle = (s16)(angle + 0x1999);
    }
    break;
  case 20:
    turn = angle;
    angle = (s16)(turn + 0x4000);
    i = 0;
    pair = out;
    while (i < 2) {
      pair[i * 2].x = x << 16;
      pair[i * 2].y = 0;
      pair[i * 2].z = z << 16;
      angle <<= 16;
      angle = (u32)angle >> 16;
      Vector_AddPolarOffset(0x280000, angle, &out[0].x);
      pair[i * 2 + 1].x = x << 16;
      {
        s32 *point = &out[1].x;
        pair[i * 2 + 1].y = 0;
        pair[i * 2 + 1].z = z << 16;
        Vector_AddPolarOffset(0x280000, (u16)angle, point);
      }
      angle = (angle + 0x8000) << 16;
      i++;
      out += 2;
      angle >>= 16;
    }
    break;
  default:
    angle = ((angle << 16) + 0x20000000) >> 16;
    for (i = 0; i < 4; i++) {
      angle <<= 16;
      angle = (u32)angle >> 16;
      out->x = x << 16;
      out->y = 0;
      out->z = z << 16;
      Vector_AddPolarOffset(0x380000, (u16)angle, &out->x);
      out++;
      angle = (s16)(angle + 0x4000);
    }
    for (i = 4; i < 10; i++)
      out++;
    break;
  }
}

struct Vec2 {
  s32 x;
  s32 y;
};

// Defines name(slot, value), which sets field of entry slot (0-3) in each of
// the first ten sprites to value.
#define DEFINE_SET_GRID_COLUMN(name, field)                                    \
  void name(s32 slot, s32 value) {                                             \
    struct AnimationObject *object = gSpriteObjects;                           \
    s32 i;                                                                     \
                                                                               \
    slot &= 3;                                                                 \
    for (i = 0; i < 10; i++) {                                                 \
      object->entries[slot]->field = value;                                    \
      object++;                                                                \
    }                                                                          \
  }

// Set the draw parameter or the priority of one layer of the viewer's ten
// sprites.
DEFINE_SET_GRID_COLUMN(Ui_SetGridColumnParameter, param)
DEFINE_SET_GRID_COLUMN(Ui_SetGridColumnPriority, priority)

// Starts animation value on layer slot of each of the viewer's ten sprites,
// each one 16 timer units behind the last.
void Ui_FillGridColumnFromMetadata(s32 slot, s32 value) {
  s32 count;
  struct AnimationObject *object;

  object = gSpriteObjects;
  count = 0;
  do {
    struct AnimationEntry *entry = object->entries[slot & 3];
    struct AnimationMetadata *metadata;
    s32 index = 0;

    if (entry->scripts != 0) {
      metadata = Resource_GetMetadataRecordFar(entry->animId);
      if (value < metadata->animationCount) {
        entry->kind = metadata->drawKind;
        entry->script = entry->scripts[value];
        entry->timer = count * 0x10;
        entry->step = 0x10;
        entry->pos = index;
        entry->frameBase = index;
        entry->frame = 0xff;
      }
      object->offsetY = metadata->adjustY;
      object->rotation = index;
    }
    count++;
    object++;
  } while (count <= 9);
}

// Gives layer slot of each of the viewer's ten sprites the sprite number no.
void Ui_SetGridColumnNumber(s32 slot, s32 no) {
  struct AnimationObject *object = gSpriteObjects;
  s32 i;

  Resource_GetMetadataRecordFar(no);
  slot &= 3;
  for (i = 0; i < 10; i++) {
    struct AnimationEntry *entry = object->entries[slot];

    entry->animId = no;
    Animation_InitWorkFromMetadata(entry);
    object++;
  }
}

// Each frame in the sprite viewer: places each copy of the sprite at its
// ring position, facing the way that position stands for.
void Battle_PlaceActorsByFormationKind(void) {
  struct AnimationObject *actor = WORK_SLOT(4);
  u32 kind = actor->entries[0]->kind;
  struct Vec2 scale;
  struct AnimationPosition *table;
  u16 angle;
  u16 step;
  u16 odd = 0;
  u32 count;
  u32 i;

  scale = (*(struct Vec2 *)&gBattle_FormationPlacementScale);
  table = WORK_SLOT(HEAP_SLOT_BATTLE);

  switch (kind) {
  case 3:
    angle = 0;
    step = 0x2aaa;
    count = 6;
    break;
  case 5:
  case 8:
  case 44:
  case 88:
    angle = 0;
    step = 0x2000;
    count = 8;
    break;
  case 4:
  case 6:
    angle = 0;
    step = 0x1999;
    count = 10;
    break;
  case 20:
    angle = 0;
    step = 0;
    odd = 0x8000;
    count = 4;
    break;
  default:
    angle = 0x2000;
    step = 0x4000;
    count = 4;
    break;
  }

  for (i = 0; i < count; i++) {
    Render_ApplyProjectedPlacement(actor, &table[i].x, (s32 *)&scale, angle);
    actor++;
    angle += step;
    if (i & 1)
      angle += odd;
  }
}
