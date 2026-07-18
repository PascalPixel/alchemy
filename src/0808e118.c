typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
s32 Func_0808e5d8(s32);
void Func_0808e118(void)
{
  void *new_var;
  void *temp_r3;
  void **new_var2;
  u8 *new_var3;
  int new_var4;
  new_var2 = (void **) 0x03001EBC;
  temp_r3 = *new_var2;
  new_var3 = ((u8 *) (new_var = temp_r3)) + 0xCB6;
  new_var4 = 0;
  *((s16 *) new_var3) = new_var4;
  if ((*((s16 *) (((u8 *) temp_r3) + 0xCB8))) != 0)
  {
    Func_0808e5d8(0x2090);
  }
}

