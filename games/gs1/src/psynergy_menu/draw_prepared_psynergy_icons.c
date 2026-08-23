#include "owner_state.h"
#include "psynergy_menu.h"

void Func_080a345c(void);

void PsynergyMenu_DrawPreparedPsynergyIcons(s32 unused, s32 owner_id)
{
    struct PsynergyMenuState *menu = Data_03001f2c;

    OwnerState_GetFar(owner_id);
    Func_080a345c();
    PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
}

void PsynergyMenu_PreparedIconsNoOp(void)
{
}
