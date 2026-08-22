#include "object_dispatch.h"

void Func_0800baf8(void *child, s32 value);

void Func_0800c344(struct DispatchObject *object, s32 value)
{
    s32 count;
    void *child;
    void **children;

    if (object != 0) {
        switch (object->kind & 0xf) {
        case 1:
            Func_0800baf8(object->target.child, value);
            return;
        case 2:
            children = object->target.children;
            count = 3;
            do {
                child = *children++;
                if (child != 0)
                    Func_0800baf8(child, value);
                count--;
            } while (count >= 0);
            break;
        }
    }
}
