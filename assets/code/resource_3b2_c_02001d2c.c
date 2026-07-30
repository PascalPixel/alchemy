typedef signed int s32;

struct S {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern struct S *Func_02004dbc(s32 arg0);
extern struct S *Func_02004dc6(s32 arg0);
extern struct S *Func_02004dd0(s32 arg0);
extern struct S *Func_02004dda(s32 arg0);
extern struct S *Func_02004e38(s32 arg0);
extern void Func_020034f0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02003512(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02004d62(s32 arg0);
extern void Func_02004dee(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void Func_02004dfe(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);

void Func_02001d2c(void) {
    s32 x = Func_02004dbc(10)->f08 >> 20;
    s32 y = Func_02004dc6(10)->f10 >> 20;
    s32 z = Func_02004dd0(13)->f08 >> 20;
    s32 w = Func_02004dda(15)->f08 >> 20;

    if (y == 18) {
        if (w >= 31 && w <= 33) {
            Func_02003512(10, 0, -128);
        } else if (z >= 31 && z <= 33) {
            Func_02003512(10, 0, -128);
        } else {
            Func_020034f0(10, 0, -112);
            Func_02003512(10, 0, -64);
        }
    } else if (y == 10) {
        if (w >= 31 && w <= 33) {
            return;
        }
        if (z >= 31 && z <= 33) {
            return;
        }
        Func_02003512(10, 0, -48);
    } else if (y == 7) {
        return;
    }
    Func_02004d62(2);
    {
        s32 k = Func_02004e38(10)->f10 >> 20;
        s32 m = x - 1;

        Func_02004dee(m, y, 3, 1, m, k);
    }
    Func_02004dfe(0, 0, 3, 1, x - 1, y);
}
