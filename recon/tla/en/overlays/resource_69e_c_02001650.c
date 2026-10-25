// Draft, differ 56 of 92: adds count particles to a 96-entry ring in the work in
// heap slot 220: each takes a random angle from 0x6000 across three quarters
// of a turn, the cosine and negated sine of it at 64 as its direction,
// and a random speed of 16 to 31; its age starts at 0.
#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"

struct Spark {
  u8 unk00[0x0c];
  s32 dx;
  s32 dy;
  s32 speed;
  s32 age;
};

struct SparkWork {
  u8 unk0000[0x400];
  struct Spark sparks[96];
  u8 unk1480[0x918];
  s16 next;
};

u32 Random16Far(void);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);

void Func_02001650(s32 count) {
  struct SparkWork *work = Ram_HeapSlots->menuRuntime;
  struct Spark *spark;
  s32 angle;
  s32 speed;

  if (count > 0) {
  next:
    spark = &work->sparks[work->next];
    angle = ((Random16Far() * 3) >> 2) + 0x6000;
    speed = ((Random16Far() << 4) >> 16) + 16;
    spark->dx = Iwram_MulQ16(Math_Cosine(angle), 0x400000) >> 16;
    spark->dy = -Iwram_MulQ16(Math_Sine(angle), 0x400000) >> 16;
    spark->speed = speed;
    spark->age = 0;
    work->next = (s16)(work->next + 1) % 96;
    if (--count != 0)
      goto next;
  }
}
