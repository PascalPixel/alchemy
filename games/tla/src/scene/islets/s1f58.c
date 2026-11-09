#include "islets.h"
#include "event_inline.h"

void Func_02001f58(void) {
  if ((gFrameCount & 15) == 0)
    Event_PlaySound(118);
}
