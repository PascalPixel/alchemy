#include "types.h"

s32 WaitFrames(s32);
void Graphics_FadeAlphaOut(void)
{
  s32 var_r5;
  unsigned long long new_var;
  long long new_var2;
  new_var = 0x04000050;
  *((s16 *) new_var) = 0x2044;
  new_var2 = 0x04000052;
  var_r5 = 1;
  do
  {
 do { *((s16 *) new_var2) = 0x1010 - var_r5; var_r5 += 2; WaitFrames(1); } while (0);
  }
  while (var_r5 <= 0x10);
}
