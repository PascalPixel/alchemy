typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
s32 Func_080030f8(s32);
s32 Func_080b5078(s32, s16, s32, s32);
s32 Func_080d4604(void *, s32);
void Func_080eceac(void *arg0)
{
  u8 *new_var2;
  void **new_var;
  Func_080b5078(*((s32 *) (((u8 *) arg0) + 8)), *((s16 *) (((u8 *) arg0) + 0x24)), 0x18, 0x73333);
 do { Func_080030f8(0xC); } while (0); new_var2 = (u8 *) arg0;
  *((s32 *) (new_var2 + 0x18)) = 3;
  new_var = &arg0;
  Func_080d4604(*new_var, 2);
}
