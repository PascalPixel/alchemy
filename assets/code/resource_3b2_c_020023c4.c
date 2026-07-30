typedef signed int s32;

struct S {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern struct S *Func_02005454(s32 arg0);
extern struct S *Func_0200545e(s32 arg0);
extern struct S *Func_02005468(s32 arg0);
extern struct S *Func_020054a6(s32 arg0);
extern void Func_02003b80(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020053d0(s32 arg0);
extern void Func_0200545c(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void Func_0200546c(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);

void Func_020023c4(void) {
    s32 x = Func_02005454(17)->f08 >> 20;
    s32 y = Func_0200545e(17)->f10 >> 20;
    s32 z = Func_02005468(19)->f08 >> 20;

    if (y == 19) {
        if (z >= 3 && z <= 5) {
            Func_02003b80(17, 0, -16);
        } else {
            Func_02003b80(17, 0, -64);
        }
    } else if (y == 18) {
        if (z >= 3 && z <= 5) {
            return;
        }
        Func_02003b80(17, 0, -48);
    } else if (y == 15) {
        return;
    }
    Func_020053d0(2);
    {
        s32 k = Func_020054a6(17)->f10 >> 20;
        s32 m = x - 1;

        Func_0200545c(m, y, 3, 1, m, k);
    }
    Func_0200546c(0, 0, 3, 1, x - 1, y);
}
