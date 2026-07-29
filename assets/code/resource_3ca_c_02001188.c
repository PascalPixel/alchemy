typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

extern void Func_020024ba(void *);
extern void Func_020024c0(void *);
extern u16 Data_02008f31[];
extern u16 Data_02008f81[];

void Func_02001188(void)
{
    volatile u16 *reg;

    Func_020024ba(Data_02008f31);
    Func_020024c0(Data_02008f81);
    reg = (volatile u16 *)0x040000B0;
    reg[5] = 0xC5FF & reg[5];
    reg[5] = 0x7FFF & reg[5];
    reg[5];
}
