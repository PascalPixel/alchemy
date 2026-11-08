// Matches (tla-en, 2026-11-08, -mtune=gba). 8 November, 14 to 0: a mover at offset 0 clears its
// cell attribute byte (the native stores 0 there when the offset is 0, which the older draft
// left out); the collision column is reached through Ram_MapCollision, Camelot's fixed address
// (12), and set with i just before the loop (2). The for step advances collision before i.
#include "../../../../games/tla/src/scene/trial_road/trial_road.h"
#include "iwram_call.h"
#include "ram_buffer.h"

struct TrialRoadMover {
  s16 id;
  s16 phase;
  s16 offset;
  s16 period;
  s16 upAt;
  s16 downAt;
  s16 high;
  s16 low;
  s16 unk10;
  s16 speed;
  s16 unk14;
  s16 bob;
  struct Actor *marker;
};

struct TrialRoadMovers {
  u8 unk00[2];
  s16 shape;
  struct TrialRoadMover movers[16];
};


s32 Math_Sine(s32 angle);

void Func_02003da0(void) {
  struct TrialRoadMovers *work =
      (struct TrialRoadMovers *)(gData_020023c4 + 0x188);
  struct TrialRoadMover *mover = work->movers;
  struct TrialRoadMapWork *map = Ram_HeapSlots->mapWork;
  s32 shape = work->shape;
  u8 *shapes = *(u8 **)((u8 *)map + 316);
  u8 *cells = map->layers[0].cells;
  s32 *collision;
  struct Actor *object;
  struct Actor *marker;
  s32 cell;
  u8 *attributes;
  s32 i;
  i = 0;
  collision = (s32 *)Ram_MapCollision + shape;
  for (; i <= 15; collision++, i++, mover++) {
    if (mover->id == 0)
      continue;
    object = Actor_GetById(mover->id);
    if (object->x == 0 && object->z == 0)
      continue;
    if (mover->phase == mover->upAt) {
      Func_02005a68(206);
      mover->speed = 4;
    }
    if (mover->phase == mover->downAt) {
      Func_02005a68(395);
      mover->speed = -8;
    }
    if (mover->speed != 0) {
      mover->offset += mover->speed;
      if (mover->offset >= mover->high) {
        mover->offset = mover->high;
        mover->speed = 0;
        mover->bob = 0;
      }
      if (mover->offset <= mover->low) {
        mover->offset = mover->low;
        mover->speed = 0;
        mover->bob = 0;
      }
    }
    if (!GameFlag_Test(260)) {
      if (++mover->phase >= mover->period)
        mover->phase = 0;
    }
    cell = (object->x >> 20) + ((object->z >> 20) << 7);
    shapes[cell] = (u8)work->shape + i;
    *collision = ((((s16 *)&object->y)[1] + mover->offset) / 8) << 8;
    attributes = cells + cell * 4;
    if (mover->speed != 0)
      attributes[3] = 0;
    else {
      s32 bits = 128;
      bits |= attributes[3];
      attributes[3] = bits;
    }
    if (mover->offset != 0) {
      s32 bits = 16;
      bits |= attributes[3];
      attributes[3] = bits;
    } else
      attributes[3] = 0;
    marker = mover->marker;
    if (marker != 0) {
      if (mover->offset == 0) {
        marker->y = object->y - 0x100000;
      } else {
        s32 wave;
        mover->bob += 0x1000;
        wave = Iwram_MulQ16(0x20000, Math_Sine(mover->bob)) - 0xe0000;
        marker->y = object->y + (mover->offset << 16) + wave;
      }
      marker->x = object->x;
      marker->z = object->z;
    }
  }
}
