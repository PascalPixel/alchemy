typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
s32 Func_080030f8(s32);
void Func_080c0e38(void)
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
 do { *((s16 *) new_var2) = 0x1010 - var_r5; var_r5 += 2; Func_080030f8(1); } while (0);
  }
  while (var_r5 <= 0x10);
}
