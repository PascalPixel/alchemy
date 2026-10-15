#include "types.h"

struct ContigoObject
{
    u8 unk00[0x62];
    u8 unk62;
};

void Func_02003f54(struct ContigoObject *object)
{
    object->unk62 = 1;
}
