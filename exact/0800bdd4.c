#include "types.h"

s32 Func_08003f3c(u32 index);
void Func_0800bc48(void *);

struct Object_0800bdd4 {
    u8 padding_00[28];
    u8 resource;
    u8 flags;
    u8 padding_1e[10];
    void *children[4];
};

#define OBJECT_0800BDD4_OFFSET(field) \
    ((u32)&(((struct Object_0800bdd4 *)0)->field))
typedef char Object_0800bdd4_resource_offset[
    OBJECT_0800BDD4_OFFSET(resource) == 0x1c ? 1 : -1
];
typedef char Object_0800bdd4_flags_offset[
    OBJECT_0800BDD4_OFFSET(flags) == 0x1d ? 1 : -1
];
typedef char Object_0800bdd4_children_offset[
    OBJECT_0800BDD4_OFFSET(children) == 0x28 ? 1 : -1
];
#undef OBJECT_0800BDD4_OFFSET

void Func_0800bdd4(struct Object_0800bdd4 *object)
{
    u32 zero;
    u32 *source;
    volatile u32 *dma;
    u32 control;
    void **slot;
    s32 remaining;

    if (object != 0) {
        if ((object->flags & 1) == 0) {
            Func_08003f3c(object->resource);
        }
        slot = object->children;
        remaining = 3;
        do {
            Func_0800bc48(*slot++);
            remaining--;
        } while (remaining >= 0);
        source = &zero;
        *source = 0;
        dma = (volatile u32 *)0x040000d4;
        control = 0x8500000e;
        dma[0] = (u32)source;
        dma[1] = (u32)object;
        dma[2] = control;
    }
}
