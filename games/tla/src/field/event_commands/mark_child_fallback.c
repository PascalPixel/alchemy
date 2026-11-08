#include "types.h"
#include "ram_buffer.h"

void Func_080e0134();

/// Pauses the scene's child (byte 91) and runs the fallback transition.
void BattleEffect_MarkChildAndRunFallbackTransition(void) {
  (*(u8 **)((u8 *)Ram_HeapSlots->effectWork + 20))[91] = 1;
  Func_080e0134();
}
