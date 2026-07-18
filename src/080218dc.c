typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
void Func_08019000(s32, s32, s32, s32, s32);
volatile int Func_080218dc(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
  int new_var3;
  s32 temp_r3;
  s32 temp_r5;
  s32 new_var2;
  int new_var;
  temp_r5 = 2;
  temp_r5 = temp_r5;
  temp_r5 = arg3 * temp_r5;
  temp_r3 = temp_r5 + 0xF315;
  new_var2 = arg2;
  Func_08019000(arg0, 0x400 | temp_r3, arg1, new_var2, 0);
  if (1)
  {
    new_var3 = 1;
    Func_08019000(arg0, temp_r5 + 0xF314, arg1 + new_var3, arg2, 0);
    new_var = arg1 + 2;
    Func_08019000(arg0, temp_r3, new_var, arg2, 0);
  }
}

