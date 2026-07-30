typedef signed int s32;

struct S {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern struct S *Func_020051fc(s32 arg0);
extern struct S *Func_02005206(s32 arg0);
extern void Func_02005210(s32 arg0);
extern struct S *Func_02005258(s32 arg0);
extern void Func_0200390a(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02003932(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005182(s32 arg0);
extern void Func_0200520e(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void Func_0200521e(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);

void Func_0200216c(void) {
    s32 x = Func_020051fc(13)->f08 >> 20;
    s32 y = Func_02005206(13)->f10 >> 20;

    Func_02005210(15);
    if (x == 25) {
        Func_0200390a(13, 96, 0);
        Func_02003932(13, 80, 0);
    } else if (x == 31) {
        Func_02003932(13, 80, 0);
    } else if (x == 34) {
        Func_02003932(13, 32, 0);
    } else if (x == 35) {
        Func_02003932(13, 16, 0);
    } else if (x == 36) {
        return;
    }
    Func_02005182(2);
    {
        s32 k = Func_02005258(13)->f08 >> 20;
        s32 m = y - 1;

        Func_0200520e(x, m, 1, 3, k, m);
    }
    Func_0200521e(0, 0, 1, 3, x, y - 1);
}
