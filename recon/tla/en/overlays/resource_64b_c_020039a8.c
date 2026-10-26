// Near miss of Func_020039a8 of the world map (34 instructions, 6 differ):
// after the Q16 multiply the native starts the rise (`movs r2, #128`), copies
// the parent's x, then shifts; the draft copies first, and its `movs r0, #1`
// comes two rows late. The one-pass block and rise local that match the twins
// Func_02004598 and 020045e4 give 8 differ here. 2026-10-19: the rise pinned
// to r2 and the result to r0 (6), the rise behind an empty asm (6).
// Measured (2026-10-26): every spot left is a load the native issues one slot
// after a store to memory it may alias (through a pointer from a call or a
// load, against the stack, the object or a global), with an independent
// instruction between; -mthumb-slow-loads delays only a load from the address
// just stored, so this draft issues the load at once. A matched pointer
// parameter (Gabomba Catacombs' Func_020008a0) cannot alias the stack and
// shows no gap, which fits. No source form found to make it wait.
#include "types.h"
#include "iwram_call.h"
#include "object_runtime.h"

s32 Math_Sine(s32 angle);

s32 Func_020039a8(struct ObjectRuntime *object) {
  struct ObjectRuntime *parent = object->linkedObject;
  s32 step = ++object->action;
  s32 scale;

  if (step > 31)
    return 0;
  scale = Iwram_MulQ16(Math_Sine(step << 10), 0x14000);
  object->scaleX = scale;
  object->scaleY = scale;
  object->x = parent->x;
  object->y += 0x10000;
  object->z = parent->z;
  return 1;
}
