typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
void Func_0801e71c(s32 arg0)
{
  s16 new_var2;
  void *new_var;
  new_var = *((void **) 0x03001E8C);
  new_var2 = (s16) (arg0 & 0xF);
 do { *((s16 *) (new_var + 0xEAE)) = new_var2; } while (0);
}
