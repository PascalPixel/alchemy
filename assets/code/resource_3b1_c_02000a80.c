#include "types.h"

s32 Func_02006f28(s32 n);
void Func_02006f58(void);
void Func_0200708a(s32 a, s32 b);
void Func_02006f60(s32 n);
void Func_0200704e(s32 n);
void Func_02007066(s32 a, s32 b);
void Func_02006f82(void);
s32 Func_02006f5c(s32 n);
u8 *Func_02001da0(void);
void Func_02006f92(void);
void Func_02001c60(u8 *o);
void Func_0200707e(s32 n);
void Func_02005348(s32 n);
void Func_02007034(u8 *o, s32 n);
u8 *Func_02006fd2(s32 n);
void Func_02007014(u8 *o, s32 a, s32 b);
void Func_02007042(u8 *o);
void Func_02007054(u8 *o, s32 a, s32 b);
void Func_02006fe0(void);
s32 Func_02006fb8(s32 n);
s32 Func_02006fc8(s32 n);
s32 Func_02006fd8(s32 n);
void Func_0200234a(s32 n, s32 a, s32 b);
void Func_02002356(s32 n, s32 a, s32 b);

extern u8 Value_00001f47;
extern u8 Value_00001ea0;
extern u8 Value_0000092b;
extern u8 Value_00001e7e;
extern u8 Value_00000993;
extern u8 Value_0000092a;
extern u8 Value_0000091a;
extern u8 Value_00000929;
extern u8 Value_00000938;
extern u8 Value_0000092f;

void Func_02000a80(void) {
    if (Func_02006f28(0x8A0) != 0) {
        Func_02006f58();
        Func_0200708a(11, 0x102);
        Func_02006f60(40);
        Func_0200704e((s32)&Value_00001f47);
        Func_02007066(11, 0);
        Func_02006f82();
    } else if (Func_02006f5c(0x300) != 0) {
        u8 *o = Func_02001da0();
        u8 *p;

        Func_02006f92();
        Func_02001c60(o);
        Func_0200707e((s32)&Value_00001ea0);
        Func_02005348(11);
        Func_02007034(o, 2);
        p = Func_02006fd2(0);
        if (p != 0) {
            Func_02007014(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Func_02007042(o);
        Func_02007054(o, 0, 0);
        Func_02006fe0();
    } else if (Func_02006fb8((s32)&Value_0000092b) != 0) {
        Func_0200234a(11, (s32)&Value_00001e7e, (s32)&Value_00000993);
    } else if (Func_02006fc8((s32)&Value_0000092a) != 0) {
        Func_0200234a(11, (s32)&Value_00001e7e, (s32)&Value_0000091a);
    } else if (Func_02006fd8((s32)&Value_00000929) != 0) {
        Func_0200234a(11, (s32)&Value_00001e7e, (s32)&Value_00000938);
    } else {
        Func_02002356(11, (s32)&Value_00001e7e, (s32)&Value_0000092f);
    }
}
