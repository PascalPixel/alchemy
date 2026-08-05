#include "types.h"

void Func_02001568();

/* Complete 24-byte entity reset owner. */
s32 Func_02000324(u8 *entity)
{
    Func_02001568(entity, 0);
    entity[89] = 0;
    return 0;
}
