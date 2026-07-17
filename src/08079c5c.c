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
s32 Func_08079c5c(s32 arg0, s32 arg1, s32 arg2)
{
  s32 var_r0;
  unsigned int new_var;
  new_var = Func_08079b24((arg1 * 2) - 0xC8, 0) * arg0;
  new_var = arg2 * new_var;
  var_r0 = new_var;
  if (var_r0 < 0)
  {
    var_r0 += 0xFFFF;
  }
  return var_r0 >> 0x10;
}

