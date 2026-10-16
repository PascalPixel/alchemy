#include "unit/field/facing.h"
#include "unit/field/object_script.h"
#include "unit/field/operand.h"
#include "unit/field/mapdata.h"
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
#include "game_state.h"
#include "map.h"
#include "map_scroll.h"
#include "ram_buffer.h"
#include "field_sprite.h"
#include "resource_ids.h"
#include "flags.h"
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

  // Uncompressed blocks borrow their payload; compressed blocks use the output.
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
  // Consume literals or copy runs until the zero token ends the stream.
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

enum {
  RESOURCE_SLOT_COUNT = 8,
  ANIMATION_OBJECT_COUNT = 64,
  ANIMATION_LAYER_COUNT = 4,
  ANIMATION_NORMAL_STEP = 16
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
  u16 drawOrder[ANIMATION_LAYER_COUNT];
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
               gMenuCtrlWork)[HEAP_SLOT_FRAME_SCRATCH - HEAP_SLOT_MENU_CONTROL];
  if (decodeFrame == 0) {
    code = Runtime_AllocateHeapBlock(HEAP_SLOT_FRAME_SCRATCH,
                                     (s32)Render_DecodeFrameCodeSize);
    Dma_Set(Render_DecodeFrame, code,
            (((u32)gRender_DecodeFrameBuffer - (u32)Render_DecodeFrame) >> 2) |
                (DMA_ENABLE32 | DMA_32BIT),
            REG_DMA3);
    decodeFrame =
        (DrawFn)(*(void *(*)[]) & gMenuCtrlWork)[HEAP_SLOT_FRAME_SCRATCH -
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
    tile = VramCache_Load(obj->slot, size, 0);
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
    Runtime_ReleaseHeapBlock(HEAP_SLOT_FRAME_SCRATCH);
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

  // Double-size affine sprites need enlarged image bounds and shadow margins.
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
  // Place the shadow at ground height independently of the raised image.
  x = (screenX >> 16) - marginX;
  y = ((screenY - groundHeight) >> 16) - marginY;
  shadow = &sprite->part[1];
  shadow->affine = affine;
  shadow->x = x;
  shadow->y = y;
}

// Places a sprite for the scrolling camera and queues it for drawing. Position
// holds screen x, height, screen y and ground height in 16.16 pixels; scale
// holds its x/y factors and facing its angle. Builds an affine matrix when the
// sprite is mirrored, scaled or rotated, queues the shadow (when the sprite
// has one) and then the image in a draw slot by depth; an object 100 pixels
// or more below the ground takes slot 1 instead, its shadow slot 0.
void Render_ApplyProjectedPlacement(struct AnimationObject *sprite,
                                    const struct SpritePlacement *position,
                                    const struct SpriteScale *scale,
                                    s32 facing) {
  const s32 *pos = SpritePlacement_ReadValues(position);
  const s32 *scaleValues = scale->values;
  s32 matrix;
  u32 halfWidth = sprite->width >> 1;
  u32 halfHeight = sprite->height >> 1;
  s32 shadowHalfWidth = 8;
  s32 shadowHalfHeight = 4;
  s32 affine;
  s32 scaleX = *scaleValues++;
  s32 screenX = *pos++;
  s32 scaleY = *scaleValues;
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

  // Reuse an untransformed sprite when mirroring, scale and rotation allow it.
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
    shadowHalfWidth = 16;
    shadowHalfHeight = 8;
  }
  if (height <= -0x640000) {
    slot = 1;
    depth = 0;
  } else {
    slot = (screenY >> 17) + 10;
    depth = 2;
  }
  // Queue the shadow at ground height before the raised image.
  y = ((screenY - groundHeight) >> 16) - shadowHalfHeight;
  if (sprite->flags & 1) {
    if (y < 160) {
      part = &sprite->part[1];
      part->affine = affine;
      part->affineIndex = matrix;
      part->x = (screenX >> 16) - shadowHalfWidth;
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

  // Project first, then discard sprites beyond the padded screen bounds.
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
      // Quantize the perspective scale before combining it with local scale.
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
      // Mirroring uses negative affine scale when a matrix is needed.
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
      // Project the ground point separately so jumping does not lift the
      // shadow.
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
  // Invisible sprites release cached tiles unless their display flags hold
  // them.
  if (!(sprite->displayFlags & 1)) {
    VramCache_ReleaseVram(sprite->slot);
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
  u8 *pixels;
  u8 *end;
  u8 *colorMap;
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

  // Resolve the animation number to its packed resource.
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

  // Turn the frame-offset directory into pointers before the pixel payload.
  offset = buffer;
  for (count = 0; count < 256; count++) {
    if (*offset == 0)
      break;
    *offset += (u32)buffer;
    offset++;
  }
  if (variant != 0) {
    index = variant - 1;
    pixels = (u8 *)(offset + 1);
    end = (u8 *)buffer + size;
    if (index > 4)
      index = 0;
    // Values above 0xdf are kept; only the remappable palette range changes.
    colorMap = gResourceSlot_ConversionTables[index];
    for (; pixels < end; pixels++) {
      u32 colorIndex = *pixels;

      if (colorIndex <= 0xdf) {
        colorIndex = colorMap[colorIndex];
        *pixels = colorIndex;
      }
    }
  }
  return metadata->width * metadata->height;
}

// Returns the buffer loaded into the resource slot with this tag, or 0.
s32 Animation_LookupValueByKey(s32 tag) {
  u32 i;
  struct ResourceSlot *slot = gMenuCtrlWork->slots;

  for (i = 0; i < RESOURCE_SLOT_COUNT; i++, slot++) {
    if (slot->tag == tag)
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
      // The first layer supplies the dimensions and origin of the whole sprite.
      sprite->width = metadata->width;
      sprite->height = metadata->height;
      sprite->scale = metadata->scale << 8;
      sprite->offsetY = metadata->adjustY;
      sprite->offsetX = metadata->adjustX;
    }

    // Some frame directories are supplied by the resource-slot cache.
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
      // Keep the layer inactive until an animation script is selected.
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
  s32 layerHandle = (s32)sprite->entries[0];
  s32 freeIndex = 0;
  struct AnimationEntry **entry;
  struct AnimationMetadata *metadata;

  // Find an unused layer slot before reserving a layer record.
  if (layerHandle != 0) {
    entry = &sprite->entries[0];
    do {
      freeIndex++;
      if (freeIndex >= ANIMATION_LAYER_COUNT)
        break;
      entry++;
      layerHandle = (s32)*entry;
    } while (layerHandle != 0);
  }
  if (freeIndex == ANIMATION_LAYER_COUNT)
    return -1;
  layerHandle = (s32)AnimationObject_Allocate(id);
  if (layerHandle == 0)
    return 0;
  sprite->entries[freeIndex] = (struct AnimationEntry *)layerHandle;
  metadata = Resource_GetMetadataRecordFar(id);
  // Only the first registered layer establishes the sprite's geometry.
  if (sprite->count == 0) {
    sprite->width = metadata->width;
    sprite->height = metadata->height;
    sprite->scale = metadata->scale << 8;
    sprite->offsetY = metadata->adjustY;
    sprite->offsetX = metadata->adjustX;
  }
  if (freeIndex == sprite->count)
    sprite->count = freeIndex + 1;
  return layerHandle;
}

// Removes the layer handle from a sprite and clears it, trimming the
// sprite's layer count when no later layer remains.
void ResourceMetadata_Unregister(struct AnimationObject *sprite, s32 layer) {
  s32 laterLayers;
  u32 index;
  u32 i;

  if (sprite != NULL && layer != 0) {
    ResourceMetadata_ClearRecord(layer);
    // Locate the handle, then trim only empty slots at the end of the list.
    for (index = 0; index < ANIMATION_LAYER_COUNT; index++) {
      if (layer == (s32)sprite->entries[index])
        break;
    }
    if (index != ANIMATION_LAYER_COUNT) {
      sprite->entries[index] = 0;
      laterLayers = 0;
      for (i = index + 1; i < ANIMATION_LAYER_COUNT; i++) {
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

  if (sprite != NULL && index < ANIMATION_LAYER_COUNT) {
    layer = sprite->entries[index];
    if (layer != NULL) {
      ResourceMetadata_ClearRecord(layer);
      sprite->entries[index] = NULL;
      laterLayers = 0;
      // Interior holes stay available; only trailing holes reduce the count.
      for (i = index + 1; i < ANIMATION_LAYER_COUNT; i++) {
        if (sprite->entries[i] != NULL)
          laterLayers++;
      }
      if (laterLayers == 0)
        sprite->count = index;
    }
  }
}

// Starts the indexed animation on a layer when its metadata contains it.
// Bit 7 preserves the command cursor and timer; it remains part of the index.
void Animation_SetWorkEntry(struct AnimationEntry *layer, s32 animation) {
  s32 keepPlace;
  struct AnimationMetadata *metadata;
  s32 scriptAddress;

  keepPlace = 0x80 & animation;
  if (layer->scripts != 0) {
    metadata = Resource_GetMetadataRecordFar(layer->animId);
    if (animation < metadata->animationCount) {
      scriptAddress = (s32)layer->scripts[animation];
      layer->kind = metadata->drawKind;
      layer->script = (u8 *)scriptAddress;
      layer->step = ANIMATION_NORMAL_STEP;
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
          layer->step = ANIMATION_NORMAL_STEP;
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

// Sets each animated layer's timer to the given duration in frames.
// Returns 0.
s32 AnimationObjects_SetActiveFrameTimers(struct AnimationObject *sprite,
                                          s32 frames) {
  u8 count = sprite->count;
  struct AnimationEntry **layerList;
  s32 remaining;

  if (count != 0) {
    // The layer timer counts sixteenths of a frame.
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

  // World-map mode keeps the sprite and layer pools in the fixed heap blocks.
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
  // Clear both pools, then prepare the shared shadow and tile uploader.
  Palette_LoadBasicObjColors();
  Dma_Fill32Cell(zero, 0, sprites,
                 ANIMATION_OBJECT_COUNT * sizeof(struct AnimationObject));
  Dma_Fill32Cell(zero, 0, layers,
                 ANIMATION_OBJECT_COUNT * sizeof(struct AnimationEntry));
  VramCache_Load(93, 128, gObject_ShadowTiles);
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
  u8 **scripts;

  found = NULL;
  metadata = Resource_GetMetadataRecordFar(id);
  candidate = gAnimationObjects[0];
  layer = NULL;

  if (metadata->width != 0) {
    // A zero facing kind marks a free layer record.
    for (i = 0; i < ANIMATION_OBJECT_COUNT; i++, candidate++) {
      if (candidate->kind == 0) {
        found = candidate;
        break;
      }
    }
    if (found != NULL) {
      // Bind the metadata and start its first animation at command zero.
      /* FAKEMATCH: instruction order only: read as a word, the frames load does
       * not wait on the animId store, so the zero for pos and param loads first
       * as native; the typed read swaps the two (differ 2). */
      frames = (void **)*(s32 *)&metadata->frames;
      layer = found;
      layer->animId = id;
      if (frames == 0)
        frames = (void **)Animation_LookupValueByKey(id);

      scripts = metadata->animation;
      layer->scripts = scripts;
      layer->frames = frames;
      layer->mode = metadata->frameCodec;
      layer->frame = 0xff;
      layer->script = scripts[0];
      layer->pos = 0;
      layer->kind = metadata->drawKind;
      layer->param = 0;
    }
  }
  return layer;
}

// Zeroes one animation layer (struct AnimationEntry); does nothing for NULL.
void ResourceMetadata_ClearRecord(void *record) {
  if (record != 0) {
    Dma_Fill32(0, record, sizeof(struct AnimationEntry));
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
  for (i = 0; i < ANIMATION_OBJECT_COUNT; i++, sprite++) {
    if (sprite->width == 0) {
      freeSprite = sprite;
      break;
    }
  }
  if (freeSprite == NULL)
    return NULL;
  if (slot == VRAM_CACHE_ENTRY_COUNT)
    return NULL;
  tile = VramCache_Load(slot, 0, 0);
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
    // Release dependent records before clearing the sprite itself.
    if (!(sprite->flags & 1))
      Resource_ResetEntry(sprite->resource);
    layer = sprite->children;
    remaining = ANIMATION_LAYER_COUNT - 1;
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
  u8 duration;
  s32 durationSum = 0;

  metadata = Resource_GetMetadataRecordFar(id);
  if (animation >= metadata->animationCount) {
    return 0;
  }
  script = ((u8 **)metadata->animation)[animation];
  // Only timed commands count; control-flow commands terminate this scan.
  for (;;) {
    command = script[0];
    duration = script[1];
    script += 2;
    if (command == ANIM_CMD_PLAY || command == ANIM_CMD_HOLD ||
        command == ANIM_CMD_JUMP || command == ANIM_CMD_END) {
      break;
    }
    if (command == ANIM_CMD_WAIT || command == ANIM_CMD_BLANK ||
        command < ANIM_CMD_END) {
      durationSum += duration;
      frameCount--;
      if (frameCount == 0) {
        break;
      }
    }
  }
  return durationSum;
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
  // Finish all four dissolve steps before waiting for the next video frame.
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
  u8 **firstSprite;
  u8 **spriteCursor;
  s32 spritesRemaining;
  u32 step;
  u32 secondDissolveStep;
  u32 thirdDissolveStep;
  u32 fourthDissolveStep;

  firstSprite = sprites;
  step = 0;
  do {
    if (count > 0) {
      secondDissolveStep = step + 1;
      thirdDissolveStep = step + 2;
      spriteCursor = firstSprite;
      fourthDissolveStep = step + 3;
      spritesRemaining = count;
      do {
        Map_RenderAnimatedTileFrame(*spriteCursor, step);
        Map_RenderAnimatedTileFrame(*spriteCursor, secondDissolveStep);
        Map_RenderAnimatedTileFrame(*spriteCursor, thirdDissolveStep);
        spritesRemaining -= 1;
        Map_RenderAnimatedTileFrame(*spriteCursor++, fourthDissolveStep);
      } while (spritesRemaining != 0);
    }
    WaitFrames(1U);
    step += 4;
  } while (step <= 0x7fU);
}

// Writes the object's screen position, in pixels, to screenPosition[0..1] and
// returns 0 when it is on or near the screen; otherwise writes 0, 0 and
// returns -1.
s32 Map_GetScreenRelativePosition(struct ObjectRuntime *object,
                                  s32 *screenPosition) {
  struct MapScrollWork *mapWork = gMapWork[0];
#ifndef UBFIX
  s32 *viewCoordinates = &mapWork->viewX;
#endif
  s32 viewX;
  s32 viewZ;
  s32 x;
  s32 y;

  // UB: viewY is a separate member, not a second element of viewX.
  // Direct member loads change the EN save set and load order (96 bytes each).
#ifdef UBFIX
  viewX = mapWork->viewX & 0xffff0000;
  viewZ = mapWork->viewY & 0xffff0000;
#else
  viewX = viewCoordinates[0] & 0xffff0000;
  viewZ = viewCoordinates[1] & 0xffff0000;
#endif
  x = object->x - viewX;
  y = object->z - viewZ;
  if ((x >= -0x1fffff && x <= 0x10fffff) && y > 0 && y < 0xe00000) {
    *screenPosition++ = x >> 16;
    *screenPosition = y >> 16;
    return 0;
  }
  *screenPosition++ = 0;
  *screenPosition = 0;
  return -1;
}

// Sets up the object system: allocates and zeroes the 64 objects and
// registers the per-frame updates. Mode 4 moves objects with
// Object_UpdateAllMotion; modes 3 and 4 use the fixed camera.
void ObjectSystem_Initialize(s32 controlMode) {
  struct ObjectSystemWork *objectSystem;
  struct ObjectRuntime *objectPool;
  volatile u32 fill;

  objectSystem = Runtime_AllocateBlock(6, sizeof(*objectSystem));
  objectPool = Runtime_AllocateBlock(5, 64 * sizeof(*objectPool));
  ObjectSystem_Configure(controlMode);
  Dma_Fill32Cell(fill, 0, objectPool, 64 * sizeof(*objectPool));
  Dma_Fill32Cell(fill, 0, objectSystem, sizeof(*objectSystem));
  if (controlMode == 4)
    Scheduler_AddOrUpdateCallback(Object_UpdateAllMotion, 0xc8a);
  else
    Scheduler_AddOrUpdateCallback(Object_UpdateAllThumb, 0xc8a);
  if (controlMode >= 3 && controlMode <= 4) {
    Scheduler_AddOrUpdateCallback(ObjectSystem_UpdateCameraFixed, 0xc80);
  } else {
    Scheduler_AddOrUpdateCallback(ObjectSystem_UpdateCamera, 0xc80);
    gIwram_Data2 = 0;
    gIwram_Data1 = 0;
  }
  objectSystem->edge = 15;
  objectSystem->fill = 0;
}

// Does nothing.
void ResourceTable_ReservedNoOpC0C4(void) {}

// Does nothing.
void ResourceTable_ReservedNoOp2(void) {}

// Returns the first of the 64 objects not in use (with no script), or 0.
void *ObjectDispatch_FindFreeObject(void) {
  struct ObjectRuntime *object = gObjectSlots;
  void *freeObject = 0;
  s32 objectIndex = 0;

  while (objectIndex <= 63) {
    if (object->script == 0) {
      freeObject = object;
      break;
    }
    objectIndex++;
    object++;
  }
  return freeObject;
}

// Releases an object's sprites and zeroes it, freeing it.
void ObjectDispatch_Release(struct DispatchObject *object) {
  s32 spritesRemaining;
  struct ResourceObjectWork **sprites;

  if (object) {
    switch (object->kind & 15) {
    case 1: // one sprite
      ResourceObject_Release(object->target.child);
      break;
    case 2: // up to four sprites
      sprites = (struct ResourceObjectWork **)object->target.children;
      spritesRemaining = 3;
      do {
        struct ResourceObjectWork *sprite = *sprites++;
        if (sprite)
          ResourceObject_Release(sprite);
      } while (--spritesRemaining >= 0);
      break;
    }
    Dma_Fill32(0, object, sizeof(struct ObjectRuntime));
  }
}

// Creates a field object at (x, y, z), in 16.16 pixels, or returns 0 when
// none is free. id / 4096 is the kind (0: one sprite, 2: two sprites, id
// and id + 1) and the low 12 bits the sprite's animation id.
struct ObjectRuntime *FieldObject_Create(s32 encodedSprite, s32 x, s32 y,
                                         s32 z) {
  struct ObjectRuntime *object;
  struct FieldActor *actor;
  struct ScriptMotionObject *motion;
  struct ScriptObjectRuntime *scripted;
  void *sprite;
  s32 spriteStorageKind;
  void **spriteSlots;

  ObjectDispatch_FindFreeObject();
  spriteStorageKind = encodedSprite / 4096;
  encodedSprite &= 0xfff;
  object = ObjectDispatch_FindFreeObject();
  if (object != NULL) {
    actor = (struct FieldActor *)object;
    actor->radius = 16;
    switch (spriteStorageKind) {
    case 0:
      sprite = ResourceObject_Create(encodedSprite);
      if (sprite != NULL) {
        object->animationKind = 1;
        object->animation = sprite;
        actor->radius = Resource_GetMetadataRecordFar(encodedSprite)->boxY >> 1;
      } else {
        object->animationKind = 0;
      }
      break;
    case 2:
      // Take a zeroed list of four sprites, and fill it with id and id + 1.
      spriteSlots =
          (void **)((u32 *)gMenuCtrlWork + gMenuCtrlWork->count++ + 2);
      object->animationKind = spriteStorageKind;
      object->animation = spriteSlots;
      Dma_Fill32(0, spriteSlots, 0x10);
      sprite = ResourceObject_Create(encodedSprite);
      if (sprite != NULL) {
        actor->radius = Resource_GetMetadataRecordFar(encodedSprite)->boxY >> 1;
        *spriteSlots++ = sprite;
      }
      sprite = ResourceObject_Create(encodedSprite + 1);
      if (sprite != NULL)
        *spriteSlots = sprite;
      break;
    }
  }
  if (object != NULL) {
    motion = (struct ScriptMotionObject *)object;
    scripted = (struct ScriptObjectRuntime *)object;
    Object_SetPositionAndResetMotion(object, x, y, z);
    object->script = (s32 *)gObjectDispatch_DefaultScript;
    object->speedLimit = 0x20000;
    object->step = 0;
    actor->scaleX = 0x10000;
    actor->scaleY = 0x10000;
    object->acceleration = 0x10000;
    object->flags = 3;
    motion->gravity = 0x10000;
    motion->vertical.bounce = 0x4000;
    object->unk59 = 0;
    object->actionFlags = 1;
    actor->riseBaseY = 0;
    object->angle = 0x4000;
    scripted->homeX = x / 65536; // its home position, in pixels
    scripted->homeZ = z / 65536;
  }
  return object;
}

// Gives an object a script and starts it from the top.
void ObjectDispatch_Initialize(struct DispatchObject *object,
                               const s32 *script) {
  if (object != 0) {
    object->value04 = 0;
    object->value00 = (u32)script;
    object->value5b = 0;
    object->value5d = 0;
    object->value57 = 0;
  }
}

// Defines name(object, value), which calls apply(sprite, value) on an
// object's one sprite, or on each sprite of its group.
#define DEFINE_APPLY_TO_CHILDREN(name, object_type, apply)                     \
  void name(object_type rawObject, s32 value) {                                \
    struct DispatchObject *object;                                             \
    void **sprites;                                                            \
    s32 spritesRemaining;                                                      \
    void *sprite;                                                              \
                                                                               \
    object = rawObject;                                                        \
    if (object != 0) {                                                         \
      switch (object->kind & 0xf) {                                            \
      case 1:                                                                  \
        apply(object->target.child, value);                                    \
        break;                                                                 \
      case 2:                                                                  \
        sprites = object->target.children;                                     \
        spritesRemaining = 3;                                                  \
        do {                                                                   \
          sprite = *sprites++;                                                 \
          if (sprite != 0)                                                     \
            apply(sprite, value);                                              \
          spritesRemaining--;                                                  \
        } while (spritesRemaining >= 0);                                       \
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
void ObjectDispatch_ApplyPairToChildren(struct DispatchObject *object,
                                        s32 animation, s32 step) {
  void **sprites;
  void *sprite;
  s32 spritesRemaining;

  if (object != 0) {
    switch (object->kind & 15) {
    case 1:
      AnimationObjects_SelectAnimation(object->target.child, animation);
      AnimationObjects_SetActiveTimerSteps(object->target.child, step);
      break;
    case 2:
      sprites = object->target.children;
      for (spritesRemaining = 3; spritesRemaining >= 0; spritesRemaining--) {
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
void ObjectDispatch_SetChildRotation(struct DispatchObject *object,
                                     u32 rotation) {
  if (object != 0 && (object->kind & 0xf) == 1)
    ((struct FieldSprite *)object->target.child)->rotation = rotation;
}

// Gives a one-sprite object's first layer animation id animId and restarts
// its layers.
// A negative animationId does nothing.
void Animation_SetIndexAndInitObjects(void *rawDispatchObject,
                                      s32 animationId) {
  struct DispatchObject *object = rawDispatchObject;
  struct AnimationObject *sprite;

  if (object != NULL && (object->kind & 0xf) == 1) {
    sprite = object->target.child;
    if (animationId >= 0) {
      sprite->entries[0]->animId = animationId;
      Animation_InitializeObjects(sprite);
    }
  }
}

// Adds a layer of animation id animId to a one-sprite object's sprite.
// A negative animId does nothing.
void ObjectDispatch_RegisterChildMetadata(struct DispatchObject *object,
                                          s32 animationId) {
  if (object != 0 && (object->kind & 0xf) == 1) {
    struct AnimationObject *sprite = object->target.child;
    if (animationId >= 0)
      ResourceMetadata_Register(sprite, animationId);
  }
}

// Starts an object on the script that approaches its linked object
// (SC_APPROACH_LINK), linked to linkedObjectHandle. The functions below start
// the engine's other scripts (object_script.c).
void ObjectDispatch_InitFromTable5WithArgument(struct DispatchObject *object,
                                               s32 linkedObjectHandle) {
  ObjectDispatch_Initialize(object, gObjectDispatch_Table5Script);
  object->argument = linkedObjectHandle;
}

// Starts an object on the script that runs SC_KEY_COUNT every frame.
void ObjectDispatch_InitFromTable0(struct DispatchObject *object) {
  ObjectDispatch_Initialize(object, gObjectDispatch_Table0Script);
}

// Starts an object on the script that moves it by the keys (SC_KEYS).
void ObjectDispatch_InitFromTable1(struct DispatchObject *object) {
  ObjectDispatch_Initialize(object, gObjectDispatch_Table1Script);
}

// Starts an object on the player control script (SC_PLAYER).
void ObjectDispatch_InitFromTable2(struct DispatchObject *object) {
  ObjectDispatch_Initialize(object, gObjectDispatch_Table2Script);
}

// Starts an object on the world map control script (SC_WORLD).
void ObjectDispatch_InitFromTable3(struct DispatchObject *object) {
  ObjectDispatch_Initialize(object, gObjectDispatch_Table3Script);
}

// Starts an object on the idle script (a lone SC_END).
void ObjectDispatch_InitFromTable6(struct DispatchObject *object) {
  ObjectDispatch_Initialize(object, gObjectDispatch_Table6Script);
}

// Starts an object on the camera script (SC_CAMERA). With a leader to
// follow, it also gets a speed limit of 4 pixels a frame and an
// acceleration of 0.5 (value30 and value34 are those fields).
void ObjectDispatch_InitFromTable4WithArgument(struct DispatchObject *object,
                                               s32 linkedObjectHandle) {
  ObjectDispatch_Initialize(object, gObjectDispatch_Table4Script);
  if (linkedObjectHandle != 0) {
    object->value34 = 0x8000;
    object->value30 = 0x40000;
    object->argument = linkedObjectHandle;
    object->value64 = 0;
  }
}

// Waits until the object's script reaches SC_END (0x10), at most 300 frames.
void ObjectDispatch_WaitForValue16(void *rawObject) {
  struct ScriptInterpreter *interpreter = rawObject;
  s32 waitedFrames;

  waitedFrames = 0;
  if (interpreter->script[interpreter->cursor] != 0x10) {
    do {
      WaitFrames(1);
      waitedFrames++;
      if (waitedFrames > 299)
        break;
    } while (interpreter->script[interpreter->cursor] != 0x10);
  }
}

// Sets the flags of a one-sprite object's sprite (bit 0: draw its shadow).
void ObjectDispatch_SetSingleChildFlags(struct DispatchObject *object,
                                        u32 flags) {
  if (object != NULL) {
    if ((object->kind & 0xf) == 1)
      ((struct AnimationObject *)object->target.child)->flags = flags;
  }
}

// Sets the OAM blend mode of a one-sprite object's sprite.
void ObjectDispatch_SetChildBlendMode(struct DispatchObject *object, u32 mode) {
  if (object != 0 && object->kind == 1) {
    struct FieldSprite *sprite = object->target.child;
    sprite->blendMode = mode;
  }
}

// Sets bit 1 of a one-sprite object's displayFlags: when set, the fixed
// camera draws it at its own scale, not scaled by distance.
void ObjectDispatch_SetChildDisplayFlag1(struct DispatchObject *object,
                                         u32 fixedScale) {
  if (object != 0 && object->kind == 1) {
    struct FieldSprite *sprite = object->target.child;
    sprite->unk1d1 = fixedScale;
  }
}

// Sets the draw parameter of a one-sprite object's layers (see
// ObjectGroup_SetChildValueUnlessFifteen).
void Animation_ApplyChildValues(struct DispatchObject *object, u32 param) {
  if (object != NULL && object->kind == 1)
    ObjectGroup_SetChildValueUnlessFifteen(object->target.child, param);
}

// Suspends the object camera updates, starts the palette back to normal and,
// a frame later, shows BG0 and the sprites with BG1-3 hidden.
void Graphics_EnableObjLayerAndCallbacks(void) {
  Scheduler_SuspendTasks(ObjectSystem_UpdateCamera);
  Scheduler_SuspendTasks(ObjectSystem_UpdateCameraFixed);
  BattleFx_ApplyColorToTargetBufferFar(0x10000, 1);
  BattleFx_StartBufferInterpolationFar(1);
  WaitFrames(1);
  REG_DISPCNT = (REG_DISPCNT & 0xf1ff) | DISPCNT_OBJ_ON;
}

// Resumes the object camera updates and hides BG1-3 and the sprites.
void ObjectDispatch_StopCallbacksAndHideLayers(void) {
  Scheduler_ResumeTasks(ObjectSystem_UpdateCamera);
  Scheduler_ResumeTasks(ObjectSystem_UpdateCameraFixed);
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
  struct CameraPoint unusedPoints[3];
  s32 objectIndex;
  struct MapScrollWork *mapWork;
  s32 viewX;
  s32 viewZ;
  struct ObjectSystemWork *system;
  struct CameraObject *object;
  struct CameraSprite *sprite;
#ifndef UBFIX
  s32 *viewCoordinates;
#endif
  u32 kind;
  s32 dx;
  s32 dz;
  s32 screenTop;
  struct CameraTile *tile;
  u32 priority;
  struct SpriteScale scale;
  struct SpritePlacement drawPosition;
  u32 layerPlusOne;

  // MapScrollWork owns all three layer cell pointers at their 0x30-byte stride.
  // UB: map and system work are separate globals, beyond gObjectSlots.
  // Reading their named slots adds eight bytes in the forced EN build.
#ifdef UBFIX
  mapWork = gMapWork[0];
#else
  mapWork = *(struct MapScrollWork **)(((u8 *)&gObjectSlots) + 12);
#endif
#ifdef UBFIX
  viewX = mapWork->viewX & 0xffff0000;
  viewZ = mapWork->viewY & 0xffff0000;
#else
  viewCoordinates = &mapWork->viewX;
  viewX = viewCoordinates[0] & 0xffff0000;
  viewZ = viewCoordinates[1] & 0xffff0000;
#endif
#ifdef UBFIX
  system = gMenuCtrlWork;
#else
  system = *(struct ObjectSystemWork **)(((u8 *)&gObjectSlots) + 4);
#endif
  // Copy the frame decoder (Render_DecodeFrame) into its heap slot.
  RamCode_LoadSlot(HEAP_SLOT_FRAME_SCRATCH, Render_DecodeFrame,
                   Render_DecodeFrameCodeSize);
  system->used = 0;
  object = (struct CameraObject *)gObjectSlots;
  for (objectIndex = 0; objectIndex < 64; objectIndex++, object++) {
    if (object->active == 0)
      continue;
    if (object->x != 0 || object->z != 0) {
      kind = object->kind & 15;
      if (kind == 0)
        continue;
      if (kind != 1)
        continue;
      if (system->suspended != 0 && object->held == 0) {
        struct CameraSprite *frozen = object->sprite;

        VramCache_ReleaseVram(frozen->resource);
        frozen->dirty = kind;
        continue;
      }
      dx = object->x - viewX;
      dz = object->z - viewZ;
      screenTop = dz - object->y;
      sprite = object->sprite;
      if (dx > -0x200000 && dx < 0x1100000 && screenTop > -0x200000 &&
          screenTop < 0xe00000) {
        // The cell under the object.
        tile = &((struct CameraTile *)mapWork->layers[object->layer]
                     .cells)[(object->x >> 20) + ((object->z >> 20) << 7)];
        if (object->flags & 1) {
          priority = tile->priority;
          if (priority != 0) {
            sprite->priority = priority;
            sprite->secondPriority = priority;
          }
        }
        layerPlusOne = tile->layer;
        if (layerPlusOne != 0)
          object->layer = layerPlusOne - 1;
        scale.x = Iwram_MulQ16(object->scaleX, sprite->scale);
        scale.y = Iwram_MulQ16(object->scaleY, sprite->scale);
        drawPosition.x = dx;
        drawPosition.height = object->y;
        drawPosition.y = dz;
        drawPosition.groundHeight = object->height;
        if (object->flags & 2) {
          drawPosition.height += 0xfec00000; // -320 pixels
          drawPosition.y += 0xfec00000;
          drawPosition.groundHeight += 0xfec00000;
        }
        if (object->flags & 4) {
          drawPosition.height += 0x1400000; // +320 pixels
          drawPosition.y += 0x1400000;
          drawPosition.groundHeight += 0x1400000;
        }
        Render_ApplyProjectedPlacement(sprite, &drawPosition, &scale,
                                       object->angle);
        continue;
      }
      if (object->held == 0) {
        if (!(sprite->flags1d & 1)) {
          VramCache_ReleaseVram(sprite->resource);
          sprite->dirty = 1;
        }
      }
    } else {
      kind = object->kind & 15;
      if (kind == 1) {
        sprite = object->sprite;
        if (object->held == 0 && !(sprite->flags1d & 1)) {
          VramCache_ReleaseVram(sprite->resource);
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
  struct CameraObject *object;
  s32 angle;
  s32 spritesRemaining;
  s32 kind;
  s32 *objectPosition;
  s32 fixedScale;
  void **sprites;
  void *sprite;
  s32 groupScale[2];
  s32 scale[2];
  /* FAKEMATCH: frame local: unused; the native frame keeps room for two
     points above the scales. Measured (2026-10-13): without it the frame
     is 28 bytes for 52 (differ 4, the two sp adjustments). */
  struct CameraPoint unusedPoints[2];

  // UB: camera and system work are separate globals, beyond gObjectSlots.
  // Reading their named slots adds four bytes in the forced EN build.
#ifdef UBFIX
  camera = (struct FixedCamera *)gCameraWork;
  system = gMenuCtrlWork;
#else
  camera = *(struct FixedCamera **)(((u8 *)&gObjectSlots) + 0x1c);
  system = *(struct ObjectSystemWork **)(((u8 *)&gObjectSlots) + 4);
#endif
  RamCode_LoadSlot(HEAP_SLOT_FRAME_SCRATCH, Render_DecodeFrame,
                   Render_DecodeFrameCodeSize);
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
    Transform_ApplyLookAt(eye, target);
  } else {
    Transform_SetLookAt(eye, target);
  }
  object = (struct CameraObject *)gObjectSlots;
  object += 63;
  fixedScale = 0x10000;
  for (i = 63; i >= 0; i--, object--) {
    if (object->active == 0)
      continue;
    kind = object->kind & 15;
    switch (kind) {
    case 1:
      objectPosition = &object->x;
      scale[0] = object->scaleX;
      scale[1] = object->scaleY;
      sprite = object->sprite;
      if (GameFlag_TestFar(363)) {
        scale[0] = fixedScale;
        scale[1] = fixedScale;
      }
      Render_PlaceProjectedSprite(sprite, objectPosition, scale,
                                  object->angle + angle, object->layer);
      break;
    case 2:
      groupScale[0] = object->scaleX;
      groupScale[1] = object->scaleY;
      if (GameFlag_TestFar(363)) {
        groupScale[0] = fixedScale;
        groupScale[1] = fixedScale;
      }
      sprites = object->sprite;
      for (spritesRemaining = 3; spritesRemaining >= 0; spritesRemaining--) {
        sprite = *sprites++;
        if (sprite != NULL)
          Render_PlaceProjectedSprite(sprite, &object->x, groupScale,
                                      object->angle + angle, object->layer);
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

// Polls a field object's common movement state until it arrives, up to 600
// frames. Actor, camera and effect-object views share this pointer boundary.
void Script_WaitForEventTimeout(void *fieldObject) {
  s32 waitedFrames;

  waitedFrames = 0;
  while (waitedFrames < 600 && Object_IsTargetUnset(fieldObject) == 0) {
    WaitFrames(1);
    waitedFrames++;
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

// value squared.
static __inline__ s32 Square(s32 value) { return value * value; }

// Advances all 64 field objects by one frame, as Object_UpdateAll does for
// the eight script objects, with the field's extras: free flight in 3D,
// terrain following, floating, and solid objects that refuse to overlap.
void Object_UpdateAllThumb(void) {
  struct ScriptMotionObject *object;
#ifdef UBFIX
  const s32 *script;
#else
  const struct ScriptMotionWords *script;
#endif
  s32 objectsRemaining;
  s32 x;
  s32 y;
  s32 z;
  s32 ground;
  s32 arrivedAtTarget;
  s32 pos[3];
  s32 dx;
  s32 dy;
  s32 dz;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 distance;
  s32 speed;
  s32 ratio;
  s32 reducedSpeed;
  s32 maxTerrainChange;
  s32 facingStep;
  u32 command;
  u32 phase;

  object = gObjectSlots;
  for (objectsRemaining = 63; objectsRemaining >= 0;
       objectsRemaining--, object++) {
    arrivedAtTarget = 0;

    script = object->script;
    if (script == NULL)
      continue;

    if (object->hook != NULL) {
      object->hook((union FieldObject *)object);
      script = object->script;
    }
    if (script == NULL)
      continue;
    if (object->paused != 0)
      continue;

    // Count down a wait, or run commands until one yields for this frame.
    if (object->wait != 0) {
      object->wait--;
    } else {
      for (;;) {
#ifdef UBFIX
        command = script[(s16)object->scriptCursor];
#else
        command = script->word[(s16)object->scriptCursor];
#endif
        if (command <= 63) {
          if (gScriptObject_CommandTable[command](
                  (struct ScriptInterpreter *)object) == 0)
            break;
        } else {
          object->scriptCursor++;
        }
        script = object->script;
      }
      if (object->script == NULL)
        continue;
    }

    x = object->x;
    y = object->y;
    z = object->z;

    if (object->frozen == 0) {
      // Free flight (no motion flags): steer in 3D towards the target,
      // capped at the speed limit, or brake when there is none.
      if (object->motionFlags == 0) {
        if (object->targetX != TARGET_UNSET) {
          dx = (object->targetX - x) / 65536;
          dy = (object->targetY - y) / 65536;
          dz = (object->targetZ - z) / 65536;
          distance = Iwram_Sqrt(Square(dx) + Square(dy) + Square(dz));
          if (distance == 0) {
            x = object->targetX;
            y = object->targetY;
            z = object->targetZ;
          } else {
            ratio = Iwram_RatioMulQ14(distance << 16, object->acceleration);
            object->velocityX += dx * ratio;
            object->velocityY += dy * ratio;
            object->velocityZ += dz * ratio;
            velocityX = object->velocityX;
            velocityY = object->velocityY;
            velocityZ = object->velocityZ;
            speed = FixedSqrt(Iwram_MulQ16(velocityX, velocityX) +
                              Iwram_MulQ16(velocityY, velocityY) +
                              Iwram_MulQ16(velocityZ, velocityZ));
            if (speed > object->speedLimit) {
              ratio = Iwram_RatioMulQ14(speed, object->speedLimit);
              object->velocityX = Iwram_MulQ16(velocityX, ratio);
              object->velocityY = Iwram_MulQ16(velocityY, ratio);
              object->velocityZ = Iwram_MulQ16(velocityZ, ratio);
            }
          }
        } else {
          dx = object->velocityX;
          dy = object->velocityY;
          dz = object->velocityZ;
          speed = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                            Iwram_MulQ16(dz, dz));
          if (speed != 0) {
            reducedSpeed = speed - object->acceleration;
            if (reducedSpeed < 0)
              reducedSpeed = 0;
            ratio = Iwram_RatioMulQ14(speed, reducedSpeed);
            object->velocityX = Iwram_MulQ16(dx, ratio);
            object->velocityY = Iwram_MulQ16(dy, ratio);
            object->velocityZ = Iwram_MulQ16(dz, ratio);
          } else {
            object->velocityX = 0;
            object->velocityY = 0;
            object->velocityZ = 0;
          }
        }
      } else {
        // On the ground: steer in the XZ plane the same way.
        if (object->targetX != TARGET_UNSET) {
          dx = (object->targetX - x) / 65536;
          dz = (object->targetZ - z) / 65536;
          distance = Iwram_Sqrt(Square(dx) + Square(dz)) << 16;
          if (distance <= 0x00ffffff) {
            dx = object->targetX - x;
            dz = object->targetZ - z;
            distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
          }
          if (distance == 0) {
            x = object->targetX;
            z = object->targetZ;
          } else {
            ratio = Iwram_RatioMulQ14(distance, object->acceleration);
            object->velocityX += Iwram_MulQ16(dx, ratio);
            object->velocityZ += Iwram_MulQ16(dz, ratio);
            velocityX = object->velocityX;
            velocityZ = object->velocityZ;
            speed = FixedSqrt(Iwram_MulQ16(velocityX, velocityX) +
                              Iwram_MulQ16(velocityZ, velocityZ));
            if (speed > object->speedLimit) {
              ratio = Iwram_RatioMulQ14(speed, object->speedLimit);
              object->velocityX = Iwram_MulQ16(velocityX, ratio);
              object->velocityZ = Iwram_MulQ16(velocityZ, ratio);
            }
          }
        } else {
          dx = object->velocityX;
          dz = object->velocityZ;
          speed = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
          if (speed != 0) {
            reducedSpeed = speed - object->acceleration;
            if (reducedSpeed < 0)
              reducedSpeed = 0;
            ratio = Iwram_RatioMulQ14(speed, reducedSpeed);
            object->velocityX = Iwram_MulQ16(dx, ratio);
            object->velocityZ = Iwram_MulQ16(dz, ratio);
          } else {
            object->velocityX = 0;
            object->velocityZ = 0;
          }
        }

        // Follow the terrain: take the ground height where the object is
        // going and move with its change, unless the object is 4 or more
        // pixels above the ground. Unless bit 4 is set, a climb or drop also
        // slows it, by three times the change (at most half its
        // acceleration).
        if ((object->motionFlags & 1) != 0) {
          velocityX = x + object->velocityX;
          velocityZ = z + object->velocityZ;
          ground =
              Map_GetTerrainHeight(object->terrainId, velocityX, velocityZ);
          velocityY = ground - object->terrainHeight;
          if (ground - y > -0x40000)
            y += velocityY;
          if (velocityY < 0)
            velocityY = -velocityY;
          maxTerrainChange = object->acceleration / 2;
          if (velocityY > maxTerrainChange)
            velocityY = maxTerrainChange;
          velocityY *= 3;
          if (velocityY != 0 && (object->motionFlags & 0x10) == 0) {
            dx = object->velocityX;
            dy = object->velocityY;
            dz = object->velocityZ;
            speed = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                              Iwram_MulQ16(dz, dz));
            if (speed != 0) {
              reducedSpeed = speed - velocityY;
              if (reducedSpeed < 0)
                reducedSpeed = 0;
              ratio = Iwram_RatioMulQ14(speed, reducedSpeed);
              object->velocityX = Iwram_MulQ16(dx, ratio);
              object->velocityY = Iwram_MulQ16(dy, ratio);
              object->velocityZ = Iwram_MulQ16(dz, ratio);
            }
          }
          object->terrainHeight = ground;
        }

        // Gravity, and a bounce that loses energy on landing.
        if ((object->motionFlags & 2) != 0) {
          ground = object->terrainHeight;
          if (y > ground) {
            object->velocityY -= object->gravity;
          } else if (object->velocityY < 0) {
            y = ground;
            object->velocityY =
                -Iwram_MulQ16(object->velocityY, object->vertical.bounce);
            if ((object->velocityY < 0 ? -object->velocityY
                                       : object->velocityY) <= object->gravity)
              object->velocityY = 0;
          }
        }

        // Float: bob up and down on a triangle wave (bit 3 makes it bigger).
        if ((object->motionFlags & 4) != 0) {
          phase = (object->vertical.phase & 0x3f);
          if ((object->motionFlags & 8) != 0)
            object->velocityY =
                gScriptObject_CycleTable[phase >> 1] * object->gravity / 16;
          else
            object->velocityY =
                gScriptObject_CycleTable[phase >> 1] * object->gravity / 64;
          object->vertical.phase++;
        }
      }
    }

    x += object->velocityX;
    y += object->velocityY;
    z += object->velocityZ;

    // A solid object does not move into another; count the refusals.
    if ((object->collisionFlags & 0x80) != 0) {
      pos[0] = x;
      pos[1] = y;
      pos[2] = z;
      if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)object, pos) !=
          0) {
        object->refused++;
        continue;
      }
      object->refused = 0;
    }

    // Arrived: the watched axis reached the target, or crossed it.
    switch (object->arrivalAxis) {
    case ARRIVAL_AXIS_X:
      if (x == object->targetX ||
          ((object->x - object->targetX) ^ (x - object->targetX)) < 0)
        arrivedAtTarget = 1;
      break;
    case ARRIVAL_AXIS_Y:
      if (y == object->targetY ||
          ((object->y - object->targetY) ^ (y - object->targetY)) < 0)
        arrivedAtTarget = 1;
      break;
    case ARRIVAL_AXIS_Z:
      if (z == object->targetZ ||
          ((object->z - object->targetZ) ^ (z - object->targetZ)) < 0)
        arrivedAtTarget = 1;
      break;
    default:
      break;
    }

    if (arrivedAtTarget != 0) {
      if (object->snapToTarget != 0) {
        x = object->targetX;
        object->velocityX = 0;
        z = object->targetZ;
        object->velocityZ = 0;
        if (object->motionFlags == 0) {
          y = object->targetY;
          object->velocityY = 0;
        }
      }
      object->targetX = TARGET_UNSET;
      object->targetY = TARGET_UNSET;
      object->targetZ = TARGET_UNSET;
      object->arrivalAxis = 0;
    }

    object->x = x;
    object->y = y;
    object->z = z;

    // Turn to face the direction of travel, at most 0x1000 a frame.
    if ((object->steeringFlags & 1) != 0) {
      x = object->velocityX;
      z = object->velocityZ;
      if (x != 0 || z != 0) {
        s32 maximumFacingStep;

        facingStep = (s16)(ArcTan2(z, x) - object->facing);
        maximumFacingStep = 0x1000;
        if (facingStep > maximumFacingStep)
          facingStep = maximumFacingStep;
        maximumFacingStep = -0x1000;
        if (facingStep < maximumFacingStep)
          facingStep = maximumFacingStep;
        object->facing += facingStep;
      }
    }
  }
}

// Places the object at (x, y, z), in 16.16 pixels, clears its movement target
// and stops it.
void Object_SetPositionAndResetMotion(struct ObjectRuntime *object, s32 x,
                                      s32 y, s32 z) {
  object->z = z;
  object->targetX = TARGET_UNSET;
  object->targetY = TARGET_UNSET;
  object->targetZ = TARGET_UNSET;
  object->x = x;
  object->y = y;
  object->velocityX = 0;
  object->velocityY = 0;
  object->velocityZ = 0;
}

// Sends an object towards (x, y, z), in 16.16 pixels. Within a pixel it
// arrives at once. Unless it snaps to its target, the target is pulled in
// by half its braking distance (speed limit squared over acceleration), or
// to halfway when that is further, as the object coasts on. Picks the
// axis to watch for arrival: the one with the furthest to go (y only for a
// free-flying object).
void Object_SetMoveTarget(struct ObjectRuntime *object, s32 x, s32 y, s32 z) {
  s32 dx;
  s32 distance;
  s32 dz;
  s32 squaredZ;
  u8 *arrivalAxis;

  dx = (x - object->x) / 0x10000;
  distance = (y - object->y) / 0x10000;
  dz = (z - object->z) / 0x10000;
  squaredZ = dz * dz;
  distance = Iwram_Sqrt(dx * dx + distance * distance + squaredZ) << 16;
  if (distance < 0x100000) {
    dx = x - object->x;
    distance = y - object->y;
    dz = z - object->z;
    distance =
        FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(distance, distance) +
                  Iwram_MulQ16(dz, dz));
  }
  if (distance < 0x10000) {
    object->x = x;
    object->y = y;
    object->z = z;
    object->targetX = TARGET_UNSET;
    object->targetY = TARGET_UNSET;
    object->targetZ = TARGET_UNSET;
    return;
  }
  if (object->snapToTarget == 0) {
    s32 brakingDistance;

    brakingDistance =
        Iwram_RatioMulQ14(object->acceleration,
                          Iwram_MulQ16(object->speedLimit, object->speedLimit));
    if (distance > brakingDistance)
      brakingDistance = distance - brakingDistance / 2;
    else
      brakingDistance = distance / 2;
    distance = Iwram_RatioMulQ14(distance, brakingDistance);
    x = object->x + Iwram_MulQ16(x - object->x, distance);
    y = object->y + Iwram_MulQ16(y - object->y, distance);
    z = object->z + Iwram_MulQ16(z - object->z, distance);
  }
  object->targetX = x;
  object->targetY = y;
  object->targetZ = z;
  dx = x - object->x;
  distance = y - object->y;
  dz = z - object->z;
  arrivalAxis = &object->arrivalAxis;
  *arrivalAxis = ARRIVAL_AXIS_X;
  if ((dx < 0 ? -dx : dx) < (dz < 0 ? -dz : dz)) {
    *arrivalAxis = ARRIVAL_AXIS_Z;
    dx = dz;
  }
  if (object->flags == 0) {
    if (dx < 0)
      dx = -dx;
    if (dx < (distance < 0 ? -distance : distance))
      *arrivalAxis = ARRIVAL_AXIS_Y;
  }
}

// Copies Object_UpdateAll (ARM code) to the heap, runs it once and frees it.
void Object_RunUpdateAllFromHeap(void) {
  void (*updateObjects)(void);

  RamCode_Load(updateObjects, Object_UpdateAll, Object_UpdateAllCodeSize);
  updateObjects();
  MemFree(updateObjects);
}

// Advances the first 14 objects' motion by one frame, without running their
// scripts: steering towards a target or braking, gravity and bouncing,
// arrival, and turning to face the direction of travel.
void Object_UpdateAllMotion(void) {
  struct ScriptMotionObject *object;
  s32 objectsRemaining;
  s32 y;
  s32 arrivedAtTarget;
  s32 x;
  s32 z;
  s32 dx;
  s32 dz;
  s32 velocityX;
  s32 distance;
  s32 ratio;
  s32 facingStep;

  object = gObjectSlots;
  for (objectsRemaining = 13; objectsRemaining >= 0;
       objectsRemaining--, object++) {
    if (object->script == 0)
      continue;
    x = object->x;
    y = object->y;
    z = object->z;
    // UB: a frozen object otherwise reads an uninitialized arrival flag, or
    // inherits the preceding object's flag. Initializing it here changes the
    // EN load/store schedule while keeping the 788-byte function extent.
#ifdef UBFIX
    arrivedAtTarget = 0;
#endif
    if (object->frozen == 0) {
#ifndef UBFIX
      arrivedAtTarget = 0;
#endif
      if (object->targetX != TARGET_UNSET) {
        dx = (object->targetX - x) / 65536;
        dz = (object->targetZ - z) / 65536;
        distance = Iwram_Sqrt(dx * dx + dz * dz) << 16;
        if (distance <= 0xffffff) {
          dx = object->targetX - x;
          dz = object->targetZ - z;
          distance = Iwram_Sqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz))
                     << 8;
        }
        if (distance == 0) {
          x = object->targetX;
          z = object->targetZ;
        } else {
          ratio = Iwram_RatioMulQ14(distance, object->acceleration);
          velocityX = object->velocityX + Iwram_MulQ16(dx, ratio);
          object->velocityX = velocityX;
          dx = object->velocityZ + Iwram_MulQ16(dz, ratio);
          object->velocityZ = dx;
          distance = Iwram_Sqrt(Iwram_MulQ16(velocityX, velocityX) +
                                Iwram_MulQ16(dx, dx))
                     << 8;
          if (distance > object->speedLimit) {
            ratio = Iwram_RatioMulQ14(distance, object->speedLimit);
            object->velocityX = Iwram_MulQ16(velocityX, ratio);
            object->velocityZ = Iwram_MulQ16(dx, ratio);
          }
        }
      } else {
        dx = object->velocityX;
        dz = object->velocityZ;
        if ((dx | dz) != 0) {
          distance = Iwram_Sqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz))
                     << 8;
          if (distance != 0) {
            if (distance - object->acceleration < 0) {
              object->velocityX = 0;
              object->velocityZ = 0;
            } else {
              ratio =
                  Iwram_RatioMulQ14(distance, distance - object->acceleration);
              object->velocityX = Iwram_MulQ16(dx, ratio);
              object->velocityZ = Iwram_MulQ16(dz, ratio);
            }
          } else {
            object->velocityX = 0;
            object->velocityZ = 0;
          }
        }
      }
      if (object->motionFlags & 2) {
        if (y > object->terrainHeight) {
          object->velocityY -= object->gravity;
        } else if (object->velocityY < 0) {
          y = object->terrainHeight;
          object->velocityY =
              -Iwram_MulQ16(object->velocityY, object->vertical.bounce);
          if ((object->velocityY < 0 ? -object->velocityY
                                     : object->velocityY) <= object->gravity)
            object->velocityY = 0;
        }
      }
    }
    x += object->velocityX;
    y += object->velocityY;
    z += object->velocityZ;
    if (object->arrivalAxis != 0)
      switch (object->arrivalAxis) {
      case 16:
        if (x == object->targetX ||
            ((object->x - object->targetX) ^ (x - object->targetX)) < 0)
          arrivedAtTarget = 1;
        break;
      case 17:
        if (y == object->targetY ||
            ((object->y - object->targetY) ^ (y - object->targetY)) < 0)
          arrivedAtTarget = 1;
        break;
      case 18:
        if (z == object->targetZ ||
            ((object->z - object->targetZ) ^ (z - object->targetZ)) < 0)
          arrivedAtTarget = 1;
        break;
      }
    if (arrivedAtTarget) {
      if (object->snapToTarget) {
        object->velocityX = 0;
        object->velocityZ = 0;
        x = object->targetX;
        z = object->targetZ;
        if (object->motionFlags == 0) {
          y = object->targetY;
          object->velocityY = 0;
        }
      }
      object->targetX = TARGET_UNSET;
      object->targetY = TARGET_UNSET;
      object->targetZ = TARGET_UNSET;
      object->arrivalAxis = 0;
    }
    object->x = x;
    object->y = y;
    object->z = z;
    if (object->steeringFlags & 1) {
      x = object->velocityX;
      z = object->velocityZ;
      if (x != 0 || z != 0) {
        facingStep = (s16)(ArcTan2(z, x) - object->facing);
        if (facingStep > 0x1000)
          facingStep = 0x1000;
        if (facingStep < -0x1000)
          facingStep = -0x1000;
        object->facing += facingStep;
      }
    }
  }
}

// SC_WAIT(n): waits n frames; this frame counts as the first. Returns 0 to
// end the object's commands for this frame.
s32 Script_StoreLookupResult(struct ScriptInterpreter *interpreter) {
  s16 commandCursor = interpreter->cursor;

  interpreter->lookupResult = interpreter->script[commandCursor + 1] - 1;
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
  s16 initialCursor = interpreter->cursor;
  ScriptCommand callback =
      (ScriptCommand)interpreter->script[initialCursor + 1];

  if (callback(interpreter) != 0)
    return 0;
  if (interpreter->cursor == initialCursor)
    interpreter->cursor = interpreter->cursor + 2;
  return 1;
}

// Returns the cursor just past the script's label key (SC_LABEL or SC_TO,
// matched without the bit that tells them apart), searching its first 1024
// words, or 0 (the start) when key is 0 or not found. Clears the wait.
s32 Script_FindLabel(struct ScriptInterpreter *interpreter, u32 labelKey) {
  u32 *scriptWords;
  s32 wordIndex;
  s16 *waitCounter = &interpreter->lookupResult;
  s32 zero = 0;

  *waitCounter = zero;
  if (labelKey == 0) {
    return 0;
  }

  labelKey &= 0xBFFFFFFF;
  scriptWords = (u32 *)interpreter->script;
  wordIndex = 0;
  do {
    if (*scriptWords++ == labelKey) {
      return wordIndex + 1;
    }
    wordIndex++;
  } while (wordIndex <= 0x3FF);
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
s32 Script_JumpIfTrue(struct ScriptInterpreter *interpreter) {
  s32 commandCursor = interpreter->cursor;
  const s32 *script = interpreter->script;
  u32 labelKey = script[commandCursor + 1];

  if (interpreter->conditionResult != 0) {
    interpreter->cursor = Script_FindLabel(interpreter, labelKey);
  } else {
    interpreter->cursor += 2;
  }
  return 1;
}

// SC_IFNOT(to): jumps to label to when the last test was false.
s32 Script_JumpIfFalse(struct ScriptInterpreter *interpreter) {
  s32 commandCursor = interpreter->cursor;
  const s32 *script = interpreter->script;
  u32 labelKey = script[commandCursor + 1];

  if (interpreter->conditionResult == 0) {
    interpreter->cursor = Script_FindLabel(interpreter, labelKey);
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
    s32 flagId;                                                                \
                                                                               \
    flagId = interpreter->script[interpreter->cursor + 1];                     \
    interpreter->conditionResult = GameFlag_TestFar(flagId);                   \
    change(flagId);                                                            \
    interpreter->cursor += 2;                                                  \
    return 1;                                                                  \
  }

// SC_FLAG_ON(f) and SC_FLAG_OFF(f): test game flag f, then set or clear it.
DEFINE_FLAG_AND_TEST(Script_SetFlagAndTest, GameFlag_SetBitFar)
DEFINE_FLAG_AND_TEST(Script_ClearFlagAndTest, GameFlag_ClearBitFar)

// SC_FLAG_REV(f): tests game flag f, then flips it.
s32 Script_ToggleFlagAndTest(struct ScriptInterpreter *interpreter) {
  s32 flagId;
  s32 previousFlagValue;

  flagId = interpreter->script[interpreter->cursor + 1];
  previousFlagValue = GameFlag_TestFar(flagId);
  interpreter->conditionResult = previousFlagValue;
  if ((previousFlagValue << 0x18) == 0x01000000) {
    GameFlag_ClearBitFar(flagId);
  } else {
    GameFlag_SetBitFar(flagId);
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
  s32 objectIndex;
  u8 *collisionFlags;
  struct ObjectRuntime *candidate = gObjectSlots;

  collisionFlags = &candidate->unk59;
  for (objectIndex = 0; objectIndex < 64;
       objectIndex++, candidate++, collisionFlags += sizeof(*candidate)) {
    if (candidate->script != NULL && (1 & *collisionFlags) &&
        candidate != (struct ObjectRuntime *)object) {
      if (Runtime_CheckRadiusOverlap(
              &candidate->x, ((struct FieldActor *)candidate)->radius - 2,
              values, ((struct FieldActor *)object)->radius - 2) >= 0)
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
  s32 objectIndex;
  u8 *collisionFlags;
  struct ObjectRuntime *candidate = gObjectSlots;

  collisionFlags = &candidate->unk59;
  for (objectIndex = 0; objectIndex < 64;
       objectIndex++, candidate++, collisionFlags += sizeof(*candidate)) {
    if (candidate->script != NULL && (1 & *collisionFlags) &&
        candidate != (struct ObjectRuntime *)object) {
      if (Runtime_CheckRadiusOverlap(
              &candidate->x, ((struct FieldActor *)candidate)->radius - 2,
              values, ((struct FieldActor *)object)->radius - 2) >= 0)
        return (struct ScriptObjectEntry *)candidate;
    }
  }
  return NULL;
}

// Reads three coordinate words and applies the selected positioning operation.
static __inline__ s32 ApplyScriptPositionCommand(
    struct ScriptInterpreter *interpreter,
    void (*applyPosition)(struct ObjectRuntime *object, s32 x, s32 y, s32 z)) {
  s32 x;
  s32 y;
  s32 z;
  const s32 *coordinates;

  coordinates = interpreter->script + interpreter->cursor;
  coordinates++;
  x = *coordinates;
  coordinates++;
  y = *coordinates;
  coordinates++;
  z = *coordinates;
  applyPosition((struct ObjectRuntime *)interpreter, x, y, z);
  // The command and its three coordinates occupy four script words.
  interpreter->cursor += 4;
  return 1;
}

// SC_POS(x, y, z): places the object at (x, y, z) and stops it.
s32 Script_SetPositionAndResetMotion(struct ScriptInterpreter *interpreter) {
  return ApplyScriptPositionCommand(interpreter,
                                    Object_SetPositionAndResetMotion);
}

// SC_MOVE(x, y, z): sends the object towards (x, y, z).
s32 Script_ApplyAbsolutePosition(struct ScriptInterpreter *interpreter) {
  return ApplyScriptPositionCommand(interpreter, Object_SetMoveTarget);
}

// SC_MOVE_BY(x, y, z): sends the object towards its position plus (x, y, z).
s32 Script_ApplyRelativePosition(struct ScriptInterpreter *interpreter) {
  struct ScriptObjectRuntime *object =
      (struct ScriptObjectRuntime *)interpreter;

  u8 *commandBytes = (u8 *)(object->script + (s16)object->scriptCursor);
  s32 *arguments = (s32 *)(commandBytes + 4);
  s32 deltaX = *arguments++;
  s32 deltaY = *arguments++;
  s32 deltaZ = *arguments;

  Object_SetMoveTarget((struct ObjectRuntime *)object, object->x + deltaX,
                       object->y + deltaY, object->z + deltaZ);
  object->scriptCursor += 4;
  return 1;
}

// SC_FACE_LINK: stores the angle from the object to its linked object in
// scriptValue.
s32 Script_StoreAngleToLinkedObject(struct ScriptInterpreter *interpreter) {
  struct ScriptObjectRuntime *object =
      (struct ScriptObjectRuntime *)interpreter;

  struct ScriptObjectRuntime *target;

  target = object->linkedObject;
  object->scriptValue =
      ArcTan2((target->z - object->z), (target->x - object->x));
  object->scriptCursor++;
  return 1;
}

// SC_MOVE_LINK: sends the object towards its linked object.
s32 Script_ApplyLinkedObjectPosition(struct ScriptInterpreter *interpreter) {
  struct ScriptObjectRuntime *object =
      (struct ScriptObjectRuntime *)interpreter;

  struct ScriptObjectRuntime *target;

  target = object->linkedObject;
  Object_SetMoveTarget((struct ObjectRuntime *)object, target->x, target->y,
                       target->z);
  object->scriptCursor++;
  return 1;
}

// SC_MOVE_LOCAL(a, b): sends the object towards its position plus an offset
// that is never filled in; the operands are skipped unread.
s32 Script_ApplyLocalOffsetPosition(struct ScriptInterpreter *interpreter) {
  struct ScriptObjectRuntime *object =
      (struct ScriptObjectRuntime *)interpreter;

  s32 localOffset[3];

  // BUG: offset is never set, so the target is the position plus whatever
  // the stack held.
#ifdef UBFIX
  localOffset[0] = localOffset[1] = localOffset[2] = 0;
#endif
  Object_SetMoveTarget((struct ObjectRuntime *)object,
                       object->x + localOffset[0], object->y + localOffset[1],
                       object->z + localOffset[2]);
  object->scriptCursor += 3;
  return 1;
}

// SC_CAMERA: keeps the camera object on its leader (linkedObject), clamped
// to 120 pixels inside the map's left and right edges and a band above and
// below the leader. It closes an eighth of the distance a frame, at most
// its speed limit, and a quarter of a large height difference; when action
// is set it jumps there at once.
s32 Camera_FollowLeaderInBounds(struct ScriptInterpreter *interpreter) {
  struct ObjectRuntime *cameraObject = (struct ObjectRuntime *)interpreter;

  struct ObjectRuntime *leader;
  struct MapScrollWork *bounds;
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
  s32 distance;
  s32 moveStep;

  bounds = gMapWork[0];
  leader = cameraObject->linkedObject;
  minX = bounds->minX + 0x780000;
  minZ = bounds->minY + leader->y + 0x600000;
  maxX = bounds->maxX - 0x780000;
  maxZ = bounds->maxY + leader->y - 0x400000;
  cameraObject->flags = 0;
  if (leader != NULL && leader->script != NULL) {
    x = leader->x;
    y = leader->y;
    z = leader->z;
    cameraObject->targetX = TARGET_UNSET;
    cameraObject->targetY = TARGET_UNSET;
    cameraObject->targetZ = TARGET_UNSET;
    if (x < minX)
      x = minX;
    if (z < minZ)
      z = minZ;
    if (x > maxX)
      x = maxX;
    if (z > maxZ)
      z = maxZ;
    if (cameraObject->action != 0) {
      cameraObject->x = x;
      cameraObject->y = y;
      cameraObject->z = z;
    } else {
      // The distance in the XZ plane: dy is computed as in
      // Object_SetMoveTarget but not used for the distance.
      dx = (x - cameraObject->x) / 0x10000;
      /* FAKEMATCH: dead code: this dy is overwritten below and the compiler
       * drops it, but its references rank y above the bounds in global
       * allocation, keeping y in r5 as native; without it 114 rows differ in
       * the English build (y in r9). The form is the sibling
       * Object_SetMoveTarget's three-axis delta. Clamping X then Z with one
       * minimum/maximum pair and no dead dy changes 273 text bytes in EN. */
      dy = (y - cameraObject->y) / 0x10000;
      dz = (z - cameraObject->z) / 0x10000;
      distance = Iwram_Sqrt(dx * dx + dz * dz) << 16;
      dx = x - cameraObject->x;
      dy = y - cameraObject->y;
      dz = z - cameraObject->z;
      if (distance < 0x400000)
        distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
      if ((moveStep = distance / 8) > cameraObject->speedLimit)
        moveStep = cameraObject->speedLimit;
      if (distance < 0x4000) {
        cameraObject->x = x;
        cameraObject->z = z;
      } else {
        if (distance > moveStep) {
          dx = Iwram_MulQ16(Iwram_RatioMulQ14(distance, dx), moveStep);
          dz = Iwram_MulQ16(Iwram_RatioMulQ14(distance, dz), moveStep);
        }
        cameraObject->x += dx;
        cameraObject->z += dz;
      }
      if ((dy < 0 ? -dy : dy) > 0x8000)
        dy /= 4;
      cameraObject->y += dy;
    }
  }
  cameraObject->step++;
  return 1;
}

extern u8 IwramSqrt[];
s32 _call_via_r3(s32, s32, s32, s32);

// SC_APPROACH_LINK: follows the linked object at its speed. More than 16
// pixels away it walks to 16 pixels short of it (animation 2) and runs on;
// within 16 it stands (animation 1) and waits for the next frame.
s32 Object_ApproachLinkedObject(struct ScriptInterpreter *interpreter) {
  struct ObjectRuntime *object = (struct ObjectRuntime *)interpreter;

  s32 distancePixels;
  s32 moveZ;
  s32 dx;
  s32 dz;
  s32 deltaZPixels;
  s32 deltaXPixels;
  s32 remainingDistance;
  s32 moveX;
  s32 scaledDeltaX;
  struct ObjectRuntime *linkedObject;

  linkedObject = object->linkedObject;
  object->speedLimit = linkedObject->speedLimit;
  object->acceleration = linkedObject->acceleration;
  dx = linkedObject->x - object->x;
  dz = linkedObject->z - object->z;
  deltaXPixels = dx >> 0x10;
  deltaZPixels = dz >> 0x10;
  distancePixels = _call_via_r3((deltaXPixels * deltaXPixels) +
                                    (deltaZPixels * deltaZPixels),
                                dx, deltaZPixels, (u32)IwramSqrt);
  if (distancePixels > 0x10) {
    scaledDeltaX = dx;
    remainingDistance = distancePixels - 0x10;
    moveX = scaledDeltaX * remainingDistance / distancePixels;
    moveZ = dz * remainingDistance / distancePixels;
    Object_SetMoveTarget(object, object->x + moveX, object->y,
                         object->z + moveZ);
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
s32 Object_Wander(struct ScriptInterpreter *interpreter) {
  struct ScriptObjectRuntime *object =
      (struct ScriptObjectRuntime *)interpreter;

  struct FieldPosition candidatePosition;
  struct FieldPosition probePosition;
  s32 minimumDistance;
  const s32 *operands;
  s32 distanceRange;
  s32 homeRadiusSquared;
  s32 travelDistance;
  s32 heading;
  s32 attempts;
  s32 dx;
  s32 dz;

  operands = &object->script[(s16)object->scriptCursor + 1];
  minimumDistance = *operands++;
  distanceRange = *operands++;
  homeRadiusSquared = *operands / 0x10000;
  attempts = 0;
  homeRadiusSquared = homeRadiusSquared * homeRadiusSquared;
retry:
  attempts++;
  if (attempts <= 7) {
    candidatePosition.x = object->x;
    candidatePosition.y = object->y;
    candidatePosition.z = object->z;
    travelDistance = minimumDistance + Iwram_MulQ16(Random16(), distanceRange);
    heading = object->scriptValue + (Random16() >> 2) - (Random16() >> 2);
    Vector_AddPolarOffset(travelDistance, heading, &candidatePosition);
    if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)object,
                                  (s32 *)&candidatePosition) != 0 ||
        Map_CheckObjectStep((struct ObjectRuntime *)object,
                            &candidatePosition) != 0)
      goto retry;
    travelDistance += 0x80000;
    probePosition.x = object->x;
    probePosition.y = object->y;
    probePosition.z = object->z;
    Vector_AddPolarOffset(travelDistance, heading, &probePosition);
    probePosition.x = object->x;
    probePosition.y = object->y;
    probePosition.z = object->z;
    Vector_AddPolarOffset(travelDistance, heading + 0x2000, &probePosition);
    if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probePosition) !=
        0)
      goto retry;
    probePosition.x = object->x;
    probePosition.y = object->y;
    probePosition.z = object->z;
    Vector_AddPolarOffset(travelDistance, heading - 0x2000, &probePosition);
    if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probePosition) !=
        0)
      goto retry;
    /* FAKEMATCH: one-pass block: it ends cse2's block, so the division works on
     * the loaded pos.x and the copy outlives it into the move target call. As
     * plain statements cse2 swaps them (differ 16), also with dx/dz split or
     * casts. The retry gotos stay: as for(;;) with continue, loop.c hoists &pos
     * and &probe, which the native recomputes each try (differ 128).
     * Moving success into a <= limit branch without the block changes 53
     * text bytes: the coordinate copies still swap and exits reorder.
     * A shared inline home-radius predicate used here and by WanderNearHome
     * adds 8 bytes to each function in EN; that command's deltas must remain
     * live for its fallback heading. */
    do {
      dx = candidatePosition.x / 0x10000 - object->homeX;
      dz = candidatePosition.z / 0x10000 - object->homeZ;
    } while (0);
    if (dx * dx + dz * dz > homeRadiusSquared)
      goto retry;
  } else {
    object->scriptValue += 0x8000;
    object->turnedBack = 1;
    return 0;
  }
  Object_SetMoveTarget((struct ObjectRuntime *)object, candidatePosition.x,
                       candidatePosition.y, candidatePosition.z);
  object->scriptCursor += 4;
  return 1;
}

// SC_WANDER_HOME(base, range, limit): wanders as SC_WANDER does, testing
// more ground around the point, while it is within limit pixels of home;
// outside that, or when no point works, it heads back towards home.
s32 ScriptObject_WanderNearHome(struct ScriptInterpreter *interpreter) {
  struct ScriptObjectRuntime *object =
      (struct ScriptObjectRuntime *)interpreter;

  struct FieldPosition candidatePosition;
  struct FieldPosition probePosition;
  s32 minimumDistance;
  const s32 *operands;
  s32 distanceRange;
  s32 homeRadiusSquared;
  s32 travelDistance;
  u16 heading;
  u16 angle;
  s32 attempts;
  s32 dx;
  s32 dz;

  operands = &object->script[(s16)object->scriptCursor + 1];
  minimumDistance = *operands++;
  distanceRange = *operands++;
  homeRadiusSquared = *operands / 0x10000;
  angle = object->scriptValue;
  homeRadiusSquared *= homeRadiusSquared;
  attempts = 0;
  dx = object->x / 0x10000 - object->homeX;
  dz = object->z / 0x10000 - object->homeZ;
  if (dx * dx + dz * dz <= homeRadiusSquared) {
  roam:
    attempts++;
    if (attempts <= 7) {
      travelDistance =
          minimumDistance + Iwram_MulQ16(Random16(), distanceRange);
      heading = angle + (Random16() >> 2) - (Random16() >> 2);
      candidatePosition.x = object->x;
      candidatePosition.y = object->y;
      candidatePosition.z = object->z;
      Vector_AddPolarOffset(0x80000, heading, &candidatePosition);
      if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)object,
                                    (s32 *)&candidatePosition) != 0)
        goto roam;
      candidatePosition.x = object->x;
      candidatePosition.y = object->y;
      candidatePosition.z = object->z;
      Vector_AddPolarOffset(travelDistance, heading, &candidatePosition);
      if (Map_CheckObjectStep((struct ObjectRuntime *)object,
                              &candidatePosition) != 0)
        goto roam;
      probePosition.x = object->x;
      probePosition.y = object->y;
      travelDistance += 0x80000;
      probePosition.z = object->z;
      Vector_AddPolarOffset(travelDistance, heading, &probePosition);
      if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probePosition) !=
          0)
        goto roam;
      probePosition.x = object->x;
      probePosition.y = object->y;
      probePosition.z = object->z;
      Vector_AddPolarOffset(travelDistance, heading + 0x2000, &probePosition);
      if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probePosition) !=
          0)
        goto roam;
      probePosition.x = object->x;
      probePosition.y = object->y;
      probePosition.z = object->z;
      Vector_AddPolarOffset(travelDistance, heading - 0x2000, &probePosition);
      if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probePosition) !=
          0)
        goto roam;
      probePosition.x = object->x;
      probePosition.y = object->y;
      probePosition.z = object->z;
      Vector_AddPolarOffset(travelDistance, heading + 0x4000, &probePosition);
      if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probePosition) !=
          0)
        goto roam;
      probePosition.x = object->x;
      probePosition.y = object->y;
      probePosition.z = object->z;
      Vector_AddPolarOffset(travelDistance, heading - 0x4000, &probePosition);
      if (Map_CheckObjectStep((struct ObjectRuntime *)object, &probePosition) !=
          0)
        goto roam;
      dx = candidatePosition.x / 0x10000 - object->homeX;
      dz = candidatePosition.z / 0x10000 - object->homeZ;
      if (dx * dx + dz * dz > homeRadiusSquared)
        goto roam;
      object->flags59 |= 2;
      Object_SetMoveTarget((struct ObjectRuntime *)object, candidatePosition.x,
                           candidatePosition.y, candidatePosition.z);
      goto done;
    }
  }
  attempts = 0;
  angle = (u16)ArcTan2(dz, dx) + 0x8000;
back:
  attempts++;
  if (attempts <= 7) {
    travelDistance = minimumDistance + Iwram_MulQ16(Random16(), distanceRange);
    heading = angle + (Random16() >> 2) - (Random16() >> 2);
    candidatePosition.x = object->x;
    candidatePosition.y = object->y;
    candidatePosition.z = object->z;
    Vector_AddPolarOffset(0x80000, heading, &candidatePosition);
    if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)object,
                                  (s32 *)&candidatePosition) != 0)
      goto back;
    candidatePosition.x = object->x;
    candidatePosition.y = object->y;
    candidatePosition.z = object->z;
    Vector_AddPolarOffset(travelDistance, heading, &candidatePosition);
    if (Map_CheckObjectStep((struct ObjectRuntime *)object,
                            &candidatePosition) != 0)
      goto back;
    object->flags59 &= ~2;
    Object_SetMoveTarget((struct ObjectRuntime *)object, candidatePosition.x,
                         candidatePosition.y, candidatePosition.z);
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

// Defines the handler for a 32-bit work value: set it, add to it, or compare
// it (into comparisonResult).
#define DEFINE_OPERAND_WORD(name, field)                                       \
  void name(struct ScriptOperands *operands, s32 operation, s32 value) {       \
    if (operation == SCRIPT_OPERAND_SET) {                                     \
      operands->field = value;                                                 \
      return;                                                                  \
    }                                                                          \
    if (operation == SCRIPT_OPERAND_ADD) {                                     \
      operands->field += value;                                                \
      return;                                                                  \
    }                                                                          \
    operands->comparisonResult = operands->field == value;                     \
  }

// The same for an address: an add steps by value words (value * 4 bytes).
#define DEFINE_OPERAND_ADDRESS(name, field)                                    \
  void name(struct ScriptOperands *operands, s32 operation, s32 value) {       \
    if (operation == SCRIPT_OPERAND_SET) {                                     \
      operands->field = value;                                                 \
      return;                                                                  \
    }                                                                          \
    if (operation == SCRIPT_OPERAND_ADD) {                                     \
      operands->field += value * 4;                                            \
      return;                                                                  \
    }                                                                          \
    operands->comparisonResult = operands->field == value;                     \
  }

// The same for an 8- or 16-bit value: a compare narrows value to its type.
#define DEFINE_OPERAND_NARROW(name, field, type)                               \
  void name(struct ScriptOperands *operands, s32 operation, s32 value) {       \
    if (operation == SCRIPT_OPERAND_SET) {                                     \
      operands->field = value;                                                 \
    } else if (operation == SCRIPT_OPERAND_ADD) {                              \
      operands->field += value;                                                \
    } else {                                                                   \
      operands->comparisonResult = operands->field == (type)value;             \
    }                                                                          \
  }

// The work value handlers, one for each entry of enum ScriptWork (see
// gScript_OperandHandlerTable in operand.c); each names the object field it
// sets, adds to or compares.
DEFINE_OPERAND_ADDRESS(Script_SetOrCompareAddress, scriptAddress)

// Work value SW_CURSOR: sets, adds to or compares the script cursor.
void Script_SetOrCompareCursor(struct ScriptOperands *operands, s32 operation,
                               s32 value) {
  if (operation == SCRIPT_OPERAND_SET) {
    operands->cursor = value;
  } else if (operation == SCRIPT_OPERAND_ADD) {
    operands->cursor = ((s16)operands->cursor + value);
  } else {
    operands->comparisonResult = (s16)operands->cursor == (s16)value;
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
static __inline__ s32 ApplyScriptOperandCommand(struct ScriptOperands *operands,
                                                s32 operation) {
  s16 commandCursor = operands->cursor;
  const s32 *arguments =
      (const s32 *)operands->scriptAddress + commandCursor + 1;
  OperandFunc handler = gScript_OperandHandlerTable[arguments[0]];

  if (handler != 0)
    handler(operands, operation, arguments[1]);
  operands->cursor += 3;
  return 1;
}

// SC_SET(w, v): sets work value w to v.
s32 Script_ApplyOperandSet(struct ScriptInterpreter *interpreter) {
  struct ScriptOperands *operands = (struct ScriptOperands *)interpreter;

  return ApplyScriptOperandCommand(operands, SCRIPT_OPERAND_SET);
}

// SC_ADD(w, v): adds v to work value w.
s32 Script_ApplyOperandAdd(struct ScriptInterpreter *interpreter) {
  struct ScriptOperands *operands = (struct ScriptOperands *)interpreter;

  return ApplyScriptOperandCommand(operands, SCRIPT_OPERAND_ADD);
}

// SC_CMP(w, v): tests whether work value w equals v.
s32 Script_ApplyOperandCompare(struct ScriptInterpreter *interpreter) {
  struct ScriptOperands *operands = (struct ScriptOperands *)interpreter;

  return ApplyScriptOperandCommand(operands, SCRIPT_OPERAND_COMPARE);
}

// Stores value as a halfword.
static __inline__ void StoreHalfword(s16 *address, s32 value) {
  *address = value;
}

// Stores value in the word that the object's unk6c points at.
void ObjectDispatch_SetUpdateValue(void *rawObject, s32 value) {
  struct ObjectRuntime *object = rawObject;

  if (object != NULL)
    *(s32 *)object->unk6c = value;
}

#if TBS_PATCH_1

#define ASSIGNED_KEYS gKeysPressed
#else

#define ASSIGNED_KEYS gKeysHeld
#endif

// Handles a shortcut key's assignment: its top two bits choose
// what it requests (0 a Psynergy, 1 an item) and
// the low 14 bits which one. With flag 0x107 set, or in mode 3 (where L and
// R give 0xfc88 and 0xfc87), it raises a fixed trigger instead. Returns the
// request.
u32 Field_StoreAssignedKeyValue(u32 encodedAssignment) {
  u32 assignmentKind = encodedAssignment >> 14;
  u32 requestId = 0x3FFF & encodedAssignment;
  struct FieldWork *fieldWork = (struct FieldWork *)gEventWork;

  if (GameFlag_TestFar(0x107) != 0) {
    StoreHalfword(&fieldWork->event, 0xFA);
  } else if (fieldWork->mode == 3) {
    if (ASSIGNED_KEYS & KEY_R) {
      StoreHalfword(&fieldWork->event, 0xFC88);
    } else if (ASSIGNED_KEYS & KEY_L) {
      StoreHalfword(&fieldWork->event, 0xFC87);
    }
  } else {
    switch (assignmentKind) {
    case 0:
      StoreHalfword(&fieldWork->command, requestId);
      break;
    case 1:
      StoreHalfword(&fieldWork->item, requestId);
      break;
    }
  }

  return requestId;
}

// Checks the keys the player has configured: Select, A and Start set their
// flag in the event work and return 1; the two shortcut keys pass on their
// assignment (Field_StoreAssignedKeyValue). Returns 0 when none is pressed.
s32 Field_CheckConfiguredKeys(void) {
  struct FieldWork *fieldWork = (struct FieldWork *)gEventWork;
  s32 result = 0;

  if (fieldWork == NULL)
    return 0;
  if (gKeysPressed & gGameState.selectKeys) {
    fieldWork->menu = 1;
    result = 1;
  } else if (gKeysPressed & gGameState.aKeys) {
    fieldWork->examine = 1;
    result = 1;
  } else if (gKeysPressed & gGameState.startKeys) {
    fieldWork->start = 1;
    result = 1;
  } else if (gKeysPressed & gGameState.firstShortcutKeys) {
    result = Field_StoreAssignedKeyValue(gGameState.firstShortcut);
  } else if (gKeysPressed & gGameState.secondShortcutKeys) {
    result = Field_StoreAssignedKeyValue(gGameState.secondShortcut);
  }
  return result;
}

// Returns 0 when two spheres overlap: at positions a and b (16.16 pixels)
// with radii radiusA and radiusB (pixels); -1 otherwise.
s32 Runtime_CheckRadiusOverlap(s32 *firstPosition, s32 radiusA,
                               s32 *secondPosition, s32 radiusB) {
  const struct FieldPosition *firstPoint =
      (const struct FieldPosition *)firstPosition;
  const struct FieldPosition *secondPoint =
      (const struct FieldPosition *)secondPosition;
  const s32 *firstCoordinate = &firstPoint->x;
  const s32 *secondCoordinate = &secondPoint->x;
  s32 dx = (*firstCoordinate++ - *secondCoordinate++) >> 16;
  s32 dy = (*firstCoordinate++ - *secondCoordinate++) >> 16;
  s32 dz = (*firstCoordinate - *secondCoordinate) >> 16;
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
s32 FieldObject_UpdatePlayerControl(struct ScriptInterpreter *interpreter) {
  struct ObjectRuntime *object = (struct ObjectRuntime *)interpreter;

  struct FieldPosition position;
  struct FieldPosition testPosition;
  struct FieldPosition contact;
  s16 angleOffsets[22];
  s32 collision;
  s32 pushed;
  s16 heading;
  s32 angle;
  s32 movementAnimation;
  u16 direction;
  s32 footprintAngle;
  s32 angleIndex;
  s32 headingCount;
  struct ObjectRuntime *pushedObject;

  collision = 0;
  pushed = 0;
  // Debug (flag 350): beep when fewer than 136 of the 512 resource blocks
  // are free.
  if (gDebugMode != 0 && GameFlag_TestFar(350) != 0) {
    s32 freeBlocks;
    s32 blockIndex;

    freeBlocks = 0;
    for (blockIndex = 0; blockIndex < 512; blockIndex++) {
      if (gResourceBlockOwners[blockIndex] == 0xff)
        freeBlocks++;
    }
    if (freeBlocks - 136 < 0)
      AudioCommand_PlayFar(135);
  }
  /* FAKEMATCH: the volatile read puts gDebugMode's address in r5 here and at
   * the test before it, as the bytes do; read plainly both take r3 (differ 12,
   * 808 instructions either way). Grouping both debug actions under an outer
   * ordinary test, retaining a fresh read after calls, changes 6 text bytes
   * in EN: the address still uses r3 and two branch targets differ. */
  if (*(volatile u8 *)&gDebugMode != 0) {
    s32 delayKeyMask;

    delayKeyMask = KEY_L;
    if (gKeysHeld & delayKeyMask) {
      s32 delayIterations;

      delayIterations = delayKeyMask;
    wait_a:
      --delayIterations;
      if (delayIterations != 0)
        goto wait_a;
      delayIterations = 95;
    wait_b:
      delayIterations--;
      if (delayIterations >= 0)
        goto wait_b;
      delayIterations = 63;
    wait_c:
      delayIterations--;
      if (delayIterations >= 0)
        goto wait_c;
      delayIterations = 63;
    wait_d:
      delayIterations--;
      if (delayIterations >= 0)
        goto wait_d;
    }
  }
  // Walk, or run with the dash keys (animation 5 instead of 2); flag 0x17f
  // and B move much faster.
  if (gKeysHeld & gGameState.dashKeys) {
    object->speedLimit = 0x18000;
    object->acceleration = 0x4000;
    movementAnimation = 5;
  } else {
    object->speedLimit = 0x10000;
    object->acceleration = 0x4000;
    movementAnimation = 2;
  }
  if (GameFlag_TestFar(0x17f) != 0 && (gKeysHeld & KEY_B)) {
    object->speedLimit = 0x40000;
    object->acceleration = 0x10000;
    movementAnimation = 5;
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
      headingCount = 0;
      angleOffsets[headingCount++] = direction + 0x1000;
      angleOffsets[headingCount++] = direction - 0x1000;
      angleOffsets[headingCount++] = direction + 0x2000;
      angleOffsets[headingCount++] = direction - 0x2000;
      angleOffsets[headingCount++] = direction + 0x3000;
      angleOffsets[headingCount++] = direction - 0x3000;
      for (angleIndex = 0; angleIndex < headingCount; angleIndex++) {
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
      pushedObject = gObjectSlots;
      for (angleIndex = 63; angleIndex >= 0; angleIndex -= 1, pushedObject++) {
        s32 radius;

        radius = object->radius - 2;
        if (pushedObject->script == 0)
          continue;
        if ((pushedObject->unk59 & 1) == 0)
          continue;
        if (pushedObject == object)
          continue;
        if (Runtime_CheckRadiusOverlap(&pushedObject->x, radius, &contact.x,
                                       pushedObject->radius - 2) < 0)
          continue;
        if ((pushedObject->unk59 & 2) != 0 &&
            pushedObject->movementState == 0) {
          direction =
              ArcTan2(pushedObject->z - object->z, pushedObject->x - object->x);
          testPosition.x = pushedObject->x;
          testPosition.y = pushedObject->y;
          testPosition.z = pushedObject->z;
          heading = direction;
          Vector_AddPolarOffset(0x4000, direction, &testPosition);
          if (!ScriptObject_CheckOverlap(
                  (struct ScriptObjectEntry *)pushedObject,
                  (s32 *)&testPosition)) {
            testPosition.x = pushedObject->x;
            testPosition.y = pushedObject->y;
            testPosition.z = pushedObject->z;
            Vector_AddPolarOffset(0xa0000, direction, &testPosition);
            if (Map_CheckObjectStep(pushedObject, &testPosition) == 0) {
              testPosition.x = pushedObject->x;
              testPosition.y = pushedObject->y;
              testPosition.z = pushedObject->z;
              Vector_AddPolarOffset(0xa0000, direction + 0x1000, &testPosition);
              if (Map_CheckObjectStep(pushedObject, &testPosition) == 0 &&
                  Map_CheckObjectStep(pushedObject, &testPosition) == 0) {
                testPosition.x = pushedObject->x;
                testPosition.y = pushedObject->y;
                testPosition.z = pushedObject->z;
                Vector_AddPolarOffset(0xa0000, direction - 0x1000,
                                      &testPosition);
                if (Map_CheckObjectStep(pushedObject, &testPosition) == 0) {
                  Vector_AddPolarOffset(
                      0x4000, direction,
                      (struct FieldPosition *)&pushedObject->x);
                  pushedObject->targetX = TARGET_UNSET;
                  pushedObject->targetY = TARGET_UNSET;
                  pushedObject->targetZ = TARGET_UNSET;
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
    ObjectDispatch_ApplyArgumentToChildren(object, movementAnimation);
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
    s32 speed;

    Object_SetMoveTarget(object, position.x, position.y, position.z);
    speed = FixedSqrt(Iwram_MulQ16(object->velocityX, object->velocityX) +
                      Iwram_MulQ16(object->velocityZ, object->velocityZ));
    object->velocityX = 0;
    object->velocityZ = 0;
    Vector_AddPolarOffset(speed, (u16)heading,
                          (struct FieldPosition *)&object->velocityX);
    if (object->action != 0)
      object->action--;
  }

  // Leave a footprint (object kind 25) every 18 walking or 12 running
  // frames on maps that show them, alternating left and right.
  footprintAngle = (u16)angle;
  if (((struct MapScrollWork *)gMapWork[0])->footprints != 0 &&
      object->action == 0 && collision == 0) {
    struct ObjectRuntime *effect;

    effect = FieldObject_Create(25, object->x, object->y, object->z);
    if (effect != 0) {
      struct FieldSprite *animation;

      effect->terrainHeight = object->terrainHeight;
      animation = effect->animation;
      ObjectDispatch_Initialize((struct DispatchObject *)effect,
                                gWorldMap_FootprintActions);
      effect->unk23 = 2;
      effect->flags = collision;
      if (animation != 0) {
        AnimationObjects_SelectAnimation((struct AnimationObject *)animation,
                                         1);
        animation->flags = collision;
        animation->rotation = footprintAngle + 0x4000;
        animation->priority = 3;
      }
      if (object->unk66 == 2) {
        AnimationObjects_SelectAnimation((struct AnimationObject *)animation,
                                         2);
        object->unk66 = collision;
      }
      if (object->unk66 != 0)
        effect->angle = 0x8000;
      if (movementAnimation == 5)
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

#if TBS_PATCH_1
#define WORLD_PROBE_RADIUS 0x70000
#else
#define WORLD_PROBE_RADIUS 0x80000
#endif

// SC_WORLD: moves the player on the world map by the direction pad, as
// SC_PLAYER does on the field, and turns the world map camera with the pad.
// Shallow water (terrain 6) leaves a ripple and terrain 9 hides the shadow.
s32 Object_MoveOnWorldMap(struct ScriptInterpreter *interpreter) {
  struct ObjectRuntime *object = (struct ObjectRuntime *)interpreter;

  struct FieldPosition position;
#if TBS_PATCH_1
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
  s32 movementAnimation;
  s32 count;
  s32 difference;
  s32 terrainCollision;
  s32 speed;
  s32 targetCameraYaw;
  struct PerspectiveWork *perspective;
  s32 yawStep;
  struct FieldSprite *playerSprite;
  struct ObjectRuntime *ripple;
  struct FieldSprite *rippleSprite;

  movementAnimation = 2;
  collision = 0;
#if !TBS_PATCH_1
  count = 0;
#endif
  if (gKeysHeld & gGameState.dashKeys) {
    object->speedLimit = 0x10000;
    object->acceleration = 0x14000;
    movementAnimation = 5;
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
#if TBS_PATCH_1
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
    ObjectDispatch_ApplyArgumentToChildren(object, movementAnimation);

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
    speed = FixedSqrt(Iwram_MulQ16(object->velocityX, object->velocityX) +
                      Iwram_MulQ16(object->velocityZ, object->velocityZ));
    object->velocityX = collision;
    object->velocityZ = collision;
    Vector_AddPolarOffset(speed, (u16)angle,
                          (struct FieldPosition *)&object->velocityX);
    if (object->action != 0)
      object->action--;
  }

  perspective = gMapWork[0];
  targetCameraYaw = gWorldMap_KeyYaws[(gKeysHeld >> 4) & 15];
  yawStep = (s16)(targetCameraYaw - perspective->yaw) / 8;
  if (yawStep > 0x200)
    yawStep = 0x200;
  if (yawStep < -0x200)
    yawStep = -0x200;
  if (yawStep > -16 && yawStep < 16)
    yawStep = targetCameraYaw - perspective->yaw;
  perspective->yaw += yawStep;

  if (object->animationKind == 1) {
    playerSprite = object->animation;
    terrainCollision = GetWorldMapCollision((struct WorldPosition *)&object->x);
    if (terrainCollision == 9) {
      ((struct WorldMapSprite *)playerSprite)->shadow[6] = 1;
      playerSprite->flags = 0;
    } else {
      ((struct WorldMapSprite *)playerSprite)->shadow[6] = 9;
      playerSprite->flags = 1;
    }
    if (terrainCollision == 6 && object->action == 0 && collision == 0) {
      ripple = FieldObject_Create(24, object->x, object->y, object->z);
      if (ripple != 0) {
        rippleSprite = ripple->animation;
        ObjectDispatch_Initialize((struct DispatchObject *)ripple,
                                  gWorldMap_FootprintActions + 3);
        ripple->flags = collision;
        ripple->terrainId = 1;
        if (rippleSprite != 0) {
          AnimationObjects_SelectAnimation(
              (struct AnimationObject *)rippleSprite, 1);
          rippleSprite->flags = collision;
          rippleSprite->blendMode = 1;
          rippleSprite->priority = 2;
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
s32 Field_CheckConfiguredKeysAndCount(struct ScriptInterpreter *interpreter) {
  struct ObjectRuntime *object = (struct ObjectRuntime *)interpreter;

  Field_CheckConfiguredKeys();
  object->step++;
  return 1;
}

// SC_KEYS: moves an object 8 pixels at a time by the direction pad, with up
// and down changing its height (y) rather than its depth. Refuses steps
// into an occupied cell, across a change of collision code, or onto ground
// too high (up) or too low (down). Returns 1.
s32 Object_MoveByKeys(struct ScriptInterpreter *interpreter) {
  struct ObjectRuntime *object = (struct ObjectRuntime *)interpreter;

  struct FieldPosition position;
  /* FAKEMATCH: never used; its 68 bytes are the native frame space no
   * instruction touches. Without it the frame is 12 bytes, not 80 (differ 8:
   * the sp adjustments and pos's address). 68 bytes is exactly the probe locals
   * of the sibling Object_MoveOnWorldMap (testPosition and angleOffsets[28]
   * internationally, leftPosition, rightPosition and angleOffsets[22] in
   * Japan); declared here instead, they also match tbs-en and tbs-ja
   * (2026-10-13). */
  u8 unusedProbeFrame[68];
  struct MapCell *sourceCell;
  struct MapCell *destinationCell;
  u16 angle;
  s32 animation;
  s32 collisionFlags;
  s32 deltaZ;
  s32 terrainHeight;

  object->speedLimit = 0x8000;
  object->acceleration = 0x4000;
  animation = 12;
  angle = gWorldMap_KeyHeadings[(gKeysHeld >> 4) & 15];
  if (angle == 0xffff) {
    collisionFlags = 4;
  } else {
    animation = 14;
    if ((angle & 0xf000) != 0) {
      animation = 15;
      if ((angle & 0xf000) != 0x8000)
        animation = 10;
    }
    collisionFlags = 0;
    position.x = 0;
    position.y = 0;
    position.z = 0;
    Vector_AddPolarOffset(0x80000, angle, &position);
    position.x += object->x;
    deltaZ = position.z;
    if (deltaZ < 0)
      object->angle = 0xc000;
    if (deltaZ > 0)
      object->angle = 0x4000;
    position.y = object->y - position.z;
    position.z = object->z;
    sourceCell =
        &((struct MapCell *)Ram_MapCellBuffer)[object->x / 0x100000 +
                                               (position.z / 0x100000) * 128];
    destinationCell =
        &((struct MapCell *)Ram_MapCellBuffer)[position.x / 0x100000 +
                                               (position.z / 0x100000) * 128];
    if (CheckMapPositionCellOccupied((struct WorldPosition *)&position) != 0 ||
        sourceCell->collisionCode != destinationCell->collisionCode) {
      collisionFlags = 4;
      animation = 12;
    } else if (gKeysHeld & KEY_UP) {
      terrainHeight = Map_GetTerrainHeight(object->terrainId, position.x,
                                           position.z - 0x100000);
      if (terrainHeight - object->y < 0x100000)
        goto refused;
    } else if (gKeysHeld & KEY_DOWN) {
      terrainHeight =
          Map_GetTerrainHeight(object->terrainId, position.x, position.z);
      if (terrainHeight - object->y > -0x80000) {
      refused:
        collisionFlags = 1;
        animation = 12;
      }
    }
  }
  if (gEventWork != NULL) {
    if (collisionFlags & 3)
      gEventWork->blockedSteps++;
    else
      gEventWork->blockedSteps = 0;
  }
  // Commit the chosen animation before either stopping or setting the target.
  ObjectDispatch_ApplyArgumentToChildren(object, animation);
  if (collisionFlags != 0) {
    object->targetX = TARGET_UNSET;
    object->targetY = TARGET_UNSET;
    object->targetZ = TARGET_UNSET;
    object->velocityX = 0;
    object->velocityY = 0;
    object->velocityZ = 0;
  } else {
    Object_SetMoveTarget(object, position.x, position.y, position.z);
  }
  object->step++;
  return 1;
}

// Remaps byteCount bytes in place through gRuntime_ByteRemapTable.
void Runtime_RemapBytesByTable(u8 *bytes, s32 byteCount) {
  s32 remaining;
  u8 *cursor;
  u8 *remapTable;

  cursor = bytes;
  remapTable = gRuntime_ByteRemapTable;
  remaining = byteCount - 1;
  if (remaining != -1) {
    do {
      remaining -= 1;
      *cursor = remapTable[*cursor];
      cursor += 1;
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
  u16 *destinationWords;
  u16 *sourceWords;
  s32 wordCount;
  s32 i;

  sourceWords = (u16 *)(Ram_MapCellBuffer + 2);
  destinationWords = (u16 *)gMapBlocks;
  wordCount = (size - 1) / 2;
  if (size & 1) {
    switch (((u8 *)gMapCellBuffer)[1]) {
    case 0:
      for (i = 0; i < wordCount; i++)
        *destinationWords++ = *sourceWords++;
      break;
    case 1: {
      u32 previousWord = 0;
      u8 *highBytes = Ram_MapCellBuffer + 2;
      u8 *lowBytes = highBytes + wordCount;

      for (i = 0; i < wordCount; i++) {
        u32 decodedWord = ((*highBytes++ << 8) | *lowBytes++) ^ previousWord;
        *destinationWords++ = decodedWord;
        previousWord = decodedWord;
      }
      break;
    }
    case 2: {
      s32 previousWord = 0;

      for (i = 0; i < wordCount; i++) {
        s32 decodedWord = *sourceWords++ ^ previousWord;
        *destinationWords++ = decodedWord;
        previousWord = decodedWord;
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
  s32 nextPlaceholderBlock = -1;

  do {
    u32 value = *cell++;
    u32 blockId = value & mask;

    if (blockId == mask) {
      if (nextPlaceholderBlock != blockId) {
        nextPlaceholderBlock++;
      }
      value = value + nextPlaceholderBlock - blockId;
      cell[-1] = value;
    }
    remaining--;
  } while (remaining != 0);
}

// Converts the map's cells in place with Tile_ConvertMap, run from RAM, from
// a saved copy of the staged buffer.
void Tilemap_ConvertBuffer(void) {
  u8 *savedCells;
  ConvertFn convertMap;
  u32 savedBytes;

  savedBytes = 0x8000;
  savedCells = Runtime_BumpAllocateAlternatePool(savedBytes);
  Iwram_CopyWords(savedCells, Ram_MapCellBuffer, savedBytes);
  RamCode_Load(convertMap, Tile_ConvertMap, Tile_ConvertMapCodeSize);
  convertMap(((u8 *)gMapCellBuffer + 0x8000), Ram_MapCellBuffer, savedCells);
  MemFree(convertMap);
  MemFree(savedCells);
}

// Copies one tile column of a block (metatile) into a BG map: its top and
// bottom tile entries, choosing the left or right half with tileHalf.
static __inline__ void CopyMetatileHalfColumn(u32 *cell, u8 *screenMap,
                                              u32 screenRowOffset,
                                              u32 screenColumnOffset,
                                              u32 tileHalf) {
  u16 *metatileEntry;
  u8 *screenTile;
  u32 index = ((*cell << 20) >> 18) + tileHalf;

  metatileEntry = (u16 *)Ram_MapBlocks;
  metatileEntry += index;
  screenTile =
      screenMap + (screenRowOffset + screenColumnOffset + tileHalf) * 2;
  *(u16 *)screenTile = *metatileEntry;
  metatileEntry = (u16 *)(Ram_MapBlocks + 4);
  metatileEntry += index;
  *(u16 *)(screenTile + 64) = *metatileEntry;
}

// Copies a whole block (metatile) into a BG map: its top two tile entries
// and its bottom two, a map row (64 bytes) below.
static __inline__ void CopyMetatileToScreen(u32 *cell, u8 *screenMap,
                                            u32 screenRowOffset,
                                            u32 screenColumnOffset) {
  u32 *metatileRows;
  u8 *screenTile;
  u32 index = (*cell << 20) >> 19;

  metatileRows = (u32 *)Ram_MapBlocks;
  metatileRows += index;
  screenTile = screenMap + screenRowOffset * 2 + screenColumnOffset * 2;
  *(u32 *)screenTile = *metatileRows;
  metatileRows = (u32 *)(Ram_MapBlocks + 4);
  metatileRows += index;
  *(u32 *)(screenTile + 64) = *metatileRows;
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
  struct SceneEntry *sceneResources;
  struct MapScrollWork *work;
  struct SceneHeader *sceneHeader;
  struct MapLayerScroll *mapLayer;
  struct SceneLayerSource *layerSource;
  s32 *viewX;
  s32 *viewY;
  s32 layerIndex;
  u8 *graphicsBuffer;
  s32 backdrop;
  u8 *encodedData;
  s32 displayFlags;

  REG_DISPCNT &= ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON |
                   DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
  Fade_FromBlack(0);
  sceneResources = &gMap_LayeredScenes[index];
  work = Runtime_AllocateBlock(8, sizeof(struct MapScrollWork));
  Iwram_ClearWords(work, sizeof(struct MapScrollWork));
  sceneHeader = (struct SceneHeader *)Resource_GetTableEntry(
      sceneResources->resources[0] + (u32)&ResourceId_Map001);
  encodedData = (u8 *)sceneHeader + sceneHeader->tiles;
  Tilemap_DecodeStagedBuffer(
      Resource_DecodeType01(encodedData, Ram_MapCellBuffer + 1));
  encodedData = (u8 *)sceneHeader + sceneHeader->collision;
  Resource_DecodeType01(encodedData, Ram_MapCollision);
  encodedData = (u8 *)sceneHeader + sceneHeader->tilemap;
  Resource_DecodeType01(encodedData, Ram_MapCellBuffer);
  Tilemap_ConvertBuffer();
  // Optional scripts share the decoded scene's collision-work allocation.
  if (sceneHeader->animation != 0) {
    encodedData = (u8 *)sceneHeader + sceneHeader->animation;
    Resource_DecodeType01(encodedData, Ram_MapCollision + 0x1000);
    MapAnimation_StartChannels((const u16 *)(Ram_MapCollision + 0x1000));
  }
  if (sceneHeader->blend != 0) {
    encodedData = (u8 *)sceneHeader + sceneHeader->blend;
    Resource_DecodeType01(encodedData, Ram_MapCollision + 0x1e00);
    DisplayBlend_StartScript((u16 *)(Ram_MapCollision + 0x1e00));
  }
  // Publish camera bounds before deriving the three layers' parallax origins.
  work->script = (u8 *)sceneHeader + sceneHeader->script;
  work->minX = sceneHeader->origin[0] << 19;
  work->minY = sceneHeader->origin[1] << 19;
  work->maxX = sceneHeader->origin[2] << 19;
  work->maxY = sceneHeader->origin[3] << 19;
  viewX = &work->viewX;
  *viewX = 0;
  viewY = &work->viewY;
  *viewY = 0;
  work->enabled[0] = sceneHeader->priority[0];
  work->enabled[1] = sceneHeader->priority[1];
  work->enabled[2] = sceneHeader->priority[2];
  mapLayer = work->layers;
  layerSource = sceneHeader->layers;
  for (layerIndex = 0; layerIndex < 3; layerIndex++) {
    u32 x = layerSource->x;
    u32 y = layerSource->y;
    s32 scrollX;
    s32 scrollY;
    s32 baseX;
    s32 baseY;

    mapLayer->offsetX = baseX = x << 19;
    mapLayer->offsetY = baseY = y << 19;
    // Signed byte parallax and velocity use twelve fractional bits.
    // Multiplication also handles negative coefficients without a signed shift.
    scrollX = layerSource->scrollX * 4096;
    scrollY = layerSource->scrollY * 4096;
    mapLayer->scaleX = scrollX;
    mapLayer->scaleY = scrollY;
    mapLayer->speedX = layerSource->speedX * 4096;
    mapLayer->speedY = layerSource->speedY * 4096;
    mapLayer->maskX = layerSource->periodX;
    mapLayer->maskY = layerSource->periodY;
    mapLayer->phaseX = 0;
    mapLayer->phaseY = 0;
    mapLayer->cells =
        (struct MapCell *)Ram_MapCellBuffer + (y >> 1) * 128 + (x >> 1);
    mapLayer->x = Iwram_MulQ16(*viewX, scrollX) + baseX;
    mapLayer->y = Iwram_MulQ16(*viewY, scrollY) + baseY;
    layerSource++;
    mapLayer++;
  }
  // Match each enabled layer to BG3, BG2, then BG1.
  work->flags = DISPCNT_OBJ_ON;
  if (work->enabled[0] != 0)
    work->flags = DISPCNT_OBJ_ON | DISPCNT_BG3_ON;
  if (work->enabled[1] != 0)
    work->flags |= DISPCNT_BG2_ON;
  if (work->enabled[2] != 0)
    work->flags |= DISPCNT_BG1_ON;
  displayFlags = work->enabled[0] | BGCNT_CHAR_BASE(sceneHeader->charBase[0]) |
                 BGCNT_SCREEN_BASE(5);
  REG_BG3CNT = displayFlags;
  Io_Write16(work->enabled[1] | BGCNT_CHAR_BASE(sceneHeader->charBase[1]) |
                 BGCNT_SCREEN_BASE(6),
             &REG_BG2CNT);
  Io_Write16(work->enabled[2] | BGCNT_CHAR_BASE(sceneHeader->charBase[2]) |
                 BGCNT_SCREEN_BASE(7),
             &REG_BG1CNT);
  if (GameFlag_TestFar(FLAG_KEEP_LAYER_GRAPHICS) != 0) {
    GameFlag_ClearBitFar(FLAG_KEEP_LAYER_GRAPHICS);
  } else {
    graphicsBuffer = MemAlloc(0x4000);
    if (graphicsBuffer != NULL) {
      backdrop = *(s16 *)BG_PLTT;
      Resource_DecodeType01(
          Resource_GetTableEntry(sceneResources->resources[1] +
                                 (u32)&ResourceId_Map001),
          graphicsBuffer);
      *(s16 *)graphicsBuffer = backdrop;
      /* FAKEMATCH: needed to match: these four copies call the fixed entry
       * itself; through Iwram_CopyWords the scene header and the work move from
       * r5 and r8 to r8 and sl (237 of 357 instructions differ), with or
       * without the function's other devices. */
      Iwram_CopyWordsEntry((void *)BG_PLTT, graphicsBuffer, 0x1c0);
      Resource_DecodeType2(Resource_GetTableEntry(sceneResources->resources[2] +
                                                  (u32)&ResourceId_Map001),
                           graphicsBuffer);
      Iwram_CopyWordsEntry(BG_CHAR_BLOCK(1), graphicsBuffer, 0x4000);
      Resource_DecodeType2(Resource_GetTableEntry(sceneResources->resources[3] +
                                                  (u32)&ResourceId_Map001),
                           graphicsBuffer);
      Iwram_CopyWordsEntry(BG_CHAR_BLOCK(2), graphicsBuffer, 0x4000);
      Resource_DecodeType2(Resource_GetTableEntry(sceneResources->resources[4] +
                                                  (u32)&ResourceId_Map001),
                           graphicsBuffer);
      Iwram_CopyWordsEntry(BG_CHAR_BLOCK(3), graphicsBuffer, 0x4000);
      Resource_DecodeType2(Resource_GetTableEntry(sceneResources->resources[5] +
                                                  (u32)&ResourceId_Map001),
                           gMapLayerData);
      MemFree(graphicsBuffer);
    }
  }
  displayFlags = 0;
  REG_MOSAIC = displayFlags;
  REG_BLDCNT = displayFlags;
  displayFlags = DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP;
  REG_DISPCNT = displayFlags;
  // UB: the scheduler returns s32; call it through its declared type.
#ifdef UBFIX
  Scheduler_AddOrUpdateCallback(Map_UpdateLayerScroll, 0xc85);
#else
  /* FAKEMATCH: Scheduler_AddOrUpdateCallback is called as void (*)(void
   * (*)(void), s32); as s32 (*)(SchedulerCallback, s32), its prototype,
   * Map_LoadLayeredScene has 6 of 868 bytes different */
  ((void (*)(void (*)(void), s32))Scheduler_AddOrUpdateCallback)(
      Map_UpdateLayerScroll, 0xc85);
#endif
  return 2;
}

// Centres the camera on the point the map follows (origin: x, height, z),
// or on (0, 0) when there is none, and scrolls the layers there.
void Map_ApplyWorkOriginAndSpan(void) {
  struct MapScrollWork *work = gMapWork[0];
  s32 x;
  s32 depth;
  s32 height;
  s32 *followedPosition;

  followedPosition = work->origin;
  x = 0;
  height = 0;
  depth = 0;
  if (followedPosition != NULL) {
    x = *followedPosition++;
    height = *followedPosition++;
    depth = *followedPosition;
  }
  Map_SetCameraCenter(x, depth - height);
  Map_UpdateLayerScroll();
}

// Draws a row of 16 blocks (32 tiles) of a layer's cells into its BG map
// (screen block 5 + layer), starting at tile (tileX, tileY); the BG map
// wraps every 32 tiles.
void Map_RenderMetatileRow(u32 layer, s32 tileX, s32 tileY) {
  u8 *screenMap = BG_SCREEN_BLOCK(5) + (layer << 11);
  u32 sourceRow = ((tileY / 2) & 0x7F) << 7;
  u32 screenRowOffset = (tileY & 30) << 5;
  u32 sourceColumn = (tileX / 2) & 0x7F;
  u32 screenColumnOffset = tileX & 30;
  u32 metatileIndex;

  for (metatileIndex = 0; metatileIndex <= 15; metatileIndex++) {
    u32 *sourceCell = (u32 *)Ram_MapCellBuffer;
    u32 *metatileRows;
    u8 *screenTile;
    u32 metatileWordOffset;

    sourceCell += sourceRow + sourceColumn;
    metatileWordOffset = (*sourceCell << 20) >> 19;
    metatileRows = (u32 *)Ram_MapBlocks;
    metatileRows += metatileWordOffset;
    screenTile = screenMap + (screenRowOffset + screenColumnOffset) * 2;
    *(u32 *)screenTile = *metatileRows;
    metatileRows = (u32 *)(Ram_MapBlocks + 4);
    metatileRows += metatileWordOffset;
    *(u32 *)(screenTile + 64) = *metatileRows;
    sourceColumn = (sourceColumn + 1) & 0x7F;
    screenColumnOffset = (screenColumnOffset + 2) & 30;
  }
}

// Draws a column of 11 blocks of a layer's cells, one tile wide, into its BG
// map, starting at tile (tileX, tileY).
void Map_RenderMetatileColumn(u32 layer, s32 tileX, s32 tileY) {
  u8 *screenMap = BG_SCREEN_BLOCK(5) + (layer << 11);
  u32 sourceRow = ((tileY / 2) & 0x7F) << 7;
  u32 screenRowOffset = (tileY & 30) << 5;
  u32 sourceColumn = (tileX / 2) & 0x7F;
  u32 screenColumnOffset = tileX & 30;
  u32 tileHalf = tileX & 1;
  u32 metatileIndex;

  for (metatileIndex = 0; metatileIndex <= 10; metatileIndex++) {
    u32 *sourceCell = (u32 *)Ram_MapCellBuffer;

    sourceCell += sourceRow + sourceColumn;
    CopyMetatileHalfColumn(sourceCell, screenMap, screenRowOffset,
                           screenColumnOffset, tileHalf);

    sourceRow = (sourceRow + 128) & 0x3F80;
    screenRowOffset = (screenRowOffset + 64) & 0x3C0;
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
  u32 layerIndex;

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
    s32 shakeRollA = Random16();
    s32 shakeRollB = Random16();
    s32 shakeAmplitude = work->shakeX;
    baseX += Iwram_MulQ16(shakeAmplitude, shakeRollA - shakeRollB);
    work->shakeX = Iwram_MulQ16(shakeAmplitude, work->shakeDecay);
  }
  if (work->shakeY != 0) {
    s32 shakeRollA = Random16();
    s32 shakeRollB = Random16();
    s32 shakeAmplitude = work->shakeY;
    baseY += Iwram_MulQ16(shakeAmplitude, shakeRollA - shakeRollB);
    work->shakeY = Iwram_MulQ16(shakeAmplitude, work->shakeDecay);
  }
  work->viewX = baseX;
  work->viewY = baseY;
  for (layerIndex = 0; layerIndex < 3; layerIndex++, layer++) {
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
        Map_RenderMetatileColumn(layerIndex, x + 30, y);
      else
        Map_RenderMetatileColumn(layerIndex, x, y);
    }
    if ((layer->y ^ baseY) & 0x100000) {
      if (layer->y < baseY)
        Map_RenderMetatileRow(layerIndex, x, y + 20);
      else
        Map_RenderMetatileRow(layerIndex, x, y);
    }
    gBgScroll[3 - layerIndex].x = baseX >> 16;
    gBgScroll[3 - layerIndex].y = baseY >> 16;
    layer->x = baseX;
    layer->y = baseY;
  }
}

// Centres the view on (x, y) in 16.16 pixels, clamped to the map's bounds,
// and redraws every enabled layer's BG map around it.
void Map_SetCameraCenter(s32 x, s32 y) {
  u32 layerIndex;
  u8 *screenMap;
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

  for (layerIndex = 0; layerIndex < 3; layerIndex++) {
    if (work->enabled[layerIndex]) {
      u32 rows = 22;
      u32 sourceRow, sourceColumn, screenRowOffset, screenColumnOffset,
          metatileRow, metatileColumn;

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
      screenMap = BG_SCREEN_BLOCK(5) + (layerIndex << 11);
      sourceRow = ((y / 2) & 127) << 7;
      screenRowOffset = (y & 30) << 5;
      for (metatileRow = 0; metatileRow < rows / 2; metatileRow++) {
        sourceColumn = (x / 2) & 127;
        screenColumnOffset = x & 30;
        for (metatileColumn = 0; metatileColumn <= 15; metatileColumn++) {
          u32 *map = (u32 *)Ram_MapCellBuffer;
          map += sourceRow + sourceColumn;
          CopyMetatileToScreen(map, screenMap, screenRowOffset,
                               screenColumnOffset);
          sourceColumn = (sourceColumn + 1) & 127;
          screenColumnOffset = (screenColumnOffset + 2) & 30;
        }
        sourceRow = (sourceRow + 128) & 0x3f80;
        screenRowOffset = (screenRowOffset + 64) & 0x3c0;
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
  u32 *sourceCells = (u32 *)Ram_MapCellBuffer + (srcY * 128 + srcX);
  u32 *destinationCells = (u32 *)Ram_MapCellBuffer + (dstY * 128 + dstX);
  struct MapLayerScroll *layer = ((struct MapScrollWork *)gMapWork[0])->layers;
  struct TilePos visibleLayers[3];
  s32 i;
  struct TilePos *visibleLayer = visibleLayers;
  u32 y;
  u32 x;

  for (i = 2; i >= 0; i--) {
    visibleLayer->x = layer->x >> 20;
    visibleLayer->y = layer->y >> 20;
    visibleLayer++;
    layer++;
  }
  for (y = dstY; y < dstY + height; y++) {
    for (x = dstX; x < dstX + width; x++) {
      u32 sourceCell = *sourceCells++;
      u32 metatileId = sourceCell & 0xfff;
      u32 offset;
      u32 tmp3;
      u32 n;
      u32 tmp6;
      u32 tmp9;
      u32 tmp12;
      u32 tmp11;
      u32 tmp13;
      u32 tmp15;
      tmp9 = *destinationCells;
      tmp3 = tmp9 & -0x1000;
      tmp12 = x & 15;
      destinationCells[0] = metatileId | tmp3;
      tmp6 = tmp12;
      destinationCells++;
      i = 0;
      tmp15 = y & 15;
      tmp3 = tmp15;
      visibleLayer = visibleLayers;
      tmp13 = tmp3 << 5;
      tmp12 = tmp13;
      n = tmp12;
      n += tmp6;
      tmp13 = 4 * n;
      tmp11 = tmp13;
      tmp6 = tmp11;
      offset = tmp6;
      for (; i < 3; i++) {
        if (visibleLayer->x <= (s32)x && 16 + visibleLayer->x > (s32)x &&
            visibleLayer->y <= (s32)y && visibleLayer->y + 12 > (s32)y) {
          u32 screenAddress, metatileAddress;
          u8 *screenTile, *metatileRows;

          screenAddress = 0x06002800;
          screenTile = (u8 *)screenAddress;
          screenTile += offset;
          tmp11 = metatileId * 8;
          metatileAddress = (u32)Ram_MapBlocks;
          metatileRows = (u8 *)metatileAddress;
          metatileRows += tmp11;
          *(u32 *)screenTile = *(u32 *)metatileRows;

          metatileAddress = (u32)Ram_MapBlocks + 4;
          metatileRows = (u8 *)metatileAddress;
          metatileRows += tmp11;
          screenAddress = 0x06002840;
          screenTile = (u8 *)screenAddress;
          screenTile += offset;
          *(u32 *)screenTile = *(u32 *)metatileRows;
          break;
        }
        offset += 0x800;
        visibleLayer++;
      }
    }
    sourceCells += 128 - width;
    destinationCells += 128 - width;
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
void Map_PlayMetatileCopySequence(u16 *sequence, s32 destinationX,
                                  u32 destinationY) {
  struct MapCopyCommand *copyCommand = (struct MapCopyCommand *)sequence;
  u32 sourceX = copyCommand->source;
  u32 endMarker = 0xffff;
  u32 firstSourceCopy = sourceX;

  /* FAKEMATCH: the conditional cancelling updates end CSE before the
   * comparison, preserving its ip copy while source stays in r0. Both
   * updates disappear; removing them loses the copy (53 -> 52 English
   * instructions, five rows differ). */
  if (copyCommand) {
    sourceX ^= firstSourceCopy;
    sourceX ^= firstSourceCopy;
  }
  if (firstSourceCopy != endMarker) {
    u32 endMarkerCopy = endMarker;
    s16 *rectangleArgs = (s16 *)(sequence + 1);

    while (1) {
      s32 sourceY = rectangleArgs[0];
      s32 width = rectangleArgs[1];
      s32 height = rectangleArgs[2];
      s32 delay = rectangleArgs[3];
      rectangleArgs += 5;

      Map_CopyMetatileIndicesRect(sourceX, (u16)sourceY, destinationX,
                                  destinationY, (u16)width, (u16)height);
      copyCommand++;
      WaitFrames((u16)delay);
      sourceX = copyCommand->source;
      {
        u32 nextSourceCopy = sourceX;

        /* FAKEMATCH: the same ip copy after the wait. 2026-10-11: the native
         * compares a copy of source held in ip at both tests; the copy's pseudo
         * must reach regclass with only the copy and the branch as its insns,
         * which CSE's copy swap or combine's substitution removes in every
         * plain form tried (best, a do-while on the field re-read with stop in
         * a local: differ 12). */
        __asm__("" : "+r"(nextSourceCopy));
        if (nextSourceCopy == endMarkerCopy)
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
  u32 *sourceCells = (u32 *)Ram_MapCellBuffer + (srcY * 128 + srcX);
  u32 *destinationCells = &((u32 *)Ram_MapCellBuffer)[y * 128 + dstX];
  struct MapLayerScroll *layer = ((struct MapScrollWork *)*gMapWork)->layers;
  struct TilePos visibleLayers[3];
  s32 i;
  struct TilePos *visibleLayer = visibleLayers;
  s32 x;

  // The three layers' visible areas: their scroll positions in cells.
  i = 2;
  while (i >= 0) {
    s32 tmp5;

    tmp5 = layer->x >> 20;
    visibleLayer->x = tmp5;
    tmp5 = (*layer).y;
    visibleLayer->y = tmp5 >> 20;
    visibleLayer++;
    layer++;
    i--;
  }
  if (y < dstY + height) {
    do {
      for (x = dstX; x < dstX + width; x++) {
        u32 sourceCell = *sourceCells++;
        u32 metatileId;
        u32 offset;
        u32 tmp3;
        u32 *screenTile;
        u32 n;
        u32 *metatileRows;
        u32 tmp6;
        u32 tmp12;
        u32 tmp13;

        destinationCells[0] = sourceCell;
        metatileId = sourceCell & 0xfff;
        tmp12 = x & 15;
        tmp6 = tmp12;
        destinationCells += 1;
        tmp3 = y & 15;
        tmp13 = tmp3 << 5;
        visibleLayer = visibleLayers;
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
          if (visibleLayer->x <= x && 16 + (*visibleLayer).x > x &&
              visibleLayer->y <= y && visibleLayer->y + 12 > y) {
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
            u32 bytes = metatileId * 8;
            u32 tmp8;
            u32 tmp17;

            metatileId = bytes / 8;
            tmp = (u32 *)Ram_MapBlocks;
            tmp8 = 0x06002800 + offset;
            metatileRows = tmp;
            tmp2 = tmp8;
            tmp = (u32 *)tmp2;
            screenTile = tmp;
            metatileRows = (u32 *)((u8 *)metatileRows + bytes);
            tmp16 = *metatileRows;
            tmp14 = tmp16;
            tmp17 = 0x06002840 + offset;
            tmp10 = tmp14;
            *screenTile = tmp10;
            tmp7 = (u32 *)(Ram_MapBlocks + 4);
            metatileRows = tmp7;
            metatileRows = (u32 *)((u8 *)metatileRows + bytes);
            tmp8 = tmp17;
            tmp14 = tmp8;
            tmp17 = metatileRows[0];
            tmp16 = tmp17;
            tmp10 = tmp16;
            tmp4 = tmp10;
            tmp2 = tmp4;
            tmp4 = tmp14;
            tmp7 = (u32 *)tmp4;
            screenTile = tmp7;
            *screenTile = tmp2;
            break;
          }
          n = 0x800; // the next layer's BG map
          offset += n;
          visibleLayer++;
        }
      }
      sourceCells += (128 - width);
      destinationCells = 128 - width + destinationCells;
      y++;
    } while (y < dstY + height);
  }
}

// Copies only the attributes of a rectangle of cells, from (srcX, srcY) to
// (dstX, dstY), width by height cells, keeping each destination cell's
// block number.
void Map_CopyCellAttributeRect(s32 srcX, s32 srcY, s32 width, s32 height,
                               s32 dstX, s32 dstY) {
  u32 *destinationCells = MAP_CELLS;
  u32 *sourceCells = destinationCells;
  u16 rowIndex;
  u16 columnIndex;

  destinationCells += (dstY << 7) + dstX;
  sourceCells += (srcY << 7) + srcX;

  for (rowIndex = 0; rowIndex < height; rowIndex++) {
    u32 *destinationCell = destinationCells + (rowIndex << 7);
    u32 *sourceCell = sourceCells + (rowIndex << 7);

    for (columnIndex = 0; columnIndex < width; columnIndex++) {
      *destinationCell = (*destinationCell & MAP_CELL_TILE_MASK) |
                         (*sourceCell++ & MAP_CELL_ATTRIBUTE_MASK);
      destinationCell++;
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
  u32 *sourceCells = (u32 *)Ram_MapCellBuffer + (srcY * 128 + srcX);
  u32 *destinationCells = &((u32 *)Ram_MapCellBuffer)[dstY * 128 + dstX];
  struct MapLayerScroll *layer = ((struct MapScrollWork *)*gMapWork)->layers;
  struct TilePos visibleLayers[3];
  s32 i;
  struct TilePos *visibleLayer = visibleLayers;
  s32 x;

  // The three layers' visible areas: their scroll positions in cells.
  i = 2;
  while (i >= 0) {
    s32 tmp5;

    tmp5 = layer->x >> 20;
    visibleLayer->x = tmp5;
    tmp5 = (*layer).y;
    visibleLayer->y = tmp5 >> 20;
    visibleLayer++;
    layer++;
    i--;
  }
  y = dstY;
  if (y < dstY + height) {
    do {
      for (x = dstX; x < dstX + width; x++) {
        u32 sourceCell = *sourceCells++;
        u32 metatileId;
        u32 offset;
        u32 tmp3;
        u32 n;
        u32 tmp6;
        u32 tmp12;
        u32 tmp13;

        metatileId = sourceCell & MAP_CELL_TILE_MASK;
        destinationCells[0] =
            (destinationCells[0] & MAP_CELL_ATTRIBUTE_MASK) | metatileId;
        tmp12 = x & 15;
        tmp6 = tmp12;
        destinationCells += 1;
        i = 0;
        tmp3 = y & 15;
        tmp13 = tmp3 << 5;
        visibleLayer = visibleLayers;
        tmp12 = tmp13;
        n = tmp12;
        n += tmp6;
        tmp13 = n * 4;
        tmp6 = tmp13;
        offset = tmp6;
        // In a layer's visible area, write its BG map in VRAM too (0x800
        // bytes per layer).
        for (; i < 3; i += 1) {
          if (visibleLayer->x <= x && 16 + (*visibleLayer).x > x &&
              visibleLayer->y <= y && visibleLayer->y + 12 > y) {
            // The block's (8 bytes) top two tiles, and its bottom two 0x40
            // bytes further on.
            u32 screenAddress, metatileAddress;
            u8 *screenTile, *metatileRows;

            screenAddress = 0x06002800;
            screenTile = (u8 *)screenAddress;
            screenTile += offset;
            tmp13 = metatileId * 8;
            metatileAddress = (u32)Ram_MapBlocks;
            metatileRows = (u8 *)metatileAddress;
            metatileRows += tmp13;
            *(u32 *)screenTile = *(u32 *)metatileRows;

            metatileAddress = (u32)Ram_MapBlocks + 4;
            metatileRows = (u8 *)metatileAddress;
            metatileRows += tmp13;
            screenAddress = 0x06002840;
            screenTile = (u8 *)screenAddress;
            screenTile += offset;
            *(u32 *)screenTile = *(u32 *)metatileRows;
            break;
          }
          n = 0x800; // the next layer's BG map
          offset += n;
          visibleLayer++;
        }
      }
      sourceCells = sourceCells + (128 - width);
      destinationCells = 128 - width + destinationCells;
      y++;
    } while (y < dstY + height);
  }
}

// Sets bits 9-11 of the map's flags from v, keeping the others.
void Map_SetWorkFlagBits9To11(u32 layerFlags) {
  struct MapState *mapState = gMapWork[0];
  u32 mask = layerFlags & 0xe00;
  u32 flags = mapState->flags;

  flags = (flags & 0xf1ff) | mask;
  mapState->flags = flags;
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
  struct PerspectiveWork *perspective = gMapWork[0];
  u32 *tileOffsets = perspective->tiles;
  u16 *shownQuarter;
  u16 *decodedTile;
  u16 *sourceTiles;
  u8 *quarterCells;
  u16 *screenTiles;
  u32 rowIndex;
  u32 columnIndex;
  u32 metatileId;

  x &= 1;
  y &= 1;
  shownQuarter = &perspective->quarterTiles[(layer * 2 + y) * 2 + x];
  if (update == 0 && tile == *shownQuarter)
    return 0;
  *shownQuarter = tile;
  decodedTile = Runtime_AllocateHeapBlock(14, 0x400);
  Resource_DecodeByteLz((u8 *)tileOffsets + tileOffsets[tile], decodedTile);

  sourceTiles = decodedTile;
  quarterCells = Ram_MapBlocks + (((layer * 2 + y) * 32 + x) << 6);
  for (rowIndex = 0; rowIndex < 16; rowIndex++) {
    Dma_Set(sourceTiles, quarterCells, DMA_ENABLE32 | DMA_32BIT | 0x10,
            REG_DMA3);
    sourceTiles += 32;
    quarterCells += 128;
  }

  if (update != 0) {
    screenTiles =
        (u16 *)(BG_SCREEN_BLOCK(8) + ((((layer * 2 + y) << 6) + x) << 5));
    sourceTiles = decodedTile;
    for (rowIndex = 0; rowIndex < 16; rowIndex++) {
      for (columnIndex = 0; columnIndex < 16; columnIndex++) {
        u16 *bottomTiles;

        metatileId = *sourceTiles;
        screenTiles[0] = *(u16 *)(Ram_MapCellBuffer + metatileId * 4);
        bottomTiles = (u16 *)(Ram_MapCellBuffer + 2);
        bottomTiles = (u16 *)((u8 *)bottomTiles + metatileId * 4);
        screenTiles[32] = *bottomTiles;
        screenTiles++;
        sourceTiles += 2;
      }
      screenTiles += 48;
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
  struct PerspectiveWork *perspective;
  struct BattleCamera *camera;
  struct MapFrameWork *frameWork;
  struct MapAffinePair(*affinePages)[160];
  s32 *cameraPosition;
  s32 *distance;
  u16 *yaw;
  u16 *pitch;
  u16 *zoomTransitionFrames;
  /* FAKEMATCH: the fill word is this function's own volatile local, filled by
   * Dma_Fill32Cell, so it takes the native's frame slot at sp+28; plain
   * Dma_Fill32's inner word lands at sp+16 and the slots after it shift (differ
   * 4 in all six editions, measured 2026-10-13). */
  volatile u32 fill;
  struct FixedPointPosition eyeOffset;
  s32 farPlane;
  s32 tileIndex;

  REG_DISPCNT &= ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON |
                   DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
  Fade_FromBlack(0);
  perspective = Runtime_AllocateHeapBlock(8, sizeof(struct PerspectiveWork));
  Dma_Fill32Cell(fill, 0, perspective, sizeof(struct PerspectiveWork));
  perspective->viewX = 0;
  perspective->viewY = 0;
  perspective->scaleX = 0x200000;
  perspective->scaleY = 0x400000;
  perspective->limitX = 0x1fe00000;
  perspective->limitY = 0x1fe00000;
  perspective->script = NULL;
  perspective->tiles =
      Resource_GetTableEntry((s32)&ResourceId_PerspectiveDataA);
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
  perspective->fade = 0x1f00;
  perspective->fadeStep = 0x80;
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

  camera = Runtime_AllocateBlock(HEAP_SLOT_CAMERA, sizeof(struct BattleCamera));
  frameWork = Runtime_AllocateHeapBlock(7, sizeof(struct MapFrameWork));
  cameraPosition = camera->pos;
  affinePages = frameWork->pages;
  farPlane = 0x1fe0000;
  perspective->farPlane = farPlane;
  distance = &perspective->distance;
  *distance = farPlane;
  perspective->zoom = 0x10000;
  zoomTransitionFrames = &perspective->zoomTransitionFrames;
  camera->unk18 = 0;
  camera->followPos = 0;
  *zoomTransitionFrames = 0;
  // Rebuild the projection around the followed position before the affine pass.
  gProjection.centerX = 120;
  gProjection.centerY = 96;
  Projection_Set(farPlane, farPlane >> 1, farPlane << 1);
  cameraPosition[0] = 0;
  cameraPosition[1] = 0;
  cameraPosition[2] = 0;
  Render_ResetTransformState();
  SceneTransform_ApplyPosition(cameraPosition);
  yaw = &perspective->yaw;
  SceneTransform_ApplyYaw(*yaw);
  pitch = &perspective->pitch;
  SceneTransform_ApplyPitch(*pitch);
  eyeOffset.x = 0;
  eyeOffset.y = 0;
  eyeOffset.z = farPlane;
  Iwram_TransformVector(&eyeOffset, (s32 *)camera);
  Render_ResetTransformState();
  Transform_SetLookAt(camera->eye, cameraPosition);
  RamCode_LoadSlot(46, Transform_UpdateVertices, &Transform_UpdateVerticesSize);
  WorldMap_BuildScanlineTable(
      Iwram_RatioMulQ14(Trig_Cos(*pitch), Trig_Sin(*pitch)), cameraPosition,
      frameWork->scanlines);
  gScanlineAngle = 0;
  gScanlinePitch = perspective->pitch;
  ((PlaneFn)gWorkSlot[46])(camera, cameraPosition, frameWork->scanlines,
                           affinePages[gFrameCount & 1]);
  cameraPosition[0] = 0;
  cameraPosition[1] = 0;
  cameraPosition[2] = 0;
  Render_ResetTransformState();
  Io_Put16(pitch, 0xe000);
  *yaw = 0;
  Render_ResetTransformState();
  SceneTransform_ApplyPosition(cameraPosition);
  SceneTransform_ApplyYaw(*yaw);
  SceneTransform_ApplyPitch(*pitch);
  eyeOffset.x = 0;
  eyeOffset.y = 0;
  eyeOffset.z = *distance + 0x10000;
  Iwram_TransformVector(&eyeOffset, camera->eye);
  REG_MOSAIC = 0;
  Io_Write16(0x42, &REG_DISPCNT);
  gBgScroll[1].x = 0;
  gBgScroll[1].y = 0;
  gBgScroll[2].x = 0;
  gBgScroll[2].y = 0;
  gBgScroll[3].x = 0;
  gBgScroll[3].y = 0;
  perspective->windowTop = 0;
  perspective->windowBottom = 159;
  Scheduler_AddOrUpdateCallback(WorldMap_UpdateView, 0xc85);
  Scheduler_AddOrUpdateCallback(MapAnimation_ApplyAffineFrame, 0x480);
  for (tileIndex = 255; tileIndex >= 0; tileIndex--)
    perspective->tileIds[tileIndex] = tileIndex;
}

// Puts world map tile (x / 16, y / 16) at cell (px / 16, py / 16) of the
// tile id grid, and draws it at once when it is next to the view.
void Map_SetWindowCellTile(s32 x, s32 y, s32 px, s32 py) {
  struct PerspectiveWork *perspective;
  s32 *followedPosition;
  s32 originX;
  s32 originY;
  s32 tileId;

  perspective = gMapWork[0];
  originX = 0;
  originY = 0;
  followedPosition = perspective->origin;
  if (followedPosition != 0) {
    originX = *followedPosition++;
    originY = followedPosition[1];
  }

  originX >>= 24;
  originY >>= 24;
  x >>= 4;
  y >>= 4;
  px >>= 3;
  py >>= 3;
  tileId = (y << 4) + x;
  perspective->tileIds[(((py / 2) & 15) << 4) + ((px / 2) & 15)] = tileId;

  if (ABS(originX - px) <= 1 && ABS(originY - py) <= 1) {
    Map_WriteLayerCellTile(0, px / 2, py / 2, tileId, 1);
    Map_WriteLayerCellTile(1, px / 2, py / 2, tileId + 0x140, 1);
  }
}

// Loads the world map's palette and tiles for the area at (x, z): the second
// set where the ground there is terrain kind 21. Fills the BG maps, and for
// the second set swaps in its own tiles at ids 10-12 and 26-28.
void WorldMap_LoadGraphics(s32 x, s32 z) {
  struct PerspectiveWork *perspective;
  u32 *resources;
  u8 *buffer;
  u8 *tiles;
  u32 *screenTiles;
  u32 tilePair;
  s32 graphicsSet;
  s32 screenRow;
  s32 screenColumn;
  s16 backdropColor;

  graphicsSet = 0;
  buffer = MemAlloc(0x200);
  perspective = gMapWork[0];
  if (WORLD_CELLS[((x / 0x200000) & 31) + (((z / 0x200000) & 31) << 5)].kind ==
      21)
    graphicsSet = 1;
  resources = gWorldMap_GraphicsResources[graphicsSet];
  perspective->resources = resources;
  backdropColor = BG_PALETTE[0];
  Resource_DecodeByteLz(Resource_GetTableEntry(resources[0]), buffer);
  *(s16 *)buffer = backdropColor;
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
  screenTiles = (u32 *)BG_SCREEN_BLOCK(6);
  tilePair = 0x01a901a8;
  for (screenRow = 0; screenRow < 20; screenRow++) {
    for (screenColumn = 0; screenColumn < 15; screenColumn++) {
      *screenTiles++ = tilePair;
      tilePair += 0x00020002;
    }
    screenTiles++;
  }
  if (graphicsSet == 1) {
    perspective->tileIds[10] = 0x10a;
    perspective->tileIds[11] = 0x10b;
    perspective->tileIds[12] = 0x10c;
    perspective->tileIds[26] = 0x11a;
    perspective->tileIds[27] = 0x11b;
    perspective->tileIds[28] = 0x11c;
    Map_UpdateCurrentTileBlock();
  }
  MemFree(buffer);
}

// HBlank setup each frame on the world map: copies the first scanline's BG2
// and BG3 affine parameters, starts the HBlank DMA for the rest, and turns
// the window on or off for the window's top and bottom.
void MapAnimation_ApplyAffineFrame(void) {
  struct MapAffinePair(*affinePages)[160] =
      ((struct MapFrameWork *)WORK_SLOT(7))->pages;
  s32 displayControl = (s16)(REG_DISPCNT & 0xfff8);
  u32 *affineRegisters = (u32 *)&REG_BG2PA;
  struct PerspectiveWork *perspective = WORK_SLOT(8);
  volatile u16 *dma0 = REG_DMA0;
  u32 *affineSource;
  u32 mode;
  u32 windowTop;
  u32 nextDisplayControl;

  dma0[5] &= 0xc5ff;
  dma0[5] &= 0x7fff;
  (void)dma0[5];
  if (affinePages != NULL) {
    affineSource = (u32 *)affinePages[gFrameCount & 1];
    *affineRegisters++ = *affineSource++;
    *affineRegisters++ = *affineSource++;
    *affineRegisters++ = *affineSource++;
    *affineRegisters++ = *affineSource++;
    *affineRegisters++ = *affineSource++;
    *affineRegisters++ = *affineSource++;
    *affineRegisters++ = *affineSource++;
    *affineRegisters = *affineSource++;
    Dma_Set(affineSource, (void *)&REG_BG2PA, 0xa6600008, (volatile u32 *)dma0);
  }
  perspective->shownTop = perspective->windowTop;
  perspective->shownBottom = perspective->windowBottom;
  windowTop = perspective->shownTop;
  mode = 0;
  if (windowTop < 200) {
    u16 bottom = perspective->shownBottom;

    mode = (bottom != 0) << 1;
    if (windowTop <= bottom) {
      mode = 0;
      if (windowTop == 0)
        mode = 2;
    }
  }
  displayControl |= mode;
  nextDisplayControl = (u16)displayControl;
  REG_DISPCNT = nextDisplayControl;
  perspective->windowPending = 0;
}

// Copies one world map cell's two tile entries (top and bottom) to a BG map.
static __inline__ void CopyWorldMapTileColumn(u8 *cellBytes, u8 *screenTile) {
  u32 metatileIndex = *(u16 *)cellBytes;
  u16 *metatileTiles = (u16 *)Ram_MapCellBuffer;

  metatileTiles += metatileIndex * 2;
  *(u16 *)screenTile = *metatileTiles++;
  *(u16 *)(screenTile + 64) = *metatileTiles;
}

// Draws world map tileRow into the BG map: 32 cells, then 32
// more a screen block on.
void Map_RenderPaletteMappedRow(u32 tileRow) {
  u8 *sourceCell;
  u8 *screenTile;
  u32 cellsCopied;

  sourceCell = Ram_MapBlocks + ((((s32)tileRow / 2) & 31) << 7);
  screenTile = (u8 *)(0x06004000 + ((tileRow & 62) << 6));
  cellsCopied = 0;
  do {
    CopyWorldMapTileColumn(sourceCell, screenTile);
    cellsCopied++;
    screenTile += 2;
    sourceCell += 4;
  } while (cellsCopied <= 31);

  screenTile += 0xfc0;
  sourceCell += 0xf80;
  cellsCopied = 0;
  do {
    CopyWorldMapTileColumn(sourceCell, screenTile);
    cellsCopied++;
    screenTile += 2;
    sourceCell += 4;
  } while (cellsCopied <= 31);
}

// Draws world map tileColumn into the BG map: 64 cells down.
void Map_RenderPaletteMappedColumn(u32 tileColumn) {
  u8 *sourceCell;
  u8 *screenTile;
  u32 cellsCopied;

  sourceCell = Ram_MapBlocks + ((((s32)tileColumn / 2) & 31) << 2);
  screenTile = (u8 *)(0x06004000 + (tileColumn & 62));
  cellsCopied = 0;
  do {
    CopyWorldMapTileColumn(sourceCell, screenTile);
    cellsCopied++;
    screenTile += 128;
    sourceCell += 128;
  } while (cellsCopied <= 63);
}

// Each frame on the world map: loads tiles as the view moves, shakes and
// follows the player, draws the BG row or column that scrolls into view,
// moves the camera, and builds the ground plane's scanline table.
void WorldMap_UpdateView(void) {
  struct BattleCamera *camera = WORK_SLOT(HEAP_SLOT_CAMERA);
  struct MapFrameWork *frameWork = WORK_SLOT(7);
  struct PerspectiveWork *perspective = WORK_SLOT(8);
  s32 *cameraPosition = camera->pos;
  s32 *followedPosition = perspective->origin;
  struct MapAffinePair(*affinePages)[160] = frameWork->pages;
  s32 distance = perspective->farPlane;
  s32 height = perspective->distance;
  struct FixedPointPosition eyeOffset;

  Map_UpdateCurrentTileBlockUntilBlocked();
  if (followedPosition != NULL) {
    s32 x;
    s32 y;
    s32 tileColumn;
    s32 tileRow;

    y = followedPosition[2];
    x = followedPosition[0];
    if (perspective->shakeX != 0) {
      s32 shakeDifference = Random16();
      s32 shakeAmplitude;

      shakeDifference -= Random16();
      shakeAmplitude = perspective->shakeX;
      x += Iwram_MulQ16(shakeAmplitude, shakeDifference);
      perspective->shakeX =
          Iwram_MulQ16(shakeAmplitude, perspective->shakeDecay);
    }
    if (perspective->shakeY != 0) {
      s32 shakeDifference = Random16();
      s32 shakeAmplitude;

      shakeDifference -= Random16();
      shakeAmplitude = perspective->shakeY;
      y += Iwram_MulQ16(shakeAmplitude, shakeDifference);
      perspective->shakeY =
          Iwram_MulQ16(shakeAmplitude, perspective->shakeDecay);
    }
    tileColumn = x / 0x100000;
    tileRow = y / 0x100000;
    if ((perspective->viewX ^ x) & 0x100000) {
      if (perspective->viewX < x)
        Map_RenderPaletteMappedColumn(tileColumn + 16);
      else
        Map_RenderPaletteMappedColumn(tileColumn - 16);
    }
    if ((perspective->viewY ^ y) & 0x100000) {
      if (perspective->viewY < y)
        Map_RenderPaletteMappedRow(tileRow + 12);
      else
        Map_RenderPaletteMappedRow(tileRow - 18);
    }
    perspective->viewX = x;
    perspective->viewY = y;
  }
  // Rebuild the projection around the followed position before the affine pass.
  gProjection.centerX = 120;
  gProjection.centerY = 96;
  Projection_Set(distance, height / 2, height * 2);
  cameraPosition[0] = *followedPosition++;
  cameraPosition[1] = 0;
  cameraPosition[2] = followedPosition[1];
  Render_ResetTransformState();
  SceneTransform_ApplyPosition(cameraPosition);
  SceneTransform_ApplyYaw(perspective->yaw);
  SceneTransform_ApplyPitch(perspective->pitch);
  eyeOffset.x = 0;
  eyeOffset.y = 0;
  eyeOffset.z = height + 0x10000;
  Iwram_TransformVector(&eyeOffset, (s32 *)camera);
  Render_ResetTransformState();
  Transform_SetLookAt(camera->eye, cameraPosition);
  // Pitch changes require a new ground-plane intersection for every scanline.
  if (gScanlinePitch != perspective->pitch) {
    s32 cosine = Trig_Cos(perspective->pitch);
    s32 sine = Trig_Sin(perspective->pitch);

    WorldMap_BuildScanlineTable(Iwram_RatioMulQ14(cosine, sine), cameraPosition,
                                frameWork->scanlines);
    gScanlineAngle = 0;
    gScanlinePitch = perspective->pitch;
  }
  ((s32 (*)(void *, s32 *, void *, void *))gWorkSlot[46])(
      camera, cameraPosition, frameWork->scanlines,
      affinePages[gFrameCount & 1]);
}

// Loads and draws the 2x2 world map tiles around the player in both layers.
void Map_UpdateCurrentTileBlock(void) {
  struct PerspectiveWork *perspective = gMapWork[0];
  s32 firstQuarterX = 0;
  s32 firstQuarterY = 0;
  u32 layer;
  u32 row;
  u32 col;
  s32 layerTileBase;
  s32 tileId;

  if (perspective->origin != NULL) {
    s32 *followedPosition = perspective->origin;

    firstQuarterX = *followedPosition++;
    firstQuarterY = followedPosition[1];
  }
  firstQuarterX = (firstQuarterX - 0x1000000) >> 25;
  firstQuarterY = (firstQuarterY - 0x1400000) >> 25;
  layer = 0;
  layerTileBase = 0;
  for (; layer < 2; layer++) {
    for (row = 0; row < 2; row++) {
      for (col = 0; col < 2; col++) {
        tileId =
            (((firstQuarterY + row) & 15) << 4) + ((firstQuarterX + col) & 15);
        tileId = perspective->tileIds[tileId];
        tileId += layerTileBase;
        Map_WriteLayerCellTile(layer, firstQuarterX + col, firstQuarterY + row,
                               tileId, 1);
      }
    }
    layerTileBase += 320;
  }
}

// Loads the first of the 2x2 world map tiles around the player that is not
// shown yet (at most one a frame), without drawing it.
void Map_UpdateCurrentTileBlockUntilBlocked(void) {
  struct PerspectiveWork *perspective = gMapWork[0];
  s32 firstQuarterX = 0;
  s32 firstQuarterY = 0;
  u32 layer;
  u32 row;
  u32 col;
  s32 tileId;

  if (perspective->origin != NULL) {
    s32 *followedPosition = perspective->origin;

    firstQuarterX = *followedPosition++;
    firstQuarterY = followedPosition[1];
  }
  firstQuarterX = (firstQuarterX - 0x1000000) >> 25;
  firstQuarterY = (firstQuarterY - 0x1400000) >> 25;
  for (layer = 0; layer < 2; layer++) {
    for (row = 0; row < 2; row++) {
      for (col = 0; col < 2; col++) {
        tileId =
            (((firstQuarterY + row) & 15) << 4) + ((firstQuarterX + col) & 15);
        tileId = perspective->tileIds[tileId];
        tileId += layer * 320;
        if (Map_WriteLayerCellTile(layer, firstQuarterX + col,
                                   firstQuarterY + row, tileId, 0) != 0)
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
  struct MapFrameWork *frameWork = WORK_SLOT(7);
  struct PerspectiveWork *perspective = WORK_SLOT(8);
  s32 one = 1;

  perspective->animationActive = one;
  Scheduler_SuspendTasks(MapAnimation_Update);
  Dma_Set(BG_CHAR_BLOCK(1), gDecodeBuffer, DMA_ENABLE32 | DMA_32BIT | 0x800,
          REG_DMA3);
  WaitFrames(1);
  Resource_RunCopiedDecoder((s32)frameWork->pages[gFrameCount & one],
                            (s32)gMapCellBuffer);
  perspective->windowTop = 200;
  perspective->windowBottom = 255;
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
  struct PerspectiveWork *perspective;
  u32 *graphicsResources;
  u8 *decodeBuffer;
  s16 backdropColor;

  perspective = gMapWork[0];
  decodeBuffer = (u8 *)gMapCellBuffer;
  graphicsResources = perspective->resources;
  backdropColor = *(s16 *)BG_PLTT;
  Resource_DecodeByteLz(Resource_GetTableEntry(graphicsResources[0]),
                        decodeBuffer);
  *(s16 *)decodeBuffer = backdropColor;
  Dma_Set(decodeBuffer, (void *)BG_PLTT, DMA_ENABLE32 | DMA_32BIT | 0x70,
          REG_DMA3);
  Resource_DecodeType01(Resource_GetTableEntry(graphicsResources[1]),
                        gBgTileBuffer);
  Resource_DecodeType01(Resource_GetTableEntry(graphicsResources[2]),
                        Ram_BgTileBuffer + 0x2000);
  Resource_DecodeType01(Resource_GetTableEntry(graphicsResources[3]),
                        Ram_BgTileBuffer + 0x4000);
  Resource_DecodeType01(Resource_GetTableEntry(graphicsResources[4]),
                        Ram_BgTileBuffer + 0x6000);
  gVBlankHook = Map_ShowBg1FromBuffer;
  perspective->windowTop = 0;
  perspective->windowBottom = 159;
  WaitFrames(1);
  Resource_DecodeType01(
      Resource_GetTableEntry((u32)&ResourceId_DefaultMapCells), decodeBuffer);
  Map_UpdateCurrentTileBlock();
  perspective->animationActive = 0;
  Scheduler_ResumeTasks(MapAnimation_Update);
  WaitFrames(1);
}

// Puts the default world map cells back after a BG1 animation and stops it.
void Map_LoadDefaultCellsAndUpdateBlock(void) {
  struct PerspectiveWork *perspective = gMapWork[0];
  gVBlankHook = Map_ShowBg1FromBuffer;
  perspective->windowTop = 0;
  perspective->windowBottom = 0x9f;
  WaitFrames(1);
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_DefaultMapCells), gMapCellBuffer);
  Map_UpdateCurrentTileBlock();
  Scheduler_ResumeTasks(MapAnimation_Update);
  WaitFrames(1);
}

// Each frame: runs the 16 tile animation channels. A channel script is
// groups of four halfwords (source tile, count, destination tile, frames to
// wait) that copy tiles into VRAM; 0xfeNN jumps to halfword offset NN * 2,
// 0xfeff stops, and 0xffff loops to the start.
void MapAnimation_Update(void) {
  /* FAKEMATCH: each channel is written (state->anim + i), not through a channel
   * pointer: a pointer anim = state->anim + i (or &state->anim[i], declared in
   * either scope) differs 4, ip and lr swapping the 0xffff sentinel and the
   * VRAM base; a walking anim++ differs 8; state->anim[i]. differs 167, no
   * walking pointer at all. */
  struct MapState *mapState = gMapWork[0];
  u32 channelIndex;

  for (channelIndex = 0; channelIndex <= 15; channelIndex++) {
    const u16 *commands;
    u32 sourceTileOrOpcode;
    u32 tileCount;
    u32 destinationTile;

    if ((mapState->anim + channelIndex)->start == NULL ||
        (mapState->anim + channelIndex)->paused != 0)
      continue;
  next:
    if ((mapState->anim + channelIndex)->timer == 0) {
      commands = (mapState->anim + channelIndex)->cursor;
      sourceTileOrOpcode = *commands++;
      if (sourceTileOrOpcode == 0xffff) {
        (mapState->anim + channelIndex)->cursor =
            (mapState->anim + channelIndex)->start;
        goto next;
      }
      if ((sourceTileOrOpcode & 0xff00) == 0xfe00) {
        if ((sourceTileOrOpcode & 0xff) == 0xff)
          continue;
        (mapState->anim + channelIndex)->cursor =
            (mapState->anim + channelIndex)->start +
            (sourceTileOrOpcode & 0xff) * 2;
        goto next;
      }
      tileCount = *commands++;
      destinationTile = commands[0];
      (mapState->anim + channelIndex)->timer = commands[1];
      if (mapState->wideTiles == 0) {
        if (sourceTileOrOpcode >= 0x600)
          Dma_Set(Ram_DecodeBuffer + sourceTileOrOpcode * 32,
                  BG_CHAR_BLOCK(1) + destinationTile * 32,
                  DMA_ENABLE32 | DMA_32BIT | tileCount * 8, REG_DMA3);
        else
          Dma_Set(BG_CHAR_BLOCK(1) + sourceTileOrOpcode * 32,
                  BG_CHAR_BLOCK(1) + destinationTile * 32,
                  DMA_ENABLE32 | DMA_32BIT | tileCount * 8, REG_DMA3);
      } else {
        if (sourceTileOrOpcode >= 0x200)
          Dma_Set(Ram_MapBlocks + sourceTileOrOpcode * 64,
                  BG_CHAR_BLOCK(2) + destinationTile * 64,
                  DMA_ENABLE32 | DMA_32BIT | tileCount * 16, REG_DMA3);
        else
          Dma_Set(BG_CHAR_BLOCK(2) + sourceTileOrOpcode * 64,
                  BG_CHAR_BLOCK(2) + destinationTile * 64,
                  DMA_ENABLE32 | DMA_32BIT | tileCount * 16, REG_DMA3);
      }
      (mapState->anim + channelIndex)->cursor += 4;
      goto next;
    } else {
      (mapState->anim + channelIndex)->timer--;
    }
  }
}

// Resumes tile animation channel no.
void Map_ClearLayerEntryFlag(u32 channelIndex) {
  struct MapState *mapState = gMapWork[0];
  struct MapAnimation *channel = &mapState->anim[channelIndex];

  channel->paused = 0;
}

// Pauses tile animation channel no.
void Map_SetLayerEntryFlag(u32 channelIndex) {
  struct MapState *mapState = gMapWork[0];
  struct MapAnimation *channel = &mapState->anim[channelIndex];

  channel->paused = 1;
}

// Starts the tile animation channels a map's animation script lists: each
// 0xfdNN marker starts channel NN & 15 there (paused when bit 7 is set).
void MapAnimation_StartChannels(const u16 *script) {
  struct MapState *mapState;
  s32 channelsStarted;
  u32 marker;

  mapState = gMapWork[0];
  channelsStarted = 0;
  Dma_Fill32(0, mapState->anim, sizeof(mapState->anim));
  marker = *script++;
  while (marker != 0xffff) {
    if ((marker & 0xff00) == 0xfd00) {
      struct MapAnimation *channel;
      u32 channelIndex = marker & 15;
      u32 paused = 0;
      if (marker & 0x80)
        paused = 1;
      channel = &mapState->anim[channelIndex];
      channel->start = script;
      channel->cursor = script;
      channel->timer = 0;
      channel->paused = paused;
      channelsStarted++;
    }
    marker = *script++;
  }
  if (channelsStarted != 0)
    Scheduler_AddOrUpdateCallback(MapAnimation_Update, 0xc80);
}

// Suspends the tile animations, unless a BG1 animation is running.
void Map_EnableUpdateCallback(void) {
  if (((struct MapScrollWork *)gMapWork[0])->animationActive == 0)
    Scheduler_SuspendTasks(MapAnimation_Update);
}

// Resumes the tile animations, unless a BG1 animation is running.
void Map_DisableUpdateCallback(void) {
  if (((struct MapScrollWork *)gMapWork[0])->animationActive == 0)
    Scheduler_ResumeTasks(MapAnimation_Update);
}

// Each frame: runs the map's blend script. A 0x3NNN command sets BLDCNT;
// any other sets BLDALPHA (or BLDY when not alpha blending) and waits the
// halfword after it in frames; 0xfeNN jumps, 0xfeff stops and 0xffff loops.
// A for (;;) retry loop with continue hoists the blendControl address and
// changes 118 text bytes in tbs-en (2026-10-16); keep the retry edges for now.
void DisplayBlend_RunScript(void) {
  struct MapState *mapState = gMapWork[0];
  struct BlendScriptState *blend;
  u16 *commands;
  u32 instruction;
  u32 delayOrJumpIndex;
  u16 delay16;

  blend = &mapState->blend;
  if (blend->script == 0)
    return;
  if (blend->paused != 0)
    return;

again:
  delayOrJumpIndex = blend->delay;
  delay16 = delayOrJumpIndex;
  if (delay16 == 0) {
    commands = blend->cursor;
    instruction = *commands;
    commands++;
    if (instruction == 0xffff) {
      blend->cursor = blend->script;
      goto again;
    }

    if ((instruction & 0xff00) == 0xfe00) {
      delayOrJumpIndex = instruction & 0xff;
      if (delayOrJumpIndex == 0xff)
        return;
      blend->cursor = blend->script + delayOrJumpIndex * 2;
      goto again;
    }

    if ((instruction & 0xf000) == 0x3000) {
      REG_BLDCNT = instruction;
      mapState->blendControl = instruction;
      blend->cursor++;
      goto again;
    }

    if ((mapState->blendControl & 0xc0) == 0x40)
      REG_BLDALPHA = instruction;
    else
      REG_BLDY = instruction;
    blend->delay = commands[0];
    blend->cursor += 2;
    goto again;
  }
  blend->delay = delayOrJumpIndex + 0xffff;
}

// Starts a map's blend script, unless it is empty.
void DisplayBlend_StartScript(u16 *script) {
  struct BlendScriptState *blend;
  s32 hasScript;
  volatile u32 zero;

  hasScript = 0;
  zero = 0;
  blend = &((struct MapState *)gMapWork[0])->blend;
  Dma_Set((const void *)&zero, blend, 0x85000003, REG_DMA3);
  if (*script != 0xffff) {
    blend->script = script;
    blend->cursor = script;
    blend->delay = 0;
    blend->paused = 0;
    hasScript = 1;
  }
  if (hasScript)
    Scheduler_AddOrUpdateCallback(DisplayBlend_RunScript, 0xc80);
}

// Suspends the blend script.
void DisplayBlend_EnableRunScript(void) {
  Scheduler_SuspendTasks(DisplayBlend_RunScript);
}

// Resumes the blend script.
void DisplayBlend_DisableRunScript(void) {
  Scheduler_ResumeTasks(DisplayBlend_RunScript);
}

// Allocates the palette cycling queue (heap slot 28) with its four entries
// cleared.
void Runtime_AllocateAndClearQueue(void) {
  struct PaletteSnapshotWork *paletteCycles;
  struct PaletteSnapshot *cycle;
  u16 cycleIndex;
  u16 colorIndex;

  paletteCycles = Runtime_AllocateBlock(28, sizeof(struct PaletteSnapshotWork));
  cycle = paletteCycles->entries;
  for (cycleIndex = 0; cycleIndex != 4; cycleIndex++) {
    cycle->source = 0;
    cycle->position = 0;
    cycle->timer = 0;
    cycle->delay = 0;
    cycle->count = 0;
    for (colorIndex = 0; colorIndex != 16; colorIndex++) {
      cycle->colors[colorIndex] = 0;
    }
    cycle++;
  }
  paletteCycles->count = 0;
}

// Adds a palette cycle: colorCount colours of background palette paletteBank
// from firstColor, rotated by one every delay + 1 frames. Returns 0, or -1 when
// all four cycles are taken.
s32 PaletteQueue_Add(s16 paletteBank, s16 firstColor, s16 delay,
                     s16 colorCount) {
  struct PaletteSnapshotWork *paletteQueue;
  struct PaletteSnapshot *cycle;
  u16 *paletteColors;
  u32 cycleIndex;
  u16 colorCount16;

  paletteQueue = gPaletteWork;
  cycleIndex = paletteQueue->count;
  if (cycleIndex > 3)
    return -1;
  cycle = &paletteQueue->entries[cycleIndex];
  paletteColors = (u16 *)BG_PLTT + (((u16)paletteBank << 4) + (u16)firstColor);
  colorCount16 = colorCount;
  cycle->position = 0;
  cycle->timer = 0;
  cycle->count = colorCount16;
  cycle->source = paletteColors;
  cycle->delay = delay;
  Dma_Set(paletteColors, cycle->colors, 0x80000000 | colorCount16, REG_DMA3);
  paletteQueue->count++;
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
  struct PaletteSnapshotWork *paletteQueue = gPaletteWork;
  u8 cycleIndex;
  // BUG: four queued cycles become zero here, so none advance.
  u16 activeCycles = paletteQueue->count & 3;
  u16 rotatedColors[16];

  for (cycleIndex = 0; cycleIndex < activeCycles; cycleIndex++) {
    struct PaletteSnapshot *cycle = &paletteQueue->entries[cycleIndex];
    if (cycle->timer == 0) {
      u16 rotationOffset = cycle->position;
      u16 colorCount = cycle->count;
      void *paletteColors = cycle->source;
      u16 *savedColors = cycle->colors;
      u8 colorIndex;
      for (colorIndex = colorCount - rotationOffset; colorIndex < colorCount;
           colorIndex++)
        rotatedColors[colorIndex] = *savedColors++;
      for (colorIndex = 0; colorIndex < colorCount - rotationOffset;
           colorIndex++)
        rotatedColors[colorIndex] = *savedColors++;
      Dma_Set(rotatedColors, paletteColors, 0x80000000 | colorCount, REG_DMA3);
      rotationOffset++;
      if (rotationOffset >= colorCount)
        rotationOffset = 0;
      cycle->position = rotationOffset;
      cycle->timer = cycle->delay;
    } else {
      cycle->timer--;
    }
  }
}

// A sample is in eight-pixel units. UB: signed samples may be negative, so
// their left shift is undefined. Multiplication fits s32 for both s8 and u8.
#ifdef UBFIX
#define CURVE_HEIGHT_FIXED(sample) ((sample) * (1 << CURVE_VALUE_SHIFT))
#else
#define CURVE_HEIGHT_FIXED(sample) ((sample) << CURVE_VALUE_SHIFT)
#endif

// The terrain height functions (gMap_TerrainHeightFunctions): each takes a
// cell's height samples after its kind byte and a pixel (x, y) within the
// 16x16 cell, and returns the ground height there in 16.16 pixels. This one
// is flat: the first sample everywhere.
s32 Curve_GetFirstSampleA(const u8 *heightBytes, s32 pixelX, s32 pixelY) {
  const s8 *samples = (const s8 *)heightBytes;
  return CURVE_HEIGHT_FIXED(samples[0]);
}

// A slope along x: from sample 0 at x = 0 towards sample 1 at x = 16.
s32 Curve_LerpTwoSamples(const u8 *heightBytes, s32 pixelX, s32 pixelY) {
  const s8 *samples = (const s8 *)heightBytes;
  s32 startHeight;
  s32 heightDelta;

  startHeight = CURVE_HEIGHT_FIXED(samples[0]);
  heightDelta = (CURVE_HEIGHT_FIXED(samples[1]) - startHeight) * pixelX;
  if (heightDelta < 0) {
    heightDelta += CURVE_FULL_STEPS - 1;
  }
  return startHeight + (heightDelta >> 4);
}

// A slope along y: from sample 0 at y = 0 towards sample 1 at y = 16.
s32 Curve_LerpTwoSamplesB(const u8 *heightBytes, s32 pixelX, s32 pixelY) {
  const s8 *samples = (const s8 *)heightBytes;
  s32 startHeight;
  s32 heightDelta;

  startHeight = CURVE_HEIGHT_FIXED(samples[0]);
  heightDelta = (CURVE_HEIGHT_FIXED(samples[1]) - startHeight) * pixelY;
  if (heightDelta < 0) {
    heightDelta += CURVE_FULL_STEPS - 1;
  }
  return startHeight + (heightDelta >> 4);
}

// A step across the diagonal x + y = 15: sample 0 before it, sample 1 after
// it and the higher of the two on it.
s32 Curve_StepAtSummedPosition(const u8 *heightBytes, s32 x, s32 y) {
  const s8 *samples = (const s8 *)heightBytes;
  s32 firstHeight;
  s32 secondHeight;
  s32 higherHeight;
  u32 summedPixel;

  firstHeight = CURVE_HEIGHT_FIXED(samples[0]);
  secondHeight = CURVE_HEIGHT_FIXED(samples[1]);
  higherHeight = firstHeight;
  if (secondHeight > firstHeight) {
    higherHeight = secondHeight;
  }
  summedPixel = x + y;
  if (summedPixel == CURVE_FULL_STEPS - 1) {
    return higherHeight;
  }
  if (summedPixel < CURVE_FULL_STEPS - 1) {
    secondHeight = firstHeight;
  }
  return secondHeight;
}

// A step across the diagonal x = y: sample 0 where y < x, sample 1 where
// y > x and the higher of the two on it.
s32 Curve_StepAtDifferencePosition(const u8 *heightBytes, s32 x, s32 y) {
  const s8 *samples = (const s8 *)heightBytes;
  s32 firstHeight;
  s32 secondHeight;
  s32 higherHeight;
  s32 pixelDifference;
  u32 biasedDifference = x;

  firstHeight = CURVE_HEIGHT_FIXED(samples[0]);
  secondHeight = CURVE_HEIGHT_FIXED(samples[1]);
  higherHeight = firstHeight;
  if (secondHeight > firstHeight) {
    higherHeight = secondHeight;
  }
  pixelDifference = y - biasedDifference;
  biasedDifference = pixelDifference + CURVE_FULL_STEPS - 1;
  if (biasedDifference == CURVE_FULL_STEPS - 1) {
    return higherHeight;
  }
  if (biasedDifference < CURVE_FULL_STEPS - 1) {
    secondHeight = firstHeight;
  }
  return secondHeight;
}

// A slope that bends on the diagonal x + y = 15: from sample 0 at the top
// left corner to sample 1 on the diagonal to sample 2 at the bottom right.
s32 Curve_LerpThreeSamplesByRatio(const u8 *heightBytes, s32 x, s32 y) {
  const s8 *samples = (const s8 *)heightBytes;
  s32 firstHeight;
  s32 middleHeight;
  s32 lastHeight;
  s32 height;

  firstHeight = CURVE_HEIGHT_FIXED(*samples++);
  middleHeight = CURVE_HEIGHT_FIXED(*samples);
  lastHeight = CURVE_HEIGHT_FIXED(samples[1]);
  x += y;
  if (x == CURVE_FULL_STEPS - 1) {
    height = middleHeight;
  } else if ((u32)x < CURVE_FULL_STEPS - 1) {
    height =
        firstHeight + (middleHeight - firstHeight) * x / (CURVE_FULL_STEPS - 1);
  } else {
    x -= (CURVE_FULL_STEPS - 1);
    height =
        middleHeight + (lastHeight - middleHeight) * x / (CURVE_FULL_STEPS - 1);
  }
  return height;
}

// A slope that bends on the diagonal x = y: from sample 0 at the top right
// corner to sample 1 on the diagonal to sample 2 at the bottom left.
s32 Curve_LerpThreeSamplesAtDifference(const u8 *heightBytes, s32 x, s32 y) {
  const s8 *samples = (const s8 *)heightBytes;
  s32 firstHeight;
  s32 middleHeight;
  s32 lastHeight;
  s32 pixelDifference;
  s32 heightDelta;

  firstHeight = CURVE_HEIGHT_FIXED(*samples++);
  middleHeight = CURVE_HEIGHT_FIXED(samples[0]);
  lastHeight = samples[1];
  pixelDifference = y - x;
  x = pixelDifference + CURVE_FULL_STEPS - 1;
  lastHeight = CURVE_HEIGHT_FIXED(lastHeight);

  if (x == CURVE_FULL_STEPS - 1)
    return middleHeight;

  if ((u32)x < CURVE_FULL_STEPS - 1) {
    heightDelta = middleHeight - firstHeight;
    pixelDifference = heightDelta * x;
    if (pixelDifference < 0)
      pixelDifference += CURVE_FULL_STEPS - 1;
    pixelDifference >>= 4;
    return firstHeight + pixelDifference;
  }
  pixelDifference *= lastHeight - middleHeight;
  if (pixelDifference < 0)
    pixelDifference += CURVE_FULL_STEPS - 1;
  pixelDifference >>= 4;
  return middleHeight + pixelDifference;
}

// A raised or sunken rounded square: sample 1 where gCurve_LerpWeightTable is
// 1, sample 0 in the corners.
s32 Curve_LerpTwoSamplesByTable(const u8 *heightBytes, s32 x, s32 y) {
  const s8 *samples = (const s8 *)heightBytes;
  s32 startHeight;

  startHeight = CURVE_HEIGHT_FIXED(samples[0]);
  return startHeight + ((CURVE_HEIGHT_FIXED(samples[1]) - startHeight) *
                        gCurve_LerpWeightTable[x + (y * CURVE_FULL_STEPS)]);
}

// A step along x: sample 0 for x < 8, sample 1 from x = 8.
s32 Curve_StepAtHalf(const u8 *heightBytes, s32 pixelX, s32 pixelY) {
  const s8 *samples = (const s8 *)heightBytes;
  u32 cellX = pixelX;
  s32 heightSample;
  if (cellX < CURVE_HALF_STEPS)
    heightSample = samples[0];
  else
    heightSample = samples[1];
  return CURVE_HEIGHT_FIXED(heightSample);
}

// A step along y: sample 0 for y < 8, sample 1 from y = 8.
s32 Curve_StepAtHalfB(const u8 *heightBytes, s32 pixelX, s32 pixelY) {
  const s8 *samples = (const s8 *)heightBytes;
  u32 cellY = pixelY;
  s32 heightSample;
  if (cellY < CURVE_HALF_STEPS)
    heightSample = samples[0];
  else
    heightSample = samples[1];
  return CURVE_HEIGHT_FIXED(heightSample);
}

// A slope along x that bends at the middle: sample 0 at x = 0, sample 1 at
// x = 8, towards sample 2 at x = 16.
s32 Curve_LerpThreeSamplesAtHalf(const u8 *heightBytes, s32 pixelX,
                                 s32 pixelY) {
  const s8 *samples = (const s8 *)heightBytes;
  u32 cellX = pixelX;
  s32 endpointHeight;
  s32 middleHeight;
  s32 heightDelta;

  endpointHeight = CURVE_HEIGHT_FIXED(*samples++);
  middleHeight = CURVE_HEIGHT_FIXED(*samples++);
  if (cellX < CURVE_HALF_STEPS) {
    heightDelta = (middleHeight - endpointHeight) * cellX;
    if (heightDelta < 0) {
      heightDelta += CURVE_HALF_STEPS - 1;
    }
    return endpointHeight + (heightDelta >> 3);
  }

  {
    s32 offsetPastMiddle;

    endpointHeight = CURVE_HEIGHT_FIXED(*samples);
    offsetPastMiddle = cellX;
    endpointHeight -= middleHeight;
    offsetPastMiddle -= CURVE_HALF_STEPS;
    heightDelta = endpointHeight * offsetPastMiddle;
    if (heightDelta < 0) {
      heightDelta += CURVE_HALF_STEPS - 1;
    }
    return middleHeight + (heightDelta >> 3);
  }
}

// An unsigned-sample slope along y that bends at the middle.
s32 Curve_LerpThreeSamplesAtHalfB(const u8 *heightBytes, s32 pixelX,
                                  s32 pixelY) {
  const u8 *samples = heightBytes;
  u32 cellY = pixelY;
  s32 endpointHeight;
  s32 middleHeight;
  s32 offsetPastMiddle;
  s32 heightDelta;
  s32 tailDelta;

  endpointHeight = CURVE_HEIGHT_FIXED(*samples++);
  middleHeight = CURVE_HEIGHT_FIXED(*samples++);
  if (cellY < CURVE_HALF_STEPS) {
    heightDelta = (middleHeight - endpointHeight) * cellY;
    if (heightDelta < 0) {
      heightDelta += CURVE_HALF_STEPS - 1;
    }
    return endpointHeight + (heightDelta >> 3);
  }

  endpointHeight = CURVE_HEIGHT_FIXED(*samples);
  offsetPastMiddle = cellY;
  endpointHeight -= middleHeight;
  offsetPastMiddle -= CURVE_HALF_STEPS;
  tailDelta = endpointHeight * offsetPastMiddle;
  if (tailDelta < 0) {
    tailDelta += CURVE_HALF_STEPS - 1;
  }
  return middleHeight + (tailDelta >> 3);
}

// The sample gCurve_SampleIndexTable picks for (x, y).
s32 Curve_LookupSampleByTable(const u8 *heightBytes, s32 pixelX, s32 pixelY) {
  const s8 *samples = (const s8 *)heightBytes;
  u32 cellX = pixelX;
  u32 cellY = pixelY;
  return CURVE_HEIGHT_FIXED(
      samples[gCurve_SampleIndexTable[cellX + (cellY << 4)]]);
}

// The same, with the table mirrored left to right.
s32 Curve_LookupSampleByTableReversed(const u8 *heightBytes, s32 pixelX,
                                      s32 pixelY) {
  const s8 *samples = (const s8 *)heightBytes;
  u32 cellX = pixelX;
  u32 cellY = pixelY;
  return CURVE_HEIGHT_FIXED(
      samples[gCurve_SampleIndexTable[((cellY << 4) - cellX) +
                                      CURVE_FULL_STEPS - 1]]);
}

// Flat, as Curve_GetFirstSampleA.
s32 Curve_GetFirstSampleB(const u8 *heightBytes, s32 pixelX, s32 pixelY) {
  const s8 *samples = (const s8 *)heightBytes;
  return CURVE_HEIGHT_FIXED(samples[0]);
}

// Flat, as Curve_GetFirstSampleA.
s32 Curve_GetFirstSampleC(const u8 *heightBytes, s32 pixelX, s32 pixelY) {
  const s8 *samples = (const s8 *)heightBytes;
  return CURVE_HEIGHT_FIXED(samples[0]);
}

#undef CURVE_HEIGHT_FIXED

// Returns the ground height of a layer at (x, y) in 16.16 pixels: the
// height function of the cell's terrain kind applied to its samples.
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 y) {
  struct MapState *work = gMapWork[0];
  struct MapCell *cell;
  u8 *heightBytes;

  x >>= 16;
  y >>= 16;
  cell = (struct MapCell *)Ram_MapCellBuffer;
  if (work != NULL) {
    cell = work->layers[layer & 3].cells;
  }
  cell += x / 16 + (y / 16 << 7);
  heightBytes = Ram_MapCollision + cell->attributeB * 4;
  // UB: sequence the kind-byte read before passing the remaining samples.
#ifdef UBFIX
  {
    u32 kind = *heightBytes++ & 15;

    return gMap_TerrainHeightFunctions[kind](heightBytes, x & 15, y & 15);
  }
#else
  return gMap_TerrainHeightFunctions[*heightBytes++ & 15](heightBytes, x & 15,
                                                          y & 15);
#endif
}

// Returns the terrain kind (the low four bits of the first collision byte)
// of a layer's cell at (x, y) in 16.16 pixels.
s32 Map_GetCellAttributeLowNibble(s32 layer, s32 x, s32 y) {
  struct MapState *state = gMapWork[0];
  struct MapCell *cells;
  s32 cellX;
  s32 cellY;
  u32 heightAttributeIndex;

  x >>= 16;
  y >>= 16;
  cells = (struct MapCell *)gMapCellBuffer;
  if (state != 0) {
    cells = state->layers[layer & 3].cells;
  }
  cellX = x / 16;
  cellY = y / 16;
  cells += cellX + (cellY << 7);
  heightAttributeIndex = cells->attributeB;
  return *(Ram_MapCollision + heightAttributeIndex * 4) & 15;
}

// Returns the collision code of a layer's cell at (x, y) in 16.16 pixels.
s32 GetMapCellCollision(s32 layer, s32 x, s32 y) {
  struct MapState *state;
  struct MapCell *cell;

  state = gMapWork[0];
  x >>= 20;
  y >>= 20;
  cell = (struct MapCell *)gMapCellBuffer;
  if (state != NULL) {
    cell = state->layers[layer & 3].cells;
  }
  cell += x + (y << 7);
  return cell->collisionCode;
}

// Sets the collision code of a layer's cell at (x, y) in 16.16 pixels.
void SetMapCellCollision(u32 layer, s32 x, s32 y, u32 collisionCode) {
  struct MapState *state = gMapWork[0];

  x >>= 20;
  y >>= 20;
  if (state != NULL) {
    struct MapCell *cell = state->layers[layer & 3].cells;

    cell += x + (y << 7);
    cell->collisionCode = collisionCode;
  }
}

// Returns the top two bits of the block word of the cell at pixel (x, y).
s32 Map_GetCellHighFlags(s32 x, s32 y) {
  s32 cellX = x / 16;
  s32 cellY = y / 16;
  struct MapCell *mapCell =
      (struct MapCell *)gMapCellBuffer + (cellX + cellY * 128);

  return ((u8 *)&mapCell->metatileAndFlags)[1] >> 6;
}

// Returns whether an object can step to position: 2 when the cell there is
// a wall (collision code 0xff), 1 when the ground is more than 8 pixels
// higher than the object's, -1 when it is more than 12 pixels lower, else 0.
s32 Map_CheckObjectStep(struct ObjectRuntime *object,
                        struct FieldPosition *position) {
  struct MapState *mapState;
  s32 pixelX;
  s32 pixelZ;
  struct MapCell *cell;
  u8 *heightBytes;
  s32 targetGround;
  s32 stepHeight;

  pixelX = ((union FieldCoordinate *)&position->x)->part.pixel;
  pixelZ = ((union FieldCoordinate *)&position->z)->part.pixel;
  mapState = gMapWork[0];
  if (mapState == NULL)
    return 0;
  if (object->terrainId <= 2)
    cell = mapState->layers[object->terrainId].cells;
  else
    cell = (struct MapCell *)Ram_MapCellBuffer;
  cell += pixelX / 16 + (pixelZ / 16 << 7);
  if (cell->collisionCode == 0xff)
    return 2;
  heightBytes = Ram_MapCollision + cell->attributeB * 4;
  // UB: sequence the kind-byte read before passing the remaining samples.
#ifdef UBFIX
  {
    u32 kind = *heightBytes++ & 15;

    targetGround = gMap_TerrainHeightFunctions[kind](heightBytes, pixelX & 15,
                                                     pixelZ & 15);
  }
#else
  targetGround = gMap_TerrainHeightFunctions[*heightBytes++ & 15](
      heightBytes, pixelX & 15, pixelZ & 15);
#endif
  stepHeight = targetGround - object->terrainHeight;
  if (stepHeight > 0x80000)
    return 1;
  if (stepHeight < -0xc0000)
    return -1;
  return 0;
}

// Returns 0 when the cell under position (its x, and y less its height) in
// layer 2 is a wall (collision code 0xff), else -1.
s32 CheckMapPositionCellOccupied(struct WorldPosition *position) {
  s32 pixelX;
  s32 pixelY;
  s32 cellX;
  s32 cellY;
  struct MapState *mapState;
  struct MapCell *cell;

  pixelX = position->x / 65536;
  pixelY = (position->y - position->height) / 65536;
  mapState = gMapWork[0];
  if (mapState == NULL)
    return 0;
  cell = mapState->layers[2].cells;
  cellX = pixelX / 16;
  cellY = pixelY / 16;
  cell += cellX + cellY * 128;
  return (cell->collisionCode != 0xff) - 1;
}

// Returns the world map's collision value (1-15) at position: from the upper
// collision layer where it has one, else the lower, else 7. Each layer is a
// tile map of 4-bit collision pixels.
s32 GetWorldMapCollision(struct WorldPosition *position) {
  s32 halfPixelX = position->x >> 17;
  s32 y = position->y;
  s32 halfPixelY = y >> 17;
  u32 collisionCell;
  u32 pixelOffset;
  u8 collisionTile;
  u8 packedCollisions;
  u32 result;

  collisionCell = (((halfPixelY / 8) & 63) << 6) + ((halfPixelX / 8) & 63);
  pixelOffset = (((halfPixelY / 2) & 3) << 1) + ((halfPixelX / 4) & 1);

  collisionTile = BG_SCREEN_BLOCK(10)[collisionCell];
  packedCollisions =
      *(Ram_MapCollision + 0x800 + (collisionTile << 3) + pixelOffset);
  if (packedCollisions != 0) {
    if (halfPixelX & 2)
      result = packedCollisions >> 4;
    else
      result = packedCollisions & 15;
    if (result != 0)
      return result;
  }

  collisionTile = BG_SCREEN_BLOCK(8)[collisionCell];
  packedCollisions = *(Ram_MapCollision + (collisionTile << 3) + pixelOffset);
  if (packedCollisions != 0) {
    if (halfPixelX & 2)
      result = packedCollisions >> 4;
    else
      result = packedCollisions & 15;
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
  s32 collisionKind = GetWorldMapCollision(position);
  s32 x = position->x;
  s32 y;
  s32 behaviorBank = 0;
  u32 blockX;
  u32 index;
  u32 *blockWord;

  if (x < 0)
    x += 0x1fffff;
  blockX = (x >> 21) & 31;
  y = position->y;
  if (y < 0)
    y += 0x1fffff;

  index = blockX + (((y >> 21) & 31) << 5);
  blockWord = gMapBlocks + index;
  if (((u8 *)blockWord)[3] & 0x80)
    behaviorBank = 0x10;

  *terrainKind = (*blockWord << 1) >> 25;
  if (*terrainKind == 21)
    behaviorBank = 0x20;

  return gWorldMap_TerrainBehaviorTable[behaviorBank + collisionKind];
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
  s32 waitedFrames;

  work = gMapWork[0];
  waitedFrames = 0;
  while (waitedFrames < 300 && (work->shakeX > 255 || work->shakeY > 255)) {
    WaitFrames(1);
    waitedFrames++;
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
                                 struct MapScanline *scanline) {
  s32 groundPoint[3];
  s32 cameraPoint[3];
  s32 horizon;
  s32 focus;
  s32 *cameraCoordinates;
  s32 line;
  s32 distance;
  s32 difference;
  s32 scale;
  s32 horizontalSquared;
  s32 verticalSquared;
  s32 diagonal;
  s32 vertical;
  s32 zero;
  s32 horizontal;

  zero = 0;
  groundPoint[0] = position[0];
  groundPoint[1] = zero;
  groundPoint[2] = position[2];
  Iwram_TransformVector(groundPoint, cameraPoint);
  cameraCoordinates = cameraPoint;
  horizon = cameraCoordinates[1] - Iwram_MulQ16(cameraCoordinates[2], depth);
  focus = -gProjection.focal;
  for (line = 0; line < 160; line++) {
    // Lines below the screen centre have a negative displacement.
    distance = Iwram_RatioMulQ14(focus, (gProjection.centerY - line) * 65536);
    difference = distance - depth;
    if (difference == 0)
      difference = 1;
    scale = Iwram_RatioMulQ14(difference, horizon);
    if (scale < 0) {
      diagonal = Iwram_MulQ16(-scale, 0x8000);
      scanline->scale = Iwram_RatioMulQ14(gProjection.focal, diagonal);
      diagonal = Iwram_MulQ16(scale, distance);
      horizontal = (cameraCoordinates[2] - scale) >> 4;
      vertical = (diagonal - cameraCoordinates[1]) >> 4;
      horizontalSquared = Iwram_MulQ16(horizontal, horizontal);
      verticalSquared = Iwram_MulQ16(vertical, vertical);
      verticalSquared = Iwram_Sqrt(horizontalSquared + verticalSquared) << 12;
      if (vertical < 0)
        verticalSquared = -verticalSquared;
      scanline->depth = Iwram_MulQ16(verticalSquared, 0x8000);
    } else {
      scanline->scale = 0;
      scanline->depth = 0;
    }
    scanline->offsetX = 0;
    scanline->offsetY = 0;
    scanline++;
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
// ring when it is rebuilt with Start held. Select reloads the far-call
// resources (the alternate set unless B is held) and starts over.
void Ui_RunIconMonitor(void) {
  struct IconColumn layers[4];
  struct AnimationMetadata *spriteMetadata;
  struct AnimationObject *sprites;
  volatile s32 fillWord;
  s32 spriteId;
  s32 x;
  s32 y;
  u32 editMode;
  s32 shadowsEnabled;
  struct SpritePlacement *ring;
  s32 selectedLayer;
  u32 spriteSteps;
  s32 spriteOrFlagsAddress;
  s32 changed;
  /* FAKEMATCH: volatile, as gKeysHeld and gKeysRepeat are declared
   * (keystate.h): each *held and *repeat is a load of its own. A plain held
   * differs 245, 663 instructions for 672; a plain repeat 89, 669 for 672.
   * Using gKeysHeld and gKeysRepeat directly differs 283: the native keeps only
   * these two addresses in fp and r9 and reloads every other key address from
   * the pool. */
  volatile u32 *heldKeys;
  volatile u32 *repeatedKeys;
  u32 i;

  x = 144;
  y = 96;
  editMode = 1;
  shadowsEnabled = 1;
  selectedLayer = 0;
  ring = Runtime_AllocateBlock(9, 10 * sizeof(*ring));
  gDecodeFillByte = 3;

  Dma_Fill32Cell(fillWord, 0, ring, 4);
  Dma_Fill32Cell(fillWord, 0, layers, sizeof(layers));

  spriteId = Ui_FindNextNumberWithMetadata(-1, 1);
  for (i = 0; i <= 3; i++) {
    layers[i].animation = 1;
    layers[i].priority = 1;
    layers[i].number = spriteId;
  }
  layers[0].on = 1;
  gDebugMode = 2;
  REG_BLDCNT = 0x3F42;
  PLTT_BACKDROP = 0x01E0;
  REG_DISPCNT = 0x1140;
  Fade_FromBlack(1);

restart:
  Runtime_InitializeHeap();
  Scheduler_ResetTaskTable();
  ring = Runtime_AllocateBlock(9, 10 * sizeof(*ring));
  VramCache_Reset();
  ObjectSystem_Initialize(2);
  ResourceSlot_Load(0, (u32 *)Ram_MapCellBuffer, layers[0].number, 0);
  spriteMetadata = Resource_GetMetadataRecordFar(layers[0].number);
  if (spriteMetadata->drawKind == 20) {
    ResourceSlot_Load(1, (u32 *)Ram_ActorSpriteSlots, layers[0].number + 1, 0);
  }

  for (i = 0; i <= 9; i++) {
    spriteMetadata = Resource_GetMetadataRecordFar(layers[0].number);
    spriteOrFlagsAddress = 0;
    if (spriteMetadata->drawKind == 20 && (i & 1) != 0) {
      spriteOrFlagsAddress = 1;
    }
    spriteOrFlagsAddress = (s32)ResourceObject_Create(
        layers[0].number + spriteOrFlagsAddress + (spriteOrFlagsAddress << 12));
    ResourceMetadata_Register((struct AnimationObject *)spriteOrFlagsAddress,
                              layers[1].number);
    ResourceMetadata_Register((struct AnimationObject *)spriteOrFlagsAddress,
                              layers[2].number);
    ResourceMetadata_Register((struct AnimationObject *)spriteOrFlagsAddress,
                              layers[3].number);
    /* FAKEMATCH: needed to match: the native adds the flags offset to the
     * object's own register (r5) in place; a separate address takes a copy.
     * Measured 2026-10-13: ((struct AnimationObject *)alt)->flags = flag, the
     * same through a pointer local, or a byte offset from alt all differ 55; a
     * separate object variable 128 (the index and the object share r5). */
    spriteOrFlagsAddress =
        (s32)&((struct AnimationObject *)spriteOrFlagsAddress)->flags;
    *(u8 *)spriteOrFlagsAddress = shadowsEnabled;
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
  heldKeys = &gKeysHeld;
  repeatedKeys = &gKeysRepeat;
  for (;;) {

    if ((*heldKeys & KEY_START) != 0) {
      if ((*heldKeys & KEY_LEFT) != 0) {
        x--;
      }
      if ((*heldKeys & KEY_RIGHT) != 0) {
        x++;
      }
      if ((*heldKeys & KEY_UP) != 0) {
        y--;
      }
      if ((*heldKeys & KEY_DOWN) != 0) {
        y++;
      }
      Map_BuildProbeRing(x, y, ring);
    } else {
      (void)gKeysRepeat;
      if ((*repeatedKeys & KEY_UP) != 0) {
        selectedLayer = (selectedLayer - 1) & 3;
      }
      if ((gKeysRepeat & KEY_DOWN) != 0) {
        selectedLayer = (selectedLayer + 1) & 3;
      }
      if ((gKeysRepeat & KEY_LEFT) != 0) {
        editMode = (editMode - 1) & 3;
      }
      if ((gKeysRepeat & KEY_RIGHT) != 0) {
        editMode++;
        editMode &= 3;
      }
    }

    if ((gKeysPressed & KEY_START) != 0) {
      sprites = gSpriteObjects;
      shadowsEnabled ^= 1;
      for (i = 0; i <= 9; i++) {
        sprites->flags = shadowsEnabled;
        sprites++;
      }
    }

    switch (editMode) {
    case 0:
      if (selectedLayer == 1) {
        break;
      }
      if ((gKeysRepeat & KEY_L) != 0) {
        layers[selectedLayer].animation--;
        if (layers[selectedLayer].animation < 0) {
          layers[selectedLayer].animation = 0;
        }
        if (selectedLayer == 0) {
          layers[1].animation = layers[0].animation;
        }
        goto next;
      }
      if ((gKeysRepeat & KEY_R) == 0) {
        break;
      }
      layers[selectedLayer].animation++;
      if (layers[selectedLayer].animation > 99) {
        layers[selectedLayer].animation = 99;
      }
      if (selectedLayer == 0) {
        layers[1].animation = layers[0].animation;
      }
      goto next;

    case 1:
      changed = 0;
      spriteSteps = 1;
      if ((*heldKeys & KEY_START) != 0) {
        spriteSteps = 10;
      }
      if ((*repeatedKeys & KEY_L) != 0) {
        for (i = 0; i < spriteSteps; i++) {
          layers[selectedLayer].number =
              Ui_FindNextNumberWithMetadata(layers[selectedLayer].number, -1);
        }
        changed = 1;
      }
      if ((*repeatedKeys & KEY_R) != 0) {
        for (i = 0; i < spriteSteps; i++) {
          layers[selectedLayer].number =
              Ui_FindNextNumberWithMetadata(layers[selectedLayer].number, 1);
        }
        changed = 1;
      }
      if (changed == 0) {
        break;
      }
      if (selectedLayer == 0) {
        goto restart;
      }
      if (layers[selectedLayer].on != 0) {
        Ui_SetGridColumnNumber(selectedLayer, layers[selectedLayer].number);
        Ui_SetGridColumnPriority(selectedLayer, layers[selectedLayer].priority);
        Ui_SetGridColumnParameter(selectedLayer, layers[selectedLayer].param);
        Ui_FillGridColumnFromMetadata(selectedLayer,
                                      layers[selectedLayer].animation);
      }
      goto next;

    case 2:
      changed = 0;
      if ((*repeatedKeys & KEY_L) != 0) {
        layers[selectedLayer].priority--;
        if (layers[selectedLayer].priority < 0) {
          layers[selectedLayer].priority = 3;
        }
        changed = 1;
      }
      if ((*repeatedKeys & KEY_R) != 0) {
        layers[selectedLayer].priority++;
        if (layers[selectedLayer].priority > 3) {
          layers[selectedLayer].priority = 0;
        }
        changed = 1;
      }
      if (changed == 0) {
        break;
      }
      if (layers[selectedLayer].on != 0) {
        Ui_SetGridColumnPriority(selectedLayer, layers[selectedLayer].priority);
      }
      goto next;

    case 3:
      changed = 0;
      if ((*repeatedKeys & KEY_L) != 0) {
        layers[selectedLayer].param--;
        if (layers[selectedLayer].param < 0) {
          layers[selectedLayer].param = 15;
        }
        changed = 1;
      }
      if ((*repeatedKeys & KEY_R) != 0) {
        layers[selectedLayer].param++;
        if (layers[selectedLayer].param > 15) {
          layers[selectedLayer].param = 0;
        }
        changed = 1;
      }
      if (changed == 0) {
        break;
      }
      if (layers[selectedLayer].on != 0) {
        Ui_SetGridColumnParameter(selectedLayer, layers[selectedLayer].param);
      }
      goto next;

    default:
      break;
    }

    if ((*repeatedKeys & KEY_A) != 0) {
      if (editMode == 0) {
        if (selectedLayer != 1) {
          if (layers[selectedLayer].on != 0) {
            Ui_FillGridColumnFromMetadata(selectedLayer,
                                          layers[selectedLayer].animation);
            if (selectedLayer == 0) {
              Ui_FillGridColumnFromMetadata(1, layers[1].animation);
            }
          }
          goto next;
        }
      } else if (selectedLayer != 0) {
        layers[selectedLayer].on ^= 1;
        Ui_SetGridColumnPriority(
            selectedLayer,
            layers[selectedLayer].on != 0 ? layers[selectedLayer].priority : 8);
        goto next;
      }
    }

    if ((*repeatedKeys & KEY_SELECT) != 0) {
      Scheduler_ResetTaskTable();
      if ((*heldKeys & KEY_B) != 0) {
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

// Finds the next sprite with an image, stepping in direction (1 or -1)
// and wrapping within 0-511, that has an image.
s32 Ui_FindNextNumberWithMetadata(s32 spriteId, s32 direction) {
  for (;;) {
    spriteId += direction;
    if (spriteId < 0) {
      spriteId = 0x200;
      continue;
    }
    {
      /* FAKEMATCH: the upper bound in a local compares as cmp against 0x200
       * with blt; the literal compares against 0x1ff from the pool. 2026-10-13:
       * start >= 0x200 or 0x200 <= start differs 9 of 25 (ldr r3, =0x1ff and
       * ble); the tests as one else-if chain 9, start < 0x200 guarding the
       * lookup 13. */
      s32 spriteLimit = 0x200;
      if (spriteId >= spriteLimit) {
        spriteId = -1;
        continue;
      }
    }
    if (Resource_GetMetadataRecordFar(spriteId)->width) {
      return spriteId;
    }
  }
}

// Lays out the sprite viewer's ring around pixel (x, z): one position per
// facing of the first sprite's facing kind (6, 8, 10 or 4, or two opposite
// pairs for kind 20), 56 pixels out (40 for kind 20). B held turns the ring.
void Map_BuildProbeRing(s32 centerX, s32 centerY,
                        struct SpritePlacement *positions) {
  struct SpritePlacement *pairedPositions;
  s32 angle;
  s32 facingIndex;
  u16 initialHeading;
  u8 facingKind;

  angle = 0;
  // Select the viewer sprite's formation; headings wrap as 16-bit turns.
  facingKind = gSpriteObjects->entries[0]->kind;
  if ((gKeysHeld & KEY_B) != 0)
    angle = (s16)(gFrameTick << 8);
  switch ((u32)facingKind) {
  case 3:
    for (facingIndex = 0; facingIndex < 6; facingIndex++) {
      angle = (u32)angle << 16;
      positions->x = centerX << 16;
      angle = (u32)angle >> 16;
      positions->height = 0;
      positions->y = centerY << 16;
      Vector_AddPolarOffset(0x380000, (u16)angle,
                            SpritePlacement_Values(positions));
      positions++;
      angle = (s16)(angle + 0x2aaa);
    }
    for (; facingIndex < 10; facingIndex++)
      positions++;
    break;
  case 5:
  case 8:
  case 44:
  case 88:
    for (facingIndex = 0; facingIndex < 8; facingIndex++) {
      angle = (u32)angle << 16;
      positions->x = centerX << 16;
      angle = (u32)angle >> 16;
      positions->height = 0;
      positions->y = centerY << 16;
      Vector_AddPolarOffset(0x380000, (u16)angle,
                            SpritePlacement_Values(positions));
      positions++;
      angle = (s16)(angle + 0x2000);
    }
    for (; facingIndex < 10; facingIndex++)
      positions++;
    break;
  case 4:
  case 6:
    for (facingIndex = 0; facingIndex < 10; facingIndex++) {
      angle = (u32)angle << 16;
      positions->x = centerX << 16;
      angle = (u32)angle >> 16;
      positions->height = 0;
      positions->y = centerY << 16;
      Vector_AddPolarOffset(0x380000, (u16)angle,
                            SpritePlacement_Values(positions));
      positions++;
      angle = (s16)(angle + 0x1999);
    }
    break;
  case 20:
    initialHeading = angle;
    angle = (s16)(initialHeading + 0x4000);
    facingIndex = 0;
    pairedPositions = positions;
    while (facingIndex < 2) {
      pairedPositions[facingIndex * 2].x = centerX << 16;
      pairedPositions[facingIndex * 2].height = 0;
      pairedPositions[facingIndex * 2].y = centerY << 16;
      angle = (u32)angle << 16;
      angle = (u32)angle >> 16;
      Vector_AddPolarOffset(0x280000, angle,
                            SpritePlacement_Values(&positions[0]));
      pairedPositions[facingIndex * 2 + 1].x = centerX << 16;
      {
        s32 *point = SpritePlacement_Values(&positions[1]);
        pairedPositions[facingIndex * 2 + 1].height = 0;
        pairedPositions[facingIndex * 2 + 1].y = centerY << 16;
        Vector_AddPolarOffset(0x280000, (u16)angle, point);
      }
      angle = ((u32)angle + 0x8000) << 16;
      facingIndex++;
      positions += 2;
      angle >>= 16;
    }
    break;
  default:
    angle = (s32)(((u32)angle << 16) + 0x20000000) >> 16;
    for (facingIndex = 0; facingIndex < 4; facingIndex++) {
      angle = (u32)angle << 16;
      angle = (u32)angle >> 16;
      positions->x = centerX << 16;
      positions->height = 0;
      positions->y = centerY << 16;
      Vector_AddPolarOffset(0x380000, (u16)angle,
                            SpritePlacement_Values(positions));
      positions++;
      angle = (s16)(angle + 0x4000);
    }
    for (facingIndex = 4; facingIndex < 10; facingIndex++)
      positions++;
    break;
  }
}

// Defines name(slot, value), which sets field of entry slot (0-3) in each of
// the first ten sprites to value.
#define DEFINE_SET_GRID_COLUMN(name, field)                                    \
  void name(s32 layerIndex, s32 value) {                                       \
    struct AnimationObject *spriteGroup = gSpriteObjects;                      \
    s32 spriteIndex;                                                           \
                                                                               \
    layerIndex &= 3;                                                           \
    for (spriteIndex = 0; spriteIndex < 10; spriteIndex++) {                   \
      spriteGroup->entries[layerIndex]->field = value;                         \
      spriteGroup++;                                                           \
    }                                                                          \
  }

// Set the draw parameter or the priority of one layer of the viewer's ten
// sprites.
DEFINE_SET_GRID_COLUMN(Ui_SetGridColumnParameter, param)
DEFINE_SET_GRID_COLUMN(Ui_SetGridColumnPriority, priority)

// Starts animationIndex on layerIndex of each of the viewer's ten sprites,
// each one 16 timer units behind the last.
void Ui_FillGridColumnFromMetadata(s32 layerIndex, s32 animationIndex) {
  s32 spriteIndex;
  struct AnimationObject *spriteGroup;

  spriteGroup = gSpriteObjects;
  spriteIndex = 0;
  do {
    struct AnimationEntry *entry = spriteGroup->entries[layerIndex & 3];
    struct AnimationMetadata *metadata;
    s32 initialFrame = 0;

    if (entry->scripts != 0) {
      metadata = Resource_GetMetadataRecordFar(entry->animId);
      if (animationIndex < metadata->animationCount) {
        entry->kind = metadata->drawKind;
        entry->script = entry->scripts[animationIndex];
        entry->timer = spriteIndex * 0x10;
        entry->step = 0x10;
        entry->pos = initialFrame;
        entry->frameBase = initialFrame;
        entry->frame = 0xff;
      }
      spriteGroup->offsetY = metadata->adjustY;
      spriteGroup->rotation = initialFrame;
    }
    spriteIndex++;
    spriteGroup++;
  } while (spriteIndex <= 9);
}

// Sets spriteId on layerIndex of each of the viewer's ten sprites.
void Ui_SetGridColumnNumber(s32 layerIndex, s32 spriteId) {
  struct AnimationObject *spriteGroup = gSpriteObjects;
  s32 spriteIndex;

  Resource_GetMetadataRecordFar(spriteId);
  layerIndex &= 3;
  for (spriteIndex = 0; spriteIndex < 10; spriteIndex++) {
    struct AnimationEntry *entry = spriteGroup->entries[layerIndex];

    entry->animId = spriteId;
    Animation_InitWorkFromMetadata(entry);
    spriteGroup++;
  }
}

// Each frame in the sprite viewer: places each copy of the sprite at its
// ring position, facing the way that position stands for.
void Battle_PlaceActorsByFormationKind(void) {
  struct AnimationObject *sprite = WORK_SLOT(4);
  u32 facingKind = sprite->entries[0]->kind;
  struct SpriteScale scale;
  struct SpritePlacement *facingPositions;
  u16 angle;
  u16 step;
  u16 pairedFacingStep = 0;
  u32 count;
  u32 spriteIndex;

  scale = gBattle_FormationPlacementScale;
  facingPositions = WORK_SLOT(HEAP_SLOT_BATTLE);

  switch (facingKind) {
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
    pairedFacingStep = 0x8000;
    count = 4;
    break;
  default:
    angle = 0x2000;
    step = 0x4000;
    count = 4;
    break;
  }

  for (spriteIndex = 0; spriteIndex < count; spriteIndex++) {
    Render_ApplyProjectedPlacement(sprite, &facingPositions[spriteIndex],
                                   &scale, angle);
    sprite++;
    angle += step;
    if (spriteIndex & 1)
      angle += pairedFacingStep;
  }
}
