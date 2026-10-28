#include "types.h"
#include "object_runtime.h"
#include "adventure.h"
#include "call.h"

s32 Object_GetById();
s32 Func_02000348();
s32 Func_02003bc8();
s32 Func_02003c88();
s32 GameFlag_SetBit();
s32 Func_02000384();
s32 GameFlag_ClearBit();
s32 Func_02003b80();
s32 Func_02003bd0();

/// Object 11's column sets or clears flag 0x311 and writes the low byte of the
/// cell value Func_02000348 returns (its high byte 0xff when the object is
/// away) into two places; with the leader 208 pixels up a block copy follows.
void Func_02000620(void) {
  struct ObjectRuntime *object;
  struct ObjectRuntime *leader;
  s32 value;

  object = (struct ObjectRuntime *)Value1(Object_GetById, 11);
  value = Value3(Func_02000348, 3, 70, 67);
  Func_02003bc8();
  Call1(Func_02003c88, 0);
  if ((object->x >> 20) == 32) {
    Call1(GameFlag_SetBit, 0x311);
    value &= 0xff;
    Func_02000384(4, 70, 67, value);
    Call4(Func_02000384, 2, 33, 54, value);
  } else {
    Call1(GameFlag_ClearBit, 0x311);
    value = (value & 0xff) | 0xff00;
    Func_02000384(4, 70, 67, value);
    Call4(Func_02000384, 2, 33, 54, value);
  }
  leader =
      (struct ObjectRuntime *)Value1(Object_GetById, gAdventure.currentOwner);
  if ((leader->y >> 19) == 26)
    Call6(Func_02003b80, 70, 67, 1, 1, 33, 54);
  Func_02003bd0();
}
