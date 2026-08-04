#include "types.h"

extern u8 *Data_03001e70;
extern void Func_020056c2(s32);
extern s32 Func_02005668(void);
extern s32 Func_02005674(void);
extern void Func_020056a0(void);
extern void Func_0200567e(s32);
extern void Func_0200286a(void);

extern u32 Data_0200db58;
extern u32 Data_0200db38;
extern u32 Data_0200db50[];
extern u32 Data_0200db60;

s32 Func_020013d4(void)
{
    u8 **base = &Data_03001e70;
    u8 *p = base[0] + 0x104;

    Func_020056c2(0x11C);
    *(s32 *)(base[19] + 0x1C0) = 0x209;
    *(s32 *)(p + 0x1C) = 0;
    Data_0200db58 = (u16)Func_02005668();
    Data_0200db38 = (u16)Func_02005674();
    Data_0200db50[0] = 0;
    Data_0200db50[1] = 0;
    Data_0200db60 = 0;
    Func_020056a0();
    Func_0200567e(1);
    Func_0200286a();
    return 0;
}
