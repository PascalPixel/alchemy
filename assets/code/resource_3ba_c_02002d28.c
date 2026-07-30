typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

extern u8 Data_0200abed[];
extern u16 Data_0200c7f4;
extern u16 Data_0200c780;
extern u16 Data_0200c758;
extern u16 Data_0200c774;
extern u16 Data_0200c78c;

extern void Func_02005902(void);
extern void Func_0200686c(u8 *, s32);

void Func_02002d28(s32 a, s32 b, s32 c)
{
    Func_02005902();
    Data_0200c7f4 = a;
    Data_0200c780 = b;
    Data_0200c758 = c & 3;
    Data_0200c774 = 0;
    Data_0200c78c = 0;
    {
        s32 n = 3200;

        Func_0200686c(Data_0200abed, n);
    }
}
