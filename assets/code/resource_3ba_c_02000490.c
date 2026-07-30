typedef signed int s32;
typedef unsigned int u32;

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
} Obj;

extern s32 Data_02000240[];
extern volatile u32 Data_03001ae8;

extern Obj *Func_02004158(s32);
extern Obj *Func_0200417e(s32);
extern void Func_02004100(s32, s32, s32, s32, s32, s32);
extern void Func_0200084e(s32, s32, s32);
extern Obj *Func_020041d0(s32);
extern void Func_02004124(s32, s32, s32, s32, s32, s32);

void Func_02000490(void)
{
    Obj *o;
    s32 a;
    s32 b;
    s32 s;

    o = Func_02004158(Data_02000240[125]);
    a = o->f08 >> 20;
    if ((Data_03001ae8 & 32) != 0) {
        s = -1;
    }
    if ((Data_03001ae8 & 16) != 0) {
        s = 1;
    }
    o = Func_0200417e(17);
    b = o->f10 >> 20;
    if (a == 63) {
        if (b == 11) {
            return;
        }
        b = 160;
    } else if (a == 67) {
        if (b == 11 && s == -1) {
            return;
        }
        b = 96;
    } else {
        if (b == 11) {
            b = 96;
        } else {
            b = 160;
        }
        b = -b;
    }
    Func_02004100(72, 9, 1, 3, a, 9);
    Func_0200084e(18, b, 0);
    o = Func_020041d0(18);
    a = o->f08 >> 20;
    Func_02004124(63, 25, 1, 3, a, 9);
}
