typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
s32 Func_080f7460();
s32 Func_080f9010(s32);
s32 Func_080f6008(void)
{
  s32 *new_var;
  u8 *new_var2;
  if (1)
  {
    *((s16 *) 0x04000000) = 0x40;
    new_var2 = (u8 *) ((void *) 0x02000240);
    new_var = (s32 *) 0x03001CB4;
    *new_var = *((s32 *) (new_var2 + 4));
  }
  Func_080f9010(9);
  Func_080f7460();
  return 0;
}

