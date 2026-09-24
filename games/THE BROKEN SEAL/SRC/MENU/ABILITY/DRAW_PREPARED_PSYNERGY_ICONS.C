#include "OWNER_STATE.H"
#include "PSYNERGY_MENU.H"

void ItemMenu_HideAllIcons(void);

void PsynergyMenu_DrawPreparedPsynergyIcons(s32 unused, s32 owner_id)
{
    struct PsynergyMenuState *menu = gMenuWork;

    OwnerState_GetFar(owner_id);
    ItemMenu_HideAllIcons();
    PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
}

void PsynergyMenu_ReservedNoOp939C(void)
{
}
