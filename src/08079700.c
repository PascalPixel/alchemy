typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
s32 Func_08079700(s32 arg0)
{
  s32 var_r3;
  void *new_var;
  void *new_var2;
  void *new_var3;
  new_var2 = (void *) 0x02000240;
  var_r3 = *((s32 *) (((u8 *) new_var2) + 0x10));
  var_r3 = var_r3 + arg0;
  new_var3 = new_var2;
  if (var_r3 > 0xF423F)
  {
    var_r3 = 0xF423F;
  }
  if (var_r3 < 0)
  {
    var_r3 = 0;
  }
  new_var = new_var3;
  *((s32 *) (((u8 *) new_var) + 0x10)) = var_r3;
  return var_r3;
}

