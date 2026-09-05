#include "types.h"

struct S { s32 pad[2]; s32 f08; s32 f0c; s32 f10; };
struct V { s32 a; s32 b; s32 c; };

extern struct S *Func_02004c24(s32 arg0);
extern s32 Func_020035f8(struct S *arg0, struct V *arg1);

s32 CheckActorPathEast(void) {
    struct S *actor = Func_02004c24(0);
    struct V destination;

    destination.a = actor->f08 + 0x200000;
    destination.b = actor->f0c;
    destination.c = actor->f10;
    return Func_020035f8(actor, &destination);
}
