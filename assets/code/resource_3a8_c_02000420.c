typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

struct Obj {
    u8 filler00[6];
    u16 f06;
};

extern struct Obj *Func_02004042(s32);
extern s32 Func_020041b2(s32, s32);
extern void Func_02004038(void);
extern void Func_02004106(s32);
extern void Func_0200411e(s32, s32);
extern void Func_02004052(void);

void Func_02000420(void) {
    struct Obj *o;
    u32 v;

    o = Func_02004042(0);
    v = (o->f06 - 0x2000) << 16;
    if (v > 0xC0000000) {
        Func_020041b2(24, 24);
    } else {
        Func_02004038();
        Func_02004106(0x1ad5);
        Func_0200411e(24, 0);
        Func_02004052();
    }
}
