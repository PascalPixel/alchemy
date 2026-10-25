// Draft, differ 142 of 131: the spark ring's update (Func_02001650 adds sparks):
// each live spark moves from the work's start point to its end point over its
// duration (speed, in frames), bowed sideways by its direction times the sine
// of its progress, and its sprite follows (Func_020029b0 projects the point,
// Func_020029e8 draws it).
#include "types.h"
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
  s32 sine;
  s32 i;

  for (i = 95; i >= 0; i--, spark++, sprite++) {
    duration = spark->duration;
    if (spark->age < 0 || spark->age > duration)
      continue;
    sine = Math_Sine((spark->age << 15) / duration);
    position[0] = work->from[0] +
                  spark->age * (work->to[0] - work->from[0]) / duration +
                  sine * spark->dx;
    sine = Math_Sine((spark->age << 15) / duration);
    position[1] = work->from[1] +
                  spark->age * (work->to[1] - work->from[1]) / duration +
                  sine * spark->dy;
    position[2] = work->from[2] +
                  spark->age * (work->to[2] - work->from[2]) / duration;
    Func_020029b0(position);
    sprite->x = position[0];
    sprite->z = position[2];
    Func_020029e8(sprite);
    spark->age++;
  }
}
