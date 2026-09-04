#include "types.h"

#define UiWork_SetByte5AndResetHalfwordC Func_0801edcc

void UiWork_SetByte5AndResetHalfwordC(void *arg0, int arg1)
{
  void *zero;
  if (arg0)
  {
    zero = (void *) 0;
  }
  else
  {
    zero = (void *) 0;
  }
  if (arg0 != zero)
  {
    *((s8 *) (((u8 *) arg0) + 5)) = arg1;
    *((s16 *) (((u8 *) arg0) + 0xC)) = 0;
  }
}
