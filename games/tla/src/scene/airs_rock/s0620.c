#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

s32 Func_02000348();
s32 Func_02003bc8();
s32 Func_02000384();
s32 Func_02003bd0();

/// Object 11's column sets or clears flag 0x311 and writes the low byte of the
/// cell value Func_02000348 returns (its high byte 0xff when the object is
/// away) into two places; with the leader 208 pixels up a block copy follows.
void Func_02000620(void) {
  struct Actor *object;
  struct Actor *leader;
  s32 value;

  object = (struct Actor *)((s32)Actor_Get(11));
  value = Func_02000348(3, 70, 67);
  Func_02003bc8();
  FieldEvent_ResolvePendingActions(0);
  if ((object->x >> 20) == 32) {
    Event_SetFlag(0x311);
    value &= 0xff;
    Func_02000384(4, 70, 67, value);
    Func_02000384(2, 33, 54, value);
  } else {
    Event_ClearFlag(0x311);
    value = (value & 0xff) | 0xff00;
    Func_02000384(4, 70, 67, value);
    Func_02000384(2, 33, 54, value);
  }
  leader = (struct Actor *)((s32)Actor_Get(gMainRoutineWork.currentOwner));
  if ((leader->y >> 19) == 26)
    MapGrid_CopyCellAttributes(70, 67, 1, 1, 33, 54);
  Func_02003bd0();
}
