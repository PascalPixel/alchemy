typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
u32 Func_080072f0(s32, s32, u32, s32);
void Func_080cd508(u32 arg2)
{
  unsigned long new_var;
  s32 new_var2;
  new_var2 = *((s32 *) 0x03001EEC);
  Func_080072f0(new_var2 + 0x7818, 8, new_var, 0x03000164);
  new_var = arg2;
}

