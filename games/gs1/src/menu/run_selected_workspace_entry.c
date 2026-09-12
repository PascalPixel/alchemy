#include "types.h"
#include "scene.h"

extern u8 *gIw;

void Menu_RunSelectedWorkspaceEntry(void)
{
    u8 *base = gIw;
    u32 index;

    Menu_Do(base + 0x5a4);
    index = *(u16 *)(base + 0x574);
    index *= 4;
    index += 0x610;
    Ui_ApplyTableScaleToObject(*(void **)(base + index));
}
