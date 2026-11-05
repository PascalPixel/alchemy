// Draft, differ 42 (tla-en, 2026-11-05). Func_02004d50 of overlay 6ae.
// Native reads held twice, signed then raw; raw | 0xffff through the pool; a
// zero in r5 shared by the byte and the update. Left: both reads collapse
// here (CSE), through a plain struct.
#include "../../../../games/tla/src/scene/mars_lighthouse_aerie/mars_lighthouse_aerie.h"

void Func_02004d50(s32 id) {
  u8 *work = Ram_HeapSlots->windowTiles;
  struct MarsLighthouseAerieParty *party = (struct MarsLighthouseAerieParty *)&gMainRoutineWork;
  s32 held = party->held;
  u16 raw = *(u16 *)&party->held;

  if (held == id) {
    work[0x138a] = 0;
    party->held = raw | 0xffff;
    Actor_GetById(held)->update = NULL;
    Actor_SetPartAttribute(Actor_GetById(held), 0);
  }
}
