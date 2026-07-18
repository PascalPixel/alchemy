typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
void Func_08003810(void)
{
  unsigned int new_var;
  s16 *new_var3;
  int new_var2;
  if ((*((u16 *) 0x02002000)) == 0)
  {
    new_var = 0xC3FF;
    *(new_var3 = (s16 *) 0x04000132) = new_var;
    *((s8 *) 0x03001CB8) = (new_var2 = 1);
  }
}

