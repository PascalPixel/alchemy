typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
void Func_08015280(s32, s32, s32, s32, s32);
void Func_080a8cc0(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
  s32 temp_r3;
  s32 temp_r5;
  temp_r5 = arg3 * 2;
  temp_r3 = temp_r5 + 0xF281;
  if (!temp_r5)
  {
  }
  Func_08015280(arg0, 0x400 | temp_r3, arg1, arg2, 0);
  Func_08015280(arg0, temp_r5 + 0xF280, arg1 + 1, arg2, 0);
  Func_08015280(arg0, temp_r3, arg1 + 2, arg2, 0);
}

