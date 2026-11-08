/// @file
/// Walks actor 8 along a run of cells in the facing the event work names, throwing
/// sparks from both sides of it every fourth frame.
#include "actor.h"
#include "ram_buffer.h"
#include "types.h"
#include "call.h"

struct Spark {
  s32 mode;
  s32 kind;
  s32 scaleX;
  s32 scaleY;
  u8 unk10[0x18];
};

struct EventWork {
  u8 unk000[0x154];
  s16 unk154;
};

extern u32 gFrameCount;
struct Actor *Actor_GetById(s32 id);
void Func_02001fac(void);
void Func_02001fb4(void);
void Func_02002044(s32 skip);
void Func_02002054(s32 cue);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
s32 Func_0200034c(u32 layer, s32 x, s32 y);
void Func_02001f6c(struct Actor *actor, s32 x, s32 y, s32 z);
s32 Func_02001f9c(struct Actor *actor);
void Func_0200015c(s32 x, s32 y, s32 z, s32 a, s32 b, s32 c, s32 d,
                   struct Spark *spark);
void wait(s32 frames);

void Func_02000818(void) {
  s32 pos[4];
  struct Spark spark;
  struct EventWork *work;
  struct Actor *actor;
  s16 angle;
  s32 x, z, dx, dz, targetX, targetZ;
  s32 cosine, sine;
  s32 x0, y0, z0, flip;

  work = Ram_HeapSlots->eventWork;
  actor = Actor_GetById(8);
  angle = (work->unk154 - 45) << 14;
  Func_02001fac();
  Func_02002044(0);
  x = actor->x >> 20;
  z = actor->z >> 20;
  dx = Math_Cosine(angle) >> 16;
  dz = Math_Sine(angle) >> 16;
  actor->speedLimit = 0x40000;
  actor->acceleration = 0x40000;
  targetZ = 0;
  targetX = 0;
  if (Func_0200034c(1, x + (dz < 0 ? -dz : dz), z + (dx < 0 ? -dx : dx)) >> 8 ==
      work->unk154) {
    targetX = (dz < 0 ? -dz : dz) * 15 << 16;
    targetZ = (dx < 0 ? -dx : dx) * 15 << 16;
  }
  targetX += x << 20;
  targetZ += z << 20;
  while (Func_0200034c(1, x, z) >> 8 == work->unk154) {
    x += dx;
    z += dz;
    targetX += dx << 20;
    targetZ += dz << 20;
    Func_02001f6c(actor, targetX, actor->y, targetZ);
    while (Func_02001f9c(actor) == 0) {
      s32 phase = 3;
      phase &= gFrameCount;
      if (phase == 0) {
        if ((gFrameCount & 10) == 0) {
          Func_02002054(SPLIT_CONSTANT(0x156, 255));
        }
        spark.mode = 2;
        spark.kind = 7;
        spark.scaleX = SPLIT_CONSTANT(0x16600, 102);
        spark.scaleY = 0x13333;
        cosine = Math_Cosine(actor->angle);
        sine = Math_Sine(actor->angle);
        pos[0] = cosine * 12;
        pos[1] = sine * 12;
        pos[2] = -(cosine * 12);
        pos[3] = -(sine * 12);
        pos[0] += actor->x;
        pos[1] += actor->z;
        pos[2] += actor->x;
        pos[3] += actor->z;
        if (pos[1] != actor->z) {
          pos[1] -= 0xc0000;
          pos[3] -= 0xc0000;
        }
        flip = gFrameCount & 4;
        y0 = actor->y;
        x0 = pos[0];
        z0 = pos[1];
        if (flip) {
          Func_0200015c(x0 + 0x40000, y0, z0, 0x8000, 0, 0, 0xb0000, &spark);
          Func_0200015c(pos[2] - 0x40000, actor->y, pos[3], -0x8000, 0, 0,
                        0xb0000, &spark);
        } else {
          Func_0200015c(x0 - 0x40000, y0, z0, -0x8000, 0, 0, 0xb0000, &spark);
          Func_0200015c(pos[2] + 0x40000, actor->y, pos[3], 0x8000, 0, 0,
                        0xb0000, &spark);
        }
      }
      wait(1);
    }
  }
  Func_02001fb4();
}
