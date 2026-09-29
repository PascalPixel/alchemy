#include "OWNER_STATE.H"
#include "PSYNERGY_MENU.H"

void ItemMenu_HideAllIcons(void);

void PsynergyMenu_DrawPreparedPsynergyIcons(s32 unused, s32 owner_id)
{
    struct PsynergyMenuState *menu = gMenuWork;

    Owner_GetStateFar(owner_id);
    ItemMenu_HideAllIcons();
    PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
}

void PsynergyMenu_PreparedIconsNoOp(void)
{
}

/* A second empty routine; nothing in the ROM refers to either. */
void PsynergyMenu_PreparedIconsSecondNoOp(void)
{
}
