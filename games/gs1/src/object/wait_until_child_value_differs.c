#include "object_lookup.h"
#include "types.h"

struct Child_08091c44 {
    u8 padding[36];
    u8 value;
};

struct Object_08091c44 {
    u8 padding[80];
    struct Child_08091c44 *child;
    u8 active;
};

void WaitFrames(s32);

void Object_WaitUntilChildValueDiffers(s32 object_id, s32 value)
{
    struct Object_08091c44 *obj = ObjectTable_Get(object_id);

    if (obj != 0 && obj->active == 1) {
        s32 cnt = 0;
        u8 *p = &obj->child->value;

        while (cnt <= 89) {
            WaitFrames(1);
            if (value != *p) {
                break;
            }
            cnt++;
        }
    }
}
