#include "types.h"

#define ObjectGroup_SetChildValueUnlessFifteen Func_0800b684

struct Child_0800b684 {
    u8 filler0[5];
    u8 value;
};

struct Parent_0800b684 {
    u8 filler0[37];
    u8 dirty;
    u8 filler38;
    u8 count;
    struct Child_0800b684 *children[1];
};

void ObjectGroup_SetChildValueUnlessFifteen(struct Parent_0800b684 *parent, u32 value)
{
    if (parent != 0) {
        u32 n = parent->count;

        if (n != 0) {
            struct Child_0800b684 **p = parent->children;
            u32 cnt = n;
            do {
                struct Child_0800b684 *child = *p++;
                if (child->value != 15)
                    child->value = value;
                cnt--;
            } while (cnt != 0);
        }
        parent->dirty = 1;
    }
}
