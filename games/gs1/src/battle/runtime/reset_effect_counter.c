#include "scene.h"
#include "abi/battle/runtime/reset_effect_counter.h"
#include "effect_runtime.h"
#include "global_cells.h"

void Battle_ResetEffectCounter(void)
{
  void *runtime;
  void **cell;
  u8 *counter;
  int zero;
  cell = (void **)ADDR_03001EBC;
  runtime = *cell;
  counter = ((u8 *)runtime) + 0xCB6;
  zero = 0;
  *((s16 *)counter) = zero;
  if ((*((s16 *)(((u8 *)runtime) + 0xCB8))) != 0)
  {
    Battle_Check(0x2090);
  }
}
