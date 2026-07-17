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
void Func_080c0e70(void)
{
  unsigned long new_var;
  s32 var_r5;
  *((s16 *) 0x04000050) = 0x2044;
  var_r5 = 1;
  do
  {
    new_var = 1;
    if (new_var)
    {
      *((s16 *) 0x04000052) = var_r5 + 0x1000;
      var_r5 += 2;
      Func_080030f8(1);
    }
  }
  while (var_r5 <= 0x10);
}

