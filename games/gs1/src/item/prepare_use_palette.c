#include "types.h"
#include "scene.h"
#include "abi/item/prepare_use_palette.h"

void Item_PrepareUsePalette(void)
{
    Item_Run();
    Item_Do(13);
}

void Item_UseNoOpCallback(void)
{
}
