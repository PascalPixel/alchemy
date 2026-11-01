// Matches: differ 0 (tla-en, 2026-11-01)
// The same function as Gaia Rock's adopted Func_0200142c, ported with its names
// and plain C, no devices. IwramMulQ16 needs its linked name and long_call: that
// is what holds the multiply's address in r6 across both calls.
#include "types.h"
#include "frame.h"

struct Orbit {
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
// Called by its linked name, as Gaia Rock's Func_0200142c does.
s32 IwramMulQ16(s32 a, s32 b) __attribute__((long_call));

// Moves around in a circle.
s32 Func_0200147c(struct Orbit *object) {
  s32 z;

  object->angle += object->speed;
  object->x = IwramMulQ16(Math_Cosine(object->angle), object->radius);
  z = IwramMulQ16(Math_Sine(object->angle), object->radius);
  object->x += object->centreX;
  object->z = (z >> 1) + object->centreZ;
  if (object->growth != 141 && (gFrameCount & 1)) {
    object->radius = object->growth * Math_Sine(object->phase << 10);
    object->growth += 10;
  }
  if (object->speed == 0)
    return 0;
  if (object->growth == 141)
    object->speed -= 128;
  return 1;
}
