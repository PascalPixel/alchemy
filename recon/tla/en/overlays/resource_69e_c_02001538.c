// Draft, differ 71 (tla-en, 2026-11-05). Func_02001538 of overlay 69e.
// Both barriers gone: a rise from 63, as they forced allocation rank. A
// pointer local for position, block scope. Left: rank rotated, sine r6, work
// r8, the pointer sl, where natively the pointer is r6, sine r8, work sl.
#include "types.h"
#include "call.h"
#include "ram_buffer.h"

struct Spark {
  u8 unk00[0x0c];
  s32 dx;
  s32 dy;
  s32 duration;
  s32 age;
};

struct SparkSprite {
  u8 unk00[0x0c];
  s32 x;
  s32 z;
  u8 unk14[0x14];
};

struct SparkWork {
  u8 unk0000[0x400];
  struct Spark sparks[96];
  struct SparkSprite sprites[96];
  s32 from[3];
  s32 to[3];
  s16 next;
};

s32 Math_Sine(s32 angle);
void Func_020029b0(s32 *position);
void Func_020029e8(struct SparkSprite *sprite);

void Func_02001538(void) {
  struct SparkWork *work = Ram_HeapSlots->menuRuntime;
  struct Spark *spark = work->sparks;
  struct SparkSprite *sprite = work->sprites;
  s32 position[3];
  s32 duration;
  s32 i;

  for (i = 95; i >= 0; i--, spark++, sprite++) {
    duration = spark->duration;
    if (spark->age < 0 || spark->age > duration)
      continue;
    {
    s32 sine = Math_Sine((spark->age << 15) / duration);
    s32 *p = position;
    p[0] = work->from[0] +
                  (work->to[0] - work->from[0]) * spark->age / duration +
                  sine * spark->dx;
    sine = Math_Sine((spark->age << 15) / duration);
    p[1] = work->from[1] +
                  (work->to[1] - work->from[1]) * spark->age / duration +
                  sine * spark->dy;
    p[2] = work->from[2] +
                  (work->to[2] - work->from[2]) * spark->age / duration;
    Func_020029b0(p);
    sprite->x = p[0];
    sprite->z = p[2];
    Func_020029e8(sprite);
    spark->age++;
    }
  }
}
