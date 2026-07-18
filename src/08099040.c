typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
s32 Func_08009098(void *, s32);
void Func_08099040(void *arg0)
{
  s32 temp_r3;
  u8 *new_var;
  if (arg0 != ((void *) 0))
  {
    temp_r3 = *((s32 *) (((u8 *) arg0) + 0x18));
    temp_r3 = temp_r3 + 0xFFFFF000;
    *((s32 *) ((new_var = (u8 *) arg0) + 0x1C)) = (s32) ((*((s32 *) (new_var + 0x1C))) + 0xFFFFF000);
    *((s32 *) (new_var + 0x18)) = temp_r3;
    if (temp_r3 <= 0x1000)
    {
      Func_08009098(arg0, 0x0809F0B0);
    }
  }
}

