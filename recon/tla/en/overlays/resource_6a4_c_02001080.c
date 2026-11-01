// Draft, differ 126 of 856, skeleton gap 74 (tla-en, 2026-11-01): the control
// flow, calls, field accesses, stack frame (44 bytes) and allocation follow the
// native; about 60 of the gap is sched2 order and the rest GCSE, neither of
// which plain C has moved.
// Order: the native issues a store (the spill of t, the step's strb, a[0],
// position[1], the trail-point str after each __divsi3) before the constant of
// the next address, even before an extended load. -mthumb-slow-loads gives a
// store before a load 2 cycles, whole loads only, and the constant's chain
// outranks it; that is the option's approximation, not this source.
// Loads: the native reloads spark->age at each of its three tests and the frame
// at step 5's second test. Here GCSE's PRE reuses the first load and inserts a
// reload on the paths that store, which also turns step 5's test into
// ldrh/lsl/cmp #0x100000. An analysis-only -fno-gcse compile gives the native's
// loads, and so does a read in another alias set (*(long *)&spark->age at the
// second test: differ 107, gap 69), since gcse.c never merges across alias
// sets; that cast would be a new device, so it is not kept.
// Tried without gain: sparks[i] and sprites[i] indexing instead of the two
// pointers (gap 145), volatile age or frame (gap 79, 184), step 5 as a goto, as
// a break on != 16, or with its tests swapped; age through a local at the
// second test (differ 130) or the third (600); step 5 with else-if (248), through
// t (253), through a pointer to the frame (298) or with a block local (no change);
// s32 returns on the local callees (no change). age is u32 now, for the cast (no
// change). The loads need no store before them in their blocks, so nothing in the
// C explains why the native skips PRE; devices cannot reach differ 0 past the
// order above, so none is kept.
// Shaping that got here from 908: ternaries written zero-first, the products as
// (to - from) * t, a pointer to the large sprite and one read of its scale, a
// pointer to the position vector taken before the mirror test, position
// declared before a and b, and the loop's i first in its initializer.
//
// The scheduled callback of the aerie's effect that Func_020017a8 starts, a
// state machine over its step (word 0xb00) counting frames in word 0xb02:
// step 0 (30 frames) brings the two trail points to the start from 10 pixels
// to one side (the side byte picks which) and 24 pixels either way along y;
// step 1 (60 frames) moves them together 30 pixels to that side while the
// large sprite grows; step 2 sweeps both along x by half a pixel a frame until
// frame 14; step 3 (sound 131) carries the event's object from the start to
// the end over 96 frames with the points 16 pixels beside it; step 4 (sound
// 220 and the second object's reaction) shrinks the large sprite away; step 5
// stops the sparks restarting and at frame 16 sets the step to 999. Each frame
// it draws the large sprite at the carried point and the 8 small sparks, each
// restarting at one of the trail points with a random drift.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"

struct BeamSprite {
  u8 unk00[8];
  u16 tile : 10;
  u16 bits : 6;
  u8 unk0a[2];
  s32 x;
  s32 z;
  s32 scaleX;
  s32 scaleY;
  u8 unk1c[0x0c];
};

struct BeamSpark {
  s32 position[3];
  s32 unk0c;
  s32 drift;
  s32 unk14;
  u32 age;
};

struct BeamWork {
  u8 tiles[0x200];
  struct BeamSprite sprites[32];
  struct BeamSpark sparks[32];
  struct BeamSprite large;
  u8 unkaa8[0x1c];
  s32 from[3];
  s32 to[3];
  s32 carried[3];
  s32 pointA[3];
  s32 pointB[3];
  s16 step;
  s16 frame;
  s16 largeEntry;
  u16 tile;
  s8 duration;
  s8 loop;
  s8 still;
  s8 side;
  struct Actor *first;
  struct Actor *second;
};

void Func_02002b68(s32 sound);
void Func_0200105c(struct Actor *object);
void Actor_SetPositionAndResetMotion(struct Actor *object, s32 x,
                                      s32 y, s32 z);
void Func_02002b20(s32 *position);
void Func_02002b38(struct BeamSprite *sprite);
void Func_02002b40(struct BeamSprite *sprite, struct BeamSpark *spark);
u32 Random16Far(void);
s32 Math_Sine(s32 angle);
s32 Math_Cosine(s32 angle);

#define LERP(from, to, t, n) ((from) + ((to) - (from)) * (t) / (n))

void Func_02001080(void) {
  struct BeamWork *work = Ram_HeapSlots->menuRuntime;
  struct Actor *carrier =
      *(struct Actor **)((u8 *)Ram_HeapSlots->eventWork + 460);
  s32 t = work->frame;
  s32 position[3];
  s32 a[3];
  s32 b[3];
  s32 aside;
  s32 scale;
  s32 *pos;
  struct BeamSpark *spark;
  struct BeamSprite *sprite;
  struct BeamSprite *large;
  s32 i;

  switch (work->step) {
  case 0:
    work->duration = 30;
    aside = work->side == 0 ? 0xa0000 : -0xa0000;
    a[0] = work->from[0] - aside;
    a[1] = work->from[1] + 0x180000;
    a[2] = work->from[2];
    b[0] = work->from[0];
    b[1] = work->from[1];
    b[2] = work->from[2];
    work->pointA[0] = LERP(a[0], b[0], t, work->duration);
    work->pointA[1] = LERP(a[1], b[1], t, work->duration);
    work->pointA[2] = LERP(a[2], b[2], t, work->duration);
    a[1] = work->from[1] - 0x180000;
    work->pointB[0] = LERP(a[0], b[0], t, work->duration);
    work->pointB[1] = LERP(a[1], b[1], t, work->duration);
    work->pointB[2] = LERP(a[2], b[2], t, work->duration);
    if (work->frame == work->duration) {
      work->step++;
      work->frame = -1;
    }
    break;
  case 1:
    work->duration = 60;
    aside = work->side == 0 ? 0x1e0000 : -0x1e0000;
    a[0] = work->from[0];
    a[1] = work->from[1];
    a[2] = work->from[2];
    b[0] = work->from[0] - aside;
    b[1] = work->from[1];
    b[2] = work->from[2];
    work->pointA[0] = LERP(a[0], b[0], t, work->duration);
    work->pointA[1] = LERP(a[1], b[1], t, work->duration);
    work->pointA[2] = LERP(a[2], b[2], t, work->duration);
    work->pointB[0] = work->pointA[0];
    work->pointB[1] = work->pointA[1];
    work->pointB[2] = work->pointA[2];
    large = &work->large;
    large->scaleY = large->scaleX = (work->frame << 16) / 60;
    if (work->frame == work->duration) {
      work->step++;
      work->frame = -1;
    }
    break;
  case 2:
    if (work->frame == 0)
      work->still = 1;
    aside = work->side == 0 ? 0x8000 : -0x8000;
    work->pointA[0] += aside;
    work->pointB[0] += aside;
    if (work->frame == 14) {
      work->step++;
      work->frame = -1;
    }
    break;
  case 3:
    if (t == 0)
      Func_02002b68(131);
    work->carried[0] = ((work->from[0] >> 2) +
                        ((work->to[0] >> 2) - (work->from[0] >> 2)) * t / 96)
                       << 2;
    work->carried[1] = LERP(work->from[1], work->to[1], t, 96);
    work->carried[2] = LERP(work->from[2], work->to[2], t, 96);
    Actor_SetPositionAndResetMotion(carrier, work->carried[0],
                                     work->carried[1], work->carried[2]);
    aside = work->side == 0 ? 0x100000 : -0x100000;
    work->pointA[0] = work->carried[0] - aside;
    work->pointA[1] = work->carried[1];
    work->pointA[2] = work->carried[2];
    work->pointB[0] = work->pointA[0];
    work->pointB[1] = work->pointA[1];
    work->pointB[2] = work->pointA[2];
    if (work->frame == 96) {
      work->step++;
      work->frame = -1;
    }
    break;
  case 4:
    if (work->frame == 0) {
      Func_02002b68(220);
      Func_0200105c(work->second);
    }
    large = &work->large;
    large->scaleY = large->scaleX = large->scaleX - 0x800;
    if (large->scaleX <= 0x3332) {
      large->scaleX = 0;
      work->step++;
      work->frame = -1;
    }
    break;
  case 5:
    if (work->frame == 0)
      work->loop = 0;
    if (work->frame == 16)
      work->step = 999;
    break;
  }
  large = &work->large;
  scale = large->scaleX;
  pos = position;
  if (work->side == 0)
    large->scaleX = scale < 0 ? -scale : scale;
  else
    large->scaleX = -(scale < 0 ? -scale : scale);
  pos[0] = work->carried[0];
  pos[1] = work->carried[1];
  pos[2] = work->carried[2];
  Func_02002b20(pos);
  large->x = pos[0];
  large->z = pos[2];
  Func_02002b38(large);
  for (i = 0, spark = work->sparks, sprite = work->sprites; i <= 7;
       i++, sprite++, spark++) {
    if (spark->age == 0) {
      if (i & 1) {
        spark->position[0] = work->pointA[0];
        spark->position[1] = work->pointA[1];
        spark->position[2] = work->pointA[2];
      } else {
        spark->position[0] = work->pointB[0];
        spark->position[1] = work->pointB[1];
        spark->position[2] = work->pointB[2];
      }
      if (work->still == 0) {
        spark->position[1] += Math_Sine(Random16Far()) << 3;
        spark->drift = 0x4ccc;
      } else {
        spark->position[1] += Math_Sine(Random16Far()) << 1;
        spark->position[0] += Math_Cosine(Random16Far()) * 10;
        spark->drift = (Random16Far() - 0x8000) << 1;
      }
    }
    if (spark->age <= 15) {
      sprite->tile = work->tile + (spark->age & 3) * 2;
      Func_02002b40(sprite, spark);
      spark->position[1] -= spark->drift;
    }
    spark->age++;
    if (work->loop != 0 && spark->age == 16)
      spark->age = 0;
  }
  work->frame++;
}
