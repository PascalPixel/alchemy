typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
s32 Func_0808b05c(u16, u16);
unsigned char Func_0808b074(s32 arg0)
{
  s32 temp_r0;
  int new_var2;
  int new_var5;
  u16 new_var;
  u8 *new_var4;
  u8 *new_var3;
  temp_r0 = arg0 * 4;
  new_var4 = ((u8 *) temp_r0) + 0x0809D8B0;
  new_var5 = temp_r0 + 0x0809D8B0;
  new_var3 = new_var4;
  new_var = *((u16 *) new_var3);
  new_var2 = new_var5;
  Func_0808b05c(new_var, *((u16 *) (((u8 *) new_var2) + 2)));
}

