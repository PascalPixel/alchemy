// Differ 14 of 213 (tla-en, 2026-11-08, -mtune=gba). 8 November, 22 to 14: the for step advances
// collision before i (the loop end's 1 and 4 now in the native's registers; mover first 22,
// collision, mover, i 14). Left: the prologue adds the shape's offset to gMapCollision's
// address (native adds r1, r1, r2 after its pool load ahead of the shapes load; here the
// offset is the first operand and the pool load comes later), and the branch distance after it.
// Tried 8 November: collision set from work->shape (60), as a byte offset (14), set after the
// declarations (14), declared before shapes or cells (54, 46). Tried earlier: collision declared before shapes or cells (54, 62), set in the for init, as gMapCollision + shape, from work->shape (68); permuter 4 min best 25.
#include "../../../../games/tla/src/scene/trial_road/trial_road.h"
#include "iwram_call.h"

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

extern s32 gMapCollision[];

s32 Math_Sine(s32 angle);

void Func_02003da0(void) {
  struct TrialRoadMovers *work =
      (struct TrialRoadMovers *)(gData_020023c4 + 0x188);
  struct TrialRoadMover *mover = work->movers;
  struct TrialRoadMapWork *map = Ram_HeapSlots->mapWork;
  s32 shape = work->shape;
  u8 *shapes = *(u8 **)((u8 *)map + 316);
  u8 *cells = map->layers[0].cells;
  s32 *collision = &gMapCollision[shape];
  struct Actor *object;
  struct Actor *marker;
  s32 cell;
  u8 *attributes;
  s32 i;
  for (i = 0; i <= 15; collision++, i++, mover++) {
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
    }
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
