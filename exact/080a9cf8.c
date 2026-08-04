#include "object_factory.h"
#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_080a9cf8(s32 resource) {
    void **output;
    void *object;
    s32 index;
    u8 *base;
    s32 arg5;

    base = *(u8 **)0x03001F2C;
    index = 0;
    arg5 = 0xA8;
    output = (void **)(base + 0xC8);
    do {
        object = Func_080150d0(2, index, resource, 0xF8, arg5);
        index += 1;
        *output = object;
        output += 1;
    } while (index <= 7);
    return 1;
}
