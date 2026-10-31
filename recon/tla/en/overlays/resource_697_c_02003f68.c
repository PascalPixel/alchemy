// Near miss, differ 318 of 252 (tla-en, 2026-10-31): the calls, loops and
// accesses are the native's; what remains is allocation and three passes.
// GCSE's PRE keeps mover->offset from the inner loop's first test in a
// register (a zero-extended ldrh) for its later tests and the cap, where the
// native rereads it each time; CSE reuses the pseudo holding Iwram_MulQ16's
// constant argument for the cap's - 0x20000 after the call, where the native
// loads 0xfffe0000 again; and the native allocates z, sy, x and height to r5
// to r8 and the object to r10 (mover, viewY and the rest on the stack), where
// here the object takes r5 and viewY r11. The native also computes the cap's
// z - height before Math_Sine and loads the constant argument after it:
// written so (sy = z - height first, the sine in its own statement) the order
// is the native's but caller-saves puts x and height in r4 and r2, 328 to 330.
// Draws the Trial Road movers' columns: for each raised mover (of 16) it
// queues one 16 by 16 sprite per 16 pixels of its offset and a bobbing cap on
// top, animated over 4 frames every 3, projected from the map's view and kept
// to the screen, at most 80 sprites (Func_02005790 queues each three-word
// entry at its depth, as Trial Road's cell sprite does).
#include "../../../../games/tla/src/scene/trial_road/trial_road.h"
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
};

extern struct ResourceTableEntry gResourceTableEntries[];

s32 Math_Sine(s32 angle);
void Func_02005790(void *entry, s32 priority);

void Func_02003f68(void) {
  u8 *base = gData_020023c4 + 0x188;
  struct TrialRoadMover *mover = (struct TrialRoadMover *)(base + 4);
  s32 tile = gResourceTableEntries[*(s16 *)base].offset >> 5;
  u32 *entry = *(u32 **)(base + 452);
  s32 count = 0;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
  s32 viewY = map->origin[1];
  struct Actor *object;
  u32 *cursor;
  s32 frame;
  s32 lift;
  s32 x;
  s32 height;
  s32 z;
  s32 sx;
  s32 sy;
  s32 depth;
  s32 i;

  for (i = 15; i >= 0; i--, mover++) {
    if (mover->id == 0 || mover->offset == 0)
      continue;
    object = Actor_GetById(mover->id);
    frame = ((gFrameCount % 12) / 3) * 8 + 32;
    // A loop of labels: as a for loop, loop.c hoists (tile + frame) | 0x800
    // out of it, which the native computes on every pass (differ 346).
    lift = 0;
    if (lift < mover->offset) {
    sprite:
      if (count > 79)
        goto next;
      x = object->x - viewX;
      height = object->y + (lift << 16) - viewY;
      z = object->z - viewZ - viewY;
      sy = z - height;
      sy = (sy >> 16) - 16;
      depth = ((height + z) >> 16) + 58;
      sx = (x >> 16) - 8;
      if ((u32)(sx + 15) > 334 || sy <= -16 || sy > 239)
        goto next;
      cursor = entry;
      *cursor++ = 0;
      *cursor++ = (sy & 0xff) | ((sx & 0x1ff) << 16) | 0x40002000;
      *cursor = (tile + frame) | 0x800;
      Func_02005790(entry, depth);
      count++;
      entry += 3;
    next:
      lift += 16;
      if (lift < mover->offset)
        goto sprite;
    }
    if (count > 79)
      continue;
    x = object->x - viewX;
    height = object->y + (mover->offset << 16) - viewY;
    z = object->z - viewZ - viewY;
    sy = ((z - height + Iwram_MulQ16(0xfffe0000, Math_Sine(mover->bob)) -
           0x20000) >>
          16) -
         4;
    depth = ((height + z) >> 16) + 58;
    sx = (x >> 16) - 8;
    if ((u32)(sx + 15) > 334 || sy <= -16 || sy > 239)
      continue;
    cursor = entry;
    *cursor++ = 0;
    *cursor++ = (sy & 0xff) | ((sx & 0x1ff) << 16) | 0x40002000;
    *cursor = (tile + frame - 32) | 0x800;
    Func_02005790(entry, depth);
    count++;
    entry += 3;
  }
}
