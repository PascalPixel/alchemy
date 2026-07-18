typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
u32 Func_0808d458(s32 arg0, s32 arg1)
{
  s32 temp_r3;
  int new_var;
  u32 var_r0;
  if ((0xF & arg0) != 3)
  {
    return 0U;
  }
  new_var = 0x1FF & arg0;
  var_r0 = 0;
  if (new_var != 3)
  {
    temp_r3 = (0xFFF00000 & arg1) ^ 0x500000;
    var_r0 = (u32) (temp_r3 | (0 - temp_r3));
    var_r0 = var_r0 >> 0x1F;
  }
  return var_r0;
}

