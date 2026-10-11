#include "types.h"

struct Ov694Object {
    u8 unknown_00[0x62];
    u8 unknown_62;
};

void Func_02003f54(struct Ov694Object *object)
{
    object->unknown_62 = 1;
}
