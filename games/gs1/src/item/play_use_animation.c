#include "types.h"
#include "scene.h"

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void *Item_GetData();

void Item_PlayUseAnimation(void)
{
    Ability_PlayUseAnimation(0x3fff & FIELD(Item_GetData(), u16 *, 0x28));
}
