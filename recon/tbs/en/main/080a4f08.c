/* Draft, not exact (2026-09-26): candidate=700 reference=712,
   311 differing halfwords, 148 aligned edits. Three structural hypotheses:
   (1) RedrawSavedRectFar veneers to the actual void RenderOutput callee;
       correcting its return type leaves the 696-byte baseline unchanged
       (317 differing halfwords, 144 aligned edits).
   (2) Shared inventory/render types and MsgItemName follow the exact adjacent
       item-header owner. A narrowed external mask still emits an SI load:
       700 bytes, 311 differing halfwords, 148 aligned edits. The middle pool
       remains absent and the independent scalar stack slots remain wrong.
   (3) Witness b698bb7a2 groups count/other_count/changed/slot in reference order
       and narrows the redraw item. It disproves that aggregate model: a
       persistent struct base grows the frame from 28 to 32 bytes, spills all
       arguments, and the item mask still folds into a word AND. No mid-pool:
       724 bytes, 346 differing halfwords, 203 aligned edits.
   Stopped after three hypotheses; independent scalars from (2) restored.
   Caller audit: RUN_COMMANDS passes (0, quantity, single); -1 cancels and
   the result is otherwise zero-based. Func_080b0040 is a void tile writer
   through the veneer to 080b06c0. No bytes are adopted by this draft. */
/* Item menu: choose how many of the selected item to hand from one party
   member to another. Left and right step the amount, A
   confirms and B cancels with -1. */
#include "TYPES.H"
#include "DMA.H"
#include "INVENTORY_MENU.H"
#include "RENDER_INPUT.H"
extern volatile s32 gKeysRepeat;
extern volatile u32 gKeyState;
extern const u8 Data_080af08c[];
extern u8 MsgItemName;
extern u8 Value_000001ff;

void *Runtime_AllocateBlock(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
void ItemMenu_SetMsgWin7(void);
void RenderOutput_RedrawSavedRectFar(s32 window);
void RenderOutput_ClearListFar(s32 window);
s32 Func_080a3d9c(s32 owner, s32 item);
s32 Resource_FindFreeEntry(void);
s32 VramBlock_LoadCached(s32 slot, s32 size, const void *source);
struct RenderOutput *RenderOutput_CreateFar(s32 slot, s32 attributes, s32 window, s32 x, s32 y);
void UiMenu_SlideCursor(s32 x, s32 y);
void UiMenu_PositionCursor(s32 x, s32 y);
s32 Math_Mod(s32 value, s32 modulus);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void Func_080b0040(s32 value, s32 column, void *tiles);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x, s32 y);
void *Owner_GetStateFar(s32 owner);
void UiText_DrawStringAtOffsetFar(void *text, s32 window, s32 x, s32 y);
void Audio_PlayCue(s32 cue);
void WaitFrames(s32 frames);
s32 GameFlag_TestFar(s32 flag);

s32 ItemMenu_SelectGiveQuantity(s32 base, s32 range, s32 single)
{
    struct InventoryMenuState *menu = gMenuWork;
    u8 *tiles = Runtime_AllocateBlock(14, 0x400);
    s32 other_count = 0;
    s32 changed = 1;
    s32 window = menu->message_window;
    s32 count;
    s32 slot;
    s32 quantity;
    struct RenderOutput *object;

    ItemMenu_SetMsgWin7();
    RenderOutput_RedrawSavedRectFar(window);
    quantity = base;
    if (single == 0)
        other_count = InventoryMenu_GetItemQuantity(menu->target_owner,
            menu->selected_item & (u16)(s32)&Value_000001ff);
    count = InventoryMenu_GetItemQuantity(menu->item_owner,
        menu->selected_item & (u16)(s32)&Value_000001ff);
    slot = Resource_FindFreeEntry();
    if (slot == 96)
        goto done;
    VramBlock_LoadCached(slot, 256, 0);
    RenderOutput_CreateFar(slot, 0x40004000, window, 48, 32);
    object = RenderOutput_CreateFar(slot, 0x40004000, window, 80, 32);
    object->table.bits.index += 4;
    UiMenu_SlideCursor(128, 40);
    goto check_exit;

update:
    if (changed == 0)
        goto input;
    changed = 0;
    quantity = Math_Mod(range + quantity, range);
    RenderOutput_RedrawSavedRectFar(window);
    UiText_DrawCharacterAtOffsetFar(0xade, window, 32, 0);
    Dma_Set(Data_080af08c, tiles, 0x84000040, (volatile u32 *)0x040000d4);
    Func_080b0040(30, 14, tiles);
    Func_080b0040(range + base, 0, tiles);
    Func_080b0040(base + quantity + 1, 10, tiles);
    Func_080b0040(base, 2, tiles);
    VramBlock_LoadCached(slot, 256, tiles);
    UiText_DrawNumberInWindowFar(quantity + 1, 2, window, 32, 32);
    UiText_DrawCharacterAtOffsetFar((menu->selected_item & (u16)(s32)&Value_000001ff)
        + (s32)&MsgItemName, window, 16, 8);
    UiText_DrawNumberInWindowFar(count - quantity - 1, 2, window, 16, 24);
    if (single == 0)
        UiText_DrawNumberInWindowFar(other_count + quantity + 1, 2, window, 80, 24);
    UiText_DrawStringAtOffsetFar(Owner_GetStateFar(menu->item_owner), window, 16, 16);
    if (single == 0)
        UiText_DrawStringAtOffsetFar(Owner_GetStateFar(menu->target_owner), window, 80, 16);

input:
    {
        volatile u32 *keys = &gKeyState;

        if (*keys & 1) {
            Audio_PlayCue(112);
            goto done;
        }
        if (*keys & 2) {
            quantity = -1;
            Audio_PlayCue(113);
            goto done;
        }
    }
    UiMenu_PositionCursor(128, 40);
    {
        volatile s32 *keys = &gKeysRepeat;

        if (*keys & 32) {
            quantity -= 1;
            changed = 1;
            Audio_PlayCue(111);
        }
        if (*keys & 16) {
            quantity += 1;
            changed = 1;
            Audio_PlayCue(111);
        }
    }
    WaitFrames(1);

check_exit:
    if (GameFlag_TestFar(336) == 0)
        goto update;

done:
    RenderOutput_RedrawSavedRectFar(window);
    RenderOutput_ClearListFar(window);
    Runtime_ReleaseHeapBlock(14);
    menu->selected_item_icon->state = 13;
    if (GameFlag_TestFar(336))
        quantity = -1;
    return quantity;
}
