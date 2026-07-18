typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
u32 Func_080901c0(s32, s32);
void Func_08091df4(void)
{
  void *temp_r5;
  u8 *new_var;
  int new_var3;
  u8 *new_var2;
  temp_r5 = *((void **) 0x03001EBC);
  Func_080901c0(*((s32 *) (((u8 *) temp_r5) + 0x1C0)), *((s32 *) (((u8 *) temp_r5) + 0x1C8)));
  new_var2 = ((u8 *) temp_r5) + 0x1C6;
  new_var = new_var2;
  new_var3 = 0;
  *((s16 *) new_var) = new_var3;
}

