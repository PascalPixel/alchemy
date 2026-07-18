typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
void Func_08015080(s32, s32, s32, s32);
s32 Func_080150b0(s32, s32, s32, s32, s32);
void Func_080b10cc(void)
{
  u8 *new_var;
  s32 temp_r5;
  temp_r5 = *((s32 *) (((u8 *) (*((void **) 0x03001F2C))) + 0xC));
  if (temp_r5 != 0)
  {
    Func_08015080(0xC8A, temp_r5, 0, 0);
    new_var = (u8 *) 0x02000240;
    Func_080150b0(*((s32 *) (new_var + 0x10)), 6, temp_r5, 0x20, 8);
  }
}

