#include "types.h"
#include "ram_buffer.h"
#include "event_inline.h"

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

void Func_020029b0(s32 *position);
void Func_020029e8(struct SparkSprite *sprite);

void Func_02001538(void) {
  struct SparkWork *work = Ram_HeapSlots->menuRuntime;
  struct Spark *spark = work->sparks;
  struct SparkSprite *sprite = work->sprites;
  s32 position[3];
  s32 duration;
  s32 i;

  for (i = 95; i >= 0; sprite++, i--, spark++) {
    duration = spark->duration;
    if (spark->age < 0 || spark->age > duration)
      continue;
    {
      s32 sine = Math_Sin((s32)((spark->age << 15) / duration));
      s32 *p = position;
      p[0] = work->from[0] +
             (work->to[0] - work->from[0]) * spark->age / duration +
             sine * spark->dx;
      sine = Math_Sin((s32)((spark->age << 15) / duration));
      p[1] = work->from[1] +
             (work->to[1] - work->from[1]) * spark->age / duration +
             sine * spark->dy;
      p[2] =
          work->from[2] + (work->to[2] - work->from[2]) * spark->age / duration;
      Func_020029b0(p);
      sprite->x = p[0];
      sprite->z = p[2];
      Func_020029e8(sprite);
      spark->age++;
    }
  }
}
