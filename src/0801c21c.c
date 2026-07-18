typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
s32 Func_08003f3c(long);
void Func_0801c21c(void)
{
  unsigned int new_var;
  void *temp_r5;
  new_var = 0x30C;
  temp_r5 = (*((s32 *) 0x03001E98)) + new_var;
  if ((*((u16 *) (((u8 *) temp_r5) + 0xA))) != 0)
  {
    Func_08003f3c(*((u16 *) (((u8 *) temp_r5) + 0xC)));
    *((u16 *) (((u8 *) temp_r5) + 0xA)) = (new_var = 0U);
  }
}

