#include "types.h"

struct Mover {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  s32 fall;
  u8 unk18[24];
  s32 step;
  s32 height;
  u8 unk38[12];
  s32 originX;
  s32 originZ;
  s32 radius;
  u8 unk50[20];
  s16 angle;
  s16 spin;
};

void Vector_AddPolarOffsetFar(s32 distance, s32 angle, s32 *position);
void Func_020006a4(struct Mover *mover);

s32 Func_020006f8(struct Mover *mover) {
  if (mover->radius < mover->step)
    return 0;
  mover->angle += mover->spin >> 1;
  mover->x = mover->originX;
  mover->y = mover->height;
  mover->z = mover->originZ;
  // Read through a pointer of its own type, an access alchemy-gcc keeps apart
  // from the s32 stores before it: as mover->angle, the angle loads before
  // the radius (2 of 73 instructions differ).
  Vector_AddPolarOffsetFar(mover->radius, *(s16 *)&mover->angle, &mover->x);
  Func_020006a4(mover);
  mover->radius -= mover->step >> 1;
  mover->angle += mover->spin >> 1;
  mover->x = mover->originX;
  mover->y = mover->height;
  mover->z = mover->originZ;
  Vector_AddPolarOffsetFar(mover->radius, mover->angle, &mover->x);
  Func_020006a4(mover);
  mover->radius -= mover->step >> 1;
  mover->height -= mover->fall;
  return 1;
}
