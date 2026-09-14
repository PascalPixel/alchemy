#include "OWNER_STATE.H"
#include "PSYNERGY_MENU.H"

void Func_080a345c(void);

void PsynergyMenu_DrawPreparedPsynergyIcons(s32 unused, s32 owner_id)
{
    struct PsynergyMenuState *menu = Data_03001f2c;

    OwnerState_GetFar(owner_id);
    Func_080a345c();
    PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
}

void Func_080a939c(void)
{
}
