// Draft, differ 345 of 252: draws the Trial Road movers' columns: for each raised
// mover (of 16) it queues one 16 by 16 sprite per 16 pixels of its offset and
// a bobbing cap on top, animated over 4 frames every 3, projected from the
// map's view and kept to the screen, at most 80 sprites (Func_02005790 queues
// each three-word entry).
#include "../../../../games/tla/src/scene/trial_road/trial_road.h"
#include "iwram_call.h"
#include "frame.h"

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
};

extern struct ResourceTableEntry gResourceTableEntries[];

s32 Math_Sine(s32 angle);
void Func_02005790(u32 *entry);

void Func_02003f68(void) {
  u8 *base = gData_020023c4 + 0x188;
  struct TrialRoadMover *mover = (struct TrialRoadMover *)(base + 4);
  s32 tile = gResourceTableEntries[*(s16 *)base].offset >> 5;
  u32 *entry = *(u32 **)(base + 452);
  s32 count = 0;
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 viewX = *(s32 *)(map + 228) & 0xffff0000;
  s32 viewZ = *(s32 *)(map + 232) & 0xffff0000;
  s32 cameraY = (*(s32 **)map)[1];
  struct Actor *object;
  s32 frame;
  s32 height;
  s32 dx;
  s32 dy;
  s32 dz;
  s32 sx;
  s32 sy;
  s32 i;

  for (i = 15; i >= 0; i--, mover++) {
    if (mover->id == 0 || mover->offset == 0)
      continue;
    object = Actor_GetById(mover->id);
    frame = ((gFrameCount % 12) / 3) * 8 + 32;
    for (height = 0; height < mover->offset; height += 16) {
      if (count > 79)
        continue;
      dx = object->x - viewX;
      dy = object->y + (height << 16) - cameraY;
      dz = object->z - viewZ - cameraY;
      sx = (dx >> 16) - 8;
      sy = ((dz - dy) >> 16) - 16;
      if ((u32)((dx >> 16) + 7) > 334 || sy <= -16 || sy > 239)
        continue;
      entry[0] = 0;
      entry[1] = (sy & 255) | ((sx & 511) << 16) | 0x40002000;
      entry[2] = (tile + frame) | 0x800;
      Func_02005790(entry);
      count++;
      entry += 3;
    }
    if (count > 79)
      continue;
    dx = object->x - viewX;
    dy = object->y + (mover->offset << 16) - cameraY;
    dz = object->z - viewZ - cameraY;
    sx = (dx >> 16) - 8;
    sy = ((dz - dy + Iwram_MulQ16(0xfffe0000, Math_Sine(mover->bob)) -
           0x20000) >>
          16) -
         4;
    if ((u32)((dx >> 16) + 7) > 334 || sy <= -16 || sy > 239)
      continue;
    entry[0] = 0;
    entry[1] = (sy & 255) | ((sx & 511) << 16) | 0x40002000;
    entry[2] = (tile + frame - 32) | 0x800;
    Func_02005790(entry);
    count++;
    entry += 3;
  }
}
