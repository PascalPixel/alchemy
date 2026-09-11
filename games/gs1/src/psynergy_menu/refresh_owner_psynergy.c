#include "scene.h"
#include "abi/psynergy_menu/refresh_owner_psynergy.h"
#include "far_runtime.h"
#include "owner_state.h"
#include "psynergy_menu.h"

void UiText_DrawAt(s32 message, s32 *, s32 x, s32 y);

void PsynergyMenu_RefreshOwnerPsynergy(s32 owner_id)
{
    u16 *psynergies;
    struct PsynergyMenuState *menu;
    struct OwnerActionState *owner;

    menu = gIw;
    owner = (struct OwnerActionState *)OwnerState_GetFar(owner_id);
    psynergies = menu->psynergies;
    menu->psynergy_count =
        PsynergyMenu_CollectActions(owner, psynergies, 2);
    UiWindow_Commit(menu->psynergy_window);
    Sys_Place(0x6c, 0x20, 8);
    PsynergyMenu_DrawPsynergyIcons(psynergies);
    if (menu->psynergy_count == 0) {
        UiText_DrawAt(
            (s32)&PsynergyMenu_EmptyMessage,
            (s32 *)menu->psynergy_window,
            0,
            0x18);
    }
}
