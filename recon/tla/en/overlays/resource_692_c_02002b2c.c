// Near miss, differ 2 of 75 (tla-en, 2026-10-30): sched2 issues the multiply
// routine's address load (ldr r6, a constant set with no dependence) before
// the cosine call, in the stall after the angle's ldrh; the native has that
// stall empty and loads r6 after the call and after the radius load, as if
// the load depended on the call (as a pool read after a call would). Measured
// on 2026-10-30: an empty asm after the angle step, differ 4 (the angle is then
// read before the speed, r6 before the radius); an empty asm after the cosine
// call into a local, differ 2 (the angle read before the speed: the barrier
// takes the later r3 uses from the speed load, so sched2's tie goes to RTL
// order); with it the sum as *speed + *angle, differ 15; through an s16 or s32
// local, differ 10 and 8. Scheduler order; no source form reached.
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
