typedef unsigned char u8;
typedef signed int s32;

struct O {
    s32 pad0[20];
    u8 *f50;
    u8 f54;
    u8 f55;
};

extern struct O *Func_020092a8(s32);
extern void Func_020092c2(s32, s32, s32);
extern void Func_02009306(s32, s32, s32);
extern void Func_0200939a(s32, s32, s32);
extern void Func_02009344(s32, s32);
extern void Func_020092f6(s32, s32, s32);
extern void Func_0200932a(s32, s32, s32);
extern void Func_020092d0(s32);
extern void Func_02009368(s32, s32);
extern void Func_0200931e(s32, s32, s32);
extern void Func_02009352(s32, s32, s32);
extern void Func_02009306_b(s32);
extern void Func_0200939e(s32, s32);

void Func_020032b0(s32 a, s32 b, s32 c, s32 d)
{
    struct O *o = Func_020092a8(a);
    u8 *q = o->f50;
    u8 *r;
    Func_020092c2(a, 0x10000, 0x8000);
    Func_02009306(a, 0x188, 0x376);
    Func_0200939a(0, 0xc000, 10);
    q += 0x26;
    r = (u8 *)o + 0x55;
    *r = 0;
    *q = 0;
    Func_02009344(a, b);
    Func_020092f6(a, 0x4ccc, 0x2666);
    Func_0200932a(a, 0x188, 0x36b);
    Func_020092d0(10);
    Func_02009368(a, c);
    Func_0200931e(a, 0x10000, 0x8000);
    Func_02009352(a, 0x188, 0x35b);
    *q = 1;
    if (d) {
        *r = 3;
    }
    Func_02009306_b(10);
    Func_0200939e(a, 1);
}
