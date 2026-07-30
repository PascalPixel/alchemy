typedef signed int s32;

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
} Obj;

extern s32 Data_02000240[];

extern Obj *Func_020040f4(s32);
extern void Func_02004054(s32, s32, s32, s32, s32, s32);
extern void Func_020007a2(s32, s32, s32);
extern Obj *Func_02004124(s32);
extern void Func_02004078(s32, s32, s32, s32, s32, s32);

void Func_0200042c(void)
{
    Obj *o;
    s32 v;
    s32 t;

    o = Func_020040f4(Data_02000240[125]);
    v = o->f10 >> 20;
    t = -48;
    if (v <= 8) {
        t = 48;
    }
    Func_02004054(67, 8, 3, 1, 64, v);
    Func_020007a2(17, 0, t);
    o = Func_02004124(17);
    v = o->f10 >> 20;
    Func_02004078(64, 24, 3, 1, 64, v);
}
