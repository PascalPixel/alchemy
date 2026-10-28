#include "types.h"
#include "actor.h"

struct Actor *Actor_GetById(s32 id);
void WaitFrames(s32 frames);
void Func_02002eec(s32 cue);

/// Runs the cue, if any, and sets the object's motion flags to 3 while it
/// moves, for at most 60 frames until its vertical speed is 0, then clears
/// them ten frames later; Func_02000100 of Gaia Rock is the same function.
void Func_02000b30(s32 id, s32 cue) {
  struct Actor *object = Actor_GetById(id);
  /* FAKEMATCH(pin): the flags' address built in r2 with 85 in it first; as a
   * plain pointer, 17 of 32 instructions differ in tla-en. */
  register u8 *flags asm("r2");
  u8 *held;
  /* FAKEMATCH(pin): the counter in r6 and the object in r5; as a plain
   * counter, 12 of 32 instructions differ in tla-en. */
  register s32 frames asm("r6");

  flags = (u8 *)85;
  /* FAKEMATCH(barrier): keeps 85 in a register, so it is added to the object
   * rather than the object copied and 85 added; without it, 4 of 32
   * instructions differ in tla-en. */
  asm("" : "+r"(flags));
  flags += (s32)object;
  *flags = 3;
  frames = 60;
  /* FAKEMATCH(temporary): the address copied to r8 for the calls after the
   * counter starts; kept in one variable, 9 of 32 instructions differ in
   * tla-en. */
  held = flags;
  while (frames != 0) {
    WaitFrames(1);
    frames--;
    if (object->velocityY == 0)
      break;
  }
  if (cue != 0)
    Func_02002eec(cue);
  WaitFrames(10);
  *held = 0;
}
