#include "TYPES.H"
#include "SCENE.H"
#include "OBJECT_DISPATCH.H"
#include "ANIMSPR.H"

void ObjectGroup_SetChildValueUnlessFifteen(void *raw_parent, u32 value)
{
    struct AnimationObject *parent = raw_parent;
    if (parent != 0) {
        u32 n = parent->count;

        if (n != 0) {
            struct AnimationEntry **p = parent->entries;
            u32 cnt = n;
            do {
                struct AnimationEntry *child = *p++;
                if (child->param != 15)
                    child->param = value;
                cnt--;
            } while (cnt != 0);
        }
        parent->dirty = 1;
    }
}
