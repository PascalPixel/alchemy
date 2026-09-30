#include "FAR_RUNTIME.H"
#include "OWNER_STATE.H"
#include "PSYNERGY_MENU.H"

void PsynergyMenu_RefreshOwnerEntries(s32 x, s32 y, s32 spacing);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 *, s32 x, s32 y);

void PsynergyMenu_RefreshOwnerPsynergy(s32 owner_id)
{
    u16 *psynergies;
    struct PsynergyMenuState *menu;
    struct OwnerActionState *owner;

    menu = gMenuWork;
    owner = (struct OwnerActionState *)Owner_GetStateFar(owner_id);
    psynergies = menu->psynergies;
    menu->psynergy_count =
        PsynergyMenu_CollectActions(owner, psynergies, 2);
    RenderOutput_RedrawSavedRectFar(menu->psynergy_window);
    PsynergyMenu_RefreshOwnerEntries(0x6c, 0x20, 8);
    PsynergyMenu_DrawPsynergyIcons(psynergies);
    if (menu->psynergy_count == 0) {
        UiText_DrawCharacterAtOffsetFar(
            (s32)&MsgPsynergyMenuEmpty,
            (s32 *)menu->psynergy_window,
            0,
            0x18);
    }
}

s32 PsynergyMenu_ReturnTrue(void)
{
    return 1;
}

/* An empty routine after it; nothing in the ROM refers to it. */
void PsynergyMenu_NoOp(void)
{
}
