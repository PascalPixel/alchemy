typedef signed int s32;
typedef unsigned int u32;

extern s32 Data_0200d7fc;
extern s32 Func_02006840(s32);
extern void Func_020041b6(s32, s32);
extern s32 Func_0200684c(s32);
extern void Func_020041c2(s32, s32);
extern s32 Func_02006858(s32);
extern void Func_020041ce(s32, s32);
extern s32 Func_02006864(s32);
extern void Func_020041da(s32, s32);
extern s32 Func_02006872(s32);
extern void Func_020041e8(s32, s32);
extern s32 Func_0200687e(s32);
extern void Func_020041f4(s32, s32);
extern s32 Func_0200688a(s32);
extern void Func_02004200(s32, s32);
extern s32 Func_02006896(s32);
extern void Func_0200420c(s32, s32);

void Func_020020fc(void)
{
    if (((*(u32 *)0x03001E40 >> Data_0200d7fc) & 3) != 0) {
        Func_020041b6(Func_02006840(32), 1);
        Func_020041c2(Func_0200684c(33), 1);
        Func_020041ce(Func_02006858(30), 1);
        Func_020041da(Func_02006864(29), 1);
    } else {
        Func_020041e8(Func_02006872(32), 8);
        Func_020041f4(Func_0200687e(33), 8);
        Func_02004200(Func_0200688a(30), 8);
        Func_0200420c(Func_02006896(29), 8);
    }
}
