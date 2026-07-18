typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
s32 Func_0800d654(void *arg0)
{
  u8 *new_var2;
  s16 new_var4;
  int new_var3;
  u8 *new_var;
  new_var2 = (u8 *) arg0;
  new_var = ((u8 *) (((*((s16 *) (((u8 *) arg0) + 4))) * 4) + (*((s32 *) (new_var2 + 0))))) + 4;
  new_var4 = (s16) ((*((s32 *) new_var)) - 1);
  *((s16 *) (new_var2 + 0x5E)) = new_var4;
  new_var3 = 2;
  *((s16 *) (((u8 *) arg0) + 4)) = (s16) (((u16) (*((s16 *) (((u8 *) arg0) + 4)))) + new_var3);
  return 0;
}

