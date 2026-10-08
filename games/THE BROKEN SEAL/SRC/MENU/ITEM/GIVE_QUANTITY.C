#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "RESOURCE.H"
#include "MEM.H"
#include "EDITION.H"
#include "BTLSYS.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "DMA.H"
#include "INVENTORY_MENU.H"
#include "RENDER_INPUT.H"
#include "UI.H"
#include "MENU_LIST.H"
#include "text/MSG_IDS.H"
extern volatile s32 gKeysRepeat;
extern volatile s32 gKeyState;

#if EDITION_INTERNATIONAL
#define GIVE_CURSOR_X 128
#define GIVE_ASK_X    32
#define GIVE_ASK_Y    0
#else
#define GIVE_CURSOR_X 88
#define GIVE_ASK_X    0
#define GIVE_ASK_Y    32
#endif

s32 ItemMenu_SelectGiveQuantity(s32 base, s32 range, s32 single)
{
    s32 quantity = base;
    s32 slot;
    s32 changed;
    s32 other_count;
    s32 count;
    struct InventoryMenuState *menu = gMenuWork;
    void *tiles = Runtime_AllocateBlock(14, 0x400);
    s32 window;
    struct RenderOutput *object;

    changed = 1;
    other_count = 0;
    window = (s32)menu->message_window;
    ItemMenu_SetMsgWin7();
    RenderOutput_RedrawSavedRectFar((struct UiWindow *)window);
    if (single == 0)
        other_count = InventoryMenu_GetItemQuantity(menu->pane_owner[1], menu->selected_items[0] & 0x1ff);
    count = InventoryMenu_GetItemQuantity(menu->pane_owner[0], menu->selected_items[0] & 0x1ff);
    slot = Resource_FindFreeEntry();
    if (slot != 96) {
        VramBlock_LoadCached(slot, 256, 0);
        RenderOutput_CreateFar(slot, 0x40004000, (struct RenderInput *)window, 48, 32);
        object = RenderOutput_CreateFar(slot, 0x40004000, (struct RenderInput *)window, 80, 32);
        object->table.bits.index += 4;
        UiMenu_SlideCursor(GIVE_CURSOR_X, 40);
        while (!GameFlag_IsSet(0x150)) {
            if (changed) {
                changed = 0;
                quantity = (range + quantity) % range;
                RenderOutput_RedrawSavedRectFar((struct UiWindow *)window);
                UiText_DrawCharacterAtOffsetFar((s32)MsgGiveHowMany, window, GIVE_ASK_X, GIVE_ASK_Y);
                Dma_Set(Menu_QuantityTiles, tiles, 0x84000040, REG_DMA3);
                Shop_FillSelectorFar(30, 14, tiles);
                Shop_FillSelectorFar(range + base, 0, tiles);
                Shop_FillSelectorFar(base + quantity + 1, 10, tiles);
                Shop_FillSelectorFar(base, 2, tiles);
                VramBlock_LoadCached(slot, 256, tiles);
                UiNumber_DrawAt(quantity + 1, 2, window, 32, 32);
                UiText_DrawCharacterAtOffsetFar((menu->selected_items[0] & 0x1ff) + (s32)MsgItemName,
                    window, 16, 8);
                UiNumber_DrawAt(count - quantity - 1, 2, window, 16, 24);
#if EDITION_INTERNATIONAL
                if (single == 0)
                    UiNumber_DrawAt(other_count + quantity + 1, 2, window, 80, 24);
#else
                UiText_DrawCharacterAtOffsetFar((s32)MsgItemCounter, window, 32, 24);
                if (single == 0) {
                    UiNumber_DrawAt(other_count + quantity + 1, 2, window, 80, 24);
                    UiText_DrawCharacterAtOffsetFar((s32)MsgItemCounter, window, 96, 24);
                }
#endif
                UiText_DrawStringAtOffsetFar(UnitGetFar(menu->pane_owner[0])->name, window, 16, 16);
                if (single == 0)
                    UiText_DrawStringAtOffsetFar(UnitGetFar(menu->pane_owner[1])->name, window, 80, 16);
            }
            if (gKeyState & KEY_A) {
                AudioCommand_PlayFar(112);
                break;
            }
            if (gKeyState & KEY_B) {
                quantity = -1;
                AudioCommand_PlayFar(113);
                break;
            }
            UiMenu_PositionCursor(GIVE_CURSOR_X, 40);
            if (gKeysRepeat & KEY_LEFT) {
                quantity--;
                changed = 1;
                AudioCommand_PlayFar(111);
            }
            if (gKeysRepeat & KEY_RIGHT) {
                quantity++;
                changed = 1;
                AudioCommand_PlayFar(111);
            }
            WaitFrames(1);
        }
    }
    RenderOutput_RedrawSavedRectFar((struct UiWindow *)window);
    RenderOutput_ClearListFar((void *)window);
    Runtime_ReleaseHeapBlock(14);
    menu->selected_item_icon->active = 13;
    if (GameFlag_IsSet(0x150))
        quantity = -1;
    return quantity;
}
