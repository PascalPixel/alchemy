// Under -mthumb-slow-loads (2026-10-26): differ 2 of 75; left: as overlay 681's
// Func_0200142c (resource_681_c_0200142c.c).
// Draft, differ 2 of 75: overlay 692's copy of the function drafted in
// resource_681_c_0200142c.c (overlay 681's Func_0200142c, differ 2 there), whose
// header says what it is, what still differs and what was tried.
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
s32 Func_02002b2c(struct GaiaRockInsideOrbit *object) {
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
