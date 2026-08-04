#include "types.h"

struct Slot02000f10 {
    u8 head[8];
    s32 x;
    s32 y;
    s32 rank;
};

extern struct Slot02000f10 *Func_02004022(s32);
extern struct Slot02000f10 *Func_0200402a(s32);
extern void Func_02003f68(s32);

void Func_02000f10(s32 first, s32 second)
{
    struct Slot02000f10 *a = Func_02004022(first);
    struct Slot02000f10 *b = Func_0200402a(second);

    if (a->rank <= b->rank) {
        s32 t;

        t = a->x;    a->x    = b->x;    b->x    = t;
        t = a->y;    a->y    = b->y;    b->y    = t;
        t = a->rank; a->rank = b->rank; b->rank = t;
        Func_02003f68(1);
    }
}
