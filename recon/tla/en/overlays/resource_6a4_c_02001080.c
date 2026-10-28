// Draft, differ 922 of 856: the scheduled callback of the aerie's effect that
// Func_020017a8 starts, a state machine over its step (word 0xb00) counting
// frames in word 0xb02: step 0 moves the two trail points over 30 frames from
// the start toward the start 24 pixels down and to one side; step 1 over 60
// frames on toward 60 pixels aside, growing the large sprite; step 2 sweeps
// both points along x until frame 14; step 3 (sound 131) carries the event's
// object from the start to the end over 96 frames; step 4 (sound 220 and the
// first object's reaction) shrinks the large sprite away; step 5 waits 16
// frames and sets the step to 999. Each frame it draws the large sprite at the
// carried point and the 8 small sparks, each restarting at one of the trail
// points with a random drift.
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
  s32 age;
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

#define LERP(from, to, t, n) ((from) + (t) * ((to) - (from)) / (n))

void Func_02001080(void) {
  struct BeamWork *work = Ram_HeapSlots->menuRuntime;
  struct Actor *carrier =
      *(struct Actor **)((u8 *)Ram_HeapSlots->eventWork + 460);
  s32 t = work->frame;
  s32 a[3];
  s32 b[3];
  s32 aside;
  s32 position[3];
  struct BeamSpark *spark;
  struct BeamSprite *sprite;
  s32 i;

  switch (work->step) {
  case 0:
    work->duration = 30;
    aside = work->side != 0 ? -0xa0000 : 0xa0000;
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
    aside = work->side != 0 ? -0x1e0000 : 0x1e0000;
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
    work->large.scaleX = work->large.scaleY = (work->frame << 16) / 60;
    if (work->frame == work->duration) {
      work->step++;
      work->frame = -1;
    }
    break;
  case 2:
    if (work->frame == 0)
      work->still = 1;
    aside = work->side != 0 ? -0x8000 : 0x8000;
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
                        t * ((work->to[0] >> 2) - (work->from[0] >> 2)) / 96)
                       << 2;
    work->carried[1] = LERP(work->from[1], work->to[1], t, 96);
    work->carried[2] = LERP(work->from[2], work->to[2], t, 96);
    Actor_SetPositionAndResetMotion(carrier, work->carried[0],
                                     work->carried[1], work->carried[2]);
    aside = work->side != 0 ? -0x100000 : 0x100000;
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
    work->large.scaleX = work->large.scaleY = work->large.scaleX - 0x800;
    if (work->large.scaleX <= 0x3332) {
      work->large.scaleX = 0;
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
  if (work->side == 0)
    work->large.scaleX = work->large.scaleX < 0 ? -work->large.scaleX
                                                : work->large.scaleX;
  else
    work->large.scaleX = -(work->large.scaleX < 0 ? -work->large.scaleX
                                                  : work->large.scaleX);
  position[0] = work->carried[0];
  position[1] = work->carried[1];
  position[2] = work->carried[2];
  Func_02002b20(position);
  work->large.x = position[0];
  work->large.z = position[2];
  Func_02002b38(&work->large);
  spark = work->sparks;
  sprite = work->sprites;
  for (i = 0; i <= 7; i++, spark++, sprite++) {
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
    if ((u32)spark->age <= 15) {
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
