#include "object_dispatch.h"

void Func_0800ba30(void *, s32);

void Func_0800c300(struct DispatchObject *object, s32 argument)
{
    void **items;
    s32 count;
    void *item;

    if (object != 0) {
        switch (object->kind & 0xf) {
        case 1:
            Func_0800ba30(object->target.child, argument);
            break;
        case 2:
            items = object->target.children;
            count = 3;
            do {
                item = *items++;
                if (item != 0) {
                    Func_0800ba30(item, argument);
                }
                count--;
            } while (count >= 0);
            break;
        }
    }
}
