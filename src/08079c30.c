typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
s32 Func_08079b24(s32, s32);
s32 Func_08079c30(s32 arg0, s32 arg1, s32 arg2)
{
  s32 var_r0;
  s32 new_var;
  int new_var2;
  var_r0 = (Func_08079b24(arg1, 0) * arg0) * arg2;
  new_var2 = 0;
  do
  {
    if (var_r0 < new_var2)
    {
      var_r0 += 0xFFFF;
    }
    new_var = var_r0 >> 0x10;
    return new_var;
  }
  while (new_var2);
}

