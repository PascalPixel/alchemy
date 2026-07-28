#include "types.h"

struct Fields_080b7f70 {
    u8 filler[0x50];
    void *field50;
    u8 field54;
};

void *Func_080b7f70(struct Fields_080b7f70 *object, s32 item_index) {
    s32 type = object->field54 & 0xF;
    if (type == 1) {
        if (item_index == 0) {
            return object->field50;
        }
    } else if (type == 2) {
        return ((void **)object->field50)[item_index];
    }
    return NULL;
}
