#include "types.h"

extern const s16 Character_SpriteTable[];

s32 Func_08044b80(u32 index)
{
    if (index > 8)
        return 0;
    return Character_SpriteTable[index];
}
