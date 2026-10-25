// Near miss, differ 2 of 75 (was 67): the native loads the multiply
// routine's address (ldr r6) after the cosine call, beside the radius; this
// draft loads it before the call, in the free cycle after the angle is
// read. Measured: a volatile barrier after the cosine call keeps the load
// below the call but then the angle is read before the speed (differ 2 at
// another place); reading the speed into a local first changes the load
// (ldrsh, or r0 for the pointer); an in-out asm on the cosine result does
// not hold the load. An empty asm between the cosine call (into its own
// local) and the first multiply holds the load (differ 2: the angle is then
// read before the speed); the angle sum written six ways, five minutes of
// alchemy permute on that form, no better. Its copy group is led by 664's
// Func_02003f94 in another lane.
// Measured against its copies: overlay 67C's Func_0200147c (differ 2 of 75).
#include "types.h"
#include "iwram_call.h"
#include "frame.h"

struct GaiaRockInsideOrbit {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x30];
  s32 centreX;
  s32 centreZ;
  s32 radius;
  u8 unk50[0x12];
  u8 phase;
  u8 growth;
  s16 angle;
  s16 speed;
};

s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);

// Moves around in a circle.
s32 Func_0200142c(struct GaiaRockInsideOrbit *object) {
  s16 *speed = &object->speed;
  s16 *angle = &object->angle;
  u8 *growth;
  s32 z;

  *angle += *speed;
  object->x = Iwram_MulQ16(Math_Cosine(*angle), object->radius);
  z = Iwram_MulQ16(Math_Sine(*angle), object->radius);
  object->x += object->centreX;
  growth = (u8 *)angle - 1;
  object->z = (z >> 1) + object->centreZ;
  if (*growth != 141 && (gFrameCount & 1)) {
    object->radius = *growth * Math_Sine(object->phase << 10);
    *growth += 10;
  }
  if (*speed == 0)
    return 0;
  if (*growth == 141)
    *speed -= 128;
  return 1;
}
