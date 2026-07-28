#include "object_dispatch.h"

void Func_080929d8(struct DispatchObject *object, s32 value)
{
    if ((object->kind & 0xf) == 1) {
        u8 *container = object->target.child;
        u8 raw_count = container[0x27];

        if (raw_count != 0) {
            void **entry = (void **)(container + 0x28);
            u32 count = raw_count;
            do {
                void *item = *entry++;
                if (item != NULL && *(s32 *)((u8 *)item + 0x10) != 0) {
                    *((s8 *)item + 5) = value;
                }
                count--;
            } while (count != 0);
        }
        container[0x25] = 1;
    }
}
