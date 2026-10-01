/* Not exact: 4215 on the permuter scorer (was 4787). Declaring slot, changed,
   other_count and count in that order gives the reference frame, and the item
   mask is a plain halfword and. Remaining: the window takes r8 and the
   quantity r7 where the reference has r7 and r8 (their allocation priorities
   are 0.415 and 0.434 here; the reference needs the window ahead), and every
   reload uses r3 where the reference alternates r2 and r3. Message 0xade is
   the unnamed entry after MsgSwapForWhat. */
#include "TYPES.H"
#include "DMA.H"
#include "INVENTORY_MENU.H"
#include "RENDER_INPUT.H"
extern volatile s32 gKeysRepeat;
extern volatile s32 gKeyState;
extern const u8 Data_080af08c[];
extern u8 MsgItemName;

void *Runtime_AllocateBlock(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
void RenderOutput_RedrawSavedRectFar(s32 window);
void RenderOutput_ClearListFar(s32 window);
s32 Resource_FindFreeEntry(void);
s32 VramBlock_LoadCached(s32 slot, s32 size, const void *source);
struct RenderOutput *RenderOutput_CreateFar(s32 slot, s32 attributes, s32 window, s32 x, s32 y);
void UiMenu_SlideCursor(s32 x, s32 y);
void UiMenu_PositionCursor(s32 x, s32 y);
s32 __modsi3(s32 value, s32 modulus);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void Shop_FillSelectorFar(s32 value, s32 column, void *tiles);
void UiNumber_DrawAt(s32 value, s32 digits, s32 window, s32 x, s32 y);
void *Owner_GetStateFar(s32 owner);
void UiText_DrawStringAtOffsetFar(void *text, s32 window, s32 x, s32 y);
void Audio_PlayCue(s32 cue);
void WaitFrames(s32 frames);
s32 GameFlag_IsSet(s32 flag);

s32 ItemMenu_SelectGiveQuantity(s32 base, s32 range, s32 single)
{
    s32 slot;
    s32 changed;
    s32 other_count;
    s32 count;
    struct InventoryMenuState *menu = gMenuWork;
    void *tiles = Runtime_AllocateBlock(14, 0x400);
    s32 window;
    s32 quantity;
    struct RenderOutput *object;

    other_count = 0;
    changed = 1;
    window = menu->message_window;
    ItemMenu_SetMsgWin7();
    RenderOutput_RedrawSavedRectFar(window);
    quantity = base;
    if (single == 0)
        other_count = InventoryMenu_GetItemQuantity(menu->target_owner, menu->selected_item & 0x1ff);
    count = InventoryMenu_GetItemQuantity(menu->item_owner, menu->selected_item & 0x1ff);
    slot = Resource_FindFreeEntry();
    if (slot != 96) {
        VramBlock_LoadCached(slot, 256, 0);
        RenderOutput_CreateFar(slot, 0x40004000, window, 48, 32);
        object = RenderOutput_CreateFar(slot, 0x40004000, window, 80, 32);
        object->table.bits.index += 4;
        UiMenu_SlideCursor(128, 40);
        while (!GameFlag_IsSet(0x150)) {
            if (changed) {
                changed = 0;
                quantity = __modsi3(range + quantity, range);
                RenderOutput_RedrawSavedRectFar(window);
                UiText_DrawCharacterAtOffsetFar(0xade, window, 32, 0);
                Dma_Set(Data_080af08c, tiles, 0x84000040, (volatile u32 *)0x040000d4);
                Shop_FillSelectorFar(30, 14, tiles);
                Shop_FillSelectorFar(range + base, 0, tiles);
                Shop_FillSelectorFar(base + quantity + 1, 10, tiles);
                Shop_FillSelectorFar(base, 2, tiles);
                VramBlock_LoadCached(slot, 256, tiles);
                UiNumber_DrawAt(quantity + 1, 2, window, 32, 32);
                UiText_DrawCharacterAtOffsetFar((menu->selected_item & 0x1ff) + (s32)&MsgItemName,
                    window, 16, 8);
                UiNumber_DrawAt(count - quantity - 1, 2, window, 16, 24);
                if (single == 0)
                    UiNumber_DrawAt(other_count + quantity + 1, 2, window, 80, 24);
                UiText_DrawStringAtOffsetFar(Owner_GetStateFar(menu->item_owner), window, 16, 16);
                if (single == 0)
                    UiText_DrawStringAtOffsetFar(Owner_GetStateFar(menu->target_owner), window, 80, 16);
            }
            if (gKeyState & 1) {
                Audio_PlayCue(112);
                break;
            }
            if (gKeyState & 2) {
                quantity = -1;
                Audio_PlayCue(113);
                break;
            }
            UiMenu_PositionCursor(128, 40);
            if (gKeysRepeat & 32) {
                quantity--;
                changed = 1;
                Audio_PlayCue(111);
            }
            if (gKeysRepeat & 16) {
                quantity++;
                changed = 1;
                Audio_PlayCue(111);
            }
            WaitFrames(1);
        }
    }
    RenderOutput_RedrawSavedRectFar(window);
    RenderOutput_ClearListFar(window);
    Runtime_ReleaseHeapBlock(14);
    menu->selected_item_icon->state = 13;
    if (GameFlag_IsSet(0x150))
        quantity = -1;
    return quantity;
}
