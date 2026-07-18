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
s32 Func_0800387c(s32, s32);
void Func_080b5038(s32, u16, s32);
void Func_080cdd14(void)
{
  void *temp_r5;
  int new_var3;
  int new_var2;
  u8 *new_var;
  new_var = ((u8 *) (*((void **) 0x03001F00))) + 0xC;
  temp_r5 = *((void **) 0x03001E74);
  *((s32 *) new_var) = 1;
  new_var2 = 0x1541;
  Func_0800387c(0x04000000, new_var2);
  new_var3 = 1;
  Func_080030f8(new_var3);
  Func_080b5038(2, *((u16 *) (((u8 *) temp_r5) + 0x648)), 0);
  new_var2 = new_var3;
  do
  {
    Func_080030f8(new_var2);
  }
  while (0);
}

