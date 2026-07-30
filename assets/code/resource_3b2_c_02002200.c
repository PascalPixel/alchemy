typedef signed int s32;

struct S {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern struct S *Func_02005290(s32 arg0);
extern struct S *Func_0200529a(s32 arg0);
extern struct S *Func_020052a4(s32 arg0);
extern struct S *Func_020052ae(s32 arg0);
extern struct S *Func_02005320(s32 arg0);
extern void Func_020039be(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020039d6(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020039fa(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0200524a(s32 arg0);
extern void Func_020052d6(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void Func_020052e6(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);

void Func_02002200(void) {
    s32 x = Func_02005290(15)->f08 >> 20;
    s32 y = Func_0200529a(15)->f10 >> 20;
    s32 z = Func_020052a4(8)->f10 >> 20;
    s32 w = Func_020052ae(10)->f10 >> 20;

    if (x == 35) {
        if (w == 7) {
            Func_020039fa(15, -16, 0);
        } else if (z == 7) {
            Func_020039fa(15, -112, 0);
        } else {
            Func_020039be(15, -96, 0);
            Func_020039fa(15, -80, 0);
        }
    } else if (x == 34) {
        if (w == 7) {
            return;
        }
        Func_020039d6(15, -96, 0);
        Func_020039fa(15, -64, 0);
    } else if (x == 33) {
        Func_020039fa(15, -144, 0);
    } else if (x == 31) {
        Func_020039fa(15, -80, 0);
    } else if (x == 30) {
        Func_020039fa(15, -96, 0);
    } else if (x == 24) {
        return;
    }
    Func_0200524a(2);
    {
        s32 k = Func_02005320(15)->f08 >> 20;
        s32 m = y - 1;

        Func_020052d6(x, m, 1, 3, k, m);
    }
    Func_020052e6(0, 0, 1, 3, x, y - 1);
}
