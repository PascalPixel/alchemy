// Draft, differ 88 of 213: the Trial Road movers' update (Func_02004224 sets
// them up): for each of the 16 slots with a placed object, its phase counts up
// to its period (unless flag 260 is set); at the up phase it rises 4 a frame
// (sound 206), at the down phase it sinks 8 a frame (sound 395), stopping at
// its high and low marks; its cell takes the mover's shape and a collision
// height from the object's height plus its offset, is marked moving or
// raised, and its marker object follows it, bobbing while raised. The collision
// pointer is taken first, as the native reads the shape before the map. Left:
// the phase is read as a halfword for its increment at both compares, the cell
// flags are written once after a join and read back from the register where the
// native reads them again, and register choice from there on.
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
  struct ObjectRuntime *marker;
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
  s32 *collision = &gMapCollision[work->shape];
  struct TrialRoadMapWork *map = Ram_HeapSlots->mapWork;
  u8 *shapes = *(u8 **)((u8 *)map + 316);
  u8 *cells = map->layers[0].cells;
  struct TrialRoadMover *mover = work->movers;
  struct ObjectRuntime *object;
  struct ObjectRuntime *marker;
  s32 cell;
  u8 *attributes;
  s32 i;
  for (i = 0; i <= 15; i++, collision++, mover++) {
    if (mover->id == 0)
      continue;
    object = Object_GetById(mover->id);
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
    attributes[3] = mover->speed != 0 ? 0 : attributes[3] | 128;
    if (mover->offset != 0)
      attributes[3] |= 16;
    marker = mover->marker;
    if (marker != 0) {
      if (mover->offset == 0) {
        marker->y = object->y - 0x100000;
      } else {
        mover->bob += 0x1000;
        marker->y = object->y + (mover->offset << 16) +
                    Iwram_MulQ16(0x20000, Math_Sine(mover->bob)) - 0xe0000;
      }
      marker->x = object->x;
      marker->z = object->z;
    }
  }
}
