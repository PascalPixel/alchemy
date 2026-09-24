#include "TYPES.H"

void Palette_CopyObjectBankToBackground14(void);
void Palette_LightenBankHighlight(s32);

void Item_PrepareUsePalette(void)
{
    Palette_CopyObjectBankToBackground14();
    Palette_LightenBankHighlight(13);
}

void Item_UseNoOpCallback(void)
{
}
