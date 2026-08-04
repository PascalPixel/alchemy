#include "types.h"

s32 Func_02007388(s32 n);
u8 *Func_020021cc(void);
void Func_020073be(void);
void Func_0200208c(u8 *o);
void Func_020074aa(s32 n);
void Func_02005774(s32 n);
void Func_02007460(u8 *o, s32 n);
u8 *Func_020073fe(s32 n);
void Func_02007440(u8 *o, s32 a, s32 b);
void Func_0200746e(u8 *o);
void Func_02007480(u8 *o, s32 a, s32 b);
void Func_0200740c(void);
s32 Func_020073e4(s32 n);
s32 Func_020073f4(s32 n);
s32 Func_02007404(s32 n);
void Func_02002776(s32 n, s32 a, s32 b);
void Func_02002782(s32 n, s32 a, s32 b);

extern u8 Value_00001ea4;
extern u8 Value_0000092b;
extern u8 Value_00000996;
extern u8 Value_00001e8b;
extern u8 Value_0000092a;
extern u8 Value_0000091d;
extern u8 Value_00000929;
extern u8 Value_0000093b;
extern u8 Value_00000932;

void Func_02000ee0(void) {
    if (Func_02007388(0x300) != 0) {
        u8 *o = Func_020021cc();
        u8 *p;

        Func_020073be();
        Func_0200208c(o);
        Func_020074aa((s32)&Value_00001ea4);
        Func_02005774(14);
        Func_02007460(o, 2);
        p = Func_020073fe(0);
        if (p != 0) {
            Func_02007440(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Func_0200746e(o);
        Func_02007480(o, 0, 0);
        Func_0200740c();
    } else if (Func_020073e4((s32)&Value_0000092b) != 0) {
        Func_02002776(14, (s32)&Value_00001e8b, (s32)&Value_00000996);
    } else if (Func_020073f4((s32)&Value_0000092a) != 0) {
        Func_02002776(14, (s32)&Value_00001e8b, (s32)&Value_0000091d);
    } else if (Func_02007404((s32)&Value_00000929) != 0) {
        Func_02002776(14, (s32)&Value_00001e8b, (s32)&Value_0000093b);
    } else {
        Func_02002782(14, (s32)&Value_00001e8b, (s32)&Value_00000932);
    }
}
