typedef unsigned int u32;
typedef signed int s32;

extern volatile u32 Data_03001e40;
extern s32 Func_0200baf8(s32, s32);
extern void Func_0200bbf0(s32, s32);
extern void Func_0200b9f4(s32);

void Func_02005c5c(s32 p)
{
    if ((Data_03001e40 & 1) != 0)
        Func_0200bbf0(p, Func_0200baf8(Data_03001e40 >> 1, 6));
    if ((Data_03001e40 & 0xf) == 0)
        Func_0200b9f4(p);
}
