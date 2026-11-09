/// @file
/// Updates the sixteen movers at gData_020023c4 + 0x188 each frame: at its
/// upAt and downAt phases a mover calls Sound_Play (206 or 395) and moves by
/// 4 or -8 a frame until it reaches its high or low offset; its phase counts
/// round its period unless flag 260 is set. Each mover's shape, collision
/// height and cell attribute byte follow its actor and offset, and its marker
/// actor is placed over it, bobbing on a sine while the offset is not 0.

#include "trial_road.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "event_inline.h"

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

void Func_02003da0(void) {
  struct TrialRoadMovers *work =
      (struct TrialRoadMovers *)(gData_020023c4 + 0x188);
  struct TrialRoadMover *mover = work->movers;
  struct TrialRoadMapWork *map = Ram_HeapSlots->mapWork;
  s32 shape = work->shape;
  u8 *shapes = *(u8 **)((u8 *)map + 316);
  u8 *cells = map->layers[0].cells;
  s32 *collision;
  struct Actor *actor;
  struct Actor *marker;
  s32 cell;
  u8 *attributes;
  s32 i;

  i = 0;
  collision = (s32 *)Ram_MapCollision + shape;
  for (; i <= 15; collision++, i++, mover++) {
    if (mover->id == 0)
      continue;
    actor = Actor_GetById(mover->id);
    if (actor->x == 0 && actor->z == 0)
      continue;
    if (mover->phase == mover->upAt) {
      Sound_Play(206);
      mover->speed = 4;
    }
    if (mover->phase == mover->downAt) {
      Sound_Play(395);
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
    cell = (actor->x >> 20) + ((actor->z >> 20) << 7);
    shapes[cell] = (u8)work->shape + i;
    *collision = ((((s16 *)&actor->y)[1] + mover->offset) / 8) << 8;
    attributes = cells + cell * 4;
    if (mover->speed != 0)
      attributes[3] = 0;
    else {
      /* FAKEMATCH(temporary): the form kept; as attributes[3] |= 128, 4 of 213
         instructions differ in tla-en. */
      s32 bits = 128;
      bits |= attributes[3];
      attributes[3] = bits;
    }
    if (mover->offset != 0) {
      /* FAKEMATCH(temporary): the form kept; as attributes[3] |= 16, 4 of 213
         instructions differ in tla-en. */
      s32 bits = 16;
      bits |= attributes[3];
      attributes[3] = bits;
    } else
      attributes[3] = 0;
    marker = mover->marker;
    if (marker != 0) {
      if (mover->offset == 0) {
        marker->y = actor->y - 0x100000;
      } else {
        s32 wave;
        mover->bob += 0x1000;
        wave = Iwram_MulQ16(0x20000, Math_Sine(mover->bob)) - 0xe0000;
        marker->y = actor->y + (mover->offset << 16) + wave;
      }
      marker->x = actor->x;
      marker->z = actor->z;
    }
  }
}
