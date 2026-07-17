typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
s32 Func_080022ec(s32, s32);
s32 Func_080072f0(s32, s32, s32, s32);
s32 Func_0800c300(void *, s32);
s32 Func_0800d14c(void *, s32, s32, s32);
s32 Func_0800dcdc(void *arg0)
{
  s32 temp_r0;
  s32 new_var;
  s32 temp_r1;
  s32 temp_r2_2;
  u8 *new_var3;
  s32 temp_r2_3;
  s32 temp_r3;
  s32 temp_r5;
  s32 temp_r8;
  s32 new_var2;
  u8 *new_var4;
  void *temp_r2;
  new_var4 = (u8 *) arg0;
  new_var3 = new_var4;
  temp_r2 = *((void **) (new_var3 + 0x68));
  *((s32 *) (new_var3 + 0x30)) = (s32) (*((s32 *) (((u8 *) temp_r2) + 0x30)));
  *((s32 *) (new_var3 + 0x34)) = (s32) (*((s32 *) (((u8 *) temp_r2) + 0x34)));
  temp_r1 = (*((s32 *) (((u8 *) temp_r2) + 8))) - (*((s32 *) (new_var3 + 8)));
  temp_r2_2 = (*((s32 *) (((u8 *) temp_r2) + 0x10))) - (*((s32 *) (new_var3 + 0x10)));
  temp_r3 = temp_r1 >> 0x10;
  temp_r2_3 = temp_r2_2 >> 0x10;
  temp_r0 = Func_080072f0((temp_r3 * temp_r3) + (temp_r2_3 * temp_r2_3), temp_r1, temp_r2_3, 0x030001D8);
  if (temp_r0 > 0x10)
  {
    new_var2 = temp_r1;
    temp_r5 = temp_r0 - 0x10;
    temp_r8 = Func_080022ec(new_var2 * temp_r5, temp_r0);
    new_var = Func_080022ec(temp_r2_2 * temp_r5, temp_r0);
    Func_0800d14c(arg0, (*((s32 *) (new_var3 + 8))) + temp_r8, *((s32 *) (new_var3 + 0xC)), (*((s32 *) (new_var3 + 0x10))) + new_var);
    Func_0800c300(arg0, 2);
    *((u16 *) (new_var3 + 4)) = (u16) ((*((u16 *) (new_var3 + 4))) + 1);
    return 1;
  }
  Func_0800c300(arg0, 1);
  return 0;
}

