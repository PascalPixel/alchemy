#include "types.h"
#include "event_work.h"
#include "actor.h"

void WaitFrames(s32 frames);
void Func_080caa4c(s32 object);

void Event_EmptyObjectHook(void) {}

struct Actor *Event_RunObjectHookAndWait(s32 object) {
  Event_EmptyObjectHook();
  Func_080caa4c(object);
  WaitFrames(1);
  return ObjectTable_Get(Party_GetLeaderActor());
}
