typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
s32 Func_0800ca2c(void *arg0)
{
  u8 *new_var2;
  s32 new_var;
  ;
  new_var2 = (u8 *) arg0;
  *((s32 *) (((u8 *) arg0) + 0)) = (s32) (((*((s32 *) (((u8 *) arg0) + 0))) + ((*((s16 *) (((u8 *) arg0) + 4))) * 4)) + 4);
  new_var = 0;
  *((s16 *) (new_var2 + 4)) = new_var;
  return 1;
}

