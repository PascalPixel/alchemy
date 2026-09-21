#include "OWNER_STATE.H"
#include "PSYNERGY_MENU.H"

void Func_080a345c(void);
#define ItemMenu_HideAllIcons Func_080a345c

void PsynergyMenu_DrawPreparedPsynergyIcons(s32 unused, s32 owner_id)
{
    struct PsynergyMenuState *menu = Data_03001f2c;

    OwnerState_GetFar(owner_id);
    ItemMenu_HideAllIcons();
    PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
}

void Func_080a939c(void)
{
}
