#include "effect_runtime.h"
#include "global_cells.h"

#define Battle_ResetEffectCounter Func_0808e118

s32 Func_0808e5d8(s32);
void Battle_ResetEffectCounter(void)
{
  void *runtime;
  void **cell;
  u8 *counter;
  int zero;
  cell = (void **) ADDR_03001EBC;
  runtime = *cell;
  counter = ((u8 *) runtime) + 0xCB6;
  zero = 0;
  *((s16 *) counter) = zero;
  if ((*((s16 *) (((u8 *) runtime) + 0xCB8))) != 0)
  {
    Func_0808e5d8(0x2090);
  }
}
