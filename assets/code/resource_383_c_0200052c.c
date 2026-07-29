typedef unsigned char u8;
typedef signed int s32;

extern void Func_02005274(void);
extern s32 Func_0200524a(s32);
extern s32 Func_02005254(s32);
extern void Func_0200535e(s32);
extern void Func_020008a4(s32);
extern void Func_02005292(s32);
extern void Func_02004b9c(s32, s32, s32);
extern void Func_02005282(s32);
extern void Func_02005382(s32);
extern void Func_0200538a(s32);
extern void Func_020008d0(s32);
extern void Func_020052cc(void);

void Func_0200052c(void)
{
    s32 g;
    g = 0x851;
    Func_02005274();
    if (Func_0200524a(0x856) != 0) {
        if (Func_02005254(g) == 0) {
            Func_0200535e(0x1276);
            Func_020008a4(16);
            Func_02005292(10);
            Func_02004b9c(16, 3, 20);
            Func_02005282(g);
        } else {
            Func_02005382(0x1278);
        }
    } else {
        Func_0200538a(0x1250);
    }
    Func_020008d0(16);
    Func_020052cc();
}
