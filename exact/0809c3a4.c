#include "types.h"

s32 Func_080030f8(s32);
s32 Func_0800430c(void);
s32 Func_080043e0(void);
void *Func_080048f4(s32 arg0, s32 arg1);
void Func_08091660(void);
void Func_080936a0(s32, s32);
void Func_0809c314(void);

void Func_0809c3a4(void)
{
  if ((*(s16 *) (((u8 *) Func_080048f4(0x1B, 0xCCC)) + 0x19E)) == 3)
  {
    Func_0800430c();
    Func_0809c314();
    Func_08091660();
    Func_080936a0(0x9D89, 6);
    if ((*((volatile u32 *) 0x03001AE8)) & 0x200)
    {
      do
      {
        Func_080030f8(1);
      }
      while ((*((volatile u32 *) 0x03001AE8)) & 0x200);
    }
    Func_080936a0(0x10000, 6);
    Func_080043e0();
  }
}
