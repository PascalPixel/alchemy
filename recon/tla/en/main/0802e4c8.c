// ☀️ Map_BuildProbeRing; differ 10: after each Vector_AddPolarOffset call the
// native reloads the spilled loop counter one instruction earlier, inside the
// split angle step constant (arm9tdmi load scheduling).
#include "types.h"
#include "input.h"
#include "animspr.h"
#include "ram_buffer.h"

extern s32 gFrameTick;

struct ProbeShape {
  u8 unk00[4];
  u8 kind;
};

struct ProbeObjects {
  u8 unk00[40];
  struct ProbeShape *shape;
};

void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *point);

void Map_BuildProbeRing(s32 x, s32 z, struct AnimationPosition *out) {
  struct AnimationPosition *pair;
  s32 angle;
  s32 i;
  u16 turn;
  u8 kind;

  angle = 0;
  kind = ((struct ProbeObjects *)Ram_HeapSlots->unk0c[1])->shape->kind;
  if ((gInput.held & 2) != 0)
    angle = (s16)(gFrameTick << 8);
  switch ((u32)kind) {
  case 3:
    for (i = 0; i < 6; i++) {
      angle <<= 16;
      out->x = x << 16;
      angle = (u32)angle >> 16;
      out->y = 0;
      out->z = z << 16;
      Vector_AddPolarOffset(0x380000, (u16)angle, &out->x);
      out++;
      angle = (s16)(angle + 0x2aaa);
    }
    for (; i < 10; i++)
      out++;
    break;
  case 5:
  case 8:
  case 44:
  case 88:
    for (i = 0; i < 8; i++) {
      angle <<= 16;
      out->x = x << 16;
      angle = (u32)angle >> 16;
      out->y = 0;
      out->z = z << 16;
      Vector_AddPolarOffset(0x380000, (u16)angle, &out->x);
      out++;
      angle = (s16)(angle + 0x2000);
    }
    for (; i < 10; i++)
      out++;
    break;
  case 4:
  case 6:
    for (i = 0; i < 10; i++) {
      angle <<= 16;
      out->x = x << 16;
      angle = (u32)angle >> 16;
      out->y = 0;
      out->z = z << 16;
      Vector_AddPolarOffset(0x380000, (u16)angle, &out->x);
      out++;
      angle = (s16)(angle + 0x1999);
    }
    break;
  case 20:
    turn = angle;
    angle = (s16)(turn + 0x4000);
    i = 0;
    pair = out;
    while (i < 2) {
      pair[i * 2].x = x << 16;
      pair[i * 2].y = 0;
      pair[i * 2].z = z << 16;
      angle <<= 16;
      angle = (u32)angle >> 16;
      Vector_AddPolarOffset(0x280000, angle, &out[0].x);
      pair[i * 2 + 1].x = x << 16;
      {
        s32 *point = &out[1].x;
        pair[i * 2 + 1].y = 0;
        pair[i * 2 + 1].z = z << 16;
        Vector_AddPolarOffset(0x280000, (u16)angle, point);
      }
      angle = (angle + 0x8000) << 16;
      i++;
      out += 2;
      angle >>= 16;
    }
    break;
  default:
    angle = ((angle << 16) + 0x20000000) >> 16;
    for (i = 0; i < 4; i++) {
      angle <<= 16;
      angle = (u32)angle >> 16;
      out->x = x << 16;
      out->y = 0;
      out->z = z << 16;
      Vector_AddPolarOffset(0x380000, (u16)angle, &out->x);
      out++;
      angle = (s16)(angle + 0x4000);
    }
    for (i = 4; i < 10; i++)
      out++;
    break;
  }
}
