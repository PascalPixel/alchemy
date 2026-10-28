#include "types.h"
#include "object_runtime.h"
#include "adventure.h"
#include "edition.h"

// The Japanese overlay has no such function.
#if (LANGUAGE != LANGUAGE_JAPANESE)
struct MarsLighthouseAerieParty {
  u8 unk000[0x26a];
  s16 held;
};

struct ObjectRuntime *Object_GetById(s32 id);

void Func_02004e80(s32 from, s32 to) {
  struct ObjectRuntime *found = Object_GetById(from);
  struct ObjectRuntime *object;
  struct ObjectRuntime *target;
  struct Adventure *party = &gAdventure;

  party->savedByte = 0;
  object = found;
  ((struct MarsLighthouseAerieParty *)party)->held = to;
  target = Object_GetById(to);
  target->update = object->update;
  object->update = NULL;
}
#endif
