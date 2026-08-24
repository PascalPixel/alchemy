#include "types.h"

struct Obj {
    u8 pad00[6];
    u16 f06;
};

extern u8 Data_02000240[];
extern struct Obj *Func_0200558e();
extern s32 Func_02004da4();
extern s32 Func_020056ba();
extern s32 Func_020056be();
extern void Func_02004e90();

void Func_02002738(void) {
    struct Obj *p = Func_0200558e(0);
    s32 x = Func_02004da4();
    s32 m = (p->f06 + 0x2000) & 0xc000;
    s32 r = -1;

    if (Data_02000240[498] == 1 || x == 0) {
        if (m == 0xc000) {
            r = Func_020056ba();
        }
        if (m == 0x4000) {
            r = Func_020056be();
        }
    }
    if (r != 0) {
        if (Data_02000240[498] != 1) {
            Func_02004e90();
        }
    }
}
