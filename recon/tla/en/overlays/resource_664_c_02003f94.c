// Near miss, differ 2 of 75 (tla-en, 2026-10-30): scheduler order. sched2
// hoists the multiply routine's address (a bare constant set of r6, with no
// dependence) above the cosine call into the stall after the angle's load;
// the native issues it after the call and the radius load, as if that load
// depended on the call.
// Measured 30 October: a barrier holds the load only by cutting the later r3
// and r5 dependences that settle two earlier ties in sched2. Barrier right
// after the cosine call or on the radius load (asm or one-pass block): the
// angle loads before the speed (differ 2). Barrier after the radius load
// (asm, or a one-pass block setting a local pointer to the routine): mov r1,
// r8 goes before adds r5, #100 (differ 2). Barrier before the cosine call:
// differ 4. Calling through a local pointer alone: differ 2 (unchanged).
// Reading the speed first: differ 15. No barrier position settles both ties.
// 26 October: alchemy permute (5 to 6 minutes) did not move it; 30 October,
// 4 minutes on the barrier-after-radius form: no better.
// Overlay 664's copy of overlay 681's Func_0200142c (the same listing), whose
// draft holds its notes.
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
s32 Func_02003f94(struct GaiaRockInsideOrbit *object) {
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
