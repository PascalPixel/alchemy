#include "types.h"

typedef int bool;
s32 Func_080072ec(s32, s32, s32);
void Func_080fb690(s32 arg0, s32 arg1)
{
  int new_var2;
  int new_var;
  new_var2 = (new_var = 0x02004000);
  new_var = *((s32 *) new_var2);
  Func_080072ec(arg0, arg1, new_var);
}

