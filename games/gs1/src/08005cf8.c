#include "types.h"
#include "runtime_interfaces.h"

typedef void (*InterruptHandler)(void);

u32 Func_08002dd8(s32);
void Func_0800307c(s32, s32, InterruptHandler);
u32 Func_08005cf8(void)
{
  int new_var3;
  long long new_var5;
  long long new_var4;
  int new_var;
  unsigned int new_var2;
  new_var3 = 0;
  new_var5 = (new_var4 = (new_var2 = 0x33));
  new_var = 0;
  Func_0800307c(5, new_var, (InterruptHandler)new_var3);
 return Func_08002dd8(new_var5);
}
