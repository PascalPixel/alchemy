typedef signed int s32;
typedef unsigned short u16;

void Func_08003b70(s32);
void Func_08004458(void);
void Func_080030f8(s32);

s32 Func_080bffb8(void)
{
    volatile u16 bg0cnt;
    volatile u16 bg1cnt;
    volatile u16 bg2cnt;
    volatile u16 bg3cnt;
    s32 i;
    volatile u16 *reg;

    bg0cnt = *(volatile u16 *)0x04000008;
    *(volatile u16 *)0x04000008 = bg0cnt | 0x40;
    reg = (volatile u16 *)0x04000008;
    reg++;
    bg1cnt = *reg;
    *reg = bg1cnt | 0x40;
    reg++;
    bg2cnt = *reg;
    *reg = bg2cnt | 0x40;
    reg++;
    bg3cnt = *reg;
    *reg = bg3cnt | 0x40;
    reg += 0x21;
    *reg = 0x3eee;

    Func_08003b70(16);
    for (i = 0; i <= 15; i++) {
        Func_08004458();
        Func_08004458();
        Func_08004458();
        Func_08004458();
        *(volatile u16 *)0x0400004c = (i << 8) | i;
        Func_080030f8(1);
    }

    *(volatile u16 *)0x04000000 = 1;
    Func_080030f8(4);
    reg = (volatile u16 *)0x04000008;
    *reg++ = bg0cnt;
    *reg++ = bg1cnt;
    *reg++ = bg2cnt;
    *reg = bg3cnt;
    return 0;
}
