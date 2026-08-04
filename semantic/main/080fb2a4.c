#include "types.h"

struct Object {
    u32 pad[7];
    u16 scale;
    u16 value;
    u16 product;
    u16 pad_22[9];
    u32 tag;
};

void Func_080fb2a4(struct Object *object, u32 value)
{
    value = (u16)value;
    if (object->tag == 0x68736d53) {
        u16 scale;
        u32 scaled;

        object->value = value;
        scale = object->scale;
        scaled = value;
        scaled *= scale;
        scaled >>= 8;
        object->product = scaled;
    }
}
