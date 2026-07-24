typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

void Func_08005e88(void)
{
    s32 base;

    *(volatile u16 *)0x04000208 = 0;
    *(u16 *)0x04000200 = *(u16 *)0x04000200 & 0xff3f;
    *(volatile u16 *)0x04000208 = 1;
    *(u16 *)0x04000128 = 0x2003;
    *(u32 *)0x0400010c = 0x0000c963;
    *(u16 *)0x04000202 = 0xc0;
    base = 0x02002240;
    *((u8 *)base + 8) = 0;
}
