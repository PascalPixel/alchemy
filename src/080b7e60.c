typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
s32 Func_080b6e30();
void *Func_080b7dd0(s32);
s32 Func_080b7e60(s32 arg0)
{
  int new_var2;
  u8 *new_var;
  Func_080b6e30();
  new_var = ((u8 *) Func_080b7dd0(arg0)) + 0x28;
  new_var2 = 1;
  *((s16 *) new_var) = new_var2;
  return 0;
}

