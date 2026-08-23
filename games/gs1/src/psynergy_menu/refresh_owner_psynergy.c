#include "far_runtime.h"
#include "owner_state.h"
#include "psynergy_menu.h"

void Func_080a1bdc(s32 x, s32 y, s32 spacing);
void Func_08015080(s32 message, s32 *, s32 x, s32 y);

void PsynergyMenu_RefreshOwnerPsynergy(s32 owner_id)
{
    u16 *psynergies;
    struct PsynergyMenuState *menu;
    struct OwnerActionState *owner;

    menu = Data_03001f2c;
    owner = (struct OwnerActionState *)OwnerState_GetFar(owner_id);
    psynergies = menu->psynergies;
    menu->psynergy_count =
        PsynergyMenu_CollectActions(owner, psynergies, 2);
    Func_08015270(menu->psynergy_window);
    Func_080a1bdc(0x6c, 0x20, 8);
    PsynergyMenu_DrawPsynergyIcons(psynergies);
    if (menu->psynergy_count == 0) {
        Func_08015080(
            (s32)&PsynergyMenu_EmptyMessage,
            (s32 *)menu->psynergy_window,
            0,
            0x18);
    }
}
