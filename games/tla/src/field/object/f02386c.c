#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "dma.h"
#include "map.h"
#include "heap_release.h"
#include "iwram_call.h"
#include "actor.h"
#include "field/sprite.h"
#include "field/field.h"
#include "graphics/resource.h"

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

struct MapPriorityWork {
  u8 unk000[0x101];
  s8 basePriority;
};

// The decoder's block: its code, tables and work area (ld_script.ld). Only
// the code and tables are copied.
extern u8 Render_DecodePictureCode[];
extern u8 Render_DecodePictureCopyEnd[];
extern u8 Render_DecodePictureBlockSize[];

s32 Func_0802d45c(s32 terrainId, s32 x, s32 z);
void Func_08022318(struct PlacedSprite *sprite, struct SpritePlacement *point,
                   s32 *scale, s32 facing, s32 depth);
void Func_08021a84(struct PlacedSprite *sprite, s32 facing);

#define BASE_PRIORITY(map) (((struct MapPriorityWork *)(map))->basePriority)
// The priority field (bits 15..14) of a cell's word.
#define CELL_PRIORITY(cells, x, z)                                             \
  (((cells)[(((z) >> 20) << 7) + ((x) >> 20)] << 16) >> 30)
// The layer field (bits 13..12) of a cell's word: the layer plus one.
#define CELL_LAYER(cells, x, z)                                                \
  (((cells)[(((z) >> 20) << 7) + ((x) >> 20)] << 18) >> 30)

void Func_0802386c(void) {
  // Unused: gives the stack frame its 36 bytes, as The Broken Seal's.
  struct CameraPoint unusedPoints[3];
  struct SpritePlacement point;
  struct SpriteScale scale;
  s32 objectIndex;
  struct MapWork *map;
  s32 *view;
  s32 viewX;
  s32 viewZ;
  struct ObjectSystemWork *system;
  struct Actor *object;
  u32 kind;
  s32 dx;
  s32 dz;
  s32 originY;
  struct PlacedSprite *sprite;
  s32 screenTop;
  u8 *priorityFlags;
  s32 layer;
  u8 *terrainId;
  u32 *cells;
  s32 feetZ;
  s32 objectHeight;
  s32 probeX;
  s32 priority;
  s32 leftPriority;
  s32 middlePriority;
  s32 rightPriority;
  s32 height;
  s32 rise;
  u32 layerPlusOne;
  s8 *basePriority;

  map = Ram_HeapSlots->mapWork;
  view = &map->viewX;
  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  system = Ram_HeapSlots->objectSystem;
  originY = map->origin[1];
  // The decoder's copy in a once-only block, as in
  // ObjectSystem_UpdateCameraFixed, keeps the first call's arguments after the
  // followed point's height; as a plain call, 6 of 606 instructions differ in
  // tla-en.
  do {
    Dma_Set(Render_DecodePictureCode,
            HeapSlot_Alloc(80, (s32)Render_DecodePictureBlockSize),
            DMA_COPY_WORDS((u32)(Render_DecodePictureCopyEnd -
                                 Render_DecodePictureCode) >>
                           2),
            REG_DMA3);
  } while (0);
  {
    // The zero is set before the objects are read.
    u32 zero = 0;

    object = Ram_HeapSlots->scriptObjects;
    system->used = zero;
  }
  for (objectIndex = 0; objectIndex < 64; objectIndex++, object++) {
    if (object->script == NULL)
      continue;
    if (object->x != 0 || object->z != 0) {
      kind = object->animationKind & 15;
      if (kind == 0)
        continue;
      if (kind != 1)
        continue;
      if ((u16)system->suspended != 0 && !(object->unk5c[0] & 1)) {
        struct PlacedSprite *frozen = object->animation;

        VramCache_ReleaseVram(frozen->slot);
        frozen->dirty = kind;
        continue;
      }
      sprite = object->animation;
      dx = object->x - viewX;
      dz = object->z - viewZ;
      screenTop = dz - object->y;
      if (dx > -0x200000 && dx < 0x1100000 && screenTop > -0x200000 &&
          screenTop < 0xe00000 &&
          (priorityFlags = &object->priorityFlags, !(*priorityFlags & 16))) {
        terrainId = &object->terrainId;
        layer = *terrainId & 3;
        cells = (u32 *)map->layers[layer].cells;
        if (*priorityFlags & 1) {
          probeX = object->x - 0x70000;
          feetZ = object->z + 0x20000;
          objectHeight = object->terrainHeight >> 16;
          priority = 0;
          leftPriority = 0;
          middlePriority = 0;
          if (!(*priorityFlags & 32)) {
            /* FAKEMATCH(temporary): the base priority read through a
             * function-scope pointer, which gives the two spills at sp+12 and
             * sp+16 their order; as priority = BASE_PRIORITY(map), 8 of 606
             * instructions differ in tla-en. */
            basePriority = &BASE_PRIORITY(map);
            priority = *basePriority;
            if (Func_0802d45c(layer, probeX, feetZ) == object->terrainHeight) {
              leftPriority = CELL_PRIORITY(cells, probeX, feetZ);
              height = Func_0802d45c(layer, probeX, feetZ + 0x100000) >> 16;
              rise = (Func_0802d45c(layer, probeX, feetZ + 0x200000) >> 16) - 16;
              if (rise > height)
                height = rise;
              if (height > objectHeight)
                priority = BASE_PRIORITY(map) + 1;
            }
            probeX += 0xd0000;
            if (Func_0802d45c(layer, probeX, feetZ) == object->terrainHeight) {
              middlePriority = CELL_PRIORITY(cells, probeX, feetZ);
              height = Func_0802d45c(layer, probeX, feetZ + 0x100000) >> 16;
              rise = (Func_0802d45c(layer, probeX, feetZ + 0x200000) >> 16) - 16;
              if (rise > height)
                height = rise;
              if (height > objectHeight)
                priority = BASE_PRIORITY(map) + 1;
            }
            probeX -= 0x60000;
            rightPriority = CELL_PRIORITY(cells, probeX, feetZ);
            height = Func_0802d45c(layer, probeX, feetZ + 0x100000) >> 16;
            rise = (Func_0802d45c(layer, probeX, feetZ + 0x200000) >> 16) - 16;
            if (rise > height)
              height = rise;
            if (height > objectHeight)
              priority = BASE_PRIORITY(map) + 1;
          } else {
            if (object->terrainHeight == Func_0802d45c(layer, probeX, feetZ))
              leftPriority = CELL_PRIORITY(cells, probeX, feetZ);
            probeX += 0xd0000;
            if (object->terrainHeight == Func_0802d45c(layer, probeX, feetZ))
              middlePriority = CELL_PRIORITY(cells, probeX, feetZ);
            probeX -= 0x60000;
            rightPriority = CELL_PRIORITY(cells, probeX, feetZ);
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
          if (priority != 0 && object->animationKind == 1) {
            struct PlacedSprite *placed = object->animation;

            if ((u32)priority > 3)
              priority = 3;
            placed->priority = priority;
            placed->shadowPriority = priority;
          }
        }
        layerPlusOne = CELL_LAYER(cells, object->x, object->z);
        if (layerPlusOne != 0)
          *terrainId = layerPlusOne - 1;
        scale.x = Iwram_MulQ16(object->scaleX, sprite->scale);
        scale.y = Iwram_MulQ16(object->scaleY, sprite->scale);
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
          Func_08022318(sprite, &point, scale.values, object->angle,
                        ((u8 *)object)[0x63]);
        else
          Render_ApplyProjectedPlacement(sprite, SpritePlacement_Values(&point),
                                         scale.values, object->angle);
        continue;
      }
      if (object->unk5c[0] & 2) {
        Func_08021a84(sprite, object->angle);
      } else if (!(object->unk5c[0] & 1) && !(sprite->retainCacheEntry & 1)) {
        VramCache_ReleaseVram(sprite->slot);
        sprite->dirty = 1;
      }
    } else {
      // At (0, 0): not placed; only the sprite's VRAM is given up.
      kind = object->animationKind & 15;
      if (kind == 1) {
        struct PlacedSprite *frozen = object->animation;

        if (object->unk5c[0] == 0 && !(frozen->retainCacheEntry & 1)) {
          VramCache_ReleaseVram(frozen->slot);
          frozen->dirty = kind;
        }
      }
    }
  }
  HeapSlot_Release(80);
}
