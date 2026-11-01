// Draft, differ 206 of 315, skeleton gap 54 (tla-en, 2026-11-01): control flow,
// calls, the frame (88 bytes, same slots) and memory accesses follow the
// listing; what is left comes from one thing. The listing materialises center
// three times (`add r1, sp, #76` before the switch, `add r5, sp, #76` after it,
// `add r3, sp, #76` / r11 after the nudge) and keeps 0x8000 in r11 across the
// flip test. Here GCSE merges all of it: PRE makes the later addresses copies
// and cprop folds them into one pseudo live through the function (r7), and
// cprop puts the constant back at each use, even with explicit pointers to
// center at each site (same output). -fno-gcse gives the three; no plain C form
// was found that keeps (plus sfp 76) from PRE, and sfp is never killed. No
// function with a repeated `add rN, sp, #K` has matched yet, so this may be a
// compiler question (79 listing hits; K3, Pascal's call). The consequences: object r9, not
// r11; sine r9, not r7; IwramMulQ16 and x swap r6/r8; the x nudge takes r5 for
// 0x8000 where the listing, with r5 busy, has r1 in both arms and cross-jumps.
// Tried: x nudge through a local or a ternary, which pulls x into a long pseudo
// (gap 88+); `object->x` for *pos, differ 192 but gap 64, kept out as the gap is
// the aim; z read once and stored once is the best z form; a `speed` local,
// gap 4 worse (cprop undoes it). px, py and pz are read before the flip test
// because the listing loads them there.
// Shape follows Func_0200043c (s043c.c), including its forwarded SeaOfTime_Cos
// and SeaOfTime_Sin; plain Math_Cosine/Math_Sine there gives 242.
#include "../../../../games/tla/src/scene/sea_of_time/sea_of_time.h"

struct SeaOfTimeEventWork0154 {
  u8 unk000[0x154];
  s16 unk154;
};

void Func_0200054c(void) {
  struct SeaOfTimeEventWork0154 *work =
      (struct SeaOfTimeEventWork0154 *)Ram_HeapSlots->eventWork;
  struct SeaOfTimeObject *object = Actor_GetById(8);
  s32 center[3];
  s32 ends[4];
  struct SeaOfTimeSpark spark;
  s32 *pos;
  s32 *offset;
  s32 cosine;
  s32 sine;
  s32 dx;
  s32 dz;
  s32 x;
  s32 z;
  s32 phase;
  s32 c;
  s32 s;
  s32 flip;
  s32 px;
  s32 py;
  s32 pz;

  offset = center;
  pos = &object->x;
  switch (work->unk154) {
  case 30:
    offset[0] = 0;
    offset[2] = -1;
    break;
  case 32:
    offset[0] = -1;
    offset[2] = 0;
    break;
  case 34:
    offset[0] = 0;
    offset[2] = 1;
    break;
  case 36:
    offset[0] = 1;
    offset[2] = 0;
    break;
  case 31:
    offset[0] = -1;
    offset[2] = -1;
    break;
  case 33:
    offset[0] = -1;
    offset[2] = 1;
    break;
  case 35:
    offset[0] = 1;
    offset[2] = 1;
    break;
  case 37:
    offset[0] = 1;
    offset[2] = -1;
    break;
  }
  center[0] = (((object->x >> 21) + center[0]) << 21) + 0x100000;
  center[2] = (((object->z >> 21) + center[2]) << 21) + 0x100000;
  if (Func_020003d0(pos, center) > 46) {
    if (*pos < center[0])
      *pos += 0x8000;
    else
      *pos -= 0x8000;
    z = object->z;
    if (z < center[2])
      z += 0x8000;
    else
      z -= 0x8000;
    object->z = z;
  }
  if (Func_0200034c(1, center[0] >> 20, center[2] >> 20) >> 8 == 40) {
    cosine = SeaOfTime_Cos(-0x200);
    sine = SeaOfTime_Sin(-0x200);
  } else {
    cosine = SeaOfTime_Cos(0x200);
    sine = SeaOfTime_Sin(0x200);
  }
  dx = object->x - center[0];
  dz = object->z - center[2];
  x = Iwram_MulQ16(dx, cosine) - Iwram_MulQ16(dz, sine);
  z = Iwram_MulQ16(dx, sine) + Iwram_MulQ16(dz, cosine);
  x += center[0];
  z += center[2];
  object->z = z;
  object->x = x;

  phase = gFrameCount & 3;
  if (phase == 0) {
    if ((gFrameCount & 10) == 0)
      Func_02002054(0x255);
    spark.mode = 2;
    spark.kind = 7;
    spark.scaleX = 0xcccc;
    spark.scaleY = 0xcccc;
    c = Math_Cosine(object->facing);
    s = Math_Sine(object->facing);
    ends[0] = c * 10;
    ends[1] = s * 10;
    ends[2] = -(c * 10);
    ends[3] = -(s * 10);
    ends[0] += object->x;
    ends[1] += object->z;
    ends[2] += object->x;
    ends[3] += object->z;
    if (ends[1] != object->z) {
      ends[1] += -0xc0000;
      ends[3] += -0xc0000;
    }
    flip = gFrameCount & 4;
    px = ends[0];
    py = object->y;
    pz = ends[1];
    if (flip) {
      Func_0200015c(px + 0x30000, py, pz, 0x8000, 0, 0, 0xb0000, &spark);
      Func_0200015c(ends[2] - 0x30000, object->y, ends[3], -0x8000, 0, 0,
                    0xb0000, &spark);
    } else {
      Func_0200015c(px - 0x30000, py, pz, -0x8000, 0, 0, 0xb0000, &spark);
      Func_0200015c(ends[2] + 0x30000, object->y, ends[3], 0x8000, 0, 0,
                    0xb0000, &spark);
    }
  }
}
