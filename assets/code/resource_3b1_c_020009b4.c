#include "types.h"

s32 Func_02006e5c(s32 n);
u8 *Func_02001ca0(void);
void Func_02006e92(void);
void Func_02001b60(u8 *o);
void Func_02006f7e(s32 n);
void Func_02005248(s32 n);
void Func_02006f34(u8 *o, s32 n);
u8 *Func_02006ed2(s32 n);
void Func_02006f14(u8 *o, s32 a, s32 b);
void Func_02006f42(u8 *o);
void Func_02006f54(u8 *o, s32 a, s32 b);
void Func_02006ee0(void);
s32 Func_02006eb8(s32 n);
s32 Func_02006ec8(s32 n);
s32 Func_02006ed8(s32 n);
void Func_0200224a(s32 n, s32 a, s32 b);
void Func_02002256(s32 n, s32 a, s32 b);

extern u8 Value_00001e9f;
extern u8 Value_0000092b;
extern u8 Value_00000992;
extern u8 Value_00001e7b;
extern u8 Value_0000092a;
extern u8 Value_00000919;
extern u8 Value_00000929;
extern u8 Value_00000937;
extern u8 Value_0000092e;

void Func_020009b4(void) {
    if (Func_02006e5c(0x300) != 0) {
        u8 *o = Func_02001ca0();
        u8 *p;

        Func_02006e92();
        Func_02001b60(o);
        Func_02006f7e((s32)&Value_00001e9f);
        Func_02005248(10);
        Func_02006f34(o, 2);
        p = Func_02006ed2(0);
        if (p != 0) {
            Func_02006f14(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Func_02006f42(o);
        Func_02006f54(o, 0, 0);
        Func_02006ee0();
    } else if (Func_02006eb8((s32)&Value_0000092b) != 0) {
        Func_0200224a(10, (s32)&Value_00001e7b, (s32)&Value_00000992);
    } else if (Func_02006ec8((s32)&Value_0000092a) != 0) {
        Func_0200224a(10, (s32)&Value_00001e7b, (s32)&Value_00000919);
    } else if (Func_02006ed8((s32)&Value_00000929) != 0) {
        Func_0200224a(10, (s32)&Value_00001e7b, (s32)&Value_00000937);
    } else {
        Func_02002256(10, (s32)&Value_00001e7b, (s32)&Value_0000092e);
    }
}
