#include "types.h"

struct Child_080ba918 {
    u8 filler_00[5];
    s8 value;
    u8 filler_06[16];
    u8 flags : 8;
};

struct Record_080ba918 {
    u8 filler_00[39];
    u8 child_count;
    struct Child_080ba918 *first_child;
    struct Child_080ba918 *children[1];
};

struct Fields_080b7f70;

void *Func_080b7f70(struct Fields_080b7f70 *, s32);

struct Record_080ba918 *Func_080ba918(void *object, s32 value)
{
    u32 first_mask;
    void *saved_object;
    s32 saved_value;
    s32 object_index;
    struct Record_080ba918 *record;

    first_mask = (u8)((u32)object | ~(u32)object);
    saved_object = (void *)((u32)object +
        (((u32)object | ~(u32)object) + 1));
    saved_value = value + (((u32)object | ~(u32)object) + 1);
    object_index = 0;
    while ((record = Func_080b7f70(saved_object, object_index)) != NULL) {
        struct Child_080ba918 *child;
        struct Child_080ba918 **children;
        s32 child_count;
        s32 zero;
        u32 inner_mask;
        s32 remaining;

        child = record->first_child;
        children = record->children;
        child->flags |= first_mask;
        child_count = record->child_count;
        child->value = saved_value;
        if (child_count > 1) {
            zero = 0;
            inner_mask = 0xff;
            remaining = child_count - 1;
            do {
                child = *children++;
                child->value = zero;
                child->flags |= inner_mask;
                remaining--;
            } while (remaining != 0);
        }
        object_index++;
    }
    return record;
}
