#include "types.h"

extern const s16 gCharacter_SpriteTable[];

s32 Func_08044b80(u32 index)
{
    if (index > 8)
        return 0;
    return gCharacter_SpriteTable[index];
}
