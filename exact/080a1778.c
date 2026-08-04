#include "types.h"

s32 Func_08003fa4(s32 entry_no, s32 mode, s32 data);
s32 Func_08004080(void);
s32 Func_080150c8(s32 entry_no, s32 flags, s32 first, s32 second, s32 third);
s32 Func_080a1778(s32 first, s32 second, s32 third)
{
  s32 temp_r0;
  unsigned char new_var;
  int new_var2;
  s32 var_r6;
  var_r6 = 0;
  var_r6 = 0;
  temp_r0 = Func_08004080();
  new_var2 = 0x80;
  if (temp_r0 != 0)
  {
    Func_08003fa4(temp_r0, new_var = new_var2, 0x080AEA4C);
    var_r6 = Func_080150c8(temp_r0, 0x40000000, first, second, third);
  }
  return var_r6;
}
