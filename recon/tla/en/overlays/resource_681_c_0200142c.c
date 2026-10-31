// Matches: differ 0 of 75 (tla-en, 2026-10-30); differ 0 in tla-ja, tla-de,
// tla-es, tla-fr and tla-it as well.
// The multiply is called by IwramMulQ16's own linked name (0x0300021c in the
// map), declared long_call: its address is then a literal-pool load of a
// symbol, a memory read the scheduler keeps after the cosine call, as the
// native does. Through iwram_call.h's Iwram_MulQ16 (a cast constant address)
// the same load has no dependence on the call and rose into the load stall
// before it (differ 2 of 75, ldr r6 one call early). Under that form an empty
// asm after the cosine call or after the radius read, a one-pass block, a
// cosine local, a volatile barrier and five minutes of alchemy permute only
// moved the difference (the angle read before the speed, or the speed
// pointer copied a slot early): a barrier cuts the later dependents that
// order those loads.
// Its copy group is led by 664's Func_02003f94 in another lane; this form
// also gives differ 0 against 664's Func_02003f94 and 67C's Func_0200147c.
#include "types.h"
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
s32 IwramMulQ16(s32 a, s32 b) __attribute__((long_call));

// Moves around in a circle.
s32 Func_0200142c(struct GaiaRockInsideOrbit *object) {
  s16 *speed = &object->speed;
  s16 *angle = &object->angle;
  u8 *growth;
  s32 z;

  *angle += *speed;
  object->x = IwramMulQ16(Math_Cosine(*angle), object->radius);
  z = IwramMulQ16(Math_Sine(*angle), object->radius);
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
