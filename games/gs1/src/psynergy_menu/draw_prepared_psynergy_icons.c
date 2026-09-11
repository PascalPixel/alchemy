#include "scene.h"
#include "abi/psynergy_menu/draw_prepared_psynergy_icons.h"
#include "owner_state.h"
#include "psynergy_menu.h"

void PsynergyMenu_DrawPreparedPsynergyIcons(s32 unused, s32 owner_id)
{
    struct PsynergyMenuState *menu = gIw;

    OwnerState_GetFar(owner_id);
    Sys_Run();
    PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
}

void PsynergyMenu_PreparedIconsNoOp(void)
{
}
