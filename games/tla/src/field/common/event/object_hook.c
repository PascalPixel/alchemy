#include "types.h"
#include "event_work.h"
#include "object_runtime.h"

void WaitTicks(s32 ticks);
void Func_080caa4c(s32 object);

void Event_EmptyObjectHook(void) {}

struct ObjectRuntime *Event_RunObjectHookAndWait(s32 object) {
  Event_EmptyObjectHook();
  Func_080caa4c(object);
  WaitTicks(1);
  return ObjectTable_Get(EventRuntime_GetControlledOwner());
}
