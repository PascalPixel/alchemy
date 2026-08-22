#include "types.h"

struct V6 {
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    s32 e;
    s32 f;
};

extern void Func_020047b6(void);
extern s32 Func_02001bc2(struct V6 *arg0);
extern void Func_02001d6e(struct V6 arg0);
extern void Func_020047e2(void);

void Func_02001740(void) {
    struct V6 v;

    Func_020047b6();
    if (Func_02001bc2(&v) != 0) {
        Func_02001d6e(v);
    }
    Func_020047e2();
}
