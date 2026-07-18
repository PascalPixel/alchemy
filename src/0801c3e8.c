typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
s32 Func_08004278(s32);
s32 Func_08016418(s32, s32);
void Func_0801c3e8(void)
{
  void *new_var2;
  s32 *new_var;
  u16 temp_r3;
  void *temp_r1;
  int new_var3;
  temp_r1 = *((void **) 0x03001EBC);
  new_var2 = temp_r1;
  *((u16 *) (((u8 *) new_var2) + 0x234)) = (temp_r3 = (*((u16 *) (((u8 *) new_var2) + 0x234))) + 0xFFFF);
  new_var3 = 0;
  if ((temp_r3 << 0x10) == new_var3)
  {
    Func_08016418(*(new_var = (s32 *) (((u8 *) new_var2) + 0x230)), 2);
    Func_08004278(0x0801C3E9);
  }
}

