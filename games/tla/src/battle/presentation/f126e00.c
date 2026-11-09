#include "types.h"
#include "iwram_call.h"
#include "ram_buffer.h"

struct Spark {
  s32 pos[6]; // x, y, z and their velocities, 16.16
  s32 life;
};

struct Ring {
  s32 x;
  s32 y;
  s32 velX;
  s32 velY;
  s32 age;
};

struct SparkWork {
  u8 sheet[0x11c0];
  struct Spark sparks[16];
  struct Ring rings[3];
  s32 frames;
  s32 ready;
  s32 originX;
  s32 originY;
  s32 originPending;
};

typedef void (*DrawFunc)(void *dest, u8 *src, s32 x, s32 y, s32 width,
                         s32 height);

// The Broken Seal's star tables: each frame's size and sheet offset, and
// the ring's four frames.
extern const u8 Data_08129828[];
extern const s32 Data_0812980c[];
extern const s32 Data_08129830[];

s32 FixedSqrt(s32 value);
s32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

// One frame of the star field: sixteen sparks fall in toward the centre
// along a damped spiral and shrink as they near it; while the field is
// young (24 frames) a spark that dies is born again on a random ring.
// Then three rings drift outward through their four frames. The canvas
// is marked ready for the transfer callback at the end.
void Func_08126e00(void) {
  s32 j;
  s32 frame;
  u32 size;
  s32 ringX;
  s32 ringY;
  s32 phase;
  void *dest;
  s32 i;
  struct SparkWork *work;
  DrawFunc draw;
  struct Spark *spark;
  struct Ring *ring;
  s32 *pos;
  s32 k;
  s32 dist;
  s32 scale;
  s32 step;
  u32 radius;
  s32 angle;
  u32 half;
  s32 x;
  s32 y;
  s32 life;
  s32 (*mul)(s32 a, s32 b);

  dest = Ram_HeapSlots->battleFxWork[1];
  work = (struct SparkWork *)Ram_HeapSlots->battleFxWork[0];
  work->ready = 0;
  draw = (DrawFunc)Ram_HeapSlots->unk64[1];
  for (i = 0; i < 16; i++) {
    spark = &work->sparks[i];
    life = spark->life;
    if (life != 0) {
      dist = FixedSqrt((spark->pos[0] >> 8) * (spark->pos[0] >> 8) +
                       (spark->pos[1] >> 8) * (spark->pos[1] >> 8) +
                       (spark->pos[2] >> 8) * (spark->pos[2] >> 8));
      if (dist <= 0xfff) {
        spark->life = 0;
      } else {
        scale = Iwram_RatioMulQ14(dist, 0x10000);
        spark->life--;
        // The multiply's entry held in a local keeps its address ahead of the
        // cursor and the axis count; called directly, 2 of 267 instructions
        // differ in tla-en.
        mul = Iwram_MulQ16;
        pos = spark->pos;
        for (k = 2; k >= 0; k--) {
          step = mul(mul(-*pos >> 8, scale), 0x13000);
          pos[3] = pos[3] - (pos[3] >> 7) + step;
          *pos += pos[3];
          pos++;
        }
      }
      life = spark->life;
    }
    if (life == 0 && work->frames <= 24) {
      angle = Random16();
      radius = Random16() + 0x10000;
      half = radius >> 1;
      spark->pos[0] = Iwram_MulQ16(Trig_Cos(angle), half);
      spark->pos[1] = Iwram_MulQ16(Trig_Sin(angle), half);
      if (spark->pos[0] & 1)
        spark->pos[0] = -spark->pos[0];
      if (spark->pos[1] & 1)
        spark->pos[1] = -spark->pos[1];
      spark->pos[2] = (u32)(Random16() + 0x8000) >> 2;
      spark->pos[3] = (-spark->pos[0] >> 7) + (spark->pos[1] >> 8);
      spark->pos[4] = (-spark->pos[1] >> 7) + (-spark->pos[0] >> 8);
      spark->pos[5] = 0;
      life = spark->life = (radius >> 13) + 1;
    }
    if (life != 0) {
      x = (spark->pos[0] >> 10) + 64;
      y = (spark->pos[1] >> 10) + 64;
      frame = life;
      if (frame < 0)
        frame = 0;
      else if (frame > 6)
        frame = 6;
      size = Data_08129828[frame];
      draw(dest, (u8 *)work + Data_0812980c[frame], x - (size >> 1),
           y - (size >> 1), size, size);
    }
  }
  draw = (DrawFunc)Ram_HeapSlots->unkb8[1];
  for (ring = work->rings, j = 2; j >= 0; j--, ring++) {
    ring->x += ring->velX;
    ring->y += ring->velY;
    ringX = ring->x >> 10;
    ringY = ring->y >> 10;
    phase = 3 - ring->age / 8;
    if (phase >= 0) {
      ring->age++;
      /* FAKEMATCH(barrier): an empty asm between `ring->age++;` and `draw(dest,
       * (u8 *)work + Data_08129830[phase], ringX + 48, ringY + 48, 32,`, which
       * keeps their instructions apart; as plain statements, 2 of 267
       * instructions differ in tla-en. */
      asm("");
      draw(dest, (u8 *)work + Data_08129830[phase], ringX + 48, ringY + 48, 32,
           32);
    }
  }
  work->frames++;
  work->ready = 1;
}
