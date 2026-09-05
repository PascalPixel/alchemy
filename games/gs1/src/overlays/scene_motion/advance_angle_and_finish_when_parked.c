#include "types.h"

#define SceneEffect_AdvanceAngleAndFinishWhenParked Func_02002798

struct Struct2798 {
    u8 pad00[0x18];
    s32 field18;
    u8 pad1c[0x38 - 0x1c];
    s32 field38;
    s32 field3c;
    s32 field40;
};

extern void Func_02005376(struct Struct2798 *p);

s32 SceneEffect_AdvanceAngleAndFinishWhenParked(struct Struct2798 *p) {
    p->field18 += 0x1eb8;
    if (p->field38 == (s32)0x80000000
        && p->field3c == (s32)0x80000000
        && p->field40 == (s32)0x80000000) {
        Func_02005376(p);
    }
    return 1;
}
