#include "object_lookup.h"
#include "object_dispatch.h"

extern u8 *Data_03001ebc;
void Object_Destroy(void *);
void ObjectGroup_ApplyIndexedChildValue(struct DispatchObject *);
void ObjectGroup_SetChildValue(struct DispatchObject *, s32);

void ObjectTable_DestroyById(s32 index)
{
    void *object = ObjectTable_Get(index);
    u8 *base = Data_03001ebc;
    s32 offset;

    if (object != 0) {
        Object_Destroy(object);
        offset = index * 4;
        offset += 20;
        *(s32 *)(base + offset) = 0;
    }
}

void ObjectTable_DestroyNoOp(void)
{
}

void ObjectGroup_ConfigureChildValue(s32 object_id, s32 value)
{
    s32 flags;
    struct DispatchObject *object;

    object = ObjectTable_Get(object_id);
    if (object != NULL) {
        flags = 0x100 & value;
        if (flags != 0) {
            *(void (**)(struct DispatchObject *))((u8 *)object + 0x6c) =
                ObjectGroup_ApplyIndexedChildValue;
            return;
        }
        *(s32 *)((u8 *)object + 0x6c) = flags;
        ObjectGroup_SetChildValue(object, value);
    }
}

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

void ObjectGroup_SetChildValue(struct DispatchObject *object, s32 value)
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
