typedef signed int s32;
typedef unsigned char u8;

typedef struct Obj {
    u8 pad0[0x10];
    s32 z;          /* 0x10 */
} Obj;

Obj *Func_02006244(s32);
void Func_020061fc(s32, s32, s32);
void Func_0200620a(s32, s32, s32);
void Func_020063c8(s32);
void Func_0200620c(s32, s32, s32, s32, s32, s32);
void Func_0200621e(s32, s32, s32, s32, s32, s32);
u8 *Func_0200629c(s32);
void Func_02006240(u8 *, s32);
void Func_0200627e(s32);

void Func_02000bec(void)
{
    if (Func_02006244(12)->z >> 20 > 22) {
        s32 f = 15;
        u8 *p;

        Func_020061fc(0x40000, 0x40000, 0x10000);
        Func_0200620a(-1, -1, 0xe666);
        Func_020063c8(0x90);
        Func_0200620c(15, 20, 1, 1, f, 22);
        Func_0200621e(17, 23, 1, 3, f, 23);
        p = Func_0200629c(12);
        if (p != 0) {
            Func_02006240(p, 0);
            p[0x23] = 2;
        }
        Func_0200627e(0x943);
    }
}
