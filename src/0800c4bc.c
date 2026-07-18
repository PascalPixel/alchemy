typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
s32 Func_0800c2d8(void *, s32);
void Func_0800c4bc(void *arg0, s32 arg1)
{
  int new_var2;
  int new_var3;
  u8 *new_var;
  u8 *new_var4;
  Func_0800c2d8(arg0, 0x080135F0);
  new_var2 = 0;
  if (arg1 != new_var2)
  {
    new_var4 = (u8 *) arg0;
    *((s32 *) (new_var4 + 0x34)) = 0x8000;
    new_var = (u8 *) arg0;
    *((s32 *) (new_var + 0x30)) = 0x40000;
    new_var3 = 0x68;
    new_var2 = 0;
    *((s32 *) (((u8 *) arg0) + new_var3)) = arg1;
    *((s16 *) (new_var + 0x64)) = new_var2;
  }
}

