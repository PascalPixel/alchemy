#include "types.h"

void UiWork_ResetChannelTransition(void *work)
{
  int mode;
  int val;
  u8 *p;
  p = ((u8 *) work) + 0x1C;
  val = 2;
  mode = val;
  *((s16 *) p) = mode;
}
