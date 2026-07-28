#include "effect_runtime.h"

s32 Func_0808e5d8(s32);
void Func_0808e118(void)
{
  void *runtime;
  void **runtimePtr;
  u8 *counter;
  int zero;
  runtimePtr = (void **) 0x03001EBC;
  runtime = *runtimePtr;
  counter = ((u8 *) runtime) + 0xCB6;
  zero = 0;
  *((s16 *) counter) = zero;
  if ((*((s16 *) (((u8 *) runtime) + 0xCB8))) != 0)
  {
    Func_0808e5d8(0x2090);
  }
}
