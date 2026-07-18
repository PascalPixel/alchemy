typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
s32 Func_080c0cec(s32, s32, s32, s32);
void Func_080c01bc(void)
{
  s32 var_r2;
  u8 *new_var2;
  u32 *temp_r0;
  void *new_var;
  u32 temp_r1;
  u32 temp_r2;
  temp_r0 = *((u32 **) 0x03001EF8);
  temp_r1 = *temp_r0;
  new_var2 = (u8 *) (*((void **) 0x03001E80));
  var_r2 = 0x34 - temp_r1;
  if (var_r2 > 0x20)
  {
    if (1)
    {
      var_r2 = 0x20;
    }
  }
  new_var = (void *) 0x03001AD0;
  if (var_r2 < 0)
  {
    if (var_r2 || temp_r1)
    {
      var_r2 = 0;
    }
    else
    {
      var_r2 = 0;
    }
  }
  *((s16 *) (((u8 *) new_var) + 2)) = (s16) var_r2;
  if (temp_r1 <= 0x50U)
  {
    *((s16 *) (new_var2 + 0x36)) = (s16) (((45 * temp_r1) * 8) + 0xAF80);
  }
  temp_r2 = (*temp_r0 = (*temp_r0) + 1);
  if (temp_r2 <= 0x50U)
  {
    Func_080c0cec(0, 0, 0, 0xB4 - temp_r2);
    return;
  }
  Func_080c0cec(0, 0, 0, 0x64);
}

