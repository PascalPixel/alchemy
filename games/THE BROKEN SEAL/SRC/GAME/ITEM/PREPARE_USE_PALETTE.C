#include "TYPES.H"

void Palette_CopyMenuBgToObjPalette(void);
void Unnamed_080a2144(s32);

void Item_PrepareUsePalette(void)
{
    Palette_CopyMenuBgToObjPalette();
    Unnamed_080a2144(13);
}

void Item_UseNoOpCallback(void)
{
}
