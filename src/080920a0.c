typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
s32 Func_08009140();
void *Func_0808ba1c();
void Func_080920a0(void)
{
  void *temp_r0;
  unsigned char new_var;
  temp_r0 = Func_0808ba1c();
  if (temp_r0 != ((void *) 0))
  {
    new_var = 1;
    *((u8 *) (((u8 *) temp_r0) + 0x5A)) = (u8) (new_var | (*((u8 *) (((u8 *) temp_r0) + 0x5A))));
    Func_08009140();
  }
}

