#include "types.h"

typedef struct {
    u8 pad[9];
    u8 lo:2;
    u8 field:2;
    u8 hi:4;
} Object_02000030;

void Func_02000030(void *arg0, s32 arg1)
{
    Object_02000030 *object = *(Object_02000030 **)((u8 *)arg0 + 0x50);

    object->field = arg1;
}
