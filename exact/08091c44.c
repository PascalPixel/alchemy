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

void Func_080030f8(s32);

void Func_08091c44(s32 object_id, s32 value)
{
    struct Object_08091c44 *object = Func_0808ba1c(object_id);

    if (object != 0 && object->active == 1) {
        s32 index = 0;
        u8 *current = &object->child->value;

        while (index <= 89) {
            Func_080030f8(1);
            if (value != *current) {
                break;
            }
            index++;
        }
    }
}
