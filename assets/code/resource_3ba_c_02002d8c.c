typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

extern u8 Data_0200abed[];
extern u16 Data_0200c760;
extern u16 Data_0200c800;
extern u16 Data_0200c7f4;
extern u16 Data_0200c7a4;
extern u16 Data_0200c780;
extern u16 Data_0200c7bc;
extern u16 Data_0200c78c;
extern u16 Data_0200c750;

extern void Func_020068c8(u8 *, s32);

void Func_02002d8c(s32 a, s32 b, s32 c)
{
    Data_0200c760 = a;
    Data_0200c800 = b;
    Data_0200c7a4 = Data_0200c7f4;
    Data_0200c7bc = Data_0200c780;
    Data_0200c78c = c;
    Data_0200c750 = 0;
    {
        s32 n = 3200;

        Func_020068c8(Data_0200abed, n);
    }
}
