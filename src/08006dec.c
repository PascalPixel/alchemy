typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

extern s32 Func_080072f0(s32 mode, u8 *destination, u8 value, s32 status);

u16 Func_08006dec(u8 *source, u8 *destination)
{
  s32 *status;
  *(volatile u8 *) 0x0E005555 = 0xAA;
  *(volatile u8 *) 0x0E002AAA = 0x55;
  *(volatile u8 *) 0x0E005555 = 0xA0;
  *destination = *source;
  status = (s32 *) 0x02004C00;
  return Func_080072f0(1, destination, *source, *status);
}
