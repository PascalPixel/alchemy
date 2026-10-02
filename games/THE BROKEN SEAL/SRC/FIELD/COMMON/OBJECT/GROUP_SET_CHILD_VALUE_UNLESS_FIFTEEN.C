#include "TYPES.H"
#include "SCENE.H"
#include "OBJECT_DISPATCH.H"

struct AnimationChildValue {
    u8 filler0[5];
    u8 value;
};

struct AnimationChildGroup {
    u8 filler0[37];
    u8 dirty;
    u8 filler38;
    u8 count;
    struct AnimationChildValue *children[1];
};

void ObjectGroup_SetChildValueUnlessFifteen(void *raw_parent, u32 value)
{
    struct AnimationChildGroup *parent = raw_parent;
    if (parent != 0) {
        u32 n = parent->count;

        if (n != 0) {
            struct AnimationChildValue **p = parent->children;
            u32 cnt = n;
            do {
                struct AnimationChildValue *child = *p++;
                if (child->value != 15)
                    child->value = value;
                cnt--;
            } while (cnt != 0);
        }
        parent->dirty = 1;
    }
}
