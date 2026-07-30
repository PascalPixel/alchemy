typedef signed int s32;

extern s32 *Func_02005678();
extern void Func_0200572a();
extern void Func_02005732();
extern void Func_02005626();
extern void Func_02005638();
extern void Func_02005650();
extern s32 *Func_020056ce();
extern void Func_0200566e();

void Func_02002820(void) {
    s32 *p = Func_02005678(8);

    Func_0200572a(8, 1);
    Func_02005732(9, 1);
    {
        s32 k5 = 5, k6 = 19;

        Func_02005626(69, 19, 3, 3, k5, k6);
    }
    {
        s32 k5 = 17, k6 = 19;

        Func_02005638(69, 19, 3, 3, k5, k6);
    }
    {
        s32 k5 = p[2] >> 20, k6 = p[4] >> 20;

        Func_02005650(3, 3, 1, 1, k5, k6);
    }
    {
        s32 *q = Func_020056ce(9);
        s32 k5 = q[2] >> 20, k6 = q[4] >> 20;

        Func_0200566e(3, 3, 1, 1, k5, k6);
    }
}
