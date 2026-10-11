#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "mem.h"
#include "edition.h"
#include "btlsys.h"
#include "types.h"
#include "io_reg.h"
#include "dma.h"
#include "inventory_menu.h"
#include "render_input.h"
#include "ui.h"
#include "menu_list.h"
#include "text/msg_ids.h"
void UiText_DrawNumberInWindowFar(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 GameFlag_TestFar(s32 flag);

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
        object->oam.index += 4;
        UiMenu_SlideCursor(GIVE_CURSOR_X, 40);
        while (!GameFlag_TestFar(0x150)) {
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
                UiText_DrawNumberInWindowFar(quantity + 1, 2, window, 32, 32);
                UiText_DrawCharacterAtOffsetFar((menu->selected_items[0] & 0x1ff) + (s32)MsgItemName,
                    window, 16, 8);
                UiText_DrawNumberInWindowFar(count - quantity - 1, 2, window, 16, 24);
#if EDITION_INTERNATIONAL
                if (single == 0)
                    UiText_DrawNumberInWindowFar(other_count + quantity + 1, 2, window, 80, 24);
#else
                UiText_DrawCharacterAtOffsetFar((s32)MsgItemCounter, window, 32, 24);
                if (single == 0) {
                    UiText_DrawNumberInWindowFar(other_count + quantity + 1, 2, window, 80, 24);
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
    if (GameFlag_TestFar(0x150))
        quantity = -1;
    return quantity;
}
