typedef signed int s32;

struct S {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern struct S *Func_02004f84(s32 arg0);
extern struct S *Func_02004f8e(s32 arg0);
extern struct S *Func_02004fa0(s32 arg0);
extern struct S *Func_02004fce(s32 arg0);
extern void Func_0200369e(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020036ae(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02004ef8(s32 arg0);
extern void Func_02004f84_b(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void Func_02004f94(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);

void Func_02001ef4(void) {
    s32 x = Func_02004f84(11)->f08 >> 20;
    s32 y = Func_02004f8e(11)->f10 >> 20;

    if (x != 36) {
        if (x == 30) {
            if ((Func_02004fa0(10)->f10 >> 20) == 18) {
                return;
            }
            Func_0200369e(11, 96, 0);
        } else if (x == 34) {
            Func_020036ae(11, 32, 0);
        }
        Func_02004ef8(2);
        {
            s32 k = Func_02004fce(11)->f08 >> 20;
            s32 m = y - 1;

            Func_02004f84_b(x, m, 1, 3, k, m);
        }
        Func_02004f94(0, 0, 1, 3, x, y - 1);
    }
}
