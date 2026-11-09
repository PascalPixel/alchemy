#include "sea_of_time.h"

struct SeaOfTimeEventWork0154 {
  u8 unk000[0x154];
  s16 unk154;
};

void Func_0200054c(void) {
  struct SeaOfTimeEventWork0154 *work =
      (struct SeaOfTimeEventWork0154 *)Ram_HeapSlots->eventWork;
  struct SeaOfTimeObject *object = ((struct SeaOfTimeObject *)Actor_GetById(8));
  struct {
    s32 x, y, z;
  } center;
  s32 ends[4];
  struct SeaOfTimeSpark spark;
  s32 turn[1];
  s32 *pos;
  s32 dx;
  s32 dz;
  s32 x;
  s32 z;
  s32 phase;
  s32 frame;
  s32 *counter;
  s32 c;
  s32 s;
  s32 flip;
  s32 speed;
  s32 px;
  s32 py;
  s32 pz;

  switch (work->unk154) {
  case 30:
    center.x = 0;
    center.z = -1;
    break;
  case 32:
    center.x = -1;
    center.z = 0;
    break;
  case 34:
    center.x = 0;
    center.z = 1;
    break;
  case 36:
    center.x = 1;
    center.z = 0;
    break;
  case 31:
    center.x = -1;
    center.z = -1;
    break;
  case 33:
    center.x = -1;
    center.z = 1;
    break;
  case 35:
    center.x = 1;
    center.z = 1;
    break;
  case 37:
    center.x = 1;
    center.z = -1;
    break;
  }
  center.x = (((object->x >> 21) + center.x) << 21) + 0x100000;
  center.z = (((object->z >> 21) + center.z) << 21) + 0x100000;
  pos = &object->x;
  if (Func_020003d0(pos, &center) > 46) {
    if (*pos < center.x)
      *pos += 0x8000;
    else
      *pos -= 0x8000;
    if (object->z < center.z)
      object->z += 0x8000;
    else
      object->z -= 0x8000;
  }
  if (Func_0200034c(1, center.x >> 20, center.z >> 20) >> 8 == 40) {
    turn[0] = SeaOfTime_Cos(-0x200);
    s = Math_Sine(-0x200);
  } else {
    turn[0] = SeaOfTime_Cos(0x200);
    s = Math_Sine(0x200);
  }
  dx = object->x - center.x;
  dz = object->z - center.z;
  {
    s32 (*mul)(s32 a, s32 b) = Iwram_MulQ16;
    x = mul(dx, turn[0]);
    x -= mul(dz, s);
    z = mul(dx, s);
    z += mul(dz, turn[0]);
  }
  x += center.x;
  z += center.z;
  object->x = x;
  object->z = z;

  counter = &gFrameCount;
  frame = *counter;
  phase = 3;
  phase &= frame;
  if (phase == 0) {
    if ((frame & 10) == 0)
      Sound_Play(0x255);
    spark.mode = 2;
    spark.kind = 7;
    spark.scaleX = 0xcccc;
    spark.scaleY = 0xcccc;
    c = Math_Cosine(object->facing);
    {
      s32 sine = Math_Sine(object->facing);
      ends[0] = c * 10;
      ends[1] = sine * 10;
      ends[2] = -(c * 10);
      ends[3] = -(sine * 10);
    }
    ends[0] += object->x;
    ends[1] += object->z;
    speed = 0x8000;
    ends[2] += object->x;
    ends[3] += object->z;
    if (ends[1] != object->z) {
      ends[1] += -0xc0000;
      ends[3] += -0xc0000;
    }
    flip = *counter & 4;
    px = ends[0];
    py = object->y;
    pz = ends[1];
    if (flip) {
      Func_0200015c(px + 0x30000, py, pz, speed, 0, 0, 0xb0000, &spark);
      Func_0200015c(ends[2] - 0x30000, object->y, ends[3], -speed, 0, 0,
                    0xb0000, &spark);
    } else {
      Func_0200015c(px - 0x30000, py, pz, -speed, 0, 0, 0xb0000, &spark);
      Func_0200015c(ends[2] + 0x30000, object->y, ends[3], speed, 0, 0, 0xb0000,
                    &spark);
    }
  }
}
