typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
s32 Func_080f9048();
s32 Func_080199ec(void *arg0)
{
  void *new_var3;
  s32 new_var4;
  unsigned char new_var;
  s32 var_r2;
  s32 var_r6;
  void *temp_r5;
  int new_var2;
  new_var3 = *((void **) 0x03001E8C);
  temp_r5 = new_var3;
  var_r6 = 0;
  if (((*((u8 *) (((u8 *) temp_r5) + 0x12F9))) != 0) && (Func_080f9048() == 0))
  {
    var_r6 = 1;
  }
  var_r2 = (new_var4 = *((s32 *) 0x03001C94));
  new_var = 0;
  if ((*((u8 *) (temp_r5 + 0xEA4))) != new_var)
  {
    var_r2 = *((s32 *) 0x03001AF8);
  }
  if (0x303 & var_r2)
  {
    var_r6 = 1;
  }
  if (var_r6 != 0)
  {
    ;
    *((s16 *) (((u8 *) arg0) + 0x14)) = new_var;
    return 1;
  }
  return 0;
}

