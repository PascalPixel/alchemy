typedef signed int s32;

struct S {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern struct S *Func_02004eec(s32 arg0);
extern struct S *Func_02004ef6(s32 arg0);
extern struct S *Func_02004f08(s32 arg0);
extern struct S *Func_02004f1e(s32 arg0);
extern struct S *Func_02004f4c(s32 arg0);
extern void Func_0200361e(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0200362c(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02004e76(s32 arg0);
extern void Func_02004f02(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void Func_02004f12(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);

void Func_02001e5c(void) {
    s32 x = Func_02004eec(11)->f08 >> 20;
    s32 y = Func_02004ef6(11)->f10 >> 20;

    if (x != 30) {
        if (x == 34) {
            if ((Func_02004f08(10)->f10 >> 20) == 18) {
                return;
            }
            Func_0200361e(11, -64, 0);
        } else if (x == 36) {
            if ((Func_02004f1e(10)->f10 >> 20) == 18) {
                Func_0200361e(11, -32, 0);
            } else {
                Func_0200362c(11, -96, 0);
            }
        }
        Func_02004e76(2);
        {
            s32 k = Func_02004f4c(11)->f08 >> 20;
            s32 m = y - 1;

            Func_02004f02(x, m, 1, 3, k, m);
        }
        Func_02004f12(0, 0, 1, 3, x, y - 1);
    }
}
