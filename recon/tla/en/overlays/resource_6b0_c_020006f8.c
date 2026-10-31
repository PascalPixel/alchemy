// Matches: differ 0 of 73 (tla-en, 2026-10-30).
// The last two rows were the first step's call arguments: the native loads the
// radius (ldr r0, [r7, #76]) before the angle (movs r3, #0; ldrsh r1). As
// mover->angle the angle's load waits on the x, y and z stores, and sched2
// picks it over the radius on its dependents; read as *(s16 *)&mover->angle
// it waits only on the z store's r3, and the radius goes first. The permuter
// found it (as a pointer temporary). Earlier tries, each still differ 2: the
// steps as an inline helper, radius or angle or &x in a temporary, the struct
// without byte arrays, the helper taking s16; taking u16 (differ 8); volatile
// reads (radius 2, both 15).
// Under -mthumb-slow-loads (2026-10-26): differ 2 of 73.
// Near miss, differ 4 of 73 (21 before, 2026-10-12): rewritten with a struct;
// the function returns 0 when the radius is under the step and 1 after two
// steps. Two rows are left in another order, both in the first step only: the
// native loads the angle (ldrh r3) before it starts the spin's sign extension
// (lsls r2), and loads the radius into r0 before the angle for the first polar
// call; the second step matches. Tried: the polar helper declared as returning
// a value (no change), a radius temporary behind an empty asm barrier (differ
// 10), spin and angle in locals (differ 19).
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
