#include "types.h"
#include "event_inline.h"

extern s32 gData_02003274;
extern s32 gData_02003278;

void Func_02001650();

void Func_02000294(void) {
  if (gData_02003278 != 0)
    Event_PlaySound(201);
  Func_02001650(gData_02003274);
}
