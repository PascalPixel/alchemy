#include "types.h"

s32 Func_020072bc(s32 n);
u8 *Func_02002100(void);
void Func_020072f2(void);
void Func_02001fc0(u8 *o);
void Func_020073de(s32 n);
void Func_020056a8(s32 n);
void Func_02007394(u8 *o, s32 n);
u8 *Func_02007332(s32 n);
void Func_02007374(u8 *o, s32 a, s32 b);
void Func_020073a2(u8 *o);
void Func_020073b4(u8 *o, s32 a, s32 b);
void Func_02007340(void);
s32 Func_02007318(s32 n);
s32 Func_02007328(s32 n);
s32 Func_02007338(s32 n);
void Func_020026aa(s32 n, s32 a, s32 b);
void Func_020026b6(s32 n, s32 a, s32 b);

extern u8 Value_00001ea3;
extern u8 Value_0000092b;
extern u8 Value_00000995;
extern u8 Value_00001e88;
extern u8 Value_0000092a;
extern u8 Value_0000091c;
extern u8 Value_00000929;
extern u8 Value_0000093a;
extern u8 Value_00000931;

void Func_02000e14(void) {
    if (Func_020072bc(0x300) != 0) {
        u8 *o = Func_02002100();
        u8 *p;

        Func_020072f2();
        Func_02001fc0(o);
        Func_020073de((s32)&Value_00001ea3);
        Func_020056a8(13);
        Func_02007394(o, 2);
        p = Func_02007332(0);
        if (p != 0) {
            Func_02007374(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Func_020073a2(o);
        Func_020073b4(o, 0, 0);
        Func_02007340();
    } else if (Func_02007318((s32)&Value_0000092b) != 0) {
        Func_020026aa(13, (s32)&Value_00001e88, (s32)&Value_00000995);
    } else if (Func_02007328((s32)&Value_0000092a) != 0) {
        Func_020026aa(13, (s32)&Value_00001e88, (s32)&Value_0000091c);
    } else if (Func_02007338((s32)&Value_00000929) != 0) {
        Func_020026aa(13, (s32)&Value_00001e88, (s32)&Value_0000093a);
    } else {
        Func_020026b6(13, (s32)&Value_00001e88, (s32)&Value_00000931);
    }
}
