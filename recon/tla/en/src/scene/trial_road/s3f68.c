/// @file
/// Queues the Trial Road movers' sprites.
#include "../../../../../../games/tla/src/scene/trial_road/trial_road.h"
#include "iwram_call.h"
#include "frame.h"
#include "map.h"

struct ResourceTableEntry {
  u16 unk0;
  u16 offset;
};

struct TrialRoadMover {
  s16 id;
  s16 phase;
  s16 offset;
  s16 unk06[8];
  s16 bob;
  struct Actor *marker;
};

struct TrialRoadMovers {
  s16 vramEntry;
  s16 shape;
  struct TrialRoadMover movers[16];
  u32 *entries;
};

extern struct ResourceTableEntry gResourceTableEntries[];

s32 Math_Sine(s32 angle);
void Func_02005790(void *entry, s32 priority);

void Func_02003f68(void) {
  struct TrialRoadMovers *base =
      (struct TrialRoadMovers *)(gData_020023c4 + 0x188);
  struct TrialRoadMover *mover = base->movers;
  s32 i;
  s32 tile;
  s32 frame;
  s32 count;
  s32 viewX;
  s32 viewZ;
  s32 viewY;
  s32 rise;
  struct MapWork *map;
  s32 *view;
  struct Actor *object;
  u32 *entry;
  u32 *cursor;
  s32 lift;
  s32 x;
  s32 height;
  s32 z;
  s32 sy;
  s32 depth;
  s32 bob;

  tile = gResourceTableEntries[base->vramEntry].offset >> 5;
  entry = base->entries;
  count = 0;
  rise = -0x20000;
  map = Ram_HeapSlots->mapWork;
  view = &map->viewX;
  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  viewY = map->origin[1];

  for (i = 0; i < 16; i++, mover++) {
    if (mover->id == 0 || mover->offset == 0)
      continue;
    object = Actor_GetById(mover->id);
    frame = ((gFrameCount % 12) / 3) * 8 + 32;
    lift = 0;
    if (lift < mover->offset) {
    next:
      if (count > 79)
        goto skip;
      x = object->x - viewX;
      height = object->y + (lift << 16) - viewY;
      z = object->z - viewZ - viewY;
      x = (x >> 16) - 8;
      sy = z - height;
      sy = (sy >> 16) - 16;
      depth = ((height + z) >> 16) + 58;
      if ((u32)(x + 15) > 334 || sy <= -16 || sy > 239)
        goto skip;
      x &= 0x1ff;
      sy &= 0xff;
      cursor = entry;
      *cursor++ = 0;
      *cursor++ = sy | (x << 16) | 0x40002000;
      *cursor = (tile + frame) | 0x800;
      Func_02005790(entry, depth);
      entry += 3;
      count++;
    skip:
      lift += 16;
      if (lift < mover->offset)
        goto next;
    }
    if (count > 79)
      continue;
    x = object->x - viewX;
    height = object->y + (mover->offset << 16) - viewY;
    z = object->z - viewZ - viewY;
    x = (x >> 16) - 8;
    sy = z - height;
    bob = Iwram_MulQ16(rise, Math_Sine(mover->bob));
    sy = ((sy + bob + rise) >> 16) - 4;
    depth = ((height + z) >> 16) + 58;
    if ((u32)(x + 15) > 334 || sy <= -16 || sy > 239)
      continue;
    x &= SPLIT_CONSTANT(0x100, 255);
    sy &= 0xff;
    cursor = entry;
    *cursor++ = 0;
    *cursor++ = sy | (x << 16) | 0x40002000;
    *cursor = (tile + frame - 32) | 0x800;
    Func_02005790(entry, depth);
    entry += 3;
    count++;
  }
}
