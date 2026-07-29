typedef unsigned char u8;
typedef signed short s16;

extern u8 Data_0200abed[];
extern s16 Data_0200c6a6;

extern void Func_02006906(u8 *);
extern void Func_02006960(s16);

void Func_02002de8(void)
{
    Func_02006906(Data_0200abed);
    Func_02006960(Data_0200c6a6);
    Data_0200c6a6 = -1;
}
