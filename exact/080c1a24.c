#include "types.h"

typedef void (*InterruptHandler)(void);

void Func_0800307c(s32, s32, InterruptHandler);
void Func_080c1a24(void)
{
  int new_var;
  unsigned long long new_var2;
  new_var2 = 2;
  new_var = new_var2;
  new_var2 = 0;
  Func_0800307c(new_var, 0, (InterruptHandler)new_var2);
}
