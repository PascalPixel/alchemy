#include "types.h"

struct Obj {
    u8 filler00[6];
    u16 f06;
};

extern s16 Data_02000240[];

extern s32 Func_02005432(s32);
extern void Func_020052a4(s32);
extern s32 Func_02005442(s32);
extern struct Obj *Func_02005494(s32);
extern void Func_02003f50(void);

void Func_02001858(void) {
    struct Obj *o;

    if (Func_02005432(0xfd6) == 0) {
        Func_020052a4(12);
    }
    if (Func_02005442(0x915) != 0) {
        o = Func_02005494(8);
        o->f06 = 0;
    }
    if (Data_02000240[225] == 10) {
        Func_02003f50();
    }
}
