typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
s32 Func_08004080(void)
{
  s32 var_r0;
  s32 var_r2;
  void *new_var2;
  int new_var;
  void *var_r1;
  var_r1 = (void *) 0x03001B10;
  var_r0 = 0x60;
  new_var = 0;
  var_r2 = new_var;
  new_var2 = (void *) 0x03001B10;
  if ((*((u16 *) (((u8 *) new_var2) + 2))) == 0xFFFF)
  {
    return new_var;
  }
  loop_2:
  var_r2 += 1;

  var_r1 += 4;
  if (var_r2 <= 0x5F)
  {
    if ((*((u16 *) (((u8 *) var_r1) - -2))) == 0xFFFF)
    {
      var_r0 = var_r2;
    }
    else
    {
      goto loop_2;
    }
  }
  return var_r0;
}

