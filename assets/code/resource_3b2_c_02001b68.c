#include "types.h"

struct S { s32 pad[2]; s32 f08; s32 f0c; s32 f10; };
struct V { s32 a; s32 b; s32 c; };

extern struct S *Func_02004bf8(s32 arg0);
extern s32 Func_020035ca(struct S *arg0, struct V *arg1);

s32 Func_02001b68(void) {
    struct S *p = Func_02004bf8(0);
    struct V v;

    v.a = p->f08 + -0x200000;
    v.b = p->f0c;
    v.c = p->f10;
    return Func_020035ca(p, &v);
}
