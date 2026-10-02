#include "EDITION.H"
#include "TYPES.H"
#include "DMA.H"
#include "INVENTORY_MENU.H"
#include "RENDER_INPUT.H"
extern volatile s32 gKeysRepeat;
extern volatile s32 gKeyState;
extern const u8 Data_080af08c[];
extern u8 MsgItemName;
extern u8 MsgGiveHowMany;

/* The Japanese panel is narrower, asks below the tiles and counts with a
   counter word after each number. */
#if EDITION_INTERNATIONAL
#define GIVE_CURSOR_X 128
#define GIVE_ASK_X    32
#define GIVE_ASK_Y    0
#else
extern u8 MsgItemCounter;
#define GIVE_CURSOR_X 88
#define GIVE_ASK_X    0
#define GIVE_ASK_Y    32
#endif

void *Runtime_AllocateBlock(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
void RenderOutput_RedrawSavedRectFar(struct UiWindow *window);
void RenderOutput_ClearListFar(void *window);
s32 Resource_FindFreeEntry(void);
s32 VramBlock_LoadCached(s32 slot, s32 size, const void *source);
struct RenderOutput *RenderOutput_CreateFar(s32 slot, s32 attributes, s32 window, s32 x, s32 y);
void UiMenu_SlideCursor(s32 x, s32 y);
void UiMenu_PositionCursor(s32 x, s32 y);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void Shop_FillSelectorFar(s32 value, s32 column, void *tiles);
void UiNumber_DrawAt(s32 value, s32 digits, s32 window, s32 x, s32 y);
void *Owner_GetStateFar(s32 owner);
void UiText_DrawStringAtOffsetFar(void *text, s32 window, s32 x, s32 y);
void AudioCommand_PlayFar(s32 cue);
void WaitFrames(s32 frames);
s32 GameFlag_IsSet(s32 flag);

/* Asks how many of the selected item to hand over, between base + 1 and
 * base + range, and returns the count less one, or -1 when cancelled. With
 * single clear the receiving unit's stock is shown beside the giver's. */
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
        RenderOutput_CreateFar(slot, 0x40004000, window, 48, 32);
        object = RenderOutput_CreateFar(slot, 0x40004000, window, 80, 32);
        object->table.bits.index += 4;
        UiMenu_SlideCursor(GIVE_CURSOR_X, 40);
        while (!GameFlag_IsSet(0x150)) {
            if (changed) {
                changed = 0;
                quantity = (range + quantity) % range;
                RenderOutput_RedrawSavedRectFar((struct UiWindow *)window);
                UiText_DrawCharacterAtOffsetFar((s32)&MsgGiveHowMany, window, GIVE_ASK_X, GIVE_ASK_Y);
                Dma_Set(Data_080af08c, tiles, 0x84000040, (volatile u32 *)0x040000d4);
                Shop_FillSelectorFar(30, 14, tiles);
                Shop_FillSelectorFar(range + base, 0, tiles);
                Shop_FillSelectorFar(base + quantity + 1, 10, tiles);
                Shop_FillSelectorFar(base, 2, tiles);
                VramBlock_LoadCached(slot, 256, tiles);
                UiNumber_DrawAt(quantity + 1, 2, window, 32, 32);
                UiText_DrawCharacterAtOffsetFar((menu->selected_items[0] & 0x1ff) + (s32)&MsgItemName,
                    window, 16, 8);
                UiNumber_DrawAt(count - quantity - 1, 2, window, 16, 24);
#if EDITION_INTERNATIONAL
                if (single == 0)
                    UiNumber_DrawAt(other_count + quantity + 1, 2, window, 80, 24);
#else
                UiText_DrawCharacterAtOffsetFar((s32)&MsgItemCounter, window, 32, 24);
                if (single == 0) {
                    UiNumber_DrawAt(other_count + quantity + 1, 2, window, 80, 24);
                    UiText_DrawCharacterAtOffsetFar((s32)&MsgItemCounter, window, 96, 24);
                }
#endif
                UiText_DrawStringAtOffsetFar(Owner_GetStateFar(menu->pane_owner[0]), window, 16, 16);
                if (single == 0)
                    UiText_DrawStringAtOffsetFar(Owner_GetStateFar(menu->pane_owner[1]), window, 80, 16);
            }
            if (gKeyState & 1) {
                AudioCommand_PlayFar(112);
                break;
            }
            if (gKeyState & 2) {
                quantity = -1;
                AudioCommand_PlayFar(113);
                break;
            }
            UiMenu_PositionCursor(GIVE_CURSOR_X, 40);
            if (gKeysRepeat & 32) {
                quantity--;
                changed = 1;
                AudioCommand_PlayFar(111);
            }
            if (gKeysRepeat & 16) {
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
    menu->selected_item_icon->state = 13;
    if (GameFlag_IsSet(0x150))
        quantity = -1;
    return quantity;
}
