#include "types.h"

struct ChildObject {
    u8 unknown_00[2];
    u16 value;
    u8 unknown_04[8];
    s32 active;
    u8 unknown_10[5];
    u8 mode;
};

struct ChildGroup {
    u8 unknown_00[27];
    u8 count;
    u8 unknown_1c[12];
    struct ChildObject *children[1];
};

s32 Func_08022b80(struct ChildGroup *group, s32 value)
{
    s32 i;

    for (i = 0; i < group->count; i++) {
        struct ChildObject *child = group->children[i];

        if (child != NULL && child->active != 0) {
            child->value = value << 4;
            /* FAKEMATCH: ⚓️ stores the value before it reloads the count */
            asm volatile("");
        }
    }
    return 0;
}

void Animation_ApplyChildValue(struct ChildGroup *group, s32 value)
{
    s32 i;

    for (i = 0; i < group->count; i++) {
        struct ChildObject *child = group->children[i];

        if (child != NULL && child->active != 0)
            *((u8 *)child + 21) = value;
    }
}
