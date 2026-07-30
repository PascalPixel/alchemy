typedef unsigned int u32;
typedef signed int s32;

extern volatile u32 Data_03001e40;
extern s32 Func_0200bb34(s32, s32);
extern void Func_0200bc2c(s32, s32);

void Func_02005c98(s32 p)
{
    if ((Data_03001e40 & 1) != 0)
        Func_0200bc2c(p, Func_0200bb34(Data_03001e40 >> 1, 6));
}
