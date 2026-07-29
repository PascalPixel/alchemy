typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

s32 Func_02006d94(s32 n);
u8 *Func_02001bd8(void);
void Func_02006dca(void);
void Func_02001a98(u8 *o);
void Func_02006eb6(s32 n);
void Func_02005180(s32 n);
void Func_02006e6c(u8 *o, s32 n);
u8 *Func_02006e0a(s32 n);
void Func_02006e4c(u8 *o, s32 a, s32 b);
void Func_02006e7a(u8 *o);
void Func_02006e8c(u8 *o, s32 a, s32 b);
void Func_02006e18(void);
s32 Func_02006df0(s32 n);
s32 Func_02006e02(s32 n);
s32 Func_02006e12(s32 n);
void Func_02002184(s32 n, s32 a, s32 b);
void Func_02002190(s32 n, s32 a, s32 b);

extern u8 Value_00001e9e;
extern u8 Value_0000092b;
extern u8 Value_00001e78;
extern u8 Value_0000092a;
extern u8 Value_00000917;
extern u8 Value_00000929;
extern u8 Value_00000935;
extern u8 Value_0000092c;

void Func_020008ec(void) {
    if (Func_02006d94(0x300) != 0) {
        u8 *o = Func_02001bd8();
        u8 *p;

        Func_02006dca();
        Func_02001a98(o);
        Func_02006eb6((s32)&Value_00001e9e);
        Func_02005180(8);
        Func_02006e6c(o, 2);
        p = Func_02006e0a(0);
        if (p != 0) {
            Func_02006e4c(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Func_02006e7a(o);
        Func_02006e8c(o, 0, 0);
        Func_02006e18();
    } else if (Func_02006df0((s32)&Value_0000092b) != 0) {
        Func_02002184(8, (s32)&Value_00001e78, 0x990);
    } else if (Func_02006e02((s32)&Value_0000092a) != 0) {
        Func_02002184(8, (s32)&Value_00001e78, (s32)&Value_00000917);
    } else if (Func_02006e12((s32)&Value_00000929) != 0) {
        Func_02002184(8, (s32)&Value_00001e78, (s32)&Value_00000935);
    } else {
        Func_02002190(8, (s32)&Value_00001e78, (s32)&Value_0000092c);
    }
}
