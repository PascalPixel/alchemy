typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
s32 Func_08077090(s32, s32);
s32 Func_080770c0(s32);
s32 Func_08091814(u32 arg0)
{
  int new_var2;
  int new_var3;
  s32 temp_r5;
  int new_var;
 new_var2 = -1; temp_r5 = (arg0 >> 0xA) & 0xF; new_var3 = 0x3FF & arg0; if (temp_r5 > 7) {
    return new_var2;
  }
  if (Func_080770c0(temp_r5) == 0)
  {
    return -2;
  }
  if (Func_08077090(temp_r5, new_var3) == 0)
  {
    return new_var = -3;
  }
  return 0;
}
