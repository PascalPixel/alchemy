typedef signed int s32;
typedef unsigned short u16;
typedef unsigned char u8;

extern s32 Data_03001ae8;
extern s32 Data_03001b04;

extern void Func_020004fc(u16 *, u8 *);

void Func_020001d4(void)
{
    u16 *header = (u16 *)0x02008590;

    header[0] = Data_03001ae8;
    header[1] = Data_03001b04;
    Func_020004fc(header, (u8 *)0x020085B0);
}
