typedef signed int s32;

struct S {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern struct S *Func_02005358(s32 arg0);
extern struct S *Func_02005362(s32 arg0);
extern struct S *Func_0200536c(s32 arg0);
extern struct S *Func_02005376(s32 arg0);
extern struct S *Func_0200541c(s32 arg0);
extern void Func_02003a86(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02003a9a(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02003aaa(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02003af6(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005346(s32 arg0);
extern void Func_020053d2(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void Func_020053e2(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);

void Func_020022c8(void) {
    s32 x = Func_02005358(15)->f08 >> 20;
    s32 y = Func_02005362(15)->f10 >> 20;
    s32 z = Func_0200536c(10)->f10 >> 20;
    s32 w = Func_02005376(13)->f08 >> 20;

    if (x == 24) {
        if (z == 7 || w == 31) {
            Func_02003af6(15, 96, 0);
        } else if (w == 34) {
            Func_02003a86(15, 64, 0);
            Func_02003af6(15, 80, 0);
        } else if (w == 35) {
            Func_02003a9a(15, 80, 0);
            Func_02003af6(15, 80, 0);
        } else {
            Func_02003aaa(15, 80, 0);
            Func_02003af6(15, 96, 0);
        }
    } else if (x == 30 || w == 31) {
        if (z == 7) {
            return;
        }
        if (w == 34) {
            Func_02003af6(15, 48, 0);
        } else if (w == 35) {
            Func_02003af6(15, 64, 0);
        } else {
            Func_02003af6(15, 80, 0);
        }
    } else if (x == 33) {
        if (w == 34) {
            return;
        }
        if (w == 35) {
            Func_02003af6(15, 16, 0);
        } else {
            Func_02003af6(15, 32, 0);
        }
    } else if (x == 34) {
        Func_02003af6(15, 16, 0);
    } else if (x == 35) {
        return;
    }
    Func_02005346(2);
    {
        s32 k = Func_0200541c(15)->f08 >> 20;
        s32 m = y - 1;

        Func_020053d2(x, m, 1, 3, k, m);
    }
    Func_020053e2(0, 0, 1, 3, x, y - 1);
}
