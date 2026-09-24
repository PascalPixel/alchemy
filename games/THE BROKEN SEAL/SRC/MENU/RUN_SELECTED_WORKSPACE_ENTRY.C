#include "TYPES.H"
#include "SCENE.H"

extern u8 *gSelectionWork;

void Menu_RunSelectedWorkspaceEntry(void)
{
    u8 *base = gSelectionWork;
    u32 index;

    ShopCursor_AdvanceFar(base + 0x5a4);
    index = *(u16 *)(base + 0x574);
    index *= 4;
    index += 0x610;
    Ui_ApplyTableScaleToObject(*(void **)(base + index));
}
