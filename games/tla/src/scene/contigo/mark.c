#include "types.h"

struct ContigoObject {
    u8 unknown_00[0x62];
    u8 unknown_62;
};

void Func_02003f54(struct ContigoObject *object)
{
    object->unknown_62 = 1;
}
