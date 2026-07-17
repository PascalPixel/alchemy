typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
s16 Func_080030f8(s32);
s16 Func_0800430c();
s16 Func_080043e0();
void *Func_080048f4(s32, s32);
s16 Func_08091660();
void Func_080936a0(s32, s32);
s16 Func_0809c314();

void Func_0809c3a4(void)
{
  if ((*(s16 *) (((u8 *) Func_080048f4(0x1B, 0xCCC)) + 0x19E)) == 3)
  {
    Func_0800430c();
    Func_0809c314();
    Func_08091660();
    Func_080936a0(0x9D89, 6);
    if ((*((s32 *) 0x03001AE8)) & 0x200)
    {
      do
      {
        Func_080030f8(1);
      }
      while ((*((s32 *) 0x03001AE8)) & 0x200);
    }
    Func_080936a0(0x10000, 6);
    Func_080043e0();
  }
}
