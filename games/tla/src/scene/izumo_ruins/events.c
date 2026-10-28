#include "types.h"
#include "actor.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"

void Func_02000eac();
struct Actor *Actor_GetById();

void Func_02001184(void) {
  struct Actor *object0;
  s32 result1;

  object0 = Actor_GetById(gAdventure.currentOwner);
  result1 = Value3((s32 (*)())Func_02000eac, 2, (object0->x >> 20),
                   (object0->z >> 20));
  if ((result1 >> 8) != 232)
    goto L_020091b2;
  object0->terrainId = 2;
L_020091b2:
  return;
}

void Func_020011b8(void) {
  struct Actor *object0;

  object0 = Actor_GetById(gAdventure.currentOwner);
  object0->terrainId = 0;
  return;
}
