#include "types.h"
#include "ram_buffer.h"

struct PsynergyMenuState {
    u8 unknown_000[0x24];
    s32 psynergy_window;
    u8 unknown_028[0x19c];
    u16 psynergies[40];
    u8 psynergy_count;
};

extern char MsgPsynergyMenuEmpty[];

void *UnitGet(s32 owner);
s32 Func_080fd6f0(void *owner, u16 *out, s32 mode);
void Func_080fd6b0(u16 *psynergies);
void RenderOutput_RedrawSavedRectFar(s32 window);
void PsynergyMenu_RefreshOwnerEntries(s32 x, s32 y, s32 count);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);

// 持ち主のエナジーを集めて一覧を描きなおす。ひとつもなければその旨を出す
void PsynergyMenu_RefreshOwnerPsynergy(s32 owner_id)
{
    u16 *psynergies;
    struct PsynergyMenuState *menu;
    void *owner;

    menu = Ram_HeapSlots->menu_runtime;
    owner = UnitGet(owner_id);
    psynergies = menu->psynergies;
    menu->psynergy_count = Func_080fd6f0(owner, psynergies, 2);
    RenderOutput_RedrawSavedRectFar(menu->psynergy_window);
    PsynergyMenu_RefreshOwnerEntries(0x6c, 0x20, 8);
    Func_080fd6b0(psynergies);
    if (menu->psynergy_count == 0) {
        UiText_DrawCharacterAtOffsetFar((s32)MsgPsynergyMenuEmpty, menu->psynergy_window, 0, 0x18);
    }
}
