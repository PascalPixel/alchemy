// Draft, differ 660 of 606: The Broken Seal's ObjectSystem_PlaceSprites
// (games/tbs/src/field/field/object_camera.c), the scrolling camera that
// places every object's sprite each frame (Func_080230e0 adds it at 0x480).
// This game's changes: priorityFlags bit 4 hides an object as if off
// screen; with automatic priority (bit 0) the priority comes from three
// cells under the object's feet (7 pixels left, 6 right and centre; the two
// sides only where the ground there is the object's own height), an odd one
// preferred, and the map's base priority plus one when ground 16 or 32
// pixels south rises above the object (bit 5 skips the heights); heights
// are taken relative to the followed point's; bit 3 places through
// Func_08022318 with the object's byte 0x63, and an object off screen with
// byte 0x5c bit 1 set is placed by Func_08021a84 instead of releasing its
// VRAM. Left: register allocation from the loop on (the native keeps the
// object in r9 and its terrainId and animationKind addresses on the stack;
// here the object's x address takes r9), so the alignment is poor though the
// structure follows the listing.
#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "dma.h"
#include "map.h"
#include "mem.h"
#include "iwram_call.h"
#include "object_runtime.h"
#include "field/animspr.h"

struct PlacedSprite {
  u8 unk00[9];
  u8 low09 : 2;
  u8 priority : 2;
  u8 high09 : 4;
  u8 unk0a[2];
  s32 scale; ///< 16.16 fixed point
  u8 slot;
  u8 retainCacheEntry; ///< bit 0
  u8 unk12[7];
  u8 dirty;
  u8 unk1a[0x0b];
  u8 low25 : 2;
  u8 shadowPriority : 2;
  u8 high25 : 4;
};

struct CameraPoint {
  s32 x;
  s32 y;
  s32 z;
};

struct PlacementPoint {
  s32 x;
  s32 height;
  s32 y;
  s32 groundHeight;
};

struct MapPriorityWork {
  u8 unk000[0x101];
  s8 basePriority;
};

// The decoder's block: its code, tables and work area (constants.ld). Only
// the code and tables are copied.
extern u8 Render_DecodePictureCode[];
extern u8 Render_DecodePictureCopyEnd[];
extern u8 Render_DecodePictureBlockSize[];

s32 Func_0802d45c(s32 terrainId, s32 x, s32 z);
void VramCache_ReleaseVram(s32 slot);
void Render_ApplyProjectedPlacement(struct PlacedSprite *sprite,
                                    struct PlacementPoint *point, s32 *scale,
                                    s32 facing);
void Func_08022318(struct PlacedSprite *sprite, struct PlacementPoint *point,
                   s32 *scale, s32 facing, s32 depth);
void Func_08021a84(struct PlacedSprite *sprite, s32 facing);

// The priority field (bits 15..14) of a cell's word.
#define CELL_PRIORITY(cells, x, row)                                           \
  ((((u32 *)(cells))[((row) << 7) + ((x) >> 20)] << 16) >> 30)
// The higher of the ground 16 pixels south and 16 below the ground 32 south,
// in whole pixels.
#define SOUTH_RISE(layer, x, z, h)                                             \
  ((h) = Func_0802d45c(layer, x, (z) + 0x100000) >> 16,                        \
   (rise = (Func_0802d45c(layer, x, (z) + 0x200000) >> 16) - 16) > (h)         \
       ? (h) = rise                                                            \
       : 0)

void Func_0802386c(void) {
  /* An unused local for the room the native stack frame keeps above the
   * placement, as The Broken Seal's. */
  struct CameraPoint unusedPoints[3];
  struct PlacementPoint point;
  s32 scale[2];
  s32 objectIndex;
  struct MapWork *map;
  s32 viewX;
  s32 viewZ;
  struct ObjectSystemWork *system;
  s32 dx;
  s32 dz;
  s32 originY;
  struct PlacedSprite *sprite;
  u8 *priorityFlags;
  s32 layer;
  u32 *cells;
  s32 objectHeight;
  s32 leftPriority;
  s32 middlePriority;
  s8 *basePriority;
  u8 *terrainId;
  u8 *animationKind;
  struct ObjectRuntime *object;
  s32 rightPriority;
  s32 priority;
  s32 x;
  s32 z;
  s32 feetZ;
  s32 probeX;
  s32 row;
  s32 height;
  s32 rise;
  u32 layerPlusOne;

  map = Ram_HeapSlots->mapWork;
  viewX = map->viewX & 0xffff0000;
  viewZ = map->viewZ & 0xffff0000;
  system = Ram_HeapSlots->objectSystem;
  originY = map->origin[1];
  Dma_Set(Render_DecodePictureCode,
          HeapSlot_Alloc(80, (s32)Render_DecodePictureBlockSize),
          (DMA_ENABLE | DMA_32BIT) << 16 |
              ((u32)(Render_DecodePictureCopyEnd - Render_DecodePictureCode) >> 2),
          REG_DMA3);
  object = Ram_HeapSlots->scriptObjects;
  system->used = 0;
  terrainId = &object->terrainId;
  animationKind = &object->animationKind;
  for (objectIndex = 63; objectIndex >= 0; objectIndex--, object++,
      terrainId += sizeof(*object), animationKind += sizeof(*object)) {
    u32 kind;

    if (object->script == NULL)
      continue;
    x = object->x;
    if (x == 0 && object->z == 0)
      goto at_origin;
    kind = *animationKind & 15;
    if (kind == 0)
      continue;
    if (kind != 1)
      continue;
    if (system->suspended != 0 && !(animationKind[8] & kind)) {
      sprite = object->animation;
      VramCache_ReleaseVram(sprite->slot);
      sprite->dirty = kind;
      continue;
    }
    sprite = object->animation;
    dx = x - viewX;
    z = object->z;
    dz = z - viewZ;
    {
      s32 screenTop = dz - object->y;

      if (dx <= -0x200000 || dx >= 0x1100000 || screenTop <= -0x200000 ||
          screenTop > 0xdfffff)
        goto off_screen;
    }
    priorityFlags = &object->priorityFlags;
    if (*priorityFlags & 16)
      goto off_screen;
    layer = *terrainId & 3;
    cells = (u32 *)map->layers[layer].cells;
    if (*priorityFlags & 1) {
      feetZ = z + 0x20000;
      objectHeight = ((s16 *)&object->terrainHeight)[1];
      probeX = x - 0x70000;
      priority = 0;
      leftPriority = 0;
      middlePriority = 0;
      if (!(*priorityFlags & 32)) {
        basePriority = &((struct MapPriorityWork *)map)->basePriority;
        priority = *basePriority;
        if (Func_0802d45c(layer, probeX, feetZ) == object->terrainHeight) {
          row = feetZ >> 20;
          leftPriority = CELL_PRIORITY(cells, probeX, row);
          SOUTH_RISE(layer, probeX, feetZ, height);
          if (height > objectHeight)
            priority = *basePriority + 1;
        } else {
          row = feetZ >> 20;
        }
        probeX += 0xd0000;
        if (Func_0802d45c(layer, probeX, feetZ) == object->terrainHeight) {
          middlePriority = CELL_PRIORITY(cells, probeX, row);
          SOUTH_RISE(layer, probeX, feetZ, height);
          if (height > objectHeight)
            priority = *basePriority + 1;
        }
        probeX += 0xfffa0000;
        rightPriority = CELL_PRIORITY(cells, probeX, row);
        SOUTH_RISE(layer, probeX, feetZ, height);
        if (height > objectHeight)
          priority = *basePriority + 1;
      } else {
        if (object->terrainHeight == Func_0802d45c(layer, probeX, feetZ)) {
          row = feetZ >> 20;
          leftPriority = CELL_PRIORITY(cells, probeX, row);
        } else {
          row = feetZ >> 20;
        }
        probeX = x + 0x60000;
        if (object->terrainHeight == Func_0802d45c(layer, probeX, feetZ))
          middlePriority = CELL_PRIORITY(cells, probeX, row);
        rightPriority = CELL_PRIORITY(cells, x, row);
      }
      // An odd priority is preferred, left first.
      if (leftPriority != 0 || middlePriority != 0 || rightPriority != 0) {
        priority = leftPriority;
        if (!(priority & 1)) {
          if (middlePriority != 0)
            priority = middlePriority;
          if (!(priority & 1) && rightPriority != 0)
            priority = rightPriority;
        }
      }
      if (priority != 0 && *animationKind == 1) {
        struct PlacedSprite *placed = object->animation;

        if ((u32)priority > 3)
          priority = 3;
        placed->priority = priority;
        placed->shadowPriority = priority;
      }
      x = object->x;
      z = object->z;
    }
    layerPlusOne = (cells[((z >> 20) << 7) + (x >> 20)] << 18) >> 30;
    if (layerPlusOne != 0)
      *terrainId = layerPlusOne - 1;
    scale[0] = Iwram_MulQ16(object->scaleX, sprite->scale);
    scale[1] = Iwram_MulQ16(object->scaleY, sprite->scale);
    point.x = dx;
    point.height = object->y - originY;
    point.y = dz - originY;
    point.groundHeight = object->terrainHeight - originY;
    if (*priorityFlags & 2) {
      point.height += 0xfec00000;
      point.y += 0xfec00000;
      point.groundHeight += 0xfec00000;
    }
    if (*priorityFlags & 4) {
      point.height += 0x1400000;
      point.y += 0x1400000;
      point.groundHeight += 0x1400000;
    }
    if (*priorityFlags & 8)
      Func_08022318(sprite, &point, scale, object->angle, ((u8 *)object)[0x63]);
    else
      Render_ApplyProjectedPlacement(sprite, &point, scale, object->angle);
    continue;

  off_screen:
    if (animationKind[8] & 2) {
      Func_08021a84(sprite, object->angle);
      continue;
    }
    if (!(animationKind[8] & 1) && !(sprite->retainCacheEntry & 1)) {
      VramCache_ReleaseVram(sprite->slot);
      sprite->dirty = 1;
    }
    continue;

  at_origin:
    // At (0, 0): not placed; only the sprite's VRAM is given up.
    kind = *animationKind & 15;
    if (kind != 1)
      continue;
    sprite = object->animation;
    if (animationKind[8] != 0)
      continue;
    if (sprite->retainCacheEntry & 1)
      continue;
    VramCache_ReleaseVram(sprite->slot);
    sprite->dirty = kind;
  }
  HeapSlot_Release(80);
}
