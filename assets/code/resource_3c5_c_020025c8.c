typedef signed int s32;
typedef signed short s16;
typedef unsigned char u8;

extern u8 *Data_03001ebc;
extern void Func_0200540a();
extern s32 Func_020053fc();
extern void Func_02005400();
extern void Func_02005586();
extern void Func_020053ea();
extern void Func_02005440();
extern void Func_02005456();
extern void Func_02004afa();
extern void Func_0200550a();
extern void Func_02005524();
extern void Func_02005480();

void Func_020025c8(void) {
    u8 *base = Data_03001ebc;
    s16 *h;

    Func_0200540a();
    h = (s16 *)(base + 0xcb8);
    if (h[0] != 0) {
        if (Func_020053fc(0x985) == 0) {
            s32 k5 = 17, k6 = 78;

            Func_02005400(0x1528, 1);
            Func_02005586(155);
            Func_020053ea(35, 78, 1, 2, k5, k6);
            Func_02005440(10);
            Func_02005400(34, 78, 1, 2, k5, k6);
            Func_02005456(10);
            Func_02004afa();
        }
    } else {
        Func_0200550a(0x2756);
        Func_02005524(-1, 0);
    }
    Func_02005480();
}
