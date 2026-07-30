typedef signed int s32;
typedef unsigned char u8;

extern u8 Data_0200c550[];

extern void Func_020081dc(void);
extern void Func_02008212(s32, s32, s32);
extern void Func_02008358(s32);
extern void Func_0200817a(s32, s32, s32);
extern void Func_02008266(s32, s32, s32);
extern void Func_02008204(s32);
extern void Func_02008312(s32);
extern void Func_0200821e(void);

void Func_02003df8(void)
{
    Func_020081dc();
    Func_02008212(0, 0x8000, 0x4000);
    Func_02008358(158);
    Func_0200817a((s32)Data_0200c550, 36, 10);
    Func_02008266(0, 2, -16);
    Func_02008204(16);
    Func_02008312(2);
    Func_0200821e();
}
