#include "types.h"

s32 Resource_ResetEntry(s32);

/* 受け取った値を呼出し先へ渡す。 */
void TextResource_Release(s32 value)
{
    Resource_ResetEntry(value);
}
