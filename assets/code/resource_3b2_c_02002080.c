typedef signed int s32;

struct S {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern struct S *Func_02005110(s32 arg0);
extern struct S *Func_0200511a(s32 arg0);
extern struct S *Func_02005124(s32 arg0);
extern struct S *Func_0200512e(s32 arg0);
extern struct S *Func_020051c4(s32 arg0);
extern void Func_02003846(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02003870(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0200389e(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020050ee(s32 arg0);
extern void Func_0200517a(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void Func_0200518a(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);

void Func_02002080(void) {
    s32 x = Func_02005110(13)->f08 >> 20;
    s32 y = Func_0200511a(13)->f10 >> 20;
    s32 z = Func_02005124(10)->f10 >> 20;
    s32 w = Func_0200512e(15)->f08 >> 20;

    if (x == 36) {
        if (w == 34) {
            Func_0200389e(13, -16, 0);
        } else if (z == 7) {
            Func_0200389e(13, -32, 0);
        } else if (w == 30) {
            Func_0200389e(13, -80, 0);
        } else {
            Func_02003846(13, -96, 0);
            Func_0200389e(13, -80, 0);
        }
    } else if (x == 35) {
        if (w == 34) {
            return;
        } else if (z == 7) {
            Func_0200389e(13, -16, 0);
        } else if (w == 30) {
            Func_0200389e(13, -64, 0);
        } else {
            Func_02003870(13, -80, 0);
            Func_0200389e(13, -80, 0);
        }
    } else if (x == 34) {
        if (z == 7) {
            return;
        }
        if (w == 30) {
            Func_0200389e(13, -48, 0);
        } else {
            Func_0200389e(13, -144, 0);
        }
    } else if (x == 31) {
        if (w == 30) {
            return;
        }
        Func_0200389e(13, -96, 0);
    } else if (x == 25) {
        return;
    }
    Func_020050ee(2);
    {
        s32 k = Func_020051c4(13)->f08 >> 20;
        s32 m = y - 1;

        Func_0200517a(x, m, 1, 3, k, m);
    }
    Func_0200518a(0, 0, 1, 3, x, y - 1);
}
