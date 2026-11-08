#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "version.h"

// From patch 1; the Japanese overlay has no such function.
#if TLA_PATCH_1
struct MarsLighthouseAerieParty {
  u8 unk000[0x26a];
  s16 held;
};

struct Actor *Actor_GetById(s32 id);

void Func_02004e80(s32 from, s32 to) {
  struct Actor *found = Actor_GetById(from);
  struct Actor *object;
  struct Actor *target;
  struct MainRoutineWork *party = &gMainRoutineWork;

  party->savedByte = 0;
  object = found;
  ((struct MarsLighthouseAerieParty *)party)->held = to;
  target = Actor_GetById(to);
  target->update = object->update;
  object->update = NULL;
}
#endif
