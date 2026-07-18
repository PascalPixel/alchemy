typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
void *Func_080048f4(s32, s32);
void Func_0801c930(void)
{
  void *new_var;
  void *temp_r0;
  unsigned int new_var2;
  new_var = Func_080048f4(0x13, 0x1004);
  temp_r0 = new_var;
  new_var2 = 0;
  if (1)
  {
  }
  *((s16 *) (((u8 *) temp_r0) + 0x46)) = new_var2;
  *((s16 *) (((u8 *) temp_r0) + 0x352)) = new_var2;
}

