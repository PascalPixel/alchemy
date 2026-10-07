#include "TYPES.H"

struct ChildObject {
    u8 unknown_00[5];
    u8 mode;
    u8 unknown_06[10];
    s32 active;
};

struct ChildGroup {
    u8 unknown_00[25];
    u8 dirty;
    u8 unknown_1a;
    u8 count;
    u8 unknown_1c[12];
    struct ChildObject *children[1];
};

void Func_08191ae0(struct ChildGroup *group, s32 mode)
{
    s32 i;

    for (i = 0; i < group->count; i++) {
        struct ChildObject *child = group->children[i];

        if (child != NULL && child->active != 0)
            *((u8 *)child + 5) = mode;
    }
    group->dirty = 1;
}
