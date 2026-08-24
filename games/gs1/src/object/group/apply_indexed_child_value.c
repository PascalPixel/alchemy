#include "object_dispatch.h"

extern u32 Data_03001e40;
extern u8 Data_0809ed80[];

void ObjectGroup_ApplyIndexedChildValue(struct DispatchObject *object)
{

    if ((object->kind & 0xf) == 1) {
        u8 child_value;
        u8 *container;
        u8 child_count;

        child_value = Data_0809ed80[(Data_03001e40 >> 1) & 3];
        container = object->target.child;
        child_count = *(container + 0x27);
        if (child_count != 0) {
            u8 **entries = (u8 **)(container + 0x28);
            s32 remaining = child_count;
            do {
                u8 *entry = *entries++;
                if (entry != 0 && *(u32 *)(entry + 0x10) != 0) {
                    *(entry + 5) = child_value;
                }
                remaining--;
            } while (remaining != 0);
        }
        *(container + 0x25) = 1;
    }
}
