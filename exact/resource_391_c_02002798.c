#include "types.h"

struct Struct2798 {
    u8 pad00[0x18];
    s32 field18;
    u8 pad1c[0x38 - 0x1c];
    s32 field38;
    s32 field3c;
    s32 field40;
};

extern void Func_02005376(struct Struct2798 *arg0);

s32 Func_02002798(struct Struct2798 *arg0) {
    arg0->field18 += 0x1eb8;
    if (arg0->field38 == (s32)0x80000000
        && arg0->field3c == (s32)0x80000000
        && arg0->field40 == (s32)0x80000000) {
        Func_02005376(arg0);
    }
    return 1;
}
