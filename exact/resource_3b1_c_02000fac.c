#include "types.h"

s32 Func_02007454(s32 n);
u8 *Func_02002298(void);
void Func_0200748a(void);
void Func_02002158(u8 *o);
void Func_02007576(s32 n);
void Func_02005840(s32 n);
void Func_0200752c(u8 *o, s32 n);
u8 *Func_020074ca(s32 n);
void Func_0200750c(u8 *o, s32 a, s32 b);
void Func_0200753a(u8 *o);
void Func_0200754c(u8 *o, s32 a, s32 b);
void Func_020074d8(void);
s32 Func_020074b0(s32 n);
s32 Func_020074c0(s32 n);
s32 Func_020074d0(s32 n);
void Func_02002842(s32 n, s32 a, s32 b);
void Func_0200284e(s32 n, s32 a, s32 b);

extern u8 Value_00001ea5;
extern u8 Value_0000092b;
extern u8 Value_00000997;
extern u8 Value_00001e8e;
extern u8 Value_0000092a;
extern u8 Value_0000091e;
extern u8 Value_00000929;
extern u8 Value_0000093c;
extern u8 Value_00000933;

void Func_02000fac(void) {
    if (Func_02007454(0x300) != 0) {
        u8 *o = Func_02002298();
        u8 *p;

        Func_0200748a();
        Func_02002158(o);
        Func_02007576((s32)&Value_00001ea5);
        Func_02005840(15);
        Func_0200752c(o, 2);
        p = Func_020074ca(0);
        if (p != 0) {
            Func_0200750c(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Func_0200753a(o);
        Func_0200754c(o, 0, 0);
        Func_020074d8();
    } else if (Func_020074b0((s32)&Value_0000092b) != 0) {
        Func_02002842(15, (s32)&Value_00001e8e, (s32)&Value_00000997);
    } else if (Func_020074c0((s32)&Value_0000092a) != 0) {
        Func_02002842(15, (s32)&Value_00001e8e, (s32)&Value_0000091e);
    } else if (Func_020074d0((s32)&Value_00000929) != 0) {
        Func_02002842(15, (s32)&Value_00001e8e, (s32)&Value_0000093c);
    } else {
        Func_0200284e(15, (s32)&Value_00001e8e, (s32)&Value_00000933);
    }
}
