#include "types.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "actor.h"
#include "version.h"
#include "event_inline.h"

struct Trigger;

void Func_02000bc0(s32 id, s32 flag);

extern const u8 gData_0200102c[];
extern const u8 gData_02001044[];
extern const u8 gData_02001194[];
extern const u8 gData_020012cc[];

const struct Trigger *Func_02000080(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov680Room1)
    return (const struct Trigger *)gData_02001044;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov680Room2)
    return (const struct Trigger *)gData_02001194;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov680Room3)
    return (const struct Trigger *)gData_020012cc;
  return (const struct Trigger *)gData_0200102c;
}

void Func_020000d0(void) { Func_02000bc0(17, 0x300); }

void Func_020000e0(void) { Func_02000bc0(18, 0x301); }

void Func_020000f0(void) { Func_02000bc0(19, 0x302); }

// Sets the object's motion flags to 3 while it moves, for at most 60 frames
// until its vertical speed is 0, then clears them ten frames later. The
// editions after the Japanese one also run the given cue first.
#if !TLA_PATCH_1
void Func_02000100(s32 id) {
  struct Actor *object = Actor_Get(id);
  /* FAKEMATCH(pin): the counter fixed to r6, so the object takes r5; as a
   * plain local, 12 instructions differ. */
  register s32 frames asm("r6");

  object->motionFlags = 3;
  for (frames = 60; frames != 0; frames--) {
    System_Wait(1);
    if (object->velocityY == 0)
      break;
  }
  System_Wait(10);
  object->motionFlags = 0;
}
#else
void Func_02000100(s32 id, s32 cue) {
  struct Actor *object = Actor_Get(id);
  /* FAKEMATCH(pin): the flags' address fixed to r2, so it is built with 85 in
   * it first (movs r2, #85; adds r2, r2, r5) and the 3 takes r3; as a plain
   * local, r2 and r3 swap and 10 instructions differ. */
  register u8 *flags asm("r2");
  u8 *held;
  /* FAKEMATCH(pin): the counter fixed to r6, so the object takes r5; as a
   * plain local, 14 instructions differ. */
  register s32 frames asm("r6");

  flags = (u8 *)85;
  /* FAKEMATCH(barrier): an empty asm on flags, so 85 stays in a register and
   * is added to the object rather than the object copied and 85 added; without
   * it, 4 instructions differ. */
  asm("" : "+r"(flags));
  flags += (s32)object;
  *flags = 3;
  frames = 60;
  /* FAKEMATCH(temporary): a copy of the flags' address, so it is copied to r8
   * for the calls after the counter starts; as flags itself, its reload copy
   * comes before the store and 2 instructions differ. */
  held = flags;
  while (frames != 0) {
    System_Wait(1);
    frames--;
    if (object->velocityY == 0)
      break;
  }
  if (cue != 0)
    Event_PlaySound(cue);
  System_Wait(10);
  *held = 0;
}
#endif
